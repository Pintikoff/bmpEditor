#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void readHeader(char* link, Header* header,  InfoHeader* infoHeader) {
    FILE* fp = fopen(link, "rb");
    if (!fp) {
        perror("invalid file path");
        return;
    }
    fseek(fp, 0, SEEK_END); //placing a pointer to an end
    long fileSize = ftell(fp); //telling where is a pointer located
    rewind(fp);

    uint8_t* buffer = malloc(fileSize);
    //reading every byte * fileSize in fp and saving to buffer
    fread(buffer, 1, fileSize, fp);
    //copying buffer values into a header(header is a pointer)
    memcpy(header, buffer, sizeof(Header));
    memcpy(infoHeader, buffer+sizeof(Header), sizeof(InfoHeader));

    //printing file data
    for (int i = 0; i < fileSize; i++) {
        printf("%02X ", buffer[i]);
        if (((i + 1) % 16) == 0)
        printf("\n");
    }
    printf("\n");

    free(buffer);
    fclose(fp);
}

void readPixels(Header* header, InfoHeader* infoHeader) {
  uint8_t* buffer = malloc(header->fileSize);
  Pixel* pixels = malloc(infoHeader->height * infoHeader->width * 3); //1d array
  memcpy(pixels, buffer + header->dataOffset, *buffer - header->dataOffset );
  for (int y = 0; y < infoHeader->height; y++){
    for (int x = 0; x < infoHeader->width; x++) {

    }
  }
}

int main() {
    char* link;
    Header* header = malloc(sizeof(Header));
    InfoHeader* infoHeader = malloc(sizeof(InfoHeader));
    link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; // linux
    //link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows

    readHeader(link, header, infoHeader);
    outPutStructTemp(header, infoHeader);
    readPixels(header, infoHeader);
    return 0;
    //Todo: save Pixel data into a 2d(technically 3d array)
}


