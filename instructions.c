#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"

//Register Number	Conventional Name
//$0	                $zero
//$1	                $at
//$2  - $3	            $v0, $v1
//$4  - $7	            $a0 - $a3
//$8  - $15	            $t0 - $t7
//$16 - $23	            $s0 - $s7
//$24 - $25	            $t8 - $t9
//$26 - $27	            $k0 - $k1
//$29	                $sp
//$30	                $fp
//$31	                $ra
// register[32] is LO and register[33] is HI

long registers[34] = {0};

int programCounter = 0;

//RType instructions

void add(int rd, int rs, int rt, int sa) {
    if ((registers[rt] > 0 && registers[rs] > INT_MAX - registers[rt]) ||
    (registers[rt] < 0 && registers[rs] < INT_MIN - registers[rt])) {

        printf("Adding overflow, rs : %ld, rt : %ld", registers[rs], registers[rt]);
        exit(EXIT_FAILURE);
    }
    registers[rd] = registers[rs] + registers[rt];
}

void addu(int rd, int rs, int rt, int sa) {
    registers[rd] = registers[rs] + registers[rt];
}

void and(int rd, int rs, int rt, int sa) {
    registers[rd] = (registers[rt]) & (registers[rs]);
}

void mipsDiv(int rd, int rs, int rt, int sa) {
    if (registers[rs] == 0 || (registers[rt] == INT_MIN && registers[rs] == 1)) {
        exit(EXIT_FAILURE);
    }
    int quotient = registers[rt] / registers[rs];
    int remainder = registers[rt] % registers[rs];
    registers[32] = quotient;
    registers[33] = remainder;
}

void divu(int rd, int rs, int rt, int sa) {

    int quotient = registers[rt] / registers[rs];
    int remainder = registers[rt] % registers[rs];
    registers[32] = quotient;
    registers[33] = remainder;
}

void jr(int rd, int rs, int rt, int sa) {
    programCounter = (registers[rs]) - 1;
}

void mfhi(int rd, int rs, int rt, int sa) {
    registers[rd] = registers[33];
}

void mflo(int rd, int rs, int rt, int sa) {
    registers[rd] = registers[32];
}

void mthi(int rd, int rs, int rt, int sa) {
    registers[33] = registers[rs];
}

void mtlo(int rd, int rs, int rt, int sa) {
    registers[32] = registers[rs];
}

void mult(int rd, int rs, int rt, int sa) {
    if (abs(INT_MAX / (registers[rt])) < abs((registers[rs]))) {
        printf("Multiplying overflow");
        exit(EXIT_FAILURE);
    }
    registers[32] = (registers[rs]) * (registers[rt]);
}

void multu(int rd, int rs, int rt, int sa) {
    registers[32] = registers[rt] * registers[rs];
}

void nor(int rd, int rs, int rt, int sa) {
    registers[rd] = ~((registers[rs]) | (registers[rt]));
}

void or(int rd, int rs, int rt, int sa) {
    registers[rd] = (registers[rs]) | (registers[rt]);
}

void sll(int rd, int rs, int rt, int sa) {
    registers[rd] = (registers[rt]) << sa;
}

void slt(int rd, int rs, int rt, int sa) {
    if (registers[rs] < registers[rt]) {
        registers[rd] = 1;
    } else {
        registers[rd] = 0;
    }
}

void sltu(int rd, int rs, int rt, int sa) {
    if (registers[rs] < registers[rt]) {
        registers[rd] = 1;
    } else {
        registers[rd] = 0;
    }
}

void srl(int rd, int rs, int rt, int sa) {
    registers[rd] = (registers[rt]) >> sa;
}

void sub(int rd, int rs, int rt, int sa) {
    if ((registers[rt] > 0 && INT_MIN + registers[rt] > registers[rs]) ||
    (registers[rt] < 0 && INT_MAX + registers[rt] < registers[rs])) {
        exit(EXIT_FAILURE);
    }
    registers[rd] = registers[rs] - registers[rt];
}

void subu(int rd, int rs, int rt, int sa) {
    unsigned int rsTmp = registers[rs];
    unsigned int rtTmp = registers[rt];
    registers[rd] = rsTmp - rtTmp;
}

