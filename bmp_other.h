#ifndef BMP_OTHER_H
#define BMP_OTHER_H
#include "bmp_structs.h"

void cleanMem(Header* header, InfoHeader* infoHeader, uint8_t* buffer, Pixel** pixelMap);
void writeNewFile(Header *header, InfoHeader *infoHeader, Pixel** pixelMap);
void printHelp();
void outPutStructTemp(Header* header, InfoHeader* infoHeader);
void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap);
int checkFileType(Header* header, InfoHeader* infoHeader);
#endif