#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_structs.h"

Pixel** allocate2DPixelArray(int height, int width) {
    Pixel **array = malloc(height * sizeof(Pixel*));
    if (array == NULL) {
        perror("malloc failed for height");
        return NULL;
    }
    
    for (int i = 0; i < height; i++) {
        array[i] = malloc(width * sizeof(Pixel));
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
    if((int)startX < 0 || (int)startY < 0 || (int)endX < 0 || (int)endY < 0){
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

    Pixel** newPixelMap = allocate2DPixelArray(newHeight, newWidth);
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
    //errors
    if((int)frameWidth <= 0){
        printf("ERROR: invalid frame width value (min - 0)");
        return;
    }
    if((int)frameR > 255 || (int)frameG > 255 || (int)frameB > 255){
        printf("ERROR: invalid RGB value (max - 255)");
        return;
    }
    if((int)frameR < 0 || (int)frameG < 0 || (int)frameB < 0){
        printf("ERROR: invalid RGB value (min - 0)");
        return;
    }
    
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
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

void changeTint(InfoHeader* infoHeader, Pixel **pixelMap, char tintColorString, int16_t tintValue){
    char tintColor = tintColorString;
    if(tintColor != 'b' && tintColor != 'g' && tintColor != 'r'){
        printf("ERROR: Invalid tint color value (b, g or r)");
        return;
    } 
    if((int)tintValue > 255) tintValue = 255;
    if((int)tintValue < -255) tintValue = -255;
    
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            switch(tintColor){
                int newValue;
                case 'b':
                    newValue = pixelMap[y][x].b + tintValue;
                    if(newValue > 255) newValue = 255;
                    if(newValue < 0) newValue = 0;
                    pixelMap[y][x].b = (uint8_t)newValue;
                    break;
                case 'g':
                    newValue = pixelMap[y][x].g + tintValue;
                    if(newValue > 255) newValue = 255;
                    if(newValue < 0) newValue = 0;
                    pixelMap[y][x].g = (uint8_t)newValue;
                    break;
                case 'r':
                    newValue = pixelMap[y][x].r + tintValue;
                    if(newValue > 255) newValue = 255;
                    if(newValue < 0) newValue = 0;
                    pixelMap[y][x].r = (uint8_t)newValue;
                    break;
            }   
        }
    }
}

void zoom(InfoHeader* infoHeader, Pixel ***pixelMap, uint32_t zoomValue){    
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    if((int)zoomValue < 0){
        printf("ERROR: Invalid zoom multiplier (min - 1)");
        return;
    }
    if(zoomValue == 0){
        zoomValue = 2;
    }
    
    uint32_t newWidth = width * zoomValue;
    uint32_t newHeight = height * zoomValue;
    if(newHeight > UINT32_MAX / 2|| newWidth > UINT32_MAX / 2){
        printf("ERROR: Resolution is too high");
        return;
    }
    
    Pixel** newPixelMap = allocate2DPixelArray(newHeight, newWidth);
    if(newPixelMap == NULL){
        printf("ERROR: Failed to allocate memory for zoom\n");
        return;
    }
    //fill newPixelMap
    for(int y = 0; y < newHeight; y++){
        /*
        * coords in source image
        * Example:
        *    y = 2, zoomValue = 3,
        *    src = y/zoomValue -> 2/3 = 0.66 = 0
        */
        uint32_t srcY = y/zoomValue;
        for(int x = 0; x < newWidth; x++){
            uint32_t srcX = x/zoomValue;
            newPixelMap[y][x] = (*pixelMap)[srcY][srcX];
        }
    }

    for(int i = 0; i < height; i++){
        free((*pixelMap)[i]);
    }
    free(*pixelMap);

    *pixelMap = newPixelMap;
    infoHeader->height = newHeight;
    infoHeader->width = newWidth;
}

void shrink(InfoHeader* infoHeader, Pixel ***pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    uint32_t newWidth = (width + (width % 2)) / 2;
    uint32_t newHeight = (height + (height % 2)) / 2; 
    printf("SHRINK: newWidth %d\n", newWidth);
    printf("SHRINK: newHeight %d\n", newHeight);
    
    Pixel** newPixelMap = allocate2DPixelArray(newHeight, newWidth);
    if(newPixelMap == NULL){
        printf("ERROR: Failed to allocate memory for zoom\n");
        return;
    }
    printf("SHRINK: newPixelMap created");
    /*
    ?In this loop:
    *Find coords in sourche image ([0][0] = [0] [0]; [1][0] = [2][0])
    *Devide source map into 2x2 grids
    *Count average color values in a gri
    *Fill a newPixelMap
    */
    for(int newY = 0; newY < newHeight; newY++){
        for(int newX = 0; newX < newWidth; newX++){
            //coords in source image
            uint32_t srcY = newY * 2;
            uint32_t srcX = newX * 2;
            int sumB = 0, sumG = 0, sumR = 0;
            int count = 0; //count of valid pixels
            //count avg color in a 2x2 grid
            for(int gridY = 0; gridY < 2; gridY++){
                for(int gridX = 0; gridX < 2; gridX++){
                    int y = srcY + gridY;
                    int x = srcX + gridX;
                    /*
                    * Boundary check for source pixel coordinates
                    * Prevents reading beyond original image dimensions
                    * 
                    * Example: 3×2 original → 6×4 zoomed (2x)
                    ?   newPixel[1][5] maps to srcPixel[0][2] TRUE
                    ?   newPixel[3][5] maps to srcPixel[1][2] TRUE
                    !   newPixel[4][0] maps to srcPixel[2][0] FALSE (2 >= height=2, skip)
                    */
                    // Skip if source coordinates exceed original image bounds
                    if(y < height && x < width){
                        sumB += (*pixelMap)[y][x].b;
                        sumG += (*pixelMap)[y][x].g;
                        sumR += (*pixelMap)[y][x].r;
                        count++;
                    }
                }
            } 
            newPixelMap[newY][newX].b = sumB / count;
            newPixelMap[newY][newX].g = sumG / count;
            newPixelMap[newY][newX].r = sumR / count;
        }
    }
    printf("newPixelMap filled");
    for(int i = 0; i < height; i++){
        free((*pixelMap)[i]);
    }
    printf("SHRINK: newPixelMap filled");
    free(*pixelMap);
    printf("SHRINK: free pixelMap");
    *pixelMap = newPixelMap;
    infoHeader->height = newHeight;
    infoHeader->width = newWidth;
}
