#pragma once

#include <filesystem>
#include <memory>
#include <vector>

namespace txtratl
{

class Image
{
public:
    explicit Image(const std::filesystem::path& filepath, bool deferLoading = false);
    explicit Image(size_t width, size_t height, size_t channels);

    void allocate(size_t width, size_t height, size_t channels);
    void release();

    void loadinfo(const std::filesystem::path& filepath);

    void load();
    void save(const std::filesystem::path& filename) const;

    uint8_t* data(size_t x, size_t y, size_t channel) const;

    size_t channels() const;
    size_t height() const;
    size_t width() const;

    void blitImage(const Image& source, size_t x, size_t y);

private:
    void load(const std::filesystem::path& filepath);

    std::shared_ptr<std::vector<uint8_t>> mData = nullptr;
    std::filesystem::path mFilePath{};

    size_t mWidth = 0;
    size_t mHeight = 0;
    size_t mChannels = 0;
};

} // namespace txtratl
