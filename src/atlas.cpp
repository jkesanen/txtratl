#include <cstdio>
#include <ostream>

#include "atlas.hpp"
#include "image.hpp"
#include "imageblit.hpp"

void Atlas::blitImage(Image& canvas, ImageRect const& ir) const
{
    auto& source = ir.getImage();

    assert(canvas.rows() >= ir.getY() + ir.getHeight());
    assert(canvas.cols() >= ir.getX() + ir.getWidth());

    if (source.channels() == canvas.channels())
    {
        // Source and target are in the same pixel format. Copy row by row.
        for (size_t row = 0; row < ir.getHeight(); ++row)
        {
            unsigned char* const src = source.ptr(row, 0U, 0U);
            unsigned char* const dest = canvas.ptr(ir.getY() + row, ir.getX(), 0);
            std::memcpy(dest, src, ir.getWidth() * canvas.channels());
        }
    }
    else if (source.channels() == 3 && canvas.channels() == 4)
    {
        // Convert source from RGB to RGBA format using SSE3 intrinsics
        std::cout << "Fast RGBA blit " << ir.filename() << std::endl;
        imageblit::blitRGBtoRGBA_SSE3(canvas.ptr(ir.getY(), ir.getX(), 0),
                                      source.ptr(0, 0, 0),
                                      ir.getHeight(),
                                      ir.getWidth(),
                                      canvas.cols());
    }
    else if (source.channels() == 4 && canvas.channels() == 3)
    {
#if 1
        // Convert source from RGBA to RGB format using SSE3 intrinsics
        std::cout << "Fast blit " << ir.filename() << std::endl;
        imageblit::blitRGBAtoRGB_SSE3(canvas.ptr(ir.getY(), ir.getX(), 0),
                                      source.ptr(0, 0, 0),
                                      ir.getHeight(),
                                      ir.getWidth(),
                                      canvas.cols());
#else
        // Convert source from RGBA to RGB format
        for (size_t row = 0; row < ir.getHeight(); ++row)
        {
            unsigned char* src = source.ptr(row, 0, 0);
            unsigned char* dest = canvas.ptr(ir.getY() + row, ir.getX(), 0);

            for (size_t col = 0; col < ir.getWidth(); ++col)
            {
                std::memcpy(dest, src + col * source.channels(), canvas.channels());
                dest += canvas.channels();
            }
        }
#endif
    }
}

bool Atlas::blitImages(Image& canvas) const
{
    for (auto const& image : mImages)
    {
        blitImage(canvas, image);
    }

    return true;
}

static std::string getFilename(std::string path)
{
    std::replace(path.begin(), path.end(), '\\', '/');
    std::string::size_type pos = path.find_last_of("/");

    if (pos == std::string::npos)
    {
        return path;
    }

    if (pos != path.length())
    {
        return path.substr(pos + 1);
    }

    return std::string();
}

bool Atlas::writeMetadata(std::string const& outputFilename) const
{
    // Always overwrite the output file
    std::ofstream editor;
    editor.open(outputFilename);

    if (!editor.good())
    {
        return false;
    }

    for (auto const& image : mImages)
    {
        // Create a tab separated row consisting of:
        //   filename, x coordinate, y coordinate, width, height
        editor << getFilename(image.filename()) << "\t";
        editor << image.getX() << "\t" << image.getY() << "\t";
        editor << image.getWidth() << "\t" << image.getHeight() << std::endl;
    }

    editor.flush();
    editor.close();

    return true;
}

bool Atlas::addImage(std::string const& filename)
{
    mImages.emplace_back(ImageRect(filename.c_str()));
    return true;
}

bool Atlas::packImages()
{
    std::vector<rect_xywhf*> rects;

    // Push rect pointers into a vector for passing to rectpack2D
    for (const auto& image : mImages)
    {
        rects.push_back(const_cast<rect_xywhf*>(&image.getRect()));
    }

    if (!rects.size())
    {
        return false;
    }

    std::vector<bin> bins;
    pack(rects.data(), static_cast<int>(rects.size()), ATLAS_MAX_SIDE, false, bins);

    /* TODO: Packing algorithm supports creating several atlases out of a set
     * of images if they don't fit into a single atlas, but blitting the images
     * to several atlas canvases would need some refactoring. */
    if (bins.size() != 1)
    {
        return false;
    }

    // Update the size of the target atlas
    mWidth = static_cast<size_t>(bins[0].size.w);
    mHeight = static_cast<size_t>(bins[0].size.h);

    return true;
}

bool Atlas::createAtlas(std::string const& imageFilename, std::string const& metadataFilename)
{
    assert(isValid());

    // Create an empty image for atlas
    Image canvas(mHeight, mWidth, 3);

    // Blit images into atlas image according to rect coordinates decided by packing
    if (!blitImages(canvas))
    {
        return false;
    }

    // Write the atlas image into the destionation file
    canvas.save(imageFilename.c_str(), 99);
    canvas.release();

    if (!writeMetadata(metadataFilename))
    {
        // Writing metadata failed. Remove generated files.
        std::remove(imageFilename.c_str());
        std::remove(metadataFilename.c_str());
        return false;
    }

    std::cout << "Texture atlas and atlas metadata successfully written into " << imageFilename << " and " << metadataFilename << std::endl;

    return true;
}
