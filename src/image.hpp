#pragma once

#include <filesystem>
#include <memory>
#include <vector>s

class Image
{
public:
    explicit Image(const std::filesystem::path& filepath, bool deferLoading = false);

    explicit Image(size_t rows, size_t cols, size_t channels);

    ~Image();

    void create(size_t width, size_t height, size_t channels);

    void info(const std::filesystem::path& filepath);

    void load();
    void load(const std::filesystem::path& filepath);

    uint8_t* data(size_t row, size_t col, size_t channel) const;

    size_t channels() const;

    size_t width() const;

    size_t height() const;

    void save(const std::filesystem::path& filename, size_t quality) const;

    void release();

private:
    std::shared_ptr<std::vector<uint8_t>> mData = nullptr;
    std::filesystem::path mFilePath{};
    size_t mWidth = 0;
    size_t mHeight = 0;
    size_t mChannels = 0;
};
