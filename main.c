#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_structs.h"
#include "bmp_read.h"
#include "bmp_edit.h"
#include "bmp_other.h"

/*
!--------Function list and its location---------
? bmp_edit.c:
    * Pixel** allocate2DPixelArray(int rows, int cols);

    * void mirrorX(InfoHeader *infoHeader, Pixel** pixelMap);
    * void mirrorY(InfoHeader *infoHeader, Pixel** pixelMap);

    * void rotate90(InfoHeader *infoHeader, Pixel*** pixelMap);
    * void rotate180(InfoHeader *infoHeader, Pixel** pixelMap);
    * void rotate270(InfoHeader *infoHeader, Pixel ***pixelMap);

    * void snapImage(InfoHeader *infoHeader, Pixel ***pixelMap, int startX, int startY, int endX, int endY);
    * void addFrame(InfoHeader *infoHeader, Pixel ***pixelMap, uint32_t frameWidth, uint8_t frameR, uint8_t frameG, uint8_t frameB);
    * void changeTint(InfoHeader* infoHeader, Pixel **pixelMap, char tintColorString, uint8_t tintValue);
    * void zoom(InfoHeader* infoHeader, Pixel ***pixelMap, uint32_t zoomValue);
? bmp_read.c:
    * uint8_t* readHeader(char* link, Header* header,  InfoHeader* infoHeader);
    * Pixel** readPixels(Header *header, InfoHeader *infoHeader, uint8_t *buffer);
? bmp_other.c:
    * void cleanMem(Header* header, InfoHeader* infoHeader, uint8_t* buffer, Pixel** pixelMap);
    * void writeNewFile(Header *header, InfoHeader *infoHeader, Pixel** pixelMap);
    * void printHelp();
    * void outPutStructTemp(Header* header, InfoHeader* infoHeader);
    * void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap);
    * int checkFileType(Header* header, InfoHeader* infoHeader);
*/

int main(int argc, char *argv[]) {    

    printf("=== PROGRAM START ===\n");
    printf("argc = %d\n", argc);
    for(int i = 0; i < argc; i++){
        printf("argv[%d] = '%s'\n", i, argv[i]);
    }
    printf("=====================\n\n");


    char link[512];
    Header* header;
    InfoHeader* infoHeader;
    uint8_t* buffer;
    Pixel** pixelMap;
    
    printf("Enter a link to a bmp file: \n");
    scanf("%511s", link);
        
    strcpy(link, "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"); //Win
    //strcpy(link, "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"); //Linux
    
    header = malloc(sizeof(Header));
    if(!header){
        perror("ERROR: Failed to allocate header");
        return 1;
    }

    infoHeader = malloc(sizeof(InfoHeader));
    if(!infoHeader){
        perror("ERROR: Failed to allocate infoHeader");
        cleanMem(header, NULL, NULL, NULL);
        return 1;
    }

    buffer = readHeader(link, header, infoHeader);
    if(!buffer){
        perror("ERROR: Failed to allocate buffer");
        cleanMem(header, infoHeader, NULL, NULL);
        return 1;
    }

    pixelMap = readPixels(header, infoHeader, buffer);
    if(!pixelMap){
        perror("ERROR: Faield to allocate pixelMap");
        cleanMem(header, infoHeader, buffer, NULL);
        return 1;
    }
    free(buffer);
    buffer = NULL;
    int fileType = checkFileType(header, infoHeader);
    if(fileType == 1){
        cleanMem(header, infoHeader, NULL, pixelMap);
        return 1;
    }
    
    if(argc == 1){
        outPutStructTemp(header, infoHeader);
        outputPixels(infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--mirror") == 0){
        if (argc < 3) {
            printf("ERROR: Mirror requires axis argument (x or y)\n");
            cleanMem(header, infoHeader, NULL, pixelMap);
            return 1;
        }
        
        if (strcmp(argv[2], "x") == 0) {
            mirrorX(infoHeader, pixelMap);
        } else if (strcmp(argv[2], "y") == 0) {
            mirrorY(infoHeader, pixelMap);
        } else {
            printf("ERROR: Invalid axis '%s'. Use 'x' or 'y'\n", argv[2]);
            cleanMem(header, infoHeader, NULL, pixelMap);
            return 1;
        }
        writeNewFile(header, infoHeader, pixelMap);
    } 
    else if(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--rotate") == 0){
        if (argc < 3) {
            printf("ERROR: Rotate requires angle argument (90, 180, or 270)\n");
            cleanMem(header, infoHeader, NULL, pixelMap);
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
            cleanMem(header, infoHeader, NULL, pixelMap);
            return 1;
        }
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--snap") == 0) {
        if(argc < 6){
            printf("ERROR: Snap requires 4 coordinate arguments (startX startY endX endY)\n");
            cleanMem(header, infoHeader, NULL, pixelMap);
            return 1;
        }
        snapImage(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--frame") == 0){
        if(argc < 6){
            printf("ERROR: Frame requires 4 arguments (width R G B)\n");
            cleanMem(header, infoHeader, NULL, pixelMap);
            return 1;
        }
        addFrame(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--tint") == 0){
        if(argc < 4){
            printf("ERROR: Change tint requires 2 arguments (color, color value)");
            cleanMem(header, infoHeader, NULL, pixelMap);
            return 1;
        }
        changeTint(infoHeader, pixelMap, argv[2][0], atoi(argv[3]));
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-z") == 0 || strcmp(argv[1], "--zoom") == 0){
        if(argc == 2){
            zoom(infoHeader, &pixelMap, 0);
        } else if(argc == 3){
            zoom(infoHeader, &pixelMap, atoi(argv[2]));
        }
        writeNewFile(header, infoHeader, pixelMap);
    }
    else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printHelp();
        cleanMem(header, infoHeader, NULL, pixelMap);
        return 0;
    }
    else if(strcmp(argv[1], "-test") == 0){

    }
    else{
        printf("ERROR: Unknown command '%s'. Use -h for help\n", argv[1]);
        cleanMem(header, infoHeader, NULL, pixelMap);
        return 1;
    }
    cleanMem(header, infoHeader, NULL, pixelMap);
    return 0;
    //update help page
}
