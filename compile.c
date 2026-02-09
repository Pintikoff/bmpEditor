#include <stdlib.h>
#include <stdio.h>

void compileProject() {
    printf("Compiling...\n");

    int result = system("gcc main.c bmp_read.c bmp_edit.c bmp_other.c -o ./main.exe");

    if (result == 0) {
        printf("SUCCESS: Files complied \n");
    } else {
        printf("ERROR: Compilation failed\n");
    }
}

int main() {
    compileProject();
    return 0;
}
