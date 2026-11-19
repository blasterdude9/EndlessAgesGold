#ifndef ASSIMP_EMBEDDED_TEXTURE_UTILS_H
#define ASSIMP_EMBEDDED_TEXTURE_UTILS_H

#include <cstdint>

// Layout fields needed to write an uncompressed 32-bit BMP. The engine uses
// this before allocating or writing embedded Assimp textures so malformed or
// unrepresentably large dimensions cannot wrap a BMP header field.
struct AssimpBmpLayout
{
    std::uint64_t pixelCount;
    std::uint32_t pixelBytes;
    std::uint32_t fileSize;
    std::int32_t  topDownHeight;
};

inline bool AssimpComputeBmpLayout(std::uint32_t width,
                                   std::uint32_t height,
                                   AssimpBmpLayout * layout)
{
    if(!layout || width == 0 || height == 0)
        return false;

    // BMP stores width and height as signed 32-bit values. A negative height
    // is used for top-down pixel order, so INT32_MIN cannot be negated.
    if(width > 0x7FFFFFFFU || height > 0x7FFFFFFFU)
        return false;

    const std::uint64_t pixelCount =
        static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height);
    const std::uint64_t maxPixelBytes = 0xFFFFFFFFULL - 54ULL;
    if(pixelCount > maxPixelBytes / 4ULL)
        return false;

    const std::uint32_t pixelBytes = static_cast<std::uint32_t>(pixelCount * 4ULL);
    layout->pixelCount = pixelCount;
    layout->pixelBytes = pixelBytes;
    layout->fileSize = static_cast<std::uint32_t>(54ULL + pixelBytes);
    layout->topDownHeight = -static_cast<std::int32_t>(height);
    return true;
}

#endif
