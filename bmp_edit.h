#ifndef BMP_EDIT_H
#define MBP_EDIT_H
#include "bmp_structs.h"

Pixel** allocate2DPixelArray(int rows, int cols);

void mirrorX(InfoHeader *infoHeader, Pixel** pixelMap);
void mirrorY(InfoHeader *infoHeader, Pixel** pixelMap);

void rotate90(InfoHeader *infoHeader, Pixel*** pixelMap);
void rotate180(InfoHeader *infoHeader, Pixel** pixelMap);
void rotate270(InfoHeader *infoHeader, Pixel ***pixelMap);

void snapImage(InfoHeader *infoHeader, Pixel ***pixelMap, int startX, int startY, int endX, int endY);
void addFrame(InfoHeader *infoHeader, Pixel ***pixelMap, uint32_t frameWidth, uint8_t frameR, uint8_t frameG, uint8_t frameB);
void changeTint(InfoHeader* infoHeader, Pixel **pixelMap, char* tintColorString, uint8_t tintValue);
void zoomX2(InfoHeader* infoHeader, Pixel ***pixelMap);
#endif 
