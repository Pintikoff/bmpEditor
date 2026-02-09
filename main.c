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
    if(argc == 0){
    outPutStructTemp(header, infoHeader);
    outputPixels(infoHeader, pixelMap);
}
    else if(strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--mirror") == 0){
        if (argc < 3) {
            printf("ERROR: Mirror requires axis argument (x or y)\n");
            return 1;
        }
        
        if (strcmp(argv[2], "x") == 0) {
            mirrorX(infoHeader, pixelMap);
        } else if (strcmp(argv[2], "y") == 0) {
            printf("ERROR: Invalid axis '%s'. Use 'x' or 'y'\n", argv[2]);
            return 1;
        }
        writeNewFile(header, infoHeader, pixelMap);
    } 
    else if(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--rotate") == 0){
        if (argc < 3) {
            printf("ERROR: Rotate requires angle argument (90, 180, or 270)\n");
            return 1;
        }

        if(strcmp(argv[2], "90") == 0){
            rotate90(infoHeader, &pixelMap);
        } 
        else if(strcmp(argv[2], "180") == 0){
            rotate180(infoHeader, pixelMap);
        }
        else if(strcmp(argv[2], "270") == 0){
            rotate270(infoHeader, &pixelMap);
        }
        else{
            printf("ERROR: Invalid angle '%s'. Use 90, 180, or 270\n", argv[2]);
            return 1;
        }
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--snap") == 0) {
        if(argc < 6){
            printf("ERROR: Snap requires 4 coordinate arguments (startX startY endX endY)\n");
            return 1;
        }
        snapImage(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--frame") == 0){
        if(argc < 6){
            printf("ERROR: Frame requires 4 arguments (width R G B)\n");
            return 1;
        }
        addFrame(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--tint") == 0){
        if(argc < 4){
            printf("ERROR: Change tint requires 2 arguments (color, color value)");
            return 1;
        }
        changeTint(infoHeader, pixelMap, argv[2], atoi(argv[3]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printHelp();
        return 0;
    }
    else if(strcmp(argv[1], "-test") == 0){
        writeNewFile(header, infoHeader, pixelMap);
    }
    else{
        printf("ERROR: Unknown command '%s'. Use -h for help\n", argv[1]);
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
