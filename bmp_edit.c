#include <stdio.h>
#include <stdint.h>
#include "bmp_structs.h"
// vertical mirror top -> bottom
Pixel** mirrorX(Header *header, InfoHeader *infoHeader, Pixel** pixelMap){
    printf("\nTest:\n\n");
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;

    Pixel* tempRow;
    // pixelMap[0] = pixelMap[1];
    // pixelMap[1] = tempRow;

    for(int y = 0; y < height/2; y++){
        tempRow = pixelMap[y];
        pixelMap[y] = pixelMap[(height-1) - y];
        pixelMap[(height-1) - y] = tempRow;
    }
    return pixelMap;
}

//horizontal mirror

//rotate: 90 180 270

//frame: user can give width and color of a frame

//zooom: x2 x0.5