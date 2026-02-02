#include "bmp_structs.h"

#ifndef BMP_EDIT_H
#define MBP_EDIT_H

void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap);
void mirrorX(Header *header, InfoHeader *infoHeader, Pixel** pixelMap);
void mirrorY(Header *header, InfoHeader *infoHeader, Pixel** pixelMap);

#endif