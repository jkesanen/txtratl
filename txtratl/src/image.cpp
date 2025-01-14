#include <cstring>

#include "txtratl/image.hpp"
#include "txtratl/imageblit.hpp"

namespace txtratl
{

Image::Image(const std::filesystem::path& filepath, bool deferLoading)
{
    if (deferLoading)
    {
        loadinfo(filepath);
    }
    else
    {
        load(filepath);
    }
}

Image::Image(size_t width, size_t height, size_t channels)
{
    allocate(width, height, channels);
}

void Image::allocate(size_t width, size_t height, size_t channels)
{
    mWidth = width;
    mHeight = height;
    mChannels = channels;
    mData = std::make_shared<std::vector<uint8_t>>(mWidth * mHeight * mChannels);
}

void Image::load()
{
    load(mFilePath);
}

uint8_t* Image::data(size_t x, size_t y, size_t channel) const
{
    if (!mData)
    {
        return nullptr;
    }

    size_t offset = y * mWidth * mChannels + x * mChannels + channel;
    return (*mData).data() + offset;
}

size_t Image::channels() const
{
    return mChannels;
}

size_t Image::height() const
{
    return mHeight;
}

size_t Image::width() const
{
    return mWidth;
}

void Image::release()
{
    mData = nullptr;
}

void Image::blitImage(const Image& source, size_t x, size_t y, bool releaseAfterUse)
{
    // If the image data has not been loaded, do it now.
    if (!source.data(0, 0, 0))
    {
        const_cast<Image&>(source).load();
    }

    if (source.channels() == channels())
    {
        // Source and target are in the same pixel format, copy row by row.
        for (size_t row = 0; row < source.height(); ++row)
        {
            auto src = source.data(0, row, 0U);
            auto dest = data(x, y + row, 0U);
            std::memcpy(dest, src, source.width() * source.channels());
        }
    }
    else if (source.channels() == 3 && channels() == 4)
    {
        // Convert source from RGB to RGBA format.
        blitRGBtoRGBA_SSE3(data(x, y, 0),
                           source.data(0, 0, 0),
                           source.width(),
                           source.height(),
                           width());
    }
    else if (source.channels() == 4 && channels() == 3)
    {
#if 1
        // Convert source from RGBA to RGB format.
        blitRGBAtoRGB_SSE3(data(x, y, 0),
                           source.data(0, 0, 0),
                           source.width(),
                           source.height(),
                           width());
#else
        // Convert source from RGBA to RGB format
        for (size_t row = 0; row < source.height(); ++row)
        {
            unsigned char* src = source.data(0, row, 0);
            unsigned char* dest = data(x, y + row, 0);

            for (size_t col = 0; col < source.width(); ++col)
            {
                std::memcpy(dest, src + col * source.channels(), channels());
                dest += channels();
            }
        }
#endif
    }

    if (releaseAfterUse)
    {
        const_cast<Image&>(source).release();
    }
}

} // namespace txtratl
