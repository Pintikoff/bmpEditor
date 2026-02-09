#ifndef BMP_OTHER_H
#define BMP_OTHER_H
#include "bmp_structs.h"

void writeNewFile(Header *header, InfoHeader *infoHeader, Pixel** pixelMap);
void printHelp();
void outPutStructTemp(Header* header, InfoHeader* infoHeader);
void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap);

#endif