#include <stdio.h>
#include <stdint.h>
#include "bmp_structs.h"

void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("R: %03d G: %03d: B: %03d \n", pixelMap[y][x].r, pixelMap[y][x].g, pixelMap[y][x].b);
        }
    }
}

// vertical mirror top -> bottom
void mirrorX(Header *header, InfoHeader *infoHeader, Pixel** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    Pixel* tempRow;

    for(int y = 0; y < height/2; y++){
        tempRow = pixelMap[y];
        pixelMap[y] = pixelMap[(height-1) - y];
        pixelMap[(height-1) - y] = tempRow;
    }
}

//horizontal mirror
void mirrorY(Header *header, InfoHeader *infoHeader, Pixel** pixelMap){
    printf("\nTest:\n");
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    Pixel tempCol;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width/2; x++){
            tempCol = pixelMap[y][x];
            pixelMap[y][x] = pixelMap[y][(width-1) - x];
            pixelMap[y][(width-1) - x] = tempCol;
        }
    }
}


//rotate: 90 180 270

//frame: user can give width and color of a frame

//zooom: x2 x0.5