#include <stdlib.h>
#include <stdio.h>

void compileProject() {
    printf("Compiling...\n");
    
    int result = system("gcc main.c bmp_read.c -o ./bitmap.exe");
    
    if (result == 0) {
        printf("Complied ✓\n");
    } else {
        printf("ERROR: Compilation failed\n");
    }
}

int main() {
    compileProject();
    return 0;
}