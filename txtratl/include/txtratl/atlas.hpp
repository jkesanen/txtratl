#pragma once

#include <filesystem>

#include "txtratl/image.hpp"
#include "txtratl/imagerect.hpp"

namespace txtratl
{

class Atlas
{
    static constexpr size_t ATLAS_MAX_SIDE = 24000; ///< Maximum side for atlas image in pixels

public:
    /**
     *   @brief  Adds an image file to be packed into the atlas.
     *
     *   @param  filepath is a JPEG or PNG file to be added to the atlas.
     *   @return true, if the image was added successfully.
     */
    bool addImage(const std::filesystem::path& filepath);

    /**
     *   @brief  Packs the added images into the atlas.
     *
     *   @return true, if packing was successfull
     */
    bool packImages();

    /**
     *   @brief  Creates an atlas image and a metadata file with image locations in atlas.
     *
     *   @param  imageFilename is the destination JPEG file for the atlas
     *   @param  metadataFilename is the destination TXT file for atlas metadata
     *   @return true, if files were created successfully
     */
    bool createAtlas(const std::filesystem::path& imageFilepath, const std::filesystem::path& metadataFilepath);

private:
    static constexpr size_t INVALID_ATLAS = 0;

    /**
     *   @brief  Blits image rects into a given canvas according to rects coordinates.
     *
     *   @param  canvas is the destination for image to be blit
     *   @param  ir is the destination for image to be blit
     *   @return true, if file was created successfully
     */
    bool blitImages(Image& canvas) const;

    /**
     *   @brief  Writes atlas metadata into a text file.
     *
     *   @param  outputFilename is the destination TXT file for atlas metadata
     *   @return true, if file was created successfully
     */
    bool writeMetadata(const std::filesystem::path& outputFilepath) const;

    bool isValid() const
    {
        return (mWidth != INVALID_ATLAS || mHeight != INVALID_ATLAS);
    }

    std::vector<ImageRect> mImages; ///< A store for image data

    size_t mWidth = INVALID_ATLAS;
    size_t mHeight = INVALID_ATLAS;
};

} // namespace txtratl
