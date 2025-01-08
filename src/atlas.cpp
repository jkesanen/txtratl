#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "atlas.hpp"
#include "image.hpp"
#include "imageblit.hpp"

#include "vendor/rectpack2d/src/pack.h"

void Atlas::blitImageRect(Image& canvas, const ImageRect& ir) const
{
    auto& source = ir.image();

    // If the image data has not been loaded, do it now.
    if (!source.data(0, 0, 0))
    {
        const_cast<Image&>(source).load();
    }

    if (source.channels() == canvas.channels())
    {
        // Source and target are in the same pixel format, copy row by row.
        for (size_t row = 0; row < ir.height(); ++row)
        {
            auto src = source.data(row, 0U, 0U);
            auto dest = canvas.data(ir.y() + row, ir.x(), 0U);
            std::memcpy(dest, src, ir.width() * canvas.channels());
        }
    }
    else if (source.channels() == 3 && canvas.channels() == 4)
    {
        // Convert source from RGB to RGBA format.
        std::cout << "Fast RGBA blit " << ir.filepath() << std::endl;
        imageblit::blitRGBtoRGBA_SSE3(canvas.data(ir.y(), ir.x(), 0),
                                      source.data(0, 0, 0),
                                      ir.height(),
                                      ir.width(),
                                      canvas.width());
    }
    else if (source.channels() == 4 && canvas.channels() == 3)
    {
#if 1
        // Convert source from RGBA to RGB format.
        std::cout << "Fast blit " << ir.filepath() << std::endl;
        imageblit::blitRGBAtoRGB_SSE3(canvas.data(ir.y(), ir.x(), 0),
                                      source.data(0, 0, 0),
                                      ir.height(),
                                      ir.width(),
                                      canvas.width());
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
    for (auto& imagerect : mImages)
    {
        blitImageRect(canvas, imagerect);
    }

    return true;
}

bool Atlas::writeMetadata(const std::filesystem::path& outputFilepath) const
{
    // Always overwrite the output file.
    auto editor = std::ofstream{};
    editor.open(outputFilepath);

    if (!editor.good())
    {
        return false;
    }

    for (auto const& image : mImages)
    {
        // Create a tab separated row consisting of:
        //   filename, x coordinate, y coordinate, width, height
        editor << image.filepath().filename().string() << "\t";
        editor << image.x() << "\t" << image.y() << "\t";
        editor << image.width() << "\t" << image.height() << std::endl;
    }

    editor.flush();
    editor.close();

    return true;
}

bool Atlas::addImage(const std::filesystem::path& filename)
{
    mImages.emplace_back(ImageRect(filename.c_str()));
    return true;
}

bool Atlas::packImages()
{
    auto rects = std::vector<rect_xywhf*>{};

    // Push rect pointers into a vector for passing to rectpack2D
    for (const auto& image : mImages)
    {
        rects.push_back(const_cast<rect_xywhf*>(&image.rect()));
    }

    if (!rects.size())
    {
        return false;
    }

    auto bins = std::vector<bin>{};
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

bool Atlas::createAtlas(const std::filesystem::path& imageFilepath, const std::filesystem::path& metadataFilepath)
{
    // Create an empty image for atlas
    auto canvas = Image(mHeight, mWidth, 4);

    // Blit images into atlas image according to rect coordinates decided by packing
    if (!blitImages(canvas))
    {
        return false;
    }

    // Write the atlas image into the destionation file
    canvas.save(imageFilepath);
    canvas.release();

    if (!writeMetadata(metadataFilepath))
    {
        // Writing metadata failed. Remove generated files.
        std::filesystem::remove(imageFilepath);
        std::filesystem::remove(metadataFilepath);
        return false;
    }

    std::cout << "The texture atlas and atlas metadata successfully written into " << imageFilepath.string() << " and " << metadataFilepath.string() << std::endl;

    return true;
}
