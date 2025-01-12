#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "txtratl/atlas.hpp"
#include "txtratl/image.hpp"
#include "txtratl/imageblit.hpp"

#include "rectpack2d/pack.h"

namespace txtratl
{

bool Atlas::blitImages(Image& canvas) const
{
    for (auto& imagerect : mImages)
    {
        canvas.blitImage(imagerect.image(), imagerect.x(), imagerect.y());
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

bool Atlas::addImage(const std::filesystem::path& filepath)
{
    mImages.emplace_back(ImageRect(filepath));
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

} // namespace txtratl
