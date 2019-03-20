#include <string.h>
#include "instructions.c"

int arrayToBin(char* str) {
    int bin = 0;
    int size = getSize(str);
    for (int i = 0; i < size; i++) {
        bin << 1;
        bin |= str[i] - '0';
    }
    return bin;
}

char* sliceString (char* src, char* dest, int start, int end) {
    int pos = 0;
    for (int i = start; i < end; i++) {
        dest[pos] = src[i];
        pos++;
    }
    return dest;
}

void parser(char* line) {
    if (strncmp("000000", line, 6) == 0) {
        //RType
        char function[6];
        char rs[5];
        char rd[5];
        char rt[5];
        char sa[5];
        strncpy(rs, &line[6], 5);
        strncpy(rt, &line[11], 5);
        strncpy(rd, &line[16], 5);
        strncpy(sa, &line[21], 5);
        strncpy(function, &line[26], 6);


    } else if (strncmp("00001", line, 5) == 0) {
        //JType
        char opcode[6];
        char target[26];
        strncpy(opcode, line, 6);
        strncpy(target, &line[6], 26);
    } else {
        //IType

    }
}