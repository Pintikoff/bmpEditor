#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_structs.h"

void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    int actY = height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("Row: %d Column: %d ", actY, x+1);
            printf("R: %03d G: %03d: B: %03d \n", pixelMap[y][x].r, pixelMap[y][x].g, pixelMap[y][x].b);
        }
        actY--;
    }
}

Pixel** allocate2DPixelArray(int rows, int cols) {
    Pixel **array = malloc(rows * sizeof(Pixel*));
    if (array == NULL) {
        perror("malloc failed for rows");
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(Pixel));
        if (array[i] == NULL) {
            perror("malloc failed for columns");
            // Освободить уже выделенное
            for (int j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }
    
    return array;
}

// vertical mirror top -> bottom
void mirrorX(InfoHeader *infoHeader, Pixel** pixelMap){
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
void mirrorY(InfoHeader *infoHeader, Pixel** pixelMap){
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

void rotate90(InfoHeader *infoHeader, Pixel*** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;

    Pixel** newPixelMap = allocate2DPixelArray(width, height);
    //copying rotated pixelMap into a newPixelMap
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            newPixelMap[width - 1 -x][y] = (*pixelMap)[y][x];
        }
    }
    //free old pixelMap
    for(int i = 0; i < height; i++){
        free((*pixelMap)[i]);
    }
    free(*pixelMap);

    *pixelMap = newPixelMap;
    infoHeader->width = height;
    infoHeader->height = width;
}

void rotate180(InfoHeader *infoHeader, Pixel** pixelMap){
    mirrorX(infoHeader, pixelMap);
    mirrorY(infoHeader, pixelMap);
}

void rotate270(InfoHeader *infoHeader, Pixel*** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;

    Pixel** newPixelMap = allocate2DPixelArray(width, height);
    //copying rotated pixelMap into a newPixelMap
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            newPixelMap[x][height - 1 - y] = (*pixelMap)[y][x];
        }
    }
    //free old pixelMap
    for(int i = 0; i < height; i++){
        free((*pixelMap)[i]);
    }
    free(*pixelMap);

    *pixelMap = newPixelMap;
    infoHeader->width = height;
    infoHeader->height = width;
}

void snapImage(InfoHeader *infoHeader, Pixel ***pixelMap, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY) {
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    //errors
    if(startX < 0 || startY < 0 || endX < 0 || endY < 0){
        printf("ERROR: Pixels coordinates are less then 0\n");
        return;
    }
    if(startX > width || startY > height || endX > width || endY > height){
        printf("ERROR: Pixels coordinates are bigger then original coordinates\n");
        return;
    }

    //rotating
    startY = height - startY - 1;
    endY = height - endY - 1;
    //swap
    if (endX < startX) {
        int tempX;
        tempX = startX;
        startX = endX;
        endX = tempX;
    }
    if (endY < startY) {
        int tempY;
        tempY = startY;
        startY = endY;
        endY = tempY;
    }
    //determine new size
    uint32_t newHeight = endY - startY + 1;
    uint32_t newWidth = endX - startX + 1;

    Pixel** newPixelMap = allocate2DPixelArray(newWidth, newHeight);
    //copy snaped pixelMap into a newPixelMap
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            newPixelMap[y][x] = (*pixelMap)[startY + y][startX + x];
        }
    }

    //free old pixelMap
    for (int i = 0; i < height; i++) {
        free((*pixelMap)[i]);
    }
    free(*pixelMap);

    *pixelMap = newPixelMap;
    infoHeader->height = newHeight;
    infoHeader->width = newWidth;
}

void addFrame(InfoHeader *infoHeader, Pixel ***pixelMap, uint32_t frameWidth, uint8_t frameR, uint8_t frameG, uint8_t frameB){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    //errors
    if(frameWidth <= 0){
        printf("frame width shuould be bigger then 0");
        return;
    }
    //determine new size
    uint32_t newWidth = width + (frameWidth * 2);
    uint32_t newHeight = height + (frameWidth * 2);
    printf("Size determined\n");
    Pixel** newPixelMap = allocate2DPixelArray(newHeight, newWidth);
    printf("mem allocated\n");
    for(int y = 0; y < newHeight; y++){
        if(y < frameWidth || y >= frameWidth + height){
            for(int x = 0; x < newWidth; x++){
                newPixelMap[y][x].r = frameR;
                newPixelMap[y][x].g = frameG;
                newPixelMap[y][x].b = frameB;
            }
        }else{
            for(int x = 0; x < frameWidth; x++){
                newPixelMap[y][x].r = frameR;
                newPixelMap[y][x].g = frameG;
                newPixelMap[y][x].b = frameB;
            }   
            for(int x = frameWidth; x < width + frameWidth; x++){
                newPixelMap[y][x] = (*pixelMap)[y-frameWidth][x-frameWidth];
            }
            for(int x = width + frameWidth; x < newWidth; x++){
                newPixelMap[y][x].r = frameR;
                newPixelMap[y][x].g = frameG;
                newPixelMap[y][x].b = frameB;
            }
        }
    }
    printf("2d array filled");
    for (int i = 0; i < height; i++) {
        free((*pixelMap)[i]);
    }
    free(*pixelMap);

    *pixelMap = newPixelMap;
    infoHeader->height = newHeight;
    infoHeader->width = newWidth;
}
//frame: user can give width and color of a frame

//zooom: x2 x0.5
