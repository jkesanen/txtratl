#pragma once

#include <cstddef>
#include <cstdint>

namespace txtratl
{

/**
*   @brief  Blits RGBA image (32-bit) into RGB image (24-bit) utilizing SSE3 intrinsics
*
*   @param  dest is a pointer to destination image
*   @param  src is a pointer to source image
*   @param  rows is the height of the source image
*   @param  cols is the width of the source image
*   @param  destCols is the width of the destination image
*/
void blitRGBAtoRGB_SSE3(uint8_t* dest, const uint8_t* src, const size_t rows, const size_t cols, const size_t destCols);

/**
*   @brief  Blits RGB image (24-bit) into RGBA image (32-bit) utilizing SSE3 intrinsics
*
*   @param  dest is a pointer to destination image
*   @param  src is a pointer to source image
*   @param  rows is the height of the source image
*   @param  cols is the width of the source image
*   @param  destCols is the width of the destination image
*/
void blitRGBtoRGBA_SSE3(uint8_t* dest, const uint8_t* src, const size_t rows, const size_t cols, const size_t destCols);

}; // namespace txtratl
