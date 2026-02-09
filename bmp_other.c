//This file containts functions for rewriting file, help page, outputs etc...

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_structs.h"

void writeNewFile(Header *header, InfoHeader *infoHeader, Pixel** pixelMap){
    uint32_t width = infoHeader->width;
    uint32_t height = infoHeader->height;
    uint8_t padding = (4 - (width * 3) % 4) % 4;
    char directory[448];
    char fileName[64];
    
    printf("Enter a new file DIRECTORY path: \n");
    scanf("%447s", directory);
    printf("Enter a new file NAME: \n");
    scanf("%63s", fileName);

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s/%s.bmp", directory, fileName);

    FILE* file = fopen(fullPath, "wb");
    if (!file){
        perror("Failed to create a file");
        return;
    }
    fwrite(header, 1, sizeof(*header), file);
    fwrite(infoHeader, 1, sizeof(*infoHeader), file);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            fwrite(pixelMap[y], sizeof(pixelMap), width, file);
        }
        for(int p = 0; p < padding; p++){
            fputc(0, file);
        }
    }

    printf("File '%s' has been created\n", fileName);
};

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

void outputPixels(InfoHeader* infoHeader, Pixel** pixelMap){
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

int checkFileType(Header* header){
    uint16_t signature = header->signature;
    printf("0x%04X, %d\n", signature, signature);
    if(signature != 0x4D42){
        return 1;
    }
    else{
        return 0;
    }
}