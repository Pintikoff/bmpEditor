#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_structs.h"
#include "bmp_read.h"
#include "bmp_edit.h"

Pixel createPixelBGR(uint8_t r, uint8_t g, uint8_t b) {
    Pixel px = {r, g, b};
    return px;
}

void outPutStructTemp(Header* header, InfoHeader* infoHeader){
    printf("Header:\n");
    printf("  signature: 0x%04X\n", header->signature);
    printf("  fileSize: %u bytes\n", header->fileSize);
    printf("  reserved: %u\n", header->reserved);
    printf("  dataOffset: %u\n", header->dataOffset);

    printf("\nInfoHeader:\n");
    printf("  size: %u bytes\n", infoHeader->size);
    printf("  width: %u pixels\n", infoHeader->width);
    printf("  height: %u pixels\n", infoHeader->height);
    printf("  planes: %u\n", infoHeader->planes);
    printf("  bitsPerPixel: %u\n", infoHeader->bitePerPixel);
    printf("  compression: %u\n", infoHeader->compression);
    printf("  imageSize: %u bytes\n", infoHeader->imageSize);
    printf("  XpixelsPerM: %u\n", infoHeader->XpixelsPerM);
    printf("  YpixelsPerM: %u\n", infoHeader->YpixelsPerM);
    printf("  colorsUsed: %u\n", infoHeader->colorsUsed);
    printf("  importantColors: %u\n", infoHeader->importantColors);
}

int main() {
    char* link;
    //link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; // linux
    link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows

    Header* header = malloc(sizeof(Header));
    InfoHeader *infoHeader = malloc(sizeof(InfoHeader));

    uint8_t* buffer = readHeader(link, header, infoHeader);

    Pixel **pixelMap = readPixels(header, infoHeader, buffer);
    #pragma pack(pop)

    // mirrorX(header, infoHeader, pixelMap);
    // mirrorY(infoHeader, pixelMap);
    // rotate90(infoHeader, &pixelMap);
    // rotate180(infoHeader, pixelMap);
    // rotate270(infoHeader,&pixelMap);
    // snapImage(infoHeader, &pixelMap, 1, 1, 3, 2)
    outPutStructTemp(header, infoHeader);
    outputPixels(infoHeader, pixelMap);

    freePixelMap(infoHeader, pixelMap);
    free(buffer);
    return 0;
}
