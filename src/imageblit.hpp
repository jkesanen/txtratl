#pragma once

#include <cstddef>
#include <cstdint>

namespace imageblit
{

/**
    *   @brief  Blits RGBA image into RGB image utilizing SSE3 intrinsics
    *
    *   @param  dest is a pointer to destination image
    *   @param  src is a pointer to source image
    *   @param  rows is the height of the source image
    *   @param  cols is the width of the source image
    *   @param  destStride is the number of bytes one row of the destination image has
    */
void blitRGBAtoRGB_SSE3(uint8_t* const dest, const uint8_t* src, size_t rows, const size_t cols, const size_t destStride);

}; // namespace imageblit
