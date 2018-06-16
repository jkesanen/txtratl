#pragma once

#include "vendor/rectpack2d/src/pack.h"

/**
*  @brief Class that holds all the data for an image to be packed into atlas.
*/
struct ImageRect
{
    std::string mFilename;
    zz::Image mImage;
    rect_xywhf mRect;

    ImageRect(std::string filename)
        : mFilename(filename),
          mImage(filename.c_str()),
          mRect(0, 0, mImage.cols(), mImage.rows())
    {
    }

    int getX() const { return mRect.x; }
    int getY() const { return mRect.y; }
    int getWidth() const { return mRect.w; }
    int getHeight() const { return mRect.h; }

    std::string filename() const { return mFilename; }
};
