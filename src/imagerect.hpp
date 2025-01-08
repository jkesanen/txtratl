#pragma once

#include <filesystem>

#include "image.hpp"
#include "vendor/rectpack2d/src/pack.h"

/**
 *  @brief Class that holds all the data for an image to be packed into atlas.
 */
class ImageRect
{
public:
    explicit ImageRect(const std::filesystem::path& filepath)
        : mImage(filepath, false),
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
    Image mImage{0, 0, 0};
    std::filesystem::path mFilepath{};
    rect_xywhf mRect;
};
