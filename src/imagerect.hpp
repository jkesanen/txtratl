#pragma once

#include "image.hpp"
#include "vendor/rectpack2d/src/pack.h"

/**
*  @brief Class that holds all the data for an image to be packed into atlas.
*/
class ImageRect
{
public:
    explicit ImageRect(const std::string& filename)
        : mFilename(filename),
          mImage(filename.c_str()),
          mRect(0, 0, static_cast<int>(mImage.cols()), static_cast<int>(mImage.rows()))
    {
    }

    size_t getX() const { return static_cast<size_t>(mRect.x); }
    size_t getY() const { return static_cast<size_t>(mRect.y); }
    size_t getWidth() const { return static_cast<size_t>(mRect.w); }
    size_t getHeight() const { return static_cast<size_t>(mRect.h); }
    const Image& getImage() const { return mImage; }
    const rect_xywhf& getRect() const { return mRect; }

    std::string filename() const { return mFilename; }

private:
    std::string mFilename;
    Image mImage;
    rect_xywhf mRect;
};
