#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

// register[32] is LO and register[33] is HI
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

union fourbytes{
    unsigned int uNum;
    char* str;
    int num;

};
static union fourbytes registers[34];
static int programCounter = 0;

int getSize(char* string) {
    int count = 0;
    for(;*string != '\0'; string++) {
        count++;
    }
    return count;
}

//RType instructions

void add(int rd, int rs, int rt, int sa) {
    if ((registers[rs].num > INT_MAX - registers[rt].num) ||
    (registers[rs].num < INT_MIN - registers[rt].num)) {
        printf("Adding overflow");
        exit(EXIT_FAILURE);
    }
    registers[rd].num = registers[rs].num + registers[rt].num;
}

void addu(int rd, int rs, int rt, int sa) {
    registers[rd].num = registers[rs].num + registers[rt].num;
}

void and(int rd, int rs, int rt, int sa) {
    registers[rd].num = (registers[rt].num) & (registers[rs].num);
}

void div(int rd, int rs, int rt, int sa) {
    if (registers[rs].num == 0 || (registers[rt].num == INT_MIN && registers[rs].num == 1)) {
        exit(EXIT_FAILURE);
    }
    int quotient = registers[rt].num / registers[rs].num;
    int remainder = registers[rt].num % registers[rs].num;
    registers[32].num = quotient;
    registers[33].num = remainder;
}

void divu(int rd, int rs, int rt, int sa) {

    int quotient = registers[rt].num / registers[rs].num;
    int remainder = registers[rt].num % registers[rs].num;
    registers[32].num = quotient;
    registers[33].num = remainder;
}

void jr(int rd, int rs, int rt, int sa) {
    programCounter = (registers[rs].num) - 1;
}

void mfhi(int rd, int rs, int rt, int sa) {
    registers[rd].num = registers[33].num;
}

void mflo(int rd, int rs, int rt, int sa) {
    registers[rd].num = registers[32].num;
}

void mthi(int rd, int rs, int rt, int sa) {
    registers[33].num = registers[rs].num;
}

void mtlo(int rd, int rs, int rt, int sa) {
    registers[32].num = registers[rs].num;
}

void mult(int rd, int rs, int rt, int sa) {
    if (abs(INT_MAX / (registers[rt].num)) < abs((registers[rs].num))) {
        printf("Multiplying overflow");
        exit(EXIT_FAILURE);
    }
    registers[32].num = (registers[rs].num) * (registers[rt].num);
}

void multu(int rd, int rs, int rt, int sa) {
    unsigned int rtTmp = registers[rt].num;
    unsigned int rsTmp = registers[rs].num;
    registers[32].num = rtTmp * rsTmp;
}

void nor(int rd, int rs, int rt, int sa) {
    registers[rd].num = ~((registers[rs].num) | (registers[rt].num));
}

void or(int rd, int rs, int rt, int sa) {
    registers[rd].num = (registers[rs].num) | (registers[rt].num);
}

void sll(int rd, int rs, int rt, int sa) {
    registers[rd].num = (registers[rt].num) << (*sa);
}

void slt(int rd, int rs, int rt, int sa) {
    if (registers[rs].num < registers[rt].num) {
        registers[rd].num = 1;
    } else {
        registers[rd].num = 0;
    }
}

void sltu(int rd, int rs, int rt, int sa) {
    if (registers[rs].num < registers[rt].num) {
        registers[rd].num = 1;
    } else {
        registers[rd].num = 0;
    }
}

void srl(int rd, int rs, int rt, int sa) {
    registers[rd].num = (registers[rt].num) >> (*sa);
}

void sub(int rd, int rs, int rt, int sa) {
    if ((INT_MIN + registers[rt].num > registers[rs].num) || (INT_MAX + registers[rt].num < registers[rs].num)) {
        exit(EXIT_FAILURE);
    }
    registers[rd].num = registers[rs].num - registers[rt].num;
}

void subu(int rd, int rs, int rt, int sa) {
    unsigned int rsTmp = registers[rs].num;
    unsigned int rtTmp = registers[rt].num;
    registers[rd].num = rsTmp - rtTmp;
}

void xor(int rd, int rs, int rt, int sa) {
    registers[rd].num = (registers[rs].num) ^ (registers[rt].num);
}

void syscall(int rd, int rs, int rt, int sa) {
    int value = registers[2].num;
    if (value == 1) {
        printf("%d", registers[4].num);
        return;
    }
    if (value == 4) {
        char* str = registers[4].str;
        printf("%s", str);
        return;
    }
    if (value == 5) {
        char* str;
        int *num = 0;
        scanf("%d",num);
        registers[2].num = *num;
        return;
    }
    if (value == 8) {
        char* str;
        scanf("%s", str);
        size_t size = strlen(str);
        registers[4].str = str;
        registers[5].num = size;
        return;
    }
    if (value == 9) {
        char* addr = malloc(registers[4].num);
        registers[2].str = addr;
        return;
    }
    if (value == 10) {
        exit(EXIT_SUCCESS);
    }
    if (value == 11) {
        printf("%c",registers[4].num);
        return;
    }
    if (value == 12) {
        char* ch;
        scanf("%c",ch);
        registers[2].num = *ch;
        return;
    }
    printf("No matching instruction for syscall");
    return;
}

// IType instructions

void addi(int rs, int rt, int immediate) {
    if (INT_MAX - registers[rs].num < immediate || INT_MIN - registers[rs].num > immediate) {
        exit(EXIT_FAILURE);
    }
    registers[rt].num = registers[rs].num + immediate;
}

void addiu(int rs, int rt, int immediate) {
    registers[rt].num = registers[rs].num + immediate;
}

void andi(int rs, int rt, int immediate) {
    registers[rt].num = registers[rs].num & immediate;
}

void beq(int rs, int rt, int immediate) {
    if (registers[rs].num == registers[rt].num) {
        programCounter = immediate - 1;
    }
}

void bgez(int rs, int rt, int immediate) {
    if (registers[rs].num > -1) {
        programCounter = immediate - 1;
    }
}

void bgtz(int rs, int rt, int immediate) {
    if (registers[rs].num > 0) {
        programCounter = immediate - 1;
    }
}

void blez(int rs, int rt, int immediate) {
    if (registers[rs].num < 1) {
        programCounter = immediate - 1;
    }
}

void bltz(int rs, int rt, int immediate) {
    if (registers[rs].num < 0) {
        programCounter = immediate - 1;
    }
}

void bne(int rs, int rt, int immediate) {
    if (registers[rs].num != registers[rt].num) {
        programCounter = immediate - 1;
    }
}

void lb(int rs, int rt, int immediate) {
    registers[rt].num = *(registers[rs].str + immediate);
}

void lbu(int rs, int rt, int immediate)