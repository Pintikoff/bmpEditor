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
    //creating a new map
    Pixel** newPixelMap = malloc(width*sizeof(Pixel));
    for(int i = 0; i < width; i++){
        newPixelMap[i] = malloc(height*sizeof(Pixel));
    }
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
    //creating newPixelMap
    Pixel** newPixelMap = malloc(width*sizeof(Pixel));
    for(int i = 0; i < width; i++){
        newPixelMap[i] = malloc(height*sizeof(Pixel));
    }
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

void snapImage(InfoHeader *infoHeader, Pixel ***pixelMap, int startX, int startY, int endX, int endY) {
  uint32_t width = infoHeader->width;
  uint32_t height = infoHeader->height;
  startX = width - startX;
  startY = height - startY;
  endX = width - endX;
  endY = height - endY;

  if (endX > startX) {
    int tempX;
    tempX = startX;
    startX = endX;
    endX = tempX;
  }
  if (endY > startY) {
    int tempY;
    tempY = startY;
    startY = endY;
    endY = tempY;
  }

  int newHeight = endY - startY;
  int newWidth = endX - startX;
  //creating newPixelMap
  Pixel **newPixelMap = malloc(newHeight * sizeof(Pixel*));
  for (int i = 0; i < newHeight; i++) {
    newPixelMap[i] = malloc(newWidth * sizeof(Pixel*));
  }
  //copying snaped pixelMap into a newPixelMap
  for (int y = 0; y < newHeight; y++) {
    for (int x = 0; x < newWidth; x++) {
      newPixelMap[y][x] = (*pixelMap)[startY + y][startX + x];
    }
  }
  //free old pixelMap
  for (int i = 0; i < newHeight; i++) {
    free((*pixelMap)[i]);
  }
  free(*pixelMap);

  *pixelMap = newPixelMap;
  infoHeader->height = newHeight;
  infoHeader->width = newWidth;
}

//frame: user can give width and color of a frame

//zooom: x2 x0.5
