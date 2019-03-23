//
// Created by nc_lab on 19-3-20.
//
#include "instructions.h"
#include <stdlib.h>

#ifndef ASSIGNMENT2_PARSER_H
#define ASSIGNMENT2_PARSER_H

struct binaryToR {
    char function[6];
    void (*funcPtr)(int, int, int, int);
};

struct binaryToI {
    char opcode[6];
    void (*funcPtr)(int, int, int);
};

struct binaryToJ {
    char opcode[6];
    void (*funcPtr)(int);
};

extern struct binaryToR Rtype[];

extern struct binaryToI Itype[];

extern struct binaryToJ Jtype[];

int getSize(char* string);

int bin(char* str);

void parser(char* line);

#endif //ASSIGNMENT2_PARSER_H
