#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_structs.h"
#include "bmp_read.h"
#include "bmp_edit.h"
#include "bmp_other.h"

int main(int argc, char *argv[]) {
    char link[512];
    printf("Enter a link to a bmp file: \n");
    scanf("%511s", link);

    strcpy(link, "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"); //Win
    //strcpy(link, "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"); //Linux
    Header* header = malloc(sizeof(Header));
    InfoHeader *infoHeader = malloc(sizeof(InfoHeader));
    uint8_t* buffer = readHeader(link, header, infoHeader);
    Pixel **pixelMap = readPixels(header, infoHeader, buffer);

    uint32_t height = infoHeader->height;
    if(argc < 1){
        outPutStructTemp(header, infoHeader);
        outputPixels(infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--mirror") == 0){
        if (argc < 3) {
            printf("ERROR: Mirror requires axis (x or y)\n");
            return 1;
        }
        
        if (strcmp(argv[2], "x") == 0) {
            mirrorX(infoHeader, pixelMap);
        } else if (strcmp(argv[2], "y") == 0) {
            mirrorY(infoHeader, pixelMap);
        } else {
            printf("ERROR: Invalid axis. Use 'x' or 'y'\n");
            return 1;
        }
        writeNewFile(header, infoHeader, pixelMap);
    } 
    else if(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--rotate") == 0){
        if (argc < 3) {
            printf("ERROR: Mirror requires axis (x or y)\n");
        return 1;
        }

        if(strcmp(argv[2], "90") == 0){
            rotate90(infoHeader, &pixelMap);
        } 
        else if(strcmp(argv[2], "180") == 0){
            rotate180(infoHeader, pixelMap);
        }
        else if(strcmp(argv[2], "270") == 0){
            rotate270(infoHeader,&pixelMap);
        }
        else{
            printf("ERROR: Invalid angle. Use 90, 180 or 270\n");
            return 1;
        }
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if ((strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--snap") == 0) && argc >= 6) {
    snapImage(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    writeNewFile(header, infoHeader, pixelMap);
}
    else if((strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--frame") == 0) && argc >= 6){
        addFrame(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printHelp();
        return 0;
    }
    else if(strcmp(argv[1], "-t") == 0){
        writeNewFile(header, infoHeader, pixelMap);
    }
    else{
        printf("ERROR: Invalid argument. Check -h (--help) page");
        return 1;
    }

    free(buffer);
    for(int y = 0; y < height; y++ ){
        free(pixelMap[y]);
    }
    free(pixelMap);
    printf("Pixel map freed");
    return 0;
    //add File type check
}
