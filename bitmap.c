#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

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

uint8_t* readHeader(char* link, Header* header,  InfoHeader* infoHeader) {
    FILE* fp = fopen(link, "rb");
    if (!fp) {
        perror("invalid file path");
        return NULL;
    }
    fseek(fp, 0, SEEK_END); //placing a pointer to an end
    long fileSize = ftell(fp); //telling where is a pointer located
    rewind(fp);

    uint8_t* buffer = malloc(fileSize);
    //reading every byte * fileSize in fp and saving to buffer
    fread(buffer, 1, fileSize, fp);
    //copying buffer values into a header(header is a pointer)
    memcpy(header, buffer, sizeof(Header));
    memcpy(infoHeader, buffer + sizeof(Header), sizeof(InfoHeader));

    //printing file data
    for (int i = 0; i < fileSize; i++) {
        printf("%02X ", buffer[i]);
        if (((i + 1) % 16) == 0)
        printf("\n");
    }
    printf("\n");

    fclose(fp);
    return buffer;
}

Pixel** readPixels(Header *header, InfoHeader *infoHeader, uint8_t *buffer) {
  uint32_t width = infoHeader->width;
  uint32_t height = infoHeader->height;

    int rowSize = width * sizeof(Pixel);
    int padLength = (4 - (rowSize % 4)) % 4;
    int lineLength = rowSize + padLength;

    int dst = 0;
    uint8_t* src = buffer + header->dataOffset;

    //creating an array of PIXELS without padding and putting values in it
    Pixel *pixels = malloc(rowSize * height);
    for (int y = 0; y < height; y++) {
      memcpy(pixels + (y * width), src, rowSize);
      src += rowSize + padLength;
    }
    //creating 2d array of pixels
    Pixel **pixelMap = malloc(height * sizeof(Pixel*)); //size of datatype "Pixel"
    for (int y = 0; y < height; y++) {
      pixelMap[y] = malloc(width * sizeof(Pixel));
    }
    //filling up 2d array with pixels from 1d array
    int i = 0;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        pixelMap[y][x] = pixels[i];
        printf("R: %03d G: %03d: B: %03d \n", pixelMap[y][x].r, pixelMap[y][x].g, pixelMap[y][x].b);
        i++;
      }
    }
    /*
    for(int i = 0; i < 6; i++){
        printf("Pixel %d ", i+1);
        printf("R: %03d G: %03d: B: %03d \n", pixels[i].r, pixels[i].g,
    pixels[i].b);
    }
   */

    // free pixels
    // free pixelMap
    return pixelMap;
}

int main() {
    char* link;
    link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; // linux
    //link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows

    Header* header = malloc(sizeof(Header));
    InfoHeader* infoHeader = malloc(sizeof(InfoHeader));

    uint8_t* buffer = readHeader(link, header, infoHeader);
    outPutStructTemp(header, infoHeader);
    Pixel** pixelMap = readPixels(header, infoHeader, buffer);
    free(buffer);
    return 0;
}
