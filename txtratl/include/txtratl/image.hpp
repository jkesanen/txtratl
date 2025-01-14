#pragma once

#include <filesystem>
#include <memory>
#include <vector>

namespace txtratl
{

class Image
{
public:
    /**
     * Constructs an image object from a file.
     *
     * @param filepath The path to the image file to be loaded.
     * @param deferLoading Does not load the image data and allocate memory for data, if true.
     *
     * @throws std::runtime_exception if the file access fails or the file in not an image file.
     */
    explicit Image(const std::filesystem::path& filepath, bool deferLoading = false);

    /**
     * Constructs an image object and allocates the memory with the specified dimensions.
     *
     * @param width The width of the image.
     * @param height The height of the image.
     * @param channels The number of channels the image has (3 = RGB, 4 = RGBA).
     *
     * @throws std::runtime_exception if the file access fails or the file in not an image file.
     */
    explicit Image(size_t width, size_t height, size_t channels);

    /**
     * Releases the image data owned by the object which can be reloaded.
     */
    void release();

    /**
     * Loads the image file and allocates memory for the image data.
     *
     * @throws std::runtime_exception if the file access fails or the file in not an image file.
     */
    void load();

    /**
     * Saves the image date to a PNG file.
     *
     * @throws std::runtime_exception if the file access fails or the file in not an image file.
     */
    void save(const std::filesystem::path& filename) const;

    uint8_t* data(size_t x, size_t y, size_t channel) const;

    size_t channels() const;
    size_t height() const;
    size_t width() const;

    void blitImage(const Image& source, size_t x, size_t y, bool releaseAfterUse = false);

private:
    /**
     * Loads the image file including the image data.
     *
     * @param filepath is the path to the image file to be loaded.
     *
     * @throws std::runtime_exception if the file access fails or the file in not an image file.
     */
    void load(const std::filesystem::path& filepath);

    /**
     * Loads the information from the image file without loading the image data.
     *
     * @param filepath is the path to the image file to be loaded.
     *
     * @throws std::runtime_exception if the file access fails or the file in not an image file.
     */
    void loadinfo(const std::filesystem::path& filepath);

    void allocate(size_t width, size_t height, size_t channels);

    std::shared_ptr<std::vector<uint8_t>> mData = nullptr;
    std::filesystem::path mFilePath{};

    size_t mWidth = 0;
    size_t mHeight = 0;
    size_t mChannels = 0;
};

} // namespace txtratl
