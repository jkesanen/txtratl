#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <cstring>

#include "txtratl/imageblit.hpp"

namespace txtratl
{

void blitRGBAtoRGB_SSE3(uint8_t* dest, const uint8_t* src, const size_t rows, const size_t cols, const size_t destCols)
{
    // A mask for skipping the every fourth byte out of 16 bytes.
    const int8_t maskBytes[16] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, -1, -1, -1, -1};
    __m128i mask = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(&maskBytes));

    size_t endPixels = cols % 4;
    if (!endPixels)
    {
        endPixels = 4;
    }

    for (size_t row = 0; row < rows; ++row)
    {
        uint8_t* destRow = dest + row * destCols* 3;

        // Copy bytes in 16 byte (4 pixel) chunks, skip the last 4-16 bytes.
        const uint8_t* end = src + (cols - endPixels) * 4;
        for (; src < end; src += 16, destRow += 12)
        {
            _mm_storeu_si128(reinterpret_cast<__m128i*>(destRow), _mm_shuffle_epi8(_mm_lddqu_si128(reinterpret_cast<const __m128i*>(src)), mask));
        }

        // Copy the remaining bytes manually to avoid:
        //     1) out-of-bounds access if there's less than 16 bytes available,
        //     2) adding an extra pixel to the dest (16 byte stores and max 12 bytes of data).
        for (size_t i = 0; i < endPixels; src += 4, destRow += 3, ++i)
        {
            std::memcpy(destRow, src, 3);
        }
    }
}

void blitRGBtoRGBA_SSE3(uint8_t* dest, const uint8_t* src, const size_t rows, const size_t cols, const size_t destCols)
{
    // Mask for shuffling 24-bit RGB bytes into 32-bit RGBA sets.
    const int8_t maskBytes[16] = {0, 1, 2, -1, 3, 4, 5, -1, 6, 7, 8, -1, 9, 10, 11, -1};
    __m128i mask = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(&maskBytes));

    // Mask for setting alpha bytes to 255 with _mm_or_si128.
    const int8_t orMaskBytes[16] = {0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1};
    __m128i orMask = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(&orMaskBytes));

    // Number of pixels at the end of every row that won't fill a 128-bit XMM register
    const size_t endPixels = cols % 4;

    for (size_t row = 0; row < rows; ++row)
    {
        uint8_t* destRow = (dest + row * destCols * 4);

        // Copy bytes in 12 byte (4 pixel) chunks
        const uint8_t* end = src + (cols - endPixels) * 3;
        for (; src < end; src += 12, destRow += 16)
        {
            __m128i bytes = _mm_shuffle_epi8(_mm_lddqu_si128(reinterpret_cast<const __m128i*>(src)), mask);
            bytes = _mm_or_si128(bytes, orMask);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(destRow), bytes);
        }

        // Copy the remaining bytes of the row manually if there are less than 4 pixels left
        for (size_t i = 0; i < endPixels; src += 3, destRow += 4, ++i)
        {
            uint32_t* d = reinterpret_cast<uint32_t*>(destRow);
            // Little endian, but doesn't matter as this function is for SSE3 :)
            *d = static_cast<uint32_t>((0xff << 24) | (*(src + 2) << 16) | (*(src + 1) << 8) | *(src));
        }
    }
}

} // namespace txtratl
