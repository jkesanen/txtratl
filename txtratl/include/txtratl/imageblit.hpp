#pragma once

#include <cstddef>
#include <cstdint>

namespace txtratl
{

/**
 * Blits RGBA image (32-bit) into RGB image (24-bit) utilizing SSE3 intrinsics.
 *
 * @param dest A pointer to destination image.
 * @param src A pointer to source image.
 * @param height The height of the source image.
 * @param width The width of the source image.
 * @param destCols The width of the destination image.
 */
void blitRGBAtoRGB_SSE3(uint8_t* dest, const uint8_t* src, const size_t width, const size_t height, const size_t destCols);

/**
 * Blits RGB image (24-bit) into RGBA image (32-bit) utilizing SSE3 intrinsics.
 *
 * @param dest A pointer to destination image.
 * @param src A pointer to source image.
 * @param height The height of the source image.
 * @param width The width of the source image.
 * @param destCols Ihe width of the destination image.
 */
void blitRGBtoRGBA_SSE3(uint8_t* dest, const uint8_t* src, const size_t width, const size_t height, const size_t destCols);

}; // namespace txtratl
