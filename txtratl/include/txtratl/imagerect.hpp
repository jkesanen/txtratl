#pragma once

#include <filesystem>

#include "txtratl/image.hpp"
#include "rectpack2d/pack.h"

namespace txtratl
{

 /// Holds all the data for an image to be packed into atlas.
class ImageRect
{
public:
    explicit ImageRect(const std::filesystem::path& filepath)
        : mImage(filepath, true),
          mFilepath(filepath),
          mRect(0, 0, static_cast<int>(mImage.width()), static_cast<int>(mImage.height()))
    {
    }

    size_t x() const
    {
        return static_cast<size_t>(mRect.x);
    }

    size_t y() const
    {
        return static_cast<size_t>(mRect.y);
    }

    size_t width() const
    {
        return static_cast<size_t>(mRect.w);
    }

    size_t height() const
    {
        return static_cast<size_t>(mRect.h);
    }

    const Image& image() const
    {
        return mImage;
    }

    const rect_xywhf& rect() const
    {
        return mRect;
    }

    std::filesystem::path filepath() const
    {
        return mFilepath;
    }

private:
    Image mImage;
    std::filesystem::path mFilepath{};
    rect_xywhf mRect;
};

} // namespace txtratl
