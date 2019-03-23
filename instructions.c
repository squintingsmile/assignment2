#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"

//Register Number	Conventional Name	Usage
//$0	$zero	Hard-wired to 0
//$1	$at	Reserved for pseudo-instructions
//$2 - $3	$v0, $v1	Return values from functions
//$4 - $7	$a0 - $a3	Arguments to functions - not preserved by subprograms
//$8 - $15	$t0 - $t7	Temporary data, not preserved by subprograms
//$16 - $23	$s0 - $s7	Saved registers, preserved by subprograms
//$24 - $25	$t8 - $t9	More temporary registers, not preserved by subprograms
//$26 - $27	$k0 - $k1	Reserved for kernel. Do not use.
//$28	$gp	            Global Area Pointer (base of global data segment)
//$29	$sp	            Stack Pointer
//$30	$fp	            Frame Pointer
//$31	$ra	            Return Address
// register[32] is LO and register[33] is HI

int programCounter = 0;

int registers[34] = {0};
//RType instructions

void add(int rd, int rs, int rt, int sa) {
    if ((registers[rt] > 0 && registers[rs] > INT_MAX - registers[rt]) ||
    (registers[rt] < 0 && registers[rs] < INT_MIN - registers[rt])) {

        printf("Adding overflow, rs : %d, rt : %d", registers[rs], registers[rt]);
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
    printf("%d",registers[rd]);
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
//    printf("%d",value);
    if (value == 1) {
        printf("Printing integer from $a0 : %d", registers[4]);
        return;
    }
    if (value == 4) {
        printf("Printing string from $a0 : %s", registers[4].str);
        return;
    }
    if (value == 5) {
        int *num = (int*) malloc(sizeof(int));
        printf("Please input an integer : ");
        scanf("%d", num);
        registers[2] = *num;
        free(num);
        return;
    }
    if (value == 8) {
        char* str = (char*) malloc(256 * sizeof(char));
        printf("Please input a string : ");
        scanf("%s", str);
        if (strcmp(str, "") == 0) {
            return;
        }
        int size = strlen(str);
        registers[4].str = str;
        registers[5] = size;
        return;
    }
    if (value == 9) {
        char* addr = (char*) malloc(registers[4] * 8);
        registers[2].str = addr;
        return;
    }
    if (value == 10) {
        exit(EXIT_SUCCESS);
    }
    if (value == 11) {
        printf("Printing character in $a0 : %c",registers[4]);
        return;
    }
    if (value == 12) {
        char ch;
        scanf("%c",&ch);
        registers[2] = ch;
        return;
    }
    printf("No matching instruction for syscall");
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
    registers[rt] = *(registers[rs].str + immediate);
}

void lbu(int rs, int rt, int immediate) {
    registers[rt] = *(registers[rs].str + immediate);
}

void lui(int rs, int rt, int immediate) {
    registers[rt] = immediate << 16;
}

void lw(int rs, int rt, int immediate) {
    int num =  (registers[rs].str[0] << 24) |
            (registers[rs].str[1] << 16) |
            (registers[rs].str[2] << 8)  |
            (registers[rs].str[3]);
    registers[rt] = num;

}

void ori(int rs, int rt, int immediate) {
    registers[rt] = registers[rs] | immediate;
}

void sb(int rs, int rt, int immediate) {
    *(registers[rs].str + immediate) = 0xff & registers[rt];
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
    char* str = registers[rs].str + immediate;
    char c = str[0];
    str[0] = (registers[rt] >> 24) & 0xff;
    str[1] = (registers[rt] >> 16) & 0xff;
    str[2] = (registers[rt] >> 8) & 0xff;
    str[3] = registers[rt] & 0xff;
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
