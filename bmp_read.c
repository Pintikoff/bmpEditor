#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_structs.h"

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

    fclose(fp);
    return buffer;
}

Pixel** readPixels(Header *header, InfoHeader *infoHeader, uint8_t *buffer) {
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;

    int rowSize = width * sizeof(Pixel);
    int padLength = (4 - (rowSize % 4)) % 4;
    int lineLength = rowSize + padLength;
    
    uint8_t* src = buffer + header->dataOffset;

    //creating an array of PIXELS without padding and putting values in it
    Pixel* pixels = malloc(rowSize * height);
    for (int y = 0; y < height; y++) {
        memcpy(pixels + (y * width), src, rowSize);
        src += lineLength;
    }

    int i = 0;
    //creating 2d array of pixels
    Pixel **pixelMap = malloc(height * sizeof(Pixel*)); //size of datatype "Pixel"
    //filling up 2d array with pixels from 1d array
    for (int y = 0; y < height; y++) {
        pixelMap[y] = malloc(width * sizeof(Pixel));
        for (int x = 0; x < width; x++) {
            pixelMap[y][x] = pixels[i];
            i++;
        }
    }

    free(pixels);
    return pixelMap;
}
