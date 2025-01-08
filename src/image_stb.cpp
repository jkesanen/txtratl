#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <string>

#include "image.hpp"
#include "vendor/stb/stb_image.h"
#include "vendor/stb/stb_image_write.h"

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

void Image::loadinfo(const std::filesystem::path& filepath)
{
    auto width = 0;
    auto height = 0;
    auto channels = 0;

    auto result = stbi_info(filepath.string().c_str(), &width, &height, &channels);

    if (result)
    {
        std::string msg = "Failed to get the information from file '" + filepath.string() + "': ";
        msg += stbi_failure_reason();
        throw std::runtime_error(msg);
    }

    mWidth = static_cast<size_t>(width);
    mHeight = static_cast<size_t>(height);
    mChannels = static_cast<size_t>(channels);
    mFilePath = filepath;
}

void Image::load(const std::filesystem::path& filepath)
{
    auto width = 0;
    auto height = 0;
    auto channels = 0;

    uint8_t* buffer = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);

    if (!buffer)
    {
        std::string msg = "Failed to load file '" + filepath.string() + "': ";
        msg += stbi_failure_reason();
        throw std::runtime_error(msg);
    };

    allocate(static_cast<size_t>(width), static_cast<size_t>(height), static_cast<size_t>(channels));
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

size_t Image::height() const
{
    return mHeight;
}

size_t Image::width() const
{
    return mWidth;
}

void Image::save(const std::filesystem::path& filepath) const
{
    stbi_write_png(filepath.string().c_str(), mWidth, mHeight, mChannels, (*mData).data(), mWidth * mChannels);
}

void Image::release()
{
    mData = nullptr;
    mChannels = 0;
    mHeight = 0;
    mWidth = 0;
}
