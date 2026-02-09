#ifndef MBP_READ_H
#define MBP_READ_H

#include <stdint.h>
#include "bmp_structs.h"
uint8_t* readHeader(char* link, Header* header,  InfoHeader* infoHeader);
Pixel** readPixels(Header *header, InfoHeader *infoHeader, uint8_t *buffer);

#endif