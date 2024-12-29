#include "image.hpp"

Image::~Image()
{
    release();
}

unsigned char* Image::ptr(size_t row, size_t col, size_t channel) const
{
    return mImage.ptr(static_cast<int>(row), static_cast<int>(col), static_cast<int>(channel));
}

size_t Image::channels() const
{
    return static_cast<size_t>(mImage.channels());
}

size_t Image::cols() const
{
    return static_cast<size_t>(mImage.cols());
}

size_t Image::rows() const
{
    return static_cast<size_t>(mImage.rows());
}

void Image::save(const std::string filename, size_t quality) const
{
    assert(quality <= 100);
    mImage.save(filename.c_str(), static_cast<int>(quality));
}

void Image::release()
{
    mImage.release();
}
