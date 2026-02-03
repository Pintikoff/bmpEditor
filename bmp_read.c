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

    free(pixels);
    return pixelMap;
}

void freePixelMap(InfoHeader* infoHeader,Pixel** pixelMap){
    int height = infoHeader->height;
    for(int y = 0; y < height; y++ ){
        free(pixelMap[y]);
    }
    free(pixelMap);
}