#pragma once

#include "vendor/rectpack2d/src/pack.h"
#include "vendor/zupply/zupply.hpp"

#include "imagerect.hpp"

class Atlas
{
    static constexpr int ATLAS_MAX_SIDE = 4096; ///< Maximum side for atlas image in pixels

public:
    /**
    *   @brief  Adds an image file to be packed into the atlas.
    *
    *   @param  filename is a JPEG or PNG file to be added to the atlas
    *   @return true, if image added successfully
    */
    bool addImage(std::string const& filename);

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
    bool createAtlas(std::string const& imageFilename, std::string const& metadataFilename);

private:
    /**
    *  @brief Class that holds the size of the target atlas.
    */
    struct AtlasSize
    {
        static constexpr int INVALID_ATLAS = -1;

        AtlasSize()
            : mWidth(INVALID_ATLAS),
              mHeight(INVALID_ATLAS)
        {
        }

        AtlasSize(int width, int height)
            : mWidth(width),
              mHeight(height)
        {
        }

        bool isValid()
        {
            return (mWidth != INVALID_ATLAS);
        }

        int getWidth() const { return mWidth; }
        int getHeight() const { return mHeight; }

    private:
        int mWidth;
        int mHeight;
    };

    /**
    *   @brief  Blits image rect into a given canvas according to rect's coordinates.
    *
    *   @param  canvas is the destination for image to be blit
    *   @param  ir is the destination for image to be blit
    *   @return true, if file was created successfully
    */
    void blitImage(zz::Image& canvas, ImageRect const& ir) const;

    /**
    *   @brief  Blits image rects into a given canvas according to rects coordinates.
    *
    *   @param  canvas is the destination for image to be blit
    *   @param  ir is the destination for image to be blit
    *   @return true, if file was created successfully
    */
    bool blitImages(zz::Image& canvas) const;

    /**
    *   @brief  Writes atlas metadata into a text file.
    *
    *   @param  outputFilename is the destination TXT file for atlas metadata
    *   @return true, if file was created successfully
    */
    bool writeMetadata(std::string const& outputFilename) const;

    std::vector<ImageRect> mImages; ///< A store for image data
    AtlasSize mAtlasSize;           ///< The dimensions of the atlas
};
