#include <stdio.h>
#include <stdlib.h>

// get file info (hex)
void getFileInfo(char *link) {
  int headerSize = 14;
  int infoHeaderSize = 40;

  FILE *fp = fopen(link, "rb");
  if (!fp) {
    perror("invalid file path\n");
    return;
  }

  fseek(fp, 0, SEEK_END);   //set a pos in file
  long filesize = ftell(fp); // get a position of file pointer
  printf("%ld\n", filesize);

  rewind(fp);
  unsigned char* buffer = malloc(filesize);
  fread(buffer, 1, filesize, fp); // saves to buffer
  rewind(fp);

  unsigned char* bufferHead = malloc(headerSize);
  fread(bufferHead, 1, 14, fp); // saves to buffer

  unsigned char *bufferInfo = malloc(infoHeaderSize);
  fread(bufferInfo, 1, 40, fp);

  unsigned char *bufferRgb = malloc(filesize - (headerSize + infoHeaderSize));
  fread(bufferRgb, 1, (filesize - (headerSize + infoHeaderSize)), fp);

  //full hex-table
  for (int i = 0; i < filesize; i++) {
    printf("%02X", buffer[i]);
    if (((i+1) % 16) == 0)
      printf("\n");
  }
  printf("\n");

  printf("Header:\n");
  for (int i = 0; i < headerSize; i++) {
    printf("%02X ", bufferHead[i]);
    if (((i+1) % 16) == 0)
      printf("\n");
  }
  printf("\n");

  printf("Info-header:\n");
  for (int i = 0; i < infoHeaderSize; i++) {
    printf("%02X ", bufferInfo[i]);
    if (((i+1) % 16) == 0)
      printf("\n");
  }
  printf("\n");

  printf("RGB:\n");
  for (int i = 0; i < (filesize - (headerSize + infoHeaderSize)); i++) {
    printf("%d ", bufferRgb[i]);
    if (((i+1) % 16) == 0)
      printf("\n");
  }
  printf("\n");

  fclose(fp);

  free(buffer);
  free(bufferHead);
  free(bufferInfo);
  free(bufferRgb);
}

//redact file

int main() {
  char *link;
  //printf("Enter a file path\n");
  //scanf("%s", link);

  //link = "/home/pintikoff/Code/emacs/hashImages/2by3.bmp"; //linux
  link = "C:/Users/petro/OneDrive/Documents/codeVS/C/bmpEditor/2by3.bmp"; //windows
  getFileInfo(link);
  return 0;
}
