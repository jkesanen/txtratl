#include <fstream>
#include <iostream>
#include <vector>

#include "txtratl/atlas.hpp"
#include "txtratl/image.hpp"
#include "txtratl/imageblit.hpp"
#include "txtratl/imagerect.hpp"

#include "rectpack2d/pack.h"

namespace txtratl
{

class Atlas::ImageRectImpl final
{
public:
    using ImageRects = std::vector<ImageRect>;

    ImageRects& rects()
    {
        return mRects;
    }

private:
    ImageRects mRects{};
};

Atlas::Atlas() : mImpl(std::make_unique<ImageRectImpl>())
{
}

Atlas::~Atlas() = default;

Atlas::Atlas(const Atlas& other)
    : mImpl(std::make_unique<ImageRectImpl>(*other.mImpl))
{
}

Atlas& Atlas::operator=(Atlas rhs)
{
    swap(mImpl, rhs.mImpl);
    return *this;
}

void Atlas::blitImages(Image& canvas) const
{
    for (auto& imagerect : mImpl->rects())
    {
        canvas.blitImage(imagerect.image(), imagerect.x(), imagerect.y(), true);
    }
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

    for (auto const& image : mImpl->rects())
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

void Atlas::addImage(const std::filesystem::path& filepath)
{
    mImpl->rects().emplace_back(ImageRect(filepath));
}

bool Atlas::packImages()
{
    auto rects = std::vector<rect_xywhf*>{};

    // Push rect pointers into a vector for passing to rectpack2D
    for (const auto& image : mImpl->rects())
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

    // Update the size of the target atlas.
    mWidth = static_cast<size_t>(bins[0].size.w);
    mHeight = static_cast<size_t>(bins[0].size.h);

    return true;
}

bool Atlas::createAtlas(const std::filesystem::path& imageFilepath, const std::filesystem::path& metadataFilepath)
{
    // Create an empty image for atlas.
    auto canvas = Image(mHeight, mWidth, 4);

    // Blits the images into an atlas image according to rect coordinates decided by packing.
    blitImages(canvas);

    // Write the atlas image file and release the data from memory.
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
