#include <cstring>
#include <filesystem>
#include <stdexcept>

#include "txtratl/image.hpp"

// Including stb_image and stb_image_write generates a plenty of compiler warnings.
// Ignore those warnings when including these headers to allow project to have high level of warnings enabled. 
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4365 4820 4996 5045 5219 5262)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

namespace txtratl
{

void Image::loadinfo(const std::filesystem::path& filepath)
{
    auto width = 0;
    auto height = 0;
    auto channels = 0;

    auto result = stbi_info(filepath.string().c_str(), &width, &height, &channels);

    if (result != 1)
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

void Image::save(const std::filesystem::path& filepath) const
{
    if (!stbi_write_png(filepath.string().c_str(), static_cast<int>(mWidth), static_cast<int>(mHeight), static_cast<int>(mChannels), (*mData).data(), static_cast<int>(mWidth * mChannels)))
    {

    }
}

} // namespace txtratl
