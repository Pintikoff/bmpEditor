#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_structs.h"
#include "bmp_read.h"
#include "bmp_edit.h"

void printHelp() {
    // ANSI color codes
    const char* BOLD = "\033[1m";
    const char* CYAN = "\033[36m";
    const char* GREEN = "\033[32m";
    const char* YELLOW = "\033[33m";
    const char* RESET = "\033[0m";
    
    printf("%s         BMP Image Editor - Help Page              %s\n", CYAN, RESET);
    printf("\n");
    
    printf("%sUSAGE:%s\n", BOLD, RESET);
    printf("  ./bmp_editor <input.bmp> <output.bmp> [operation] [parameters]\n\n");
    
    printf("%sOPERATIONS:%s\n\n", BOLD, RESET);
    
    printf("  %s-m, --mirror%s <x|y>\n", GREEN, RESET);
    printf("      Mirror the image\n");
    printf("        %sx%s - horizontal, %sy%s - vertical\n", YELLOW, RESET, YELLOW, RESET);
    printf("      Example: %s./main.exe -m x%s\n\n", CYAN, RESET);
    
    printf("  %s-r, --rotate%s <90|180|270>\n", GREEN, RESET);
    printf("      Rotate the image clockwise\n");
    printf("      Example: %s./main.exe -r 90%s\n\n", CYAN, RESET);
    
    printf("  %s-s, --snap%s <x1> <y1> <x2> <y2>\n", GREEN, RESET);
    printf("      Crop to rectangle\n");
    printf("      Example: %s./main.exe -s 0 0 800 600%s\n\n", CYAN, RESET);
    
    printf("  %s-f, --frame%s <thickness> <r> <g> <b>\n", GREEN, RESET);
    printf("      Add colored frame (RGB: 0-255)\n");
    printf("      Example: %s./main.exe -f 10 255 0 0%s\n\n", CYAN, RESET);
    
    printf("%sNOTES:%s\n", BOLD, RESET);
    printf("%s  -Only 24-bit BMP files supported%s\n", YELLOW, RESET);
    printf("%s  -Coordinates: (0,0) = top-left%s\n\n", YELLOW, RESET);
}
void outPutStructTemp(Header* header, InfoHeader* infoHeader){
    printf("Header:\n");
    printf("  signature: 0x%04X\n", header->signature);
    printf("  fileSize: %u bytes\n", header->fileSize);
    printf("  reserved: %u\n", header->reserved);
    printf("  dataOffset: %u\n", header->dataOffset);

    printf("\nInfoHeader:\n");
    printf("  size: %u bytes\n", infoHeader->size);
    printf("  width: %u pixels\n", infoHeader->width);
    printf("  height: %u pixels\n", infoHeader->height);
    printf("  planes: %u\n", infoHeader->planes);
    printf("  bitsPerPixel: %u\n", infoHeader->bitePerPixel);
    printf("  compression: %u\n", infoHeader->compression);
    printf("  imageSize: %u bytes\n", infoHeader->imageSize);
    printf("  XpixelsPerM: %u\n", infoHeader->XpixelsPerM);
    printf("  YpixelsPerM: %u\n", infoHeader->YpixelsPerM);
    printf("  colorsUsed: %u\n", infoHeader->colorsUsed);
    printf("  importantColors: %u\n", infoHeader->importantColors);
}
void outputPixels(InfoHeader *infoHeader, Pixel** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    int actY = height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("Row: %d Column: %d ", actY, x+1);
            printf("R: %03d G: %03d: B: %03d \n", pixelMap[y][x].r, pixelMap[y][x].g, pixelMap[y][x].b);
        }
        actY--;
    }
}
int main(int argc, char *argv[]) {
    char* link;
    //link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; // linux
    link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows

    Header* header = malloc(sizeof(Header));
    InfoHeader *infoHeader = malloc(sizeof(InfoHeader));

    uint8_t* buffer = readHeader(link, header, infoHeader);
    Pixel **pixelMap = readPixels(header, infoHeader, buffer);

    if(strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--mirror") == 0){
        if(strcmp(argv[2], "x") == 0){
            mirrorX(infoHeader, pixelMap);
        } 
        else if(strcmp(argv[2], "y") == 0){
            mirrorY(infoHeader, pixelMap);
        }
    } 
    else if(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--rotate") == 0){
        if(strcmp(argv[2], "90") == 0){
            rotate90(infoHeader, &pixelMap);
        } 
        else if(strcmp(argv[2], "180") == 0){
            rotate180(infoHeader, pixelMap);
        }
        else if(strcmp(argv[2], "270") == 0){
            rotate270(infoHeader,&pixelMap);
        }
    }
    else if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--snap") == 0 && argc >= 6){
        snapImage(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    else if(strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--frame") == 0 && argc >= 6){
        addFrame(infoHeader, &pixelMap, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printHelp();
        return 0;
    }
    else{
        printf("ERROR: Invalid argument. Check -h (--help) page");
        return 0;
    }

    outPutStructTemp(header, infoHeader);
    outputPixels(infoHeader, pixelMap);

    free(buffer);
    return 0;
    //add File type check
    //check malloc
}
