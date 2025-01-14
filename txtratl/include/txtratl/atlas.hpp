#pragma once

#include <filesystem>
#include <memory>

namespace txtratl
{

// Forward declarations.
class Image;

class Atlas
{
    /// Maximum side length in pixels for the atlas image.
    static constexpr size_t ATLAS_MAX_SIDE = 24000;

public:
    /// Constructor.
    explicit Atlas();

    /// Default destructor.
    ~Atlas();

    /// Copy constructor.
    Atlas(const Atlas& other);

    /// Assignment operator.
    Atlas& operator=(Atlas rhs);

    /**
     * Adds an image file to be packed into the atlas.
     *
     * @param filepath is a JPEG or PNG file to be added to the atlas.
     *
     * @return true, if the image was added successfully.
     */
    void addImage(const std::filesystem::path& filepath);

    /**
     * Packs the images into the atlas.
     *
     * @return true, if packing was successful.
     */
    bool packImages();

    /**
     * Creates an atlas image and a metadata file with the image locations in atlas.
     *
     * @param imageFilename is the destination image file for the atlas.
     * @param metadataFilename is the destination text file for atlas metadata.
     *
     * @return true, if files were created successfully.
     */
    bool createAtlas(const std::filesystem::path& imageFilepath, const std::filesystem::path& metadataFilepath);

private:
    static constexpr size_t INVALID_ATLAS = 0;

    /**
     * Blits image rects into a canvas according to rect coordinates.
     *
     * @param canvas is the destination for image to be blit.
     * @param ir is the destination for image to be blit.
     */
    void blitImages(Image& canvas) const;

    /**
     * Writes atlas metadata into a text file.
     *
     * @param outputFilename is the destination TXT file for atlas metadata.
     *
     * @return true, if the file was created successfully.
     */
    bool writeMetadata(const std::filesystem::path& outputFilepath) const;

    class ImageRectImpl;

    std::unique_ptr<ImageRectImpl> mImpl;

    size_t mWidth = INVALID_ATLAS;
    size_t mHeight = INVALID_ATLAS;
};

} // namespace txtratl
