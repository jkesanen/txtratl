#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <cstring>

#include "imageblit.hpp"

namespace imageblit
{

void blitRGBAtoRGB_SSE3(uint8_t* const dest, const uint8_t* src, const size_t rows, const size_t cols, const size_t destStride)
{
    // A mask for skipping the every fourth byte out of 16 bytes.
    const int8_t maskBytes[16] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, -1, -1, -1, -1};
    __m128i mask = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(&maskBytes));

    size_t endPixels = cols % 4;
    if (!endPixels)
    {
        endPixels = 4;
    }

    size_t row = 0;
    for (uint8_t* destRow = dest; row < rows; ++row, destRow = (dest + row * destStride))
    {
        // Copy bytes in 16 byte chunks, skip the last 4-16 bytes.
        const uint8_t* end = src + (cols - endPixels) * 4;
        for (; src < end; src += 16, destRow += 12)
        {
            _mm_storeu_si128(reinterpret_cast<__m128i*>(destRow), _mm_shuffle_epi8(_mm_lddqu_si128(reinterpret_cast<const __m128i*>(src)), mask));
        }

        // Copy the remaining bytes manually to avoid:
        //     1) out-of-bounds access if there's less than 16 bytes available,
        //     2) adding an extra pixel to the dest (16 byte stores and max 12 bytes of data).
        for (size_t i = 0; i < endPixels; src += 4, destRow += 3, i++)
        {
            std::memcpy(destRow, src, 3);
        }
    }
}

} // namespace imageblit