void xor(int rd, int rs, int rt, int sa) {
    registers[rd] = (registers[rs]) ^ (registers[rt]);
}

void syscall(int rd, int rs, int rt, int sa) {
    int value = registers[2];
    if (value == 1) {
        printf("Printing integer from $a0 : %ld\n", registers[4]);
        return;
    }
    if (value == 4) {
        printf("Printing string from $a0 : %s\n", (char*) registers[4]);
        return;
    }
    if (value == 5) {
        int *num = (int*) malloc(sizeof(int));
        printf("Please input an integer : \n");
        scanf("%d", num);
        registers[2] = *num;
        free(num);
        return;
    }
    if (value == 8) {
        char* str = (char*) malloc(256 * sizeof(char));
        printf("Please input a string : \n");
        scanf("%s", str);
        if (strcmp(str, "") == 0) {
            return;
        }
        int size = strlen(str);
        printf("%d",size);
        registers[4] = (long) str;
        registers[5] = size;
        return;
    }
    if (value == 9) {
        registers[2] = (long) malloc(registers[4] * 8);
        return;
    }
    if (value == 10) {
        exit(EXIT_SUCCESS);
    }
    if (value == 11) {
        printf("Printing character in $a0 : %c\n",(char)registers[4]);
        return;
    }
    if (value == 12) {
        char ch;
        scanf("%c",&ch);
        registers[2] = ch;
        return;
    }
    printf("No matching instruction for syscall\n");
}

// IType instructions

void addi(int rs, int rt, int immediate) {
    if ((immediate > 0 && INT_MAX - registers[rs] < immediate) ||
            (immediate < 0 && INT_MIN - registers[rs] > immediate)) {
        exit(EXIT_FAILURE);
    }
    registers[rt] = registers[rs] + immediate;
}

void addiu(int rs, int rt, int immediate) {
    registers[rt] = registers[rs] + immediate;
}

void andi(int rs, int rt, int immediate) {
    registers[rt] = registers[rs] & immediate;
}

void beq(int rs, int rt, int immediate) {
    if (registers[rs] == registers[rt]) {
        programCounter = immediate - 1;
    }
}

void bgez(int rs, int rt, int immediate) {
    if (registers[rs] > -1) {
        programCounter = immediate - 1;
    }
}

void bgtz(int rs, int rt, int immediate) {
    if (registers[rs] > 0) {
        programCounter = immediate - 1;
    }
}

void blez(int rs, int rt, int immediate) {
    if (registers[rs] < 1) {
        programCounter = immediate - 1;
    }
}

void bltz(int rs, int rt, int immediate) {
    if (registers[rs] < 0) {
        programCounter = immediate - 1;
    }
}

void bne(int rs, int rt, int immediate) {
    if (registers[rs] != registers[rt]) {
        programCounter = immediate - 1;
    }
}

void lb(int rs, int rt, int immediate) {
    registers[rt] = * (char*) (registers[rs] + immediate);
}

void lbu(int rs, int rt, int immediate) {
    registers[rt] = * (char*) (registers[rs] + immediate);
}

void lui(int rs, int rt, int immediate) {
    registers[rt] = immediate << 16;
}

void lw(int rs, int rt, int immediate) {
    registers[rt] =  * (int*) (registers[rs] + immediate);
}

void ori(int rs, int rt, int immediate) {
    registers[rt] = registers[rs] | immediate;
}

void sb(int rs, int rt, int immediate) {
    * (char*) (registers[rs] + immediate) = 0xff & registers[rt];
}

void slti(int rs, int rt, int immediate) {
    if (registers[rs] < immediate) {
        registers[rt] = 1;
    } else {
        registers[rt] = 0;
    }
}

void sltiu(int rs, int rt, int immediate) {
    unsigned int un = immediate;
    if (registers[rs] < un) {
        registers[rt] = 1;
    } else {
        registers[rt] = 0;
    }
}

void sw(int rs, int rt, int immediate) {
    * (int*) (registers[rs] + immediate) = registers[rt];
}

void xori(int rs, int rt, int immediate) {
    registers[rt] = registers[rs] ^ immediate;
}

//JType Instruction

void j(int target) {
    programCounter = target - 1;
}

void jal(int target) {
    registers[31] = programCounter + 1;
    programCounter = target - 1;
}
