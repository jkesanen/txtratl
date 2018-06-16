#include "atlas.hpp"
#include "imageblit.hpp"

using namespace zz;

void Atlas::blitImage(Image& canvas, ImageRect const& ir) const
{
    assert(canvas.rows() >= ir.getY() + ir.getHeight());
    assert(canvas.cols() >= ir.getX() + ir.getWidth());

    if (ir.mImage.channels() == canvas.channels())
    {
        // Source and target are in the same pixel format. Copy row by row.
        for (int row = 0; row < ir.getHeight(); ++row)
        {
            unsigned char* const src = ir.mImage.ptr(row, 0, 0);
            unsigned char* const dest = canvas.ptr(ir.getY() + row, ir.getX(), 0);
            std::memcpy(dest, src, ir.getWidth() * canvas.channels());
        }
    }
    else if (ir.mImage.channels() == 4 && canvas.channels() == 3)
    {
#if 1
        // Convert source from ARGB to RGB format using SSE3 intrinsics
        std::cout << "Fast blit " << ir.filename() << std::endl;
        imageblit::blitRGBAtoRGB_SSE3(canvas.ptr(ir.getY(), ir.getX(), 0),
                                      ir.mImage.ptr(0, 0, 0),
                                      ir.getHeight(),
                                      ir.getWidth(),
                                      canvas.cols() * canvas.channels());
#else
        // Convert source from ARGB to RGB format
        for (int row = 0; row < ir.getHeight(); ++row)
        {
            unsigned char* src = ir.mImage.ptr(row, 0, 0);
            unsigned char* dest = canvas.ptr(ir.getY() + row, ir.getX(), 0);

            for (int col = 0; col < ir.getWidth(); ++col)
            {
                std::memcpy(dest, src + col * ir.mImage.channels(), canvas.channels());
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
        editor << os::path_split_filename(image.filename()) << "\t";
        editor << image.getX() << "\t" << image.getY() << "\t";
        editor << image.getWidth() << "\t" << image.getHeight() << os::endl();
    }

    editor.flush();
    editor.close();

    return true;
}

bool Atlas::addImage(std::string const& filename)
{
    ImageRect img(filename.c_str());
    mImages.push_back(std::move(img));
    return true;
}

bool Atlas::packImages()
{
    std::vector<rect_xywhf*> rectPtrs;

    // Push rect ppointers into a vector as rectpack2D wants then
    for (auto& image : mImages)
    {
        rectPtrs.push_back(&image.mRect);
    }

    if (!rectPtrs.size())
    {
        return false;
    }

    std::vector<bin> bins;
    pack(rectPtrs.data(), static_cast<int>(rectPtrs.size()), ATLAS_MAX_SIDE, false, bins);

    /* TODO: Packing algorithm supports creating several atlases out of a set
     * of images if they don't fit into a single atlas, but blitting the images
     * to several atlas canvases would need some refactoring. */
    if (bins.size() != 1)
    {
        return false;
    }

    // Update the size of the target atlas
    mAtlasSize = {bins[0].size.w, bins[0].size.h};

    return true;
}

bool Atlas::createAtlas(std::string const& imageFilename, std::string const& metadataFilename)
{
    assert(mAtlasSize.isValid());

    // Create an empty image for atlas
    Image canvas(mAtlasSize.getHeight(), mAtlasSize.getWidth(), 3);

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
        os::remove_file(imageFilename);
        os::remove_file(metadataFilename);
        return false;
    }

    std::cout << "Texture atlas and atlas metadata successfully written into " << imageFilename << " and " << metadataFilename << std::endl;

    return true;
}
