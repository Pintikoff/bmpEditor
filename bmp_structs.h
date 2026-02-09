#ifndef BMP_STRUCTS_H
#define BMP_STRUCTS_H

#include <stdint.h>
#pragma pack(1)
struct HeaderStruct {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};

struct InfoHeaderStruct {
    uint32_t size;            // size of InfoHeader
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitePerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t XpixelsPerM;
    uint32_t YpixelsPerM;
    uint32_t colorsUsed;
    uint32_t importantColors;
};

struct PixelStruct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

typedef struct HeaderStruct Header;
typedef struct InfoHeaderStruct InfoHeader;
typedef struct PixelStruct Pixel;
#pragma pack()

#endif
