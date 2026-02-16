//This file containts functions for rewriting file, help page, outputs etc...

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp_structs.h"

void cleanMem(Header* header, InfoHeader* infoHeader, uint8_t* buffer, Pixel** pixelMap){
    if(header) free(header);
    if(infoHeader) free(infoHeader);
    if(buffer) free(buffer);
    if(pixelMap && infoHeader){
        for(int i = 0; i < infoHeader->height; i++){
            if(pixelMap[i]) free(pixelMap[i]);
        }
        free(pixelMap);
    }
    printf("cleanMem");
}

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
        perror("ERROR: Failed to create a file");
        return;
    }
    fwrite(header, 1, sizeof(*header), file);
    fwrite(infoHeader, 1, sizeof(*infoHeader), file);

    for(int y = 0; y < height; y++){
        //pixelMap[y] pointer, sizeof(Pixel) - size of one element, width - elements count, file - stream
        fwrite(pixelMap[y], sizeof(Pixel), width, file);
        for(int p = 0; p < padding; p++){
            fputc(0, file);
        }
    }
    fclose(file);
    printf("File '%s' has been created\n", fileName);
};

void printHelp() {
    // ANSI color codes
    const char* BOLD = "\033[1m";
    const char* CYAN = "\033[36m";
    const char* GREEN = "\033[32m";
    const char* YELLOW = "\033[33m";
    const char* MAGENTA = "\033[35m";
    const char* RESET = "\033[0m";
    
    printf("\n%s===============================================================%s\n", CYAN, RESET);
    printf("%s            BMP Image Editor - Help Page                %s\n", CYAN, RESET);
    printf("%s===============================================================%s\n\n", CYAN, RESET);
    
    printf("%sUSAGE:%s\n", BOLD, RESET);
    printf("  ./bmp_editor [operation] [parameters]\n\n");
    
    printf("%sOPERATIONS:%s\n\n", BOLD, RESET);
    
    // Mirror
    printf("  %s-m, --mirror%s <axis>\n", GREEN, RESET);
    printf("      Mirror the image horizontally or vertically\n");
    printf("      %sParameters:%s\n", YELLOW, RESET);
    printf("        %sx%s - horizontal mirror\n", MAGENTA, RESET);
    printf("        %sy%s - vertical mirror\n", MAGENTA, RESET);
    printf("      %sExample:%s %s./bmp_editor -m x%s\n\n", YELLOW, RESET, CYAN, RESET);
    
    // Rotate
    printf("  %s-r, --rotate%s <angle>\n", GREEN, RESET);
    printf("      Rotate the image clockwise\n");
    printf("      %sParameters:%s\n", YELLOW, RESET);
    printf("        %s90%s  - rotate 90° clockwise\n", MAGENTA, RESET);
    printf("        %s180%s - rotate 180°\n", MAGENTA, RESET);
    printf("        %s270%s - rotate 270° clockwise (90° counter-clockwise)\n", MAGENTA, RESET);
    printf("      %sExample:%s %s./bmp_editor -r 90%s\n\n", YELLOW, RESET, CYAN, RESET);
    
    // Snap/Crop
    printf("  %s-s, --snap%s <startX> <startY> <endX> <endY>\n", GREEN, RESET);
    printf("      Crop image to specified rectangle\n");
    printf("      %sParameters:%s\n", YELLOW, RESET);
    printf("        Coordinates of top-left and bottom-right corners\n");
    printf("      %sExample:%s %s./bmp_editor -s 10 10 100 100%s\n\n", YELLOW, RESET, CYAN, RESET);
    
    // Frame
    printf("  %s-f, --frame%s <width> <R> <G> <B>\n", GREEN, RESET);
    printf("      Add colored frame around the image\n");
    printf("      %sParameters:%s\n", YELLOW, RESET);
    printf("        %swidth%s - frame thickness in pixels\n", MAGENTA, RESET);
    printf("        %sR G B%s - color values (0-255)\n", MAGENTA, RESET);
    printf("      %sExample:%s %s./bmp_editor -f 5 255 0 0%s (red frame, 5px)\n\n", YELLOW, RESET, CYAN, RESET);
    
    // Tint
    printf("  %s-t, --tint%s <color> <value>\n", GREEN, RESET);
    printf("      Adjust color tint of the image\n");
    printf("      %sParameters:%s\n", YELLOW, RESET);
    printf("        %scolor%s - which channel to adjust (r/g/b)\n", MAGENTA, RESET);
    printf("        %svalue%s - adjustment amount (-255 to +255)\n", MAGENTA, RESET);
    printf("      %sExample:%s %s./bmp_editor -t r 50%s (increase red by 50)\n\n", YELLOW, RESET, CYAN, RESET);
    
    // Zoom
    printf("  %s-z, --zoom%s [factor]\n", GREEN, RESET);
    printf("      Zoom/scale the image\n");
    printf("      %sParameters:%s\n", YELLOW, RESET);
    printf("        %s0 or none%s - zoom 2x (default)\n", MAGENTA, RESET);
    printf("        %sfactor%s   - custom zoom factor\n", MAGENTA, RESET);
    printf("      %sExample:%s %s./bmp_editor -z%s (2x zoom)\n", YELLOW, RESET, CYAN, RESET);
    printf("                %s./bmp_editor -z 3%s (3x zoom)\n\n", CYAN, RESET);
    
    // Help
    printf("  %s-h, --help%s\n", GREEN, RESET);
    printf("      Display this help message\n\n");
    
    printf("%sNOTES:%s\n", BOLD, RESET);
    printf("  - Only 24-bit BMP files are supported\n");
    printf("  - Coordinate system: (0,0) = top-left corner\n");
    printf("  - Output file will be created in specified directory\n");
    printf("  - Images are stored upside-down in BMP format\n\n");
    
    printf("%sEXAMPLES:%s\n", BOLD, RESET);
    printf("  %s./bmp_editor -m y%s           Mirror vertically\n", CYAN, RESET);
    printf("  %s./bmp_editor -r 180%s         Rotate 180 degrees\n", CYAN, RESET);
    printf("  %s./bmp_editor -s 0 0 50 50%s   Crop to 50x50 from top-left\n", CYAN, RESET);
    printf("  %s./bmp_editor -f 10 0 255 0%s  Add green 10px frame\n", CYAN, RESET);
    printf("  %s./bmp_editor -t b -30%s       Decrease blue channel\n", CYAN, RESET);
    printf("  %s./bmp_editor -z 4%s           Zoom 4x\n\n", CYAN, RESET);
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

int checkFileType(Header* header, InfoHeader* infoHeader){
    uint16_t signature = header->signature;
    uint16_t bitsPerPx = infoHeader->bitePerPixel;
    //printf("0x%04X, %d\n", signature, signature);
    if(signature != 0x4D42 || bitsPerPx != 24){
        printf("invalid file data");
        return 1;
    }
    else{
        return 0;
    }
}