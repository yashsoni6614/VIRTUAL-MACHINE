#ifndef RURVM_H
#define RURVM_H
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
//THESE ARE THE PLATFORM SPECIFIC DECLARATIONS OF THE LIBRARY FILES 
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    #include <conio.h> 
    

#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <sys/termios.h>
    #include <sys/mman.h>


#endif
#define MEMORY_MAX (1 << 16)

enum {
    R_R0,
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

//COMMAND SET FOR OUR VM
enum {
    OP_BR, // branch
    OP_ADD,//add
    OP_LD, //load
    OP_ST, //store
    OP_JSR,//jump register
    OP_AND,//bitwise and
    OP_LDR,//load register
    OP_STR, //store register
    OP_RTI, //we wont be using this instruction in our implementation
    OP_NOT, //bitwise not operation
    OP_LDI, //load indirect
    OP_STI, //store indirect
    OP_JMP, //jump
    OP_RES, //we wont be using this instruction in our implementation
    OP_LEA, //load effective address
    OP_TRAP //execute trap
};
//TRAP ROUTINES
enum {
    TRAP_GETC  = 0x20, //for getting the character from the keyboard 
    TRAP_OUT   = 0x21, //output a character
    TRAP_PUTS  = 0x22, //output a word string 
    TRAP_IN    = 0x23, //gets a character from the keyboard and echo it into the terminal
    TRAP_PUTSP = 0x24, //output a byte string
    TRAP_HALT  = 0x25  //halt the program 
};

//condition flags  
enum {
    FL_POS = 1<<0, //POSITIVE
    FL_ZRO = 1<<1, //ZERO
    FL_NEG = 1<<2  //NEGATIVE
};

//Special Memory mapped Registers
enum {
    MR_KBSR = 0xFE00, //keyboard status
    MR_KBDR = 0xFE02 //keyboard data
};

//these are the platform specific  declararions of some functions 
void disable_input_buffering(void);
void restore_input_buffering(void);
uint16_t check_key(void);
#endif