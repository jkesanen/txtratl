#include <filesystem>
#include <stdexcept>
#include <string>

#include "vendor/stb/stb_image.h"
#include "vendor/stb/stb_image_write.h"
#include "image.hpp"

Image::Image(const std::filesystem::path& filepath, bool deferLoading)
{
    if (deferLoading)
    {
        info(filepath);
    }
    else
    {
        load(filepath);
    }
}

Image::Image(size_t width, size_t height, size_t channels)
{
    create(width, height, channels);
}

Image::~Image()
{
    release();
}

void Image::create(size_t width, size_t height, size_t channels)
{
    mWidth = width;
    mHeight = height;
    mChannels = channels;
    mData = std::make_shared<std::vector<uint8_t>>(mWidth * mHeight * mChannels);
}

void Image::info(const std::filesystem::path& filepath)
{
    int width;
    int height;
    int channels;
    int result = stbi_info(filepath.string().c_str(), &width, &height, &channels);

    if (result)
    {
        std::string msg = "Failed to get the information from file '" + filepath.string() + "': ";
        msg += stbi_failure_reason();
        throw std::runtime_error(msg);
    }

    mWidth = width;
    mHeight = height;
    mChannels = channels;
    mFilePath = filepath;
}

void Image::load(const std::filesystem::path& filepath)
{
    int width;
    int height;
    int channels;
    uint8_t* buffer = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);

    if (!buffer)
    {
        std::string msg = "Failed to load file '" + filepath.string() + "': ";
        msg += stbi_failure_reason();
        throw std::runtime_error(msg);
    };

    create(static_cast<size_t>(width), static_cast<size_t>(height), static_cast<size_t>(channels));
    mFilePath = filepath;

    std::memcpy((*mData).data(), buffer, sizeof(uint8_t) * mWidth * mHeight * mChannels);
    stbi_image_free(buffer);
}

void Image::load()
{
    load(mFilePath);
}

uint8_t* Image::data(size_t row, size_t col, size_t channel) const
{
    if (!mData)
    {
        return nullptr;
    }

    size_t offset = row * mWidth * mChannels + col * mChannels + channel;
    return (*mData).data() + offset;
}

size_t Image::channels() const
{
    return mChannels;
}

size_t Image::width() const
{
    return mWidth;
}

size_t Image::height() const
{
    return mHeight;
}

void Image::save(const std::filesystem::path& filepath, size_t quality) const
{
    assert(quality <= 100);
    stbi_write_png(filepath.string().c_str(), mWidth, mHeight, mChannels, (*mData).data(), mWidth * mChannels);
}

void Image::release()
{
    mChannels = 0;
    mHeight = 0;
    mWidth = 0;
    mData = nullptr;
}
