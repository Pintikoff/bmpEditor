#include "bmp_structs.h"

#ifndef BMP_EDIT_H
#define MBP_EDIT_H

void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap);
void mirrorX(InfoHeader *infoHeader, Pixel** pixelMap);
void mirrorY(InfoHeader *infoHeader, Pixel** pixelMap);

void rotate90(InfoHeader *infoHeader, Pixel*** pixelMap);
void rotate180(InfoHeader *infoHeader, Pixel** pixelMap);
void rotate270(InfoHeader *infoHeader, Pixel ***pixelMap);
void snapImage(InfoHeader *infoHeader, Pixel ***pixelMap, int startX, int startY, int endX, int endY);
#endif 
