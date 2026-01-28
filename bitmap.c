#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HeaderStruct {
  uint16_t signature;
  uint32_t fileSize;
  uint32_t reserved;
  uint32_t dataOffset;
};

struct InfoHeaderStruct {
  uint32_t size; // size of InfoHeader
  uint32_t width;
  uint32_t height;
  uint16_t planes; // research
  uint16_t bitePerPixel;
  uint32_t compression;
  uint32_t imageSize;
  uint32_t XpixelsPerM;
  uint32_t TpixelsPerM;
  uint32_t colorsUsed;
  uint32_t importantColors;
};

struct PixelStruct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};

typedef struct HeaderStruct Header;
typedef struct InfoHeaderStruct InfoHeader;
typedef struct PixelStruct Pixel;

Pixel createPixelBGR(uint8_t r, uint8_t g, uint8_t b) {
  Pixel p = {b, g, r};
  return p;
}

void readHeader(char* link, Header* header,  InfoHeader* infoHeader) {
  FILE *fp = fopen(link, "rb");
  if (!fp) {
    perror("invalid file path");
    return;
  }
  fseek(fp, 1, SEEK_END);
  long fileSize = ftell(fp);
  rewind(fp);

  uint8_t *buffer = malloc(fileSize);
  fread(buffer, 1, fileSize, fp);
  // full hex-table
  for (int i = 0; i < fileSize; i++) {
    printf("%02X ", buffer[i]);
    if (((i + 1) % 16) == 0)
      printf("\n");
  }
  printf("\n");
}

void readPixels() {

}



int main() {
  char *link;
  Header *header;
  InfoHeader *infoHeader;
  link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; // linux
  // link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows

  readHeader(link, header, infoHeader);

  return 0;
}
;
