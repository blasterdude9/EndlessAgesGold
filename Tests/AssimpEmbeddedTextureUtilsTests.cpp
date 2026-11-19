#include "AssimpEmbeddedTextureUtils.h"

#include <cstdio>

namespace
{
int failures = 0;

void Check(bool passed, const char * description)
{
    if(passed)
        return;

    std::fprintf(stderr, "FAILED: %s\n", description);
    ++failures;
}
}

int main()
{
    AssimpBmpLayout layout = {};

    Check(AssimpComputeBmpLayout(1, 1, &layout), "1x1 texture is representable");
    Check(layout.pixelCount == 1, "1x1 pixel count");
    Check(layout.pixelBytes == 4, "1x1 pixel byte count");
    Check(layout.fileSize == 58, "1x1 BMP file size includes its 54-byte header");
    Check(layout.topDownHeight == -1, "BMP height requests top-down pixel order");

    Check(AssimpComputeBmpLayout(1024, 512, &layout), "ordinary texture dimensions are accepted");
    Check(layout.pixelCount == 524288ULL, "ordinary texture pixel count");
    Check(layout.pixelBytes == 2097152U, "ordinary texture byte count");
    Check(layout.fileSize == 2097206U, "ordinary texture BMP file size");
    Check(layout.topDownHeight == -512, "ordinary texture signed height");

    Check(!AssimpComputeBmpLayout(0, 1, &layout), "zero width is rejected");
    Check(!AssimpComputeBmpLayout(1, 0, &layout), "zero height is rejected");
    Check(!AssimpComputeBmpLayout(1, 1, NULL), "null output is rejected");
    Check(!AssimpComputeBmpLayout(0x80000000U, 1, &layout), "width outside signed BMP range is rejected");
    Check(!AssimpComputeBmpLayout(1, 0x80000000U, &layout), "height outside signed BMP range is rejected");

    // Largest 32-bit BMP payload that is a whole number of RGBA pixels.
    Check(AssimpComputeBmpLayout(1073741810U, 1, &layout), "largest representable pixel payload is accepted");
    Check(layout.fileSize == 0xFFFFFFFEU, "largest representable BMP file size does not wrap");
    Check(!AssimpComputeBmpLayout(1073741811U, 1, &layout), "one pixel beyond the BMP size limit is rejected");
    Check(!AssimpComputeBmpLayout(0xFFFFFFFFU, 0xFFFFFFFFU, &layout), "overflow-sized texture dimensions are rejected");

    if(failures != 0)
        std::fprintf(stderr, "%d embedded-texture utility test(s) failed.\n", failures);
    else
        std::printf("All embedded-texture utility tests passed.\n");

    return failures == 0 ? 0 : 1;
}
