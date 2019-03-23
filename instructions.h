//
// Created by nc_lab on 19-3-21.
//

#ifndef ASSIGNMENT2_INSTRUCTIONS_H
#define ASSIGNMENT2_INSTRUCTIONS_H

extern int programCounter;

union fourbytes{
    unsigned int uNum;
    char* str;
    int num;
};

extern union fourbytes registers[34];

void add(int rd, int rs, int rt, int sa);

void addu(int rd, int rs, int rt, int sa);

void and(int rd, int rs, int rt, int sa);

void mipsDiv(int rd, int rs, int rt, int sa);

void divu(int rd, int rs, int rt, int sa);

void jr(int rd, int rs, int rt, int sa);

void mfhi(int rd, int rs, int rt, int sa);

void mflo(int rd, int rs, int rt, int sa);

void mthi(int rd, int rs, int rt, int sa);

void mtlo(int rd, int rs, int rt, int sa);

void mult(int rd, int rs, int rt, int sa);

void multu(int rd, int rs, int rt, int sa);

void nor(int rd, int rs, int rt, int sa);

void or(int rd, int rs, int rt, int sa);

void sll(int rd, int rs, int rt, int sa);

void slt(int rd, int rs, int rt, int sa);

void sltu(int rd, int rs, int rt, int sa);

void srl(int rd, int rs, int rt, int sa);

void sub(int rd, int rs, int rt, int sa);

void subu(int rd, int rs, int rt, int sa);

void xor(int rd, int rs, int rt, int sa);

void syscall(int rd, int rs, int rt, int sa);

void addi(int rs, int rt, int immediate);

void addiu(int rs, int rt, int immediate);

void andi(int rs, int rt, int immediate);

void beq(int rs, int rt, int immediate);

void bgez(int rs, int rt, int immediate);

void bgtz(int rs, int rt, int immediate);

void blez(int rs, int rt, int immediate);

void bltz(int rs, int rt, int immediate);

void bne(int rs, int rt, int immediate);

void lb(int rs, int rt, int immediate);

void lbu(int rs, int rt, int immediate);

void lui(int rs, int rt, int immediate);

void lw(int rs, int rt, int immediate);

void ori(int rs, int rt, int immediate);

void sb(int rs, int rt, int immediate);

void slti(int rs, int rt, int immediate);

void sltiu(int rs, int rt, int immediate);

void sw(int rs, int rt, int immediate);

void xori(int rs, int rt, int immediate);

void j(int target);

void jal(int target);

#endif //ASSIGNMENT2_INSTRUCTIONS_H