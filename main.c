#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#include "parser.c"
#include "instructions.c"

int main() {
    FILE* fp;
    fp = fopen("file.txt", "r");
    char** lines;
    size_t length;
    int row = 0;
    while (getline(&lines[row], &length, fp) != -1) {
        row++;
    }
}