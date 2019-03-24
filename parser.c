#include <string.h>
#include "parser.h"
#include <stdio.h>

struct binaryToR Rtype[] = {
        {"100000", &add},
        {"100001", &addu},
        {"100100", &and},
        {"011010", &mipsDiv},
        {"011011", &divu},
        {"001000", &jr},
        {"010000", &mfhi},
        {"010010", &mflo},
        {"010001", &mthi},
        {"010011", &mtlo},
        {"011000", &mult},
        {"011001", &multu},
        {"100111", &nor},
        {"100101", &or},
        {"000000", &sll},
        {"101010", &slt},
        {"101011", &sltu},
        {"000010", &srl},
        {"100010", &sub},
        {"100011", &subu},
        {"100110", &xor},
        {"001100", &syscall}
};

struct binaryToI Itype[] = {
        {"001000", &addi},
        {"001001", &addiu},
        {"001100", &andi},
        {"000100", &beq},
        {"000001", &bgez},
        {"000111", &bgtz},
        {"000110", &blez},
        {"000001", &bltz},
        {"000101", &bne},
        {"100000", &lb},
        {"100100", &lbu},
        {"001111", &lui},
        {"100011", &lw},
        {"001101", &ori},
        {"101000", &sb},
        {"001010", &slti},
        {"001011", &sltiu},
        {"101011", &sw},
        {"001110", &xori}
};

struct binaryToJ Jtype[] = {
        {"000010", &j},
        {"000011", &jal}
};

int getSize(char* string) {
    int count = 0;
    for(;*string != '\0'; string++) {
        count++;
    }
    return count;
}

int bin(char* str) {
    int bin = 0;
    int size = getSize(str);
    for (int i = 0; i < size; i++) {
        bin = bin << 1;
        bin |= str[i] - '0';
    }
    return bin;
}

void parser(char* line) {
    if (strncmp("000000", line, 6) == 0) {
        //Rtype
        char* function = (char*) malloc(6 * sizeof(char));
        char* rs = (char*) malloc(5 * sizeof(char));
        char* rd = (char*) malloc(5 * sizeof(char));
        char* rt = (char*) malloc(5 * sizeof(char));
        char* sa = (char*) malloc(5 * sizeof(char));
        strncpy(rs, &line[6], 5);
        strncpy(rt, &line[11], 5);
        strncpy(rd, &line[16], 5);
        strncpy(sa, &line[21], 5);
        strncpy(function, &line[26], 6);
        for (int i = 0; i < 22; i++) {
            if (strncmp(Rtype[i].function, function, 6) == 0) {
                Rtype[i].funcPtr(bin(rd), bin(rs), bin(rt), bin(sa));
                break;
            }
        }

    } else if (strncmp("00001", line, 5) == 0) {
        //JType
        char* opcode = (char*) malloc(6 * sizeof(char));
        char* target = (char*) malloc(26 * sizeof(char));
        strncpy(opcode, line, 6);
        strncpy(target, &line[6], 26);
        for (int i = 0; i < 2; i++) {
            if (strncmp(Jtype[i].opcode, opcode, 6) == 0) {
                Jtype[i].funcPtr(bin(target));
                break;
            }
        }
    } else {
        //IType
        char* opcode = (char*) malloc(6 * sizeof(char));
        char* rs = (char*) malloc(5 * sizeof(char));
        char* rt = (char*) malloc(5 * sizeof(char));
        char* immediate = (char*) malloc(16 * sizeof(char));
        strncpy(opcode, line, 6);
        strncpy(rs, &line[6], 5);
        strncpy(rt, &line[11], 5);
        strncpy(immediate, &line[16], 16);
        for (int i = 0; i < 20; i++) {
            if (strncmp(Itype[i].opcode, opcode, 6) == 0) {
                Itype[i].funcPtr(bin(rs), bin(rt), bin(immediate));
                break;
            }
        }
    }
}