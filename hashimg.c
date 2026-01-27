#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct pixelStruct {
    unsigned char r,g,b;
};
typedef struct pixelStruct Pixel;

Pixel createPixel (unsigned char r, unsigned char g, unsigned char b){
    Pixel newPixel = {
        r,g,b
    };
    return newPixel;
};



// get file info
void getFileInfo(char *link, int *width, int *height, unsigned char **rgb/*, unsigned char *rgbMatrix */) {
    int headerSize = 14;
    int infoHeaderSize = 40;

    FILE *fp = fopen(link, "rb");
    if (!fp) {
        perror("invalid file path");
        return;
    }

    fseek(fp, 0, SEEK_END);   //set a pos in file
    long filesize = ftell(fp); // get a position of file pointer
    rewind(fp);

    unsigned char *buffer = malloc(filesize);
    fread(buffer, 1, filesize, fp); // saves to buffer
    rewind(fp);

    unsigned char *bufferHead = buffer;
    unsigned char *bufferInfo = buffer + 14;
    unsigned char *bufferRgb = buffer + 54;

    memcpy(width, buffer + 18, 4);
    memcpy(height, buffer + 22, 4);

    // rgb array
    int padLength = 4 - ((*width * 3) % 4); //out: 3
    int lineLength = (*width * 3) + padLength; //out 12
    int pixelCountPad = lineLength * *height; //out 24

    *rgb = malloc(pixelCountPad);
    memcpy(*rgb, bufferRgb, pixelCountPad);

    //full hex-table
    for (int i = 0; i < filesize; i++) {
        printf("%02X ", buffer[i]);
        if (((i+1) % 16) == 0)
        printf("\n");
    }
    printf("\n");

    printf("Header:\n");
    for (int i = 0; i < headerSize; i++) {
        printf("%02d ", bufferHead[i]);
        if (((i+1) % 16) == 0)
        printf("\n");
    }
    printf("\n");

    printf("Info-header:\n");
    for (int i = 0; i < infoHeaderSize; i++) {
        printf("%02d ", bufferInfo[i]);
        if (((i+1) % 16) == 0)
        printf("\n");
    }
    printf("\n");

    printf("RGB:\n");
    for (int i = 0; i < (filesize - (headerSize + infoHeaderSize)); i++) {
        printf("%03d ", bufferRgb[i]);
        if (((i+1) % 16) == 0)
        printf("\n");
    }
    printf("\n");

    fclose(fp);
    free(buffer);
}

//options

//redact file

int main() {
    char *link;
    int width, height;
    unsigned char *rgb;

    //unsigned char *rgbMatrix[height][width][3];
    //printf("Enter a file path\n");
    //scanf("%s", link);  

    //link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; //linux
    link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows
    getFileInfo(link, &width, &height, &rgb);

    Pixel **map;
    map = malloc(sizeof(Pixel*) * width);
    for(int x = 0; x < width; x++){
        map[x] = malloc(sizeof(Pixel) * height);
        for(int y = 0; y < height; y++){
            map[x][y] = createPixel(1, 1, 1);
        }
    }
    Pixel x = createPixel(1,1,1);
    printf("%d, %d, %d\n", x.r, x.g, x.b);
    free(map);
    printf("width: %d, height: %d\n", width, height);

    free(rgb);
    return 0;
}
