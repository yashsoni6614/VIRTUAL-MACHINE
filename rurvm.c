//here we are gonna implement a LC-3 architecture on our own
//LC-3 architecture of computer has the 16KB of addressable values and each location can store 16 bits of the data 
//this achitecture  of the computer has 8 general purpose registers and one program counter and one condition flag register
#include<stdio.h>
#include<stdint.h>
#define MEMORY_MAX (1 << 16)
// DEFINING THE MEMORY 
uint16_t memory[MEMORY_MAX];
enum {
    R_RO,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,
    R_COND,
    R_COUNT,
};
// DEFINING THE REGISTERS
uint16_t reg[R_COUNT];

int main(int argc, char const *argv[])
{
    
    return 0;
}
