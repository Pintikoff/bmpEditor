#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// get file info (hex)
void getFileInfo(char *link, int *width, int *height) {
    int headerSize = 14;
    int infoHeaderSize = 40;

    FILE *fp = fopen(link, "rb");
    if (!fp) {
        perror("invalid file path");
        return;
    }

    fseek(fp, 0, SEEK_END);   //set a pos in file
    long filesize = ftell(fp); // get a position of file pointer
    //printf("%ld\n", filesize);

    rewind(fp);
    unsigned char *buffer = malloc(filesize);
    fread(buffer, 1, filesize, fp); // saves to buffer
    
    rewind(fp);

    unsigned char *bufferHead = buffer;
    unsigned char *bufferInfo = buffer + 14;
    unsigned char *bufferRgb = buffer + 54;

    memcpy(width, bufferInfo + 4, 4);
    memcpy(height, bufferInfo + 8, 4);

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
    free(buffer);;
}

//redact file

int main() {
    char *link;
    int width;
    int height;

    //printf("Enter a file path\n");
    //scanf("%s", link);

    //link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; //linux
    link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows
    getFileInfo(link, &width, &height);
    printf("width: %d, height: %d", width, height);
    return 0;
}
