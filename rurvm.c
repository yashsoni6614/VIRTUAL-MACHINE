//here we are gonna implement a LC-3 architecture on our own
//LC-3 architecture of computer has the 16KB of addressable values and each location can store 16 bits of the data 
//this achitecture  of the computer has 8 general purpose registers and one program counter and one condition flag register
#include"headers.h"

//OUR MAIN MEMORY FOR THIS ARCHITECTURE
uint16_t memory[MEMORY_MAX];

// unsigned 16 bit REGISTERS SET 
uint16_t reg[R_COUNT];


//function declarations
uint16_t mem_read(uint16_t);
uint16_t sign_extend(uint16_t,int);
void mem_write(uint16_t,uint16_t);
void update_flags(uint16_t );
void func_ADD (const uint16_t*);
void func_LDI(const uint16_t*);
void func_AND(const uint16_t*);
void func_NOT(const uint16_t*);
void func_BR(const uint16_t*);
void func_JMP(const uint16_t*);
void func_JSR(const uint16_t*);
void func_LD(const uint16_t*);
void func_LDR(const uint16_t*);
void func_LEA(const uint16_t*);
void func_ST(const uint16_t*);
void func_STI(const uint16_t*);
void func_STR(const uint16_t*);

//TRAP SUBROUTINES
void func_TRAP_GETC();
void func_TRAP_OUT();
void func_TRAP_PUTS();
void func_TRAP_IN();
void func_TRAP_PUTSP();
void func_TRAP_HALT(int*);
uint16_t swap16(uint16_t);
void read_image_file(FILE*);
int read_image(const char*);
void handle_interrupt(int);
//handle interrupt 


int main(int argc, char const *argv[])
{   
    //basic error handling 111 of arguements is happening there
    if(argc <2 )//no machine code is given 
    {   
        printf("Usage:\n");
        printf("rurvm [image-file1] ....\n");
        exit(2);
    }

    for(int j = 1;j<argc;++j) {
        if(!read_image(argv[j])) {
            printf("Failed to load the image: %s\n",argv[j]);
            exit(1);
        }
    }

    signal(SIGINT, handle_interrupt);
    disable_input_buffering();


    reg[R_COND] = FL_ZRO; //initially load the conditional flag register with the zero flag

    // in LC-3 user editable programs starts from the location 0x3000 so we have to load the program counter with this memory address
   
    //now the main execution loop will start from here 
    int running = 1;
    while(running) {
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr>>12; //this is for fetching the opcode the last 4 bits are for the opcode 

        switch(op) {
            case OP_ADD:
                func_ADD(&instr);
                break;
            case OP_AND:
                func_AND(&instr);
                break;
            case OP_NOT:
                func_NOT(&instr);
                break;
            case OP_BR:
                func_BR(&instr);
                break;
            case OP_JMP:
                func_JMP(&instr);
                break;
            case OP_JSR:
                func_JSR(&instr);
                break;
            case OP_LD:
                func_LD(&instr);
                break;
            case OP_LDI:
                func_LDI(&instr);
                break;
            case OP_LDR:
                func_LDR(&instr);
                break;
            case OP_LEA:
                func_LEA(&instr);
                break;
            case OP_ST:
                func_ST(&instr);
                break;
            case OP_STI:
                func_STI(&instr);
                break;
            case OP_STR:
                func_STR(&instr);
                break;
            case OP_TRAP:
                    reg[R_R7] = reg[R_PC];

                    switch (instr & 0xFF) {
                        case TRAP_GETC:
                            func_TRAP_GETC();
                            break;
                        case TRAP_OUT:
                            func_TRAP_OUT();
                            break;
                        case TRAP_PUTS:
                            func_TRAP_PUTS();
                            break;
                        case TRAP_IN:
                            func_TRAP_IN();
                            break;
                        case TRAP_PUTSP:
                            func_TRAP_PUTSP();
                            break;
                        case TRAP_HALT:
                            func_TRAP_HALT(&running);
                            break;
                    } 
                break;
            case OP_RES: abort(); break;
            case OP_RTI: abort(); break;
            default:
                //function for handling the bad opcode 
                break;
        }
    }
    //shutdown function 
    
    restore_input_buffering();
    return 0;
}

uint16_t mem_read(uint16_t address) {

    if(address == MR_KBSR) {
        if(check_key()) {
            memory[MR_KBSR] = (1<<15);
            memory[MR_KBDR] = getchar();
        }
        else {
            memory[MR_KBSR] = 0;
        }
    }
    return memory[address];
}

void mem_write(uint16_t address,uint16_t val) {
    memory[address] = val;
}

//this function is basically used for handling the immediate numbers sign 
// we are storing a 5 bit negative number in a 16 bit unsigned storage so we have to carefully and manually flip the 
// the remaining 11 bit to 1 otherwise the ALU will treat it as a positive number 

uint16_t sign_extend(uint16_t x,int bit_count){

    if((x>>(bit_count-1))&1) {
        x = x | (0xFFFF<<bit_count);
    }
    return x;
}

void update_flags(uint16_t r) {

    if(reg[r] == 0) {
        reg[R_COND] = FL_ZRO; 
    }
    else if(reg[r]>>15) {
        reg[R_COND] = FL_NEG;
    }
    else {
        reg[R_COND] = FL_POS;
    }
}

void func_ADD(const uint16_t* instr) {

    uint16_t r0 = (*instr >> 9) & 0x7; //destinition register
    // operand 1 
    uint16_t r1 = (*instr >> 6) & 0x7;
    // checking the mode 
    uint16_t imm_flag = (*instr >> 5) & 0x1;

    if(imm_flag) {
        uint16_t imm5 = sign_extend(*instr & 0x1F , 5);
        reg[r0] = reg[r1] + imm5;
    }
    else {
        uint16_t r2 = *instr & 0x7;
        reg[r0] = reg[r1]+reg[r2];
    }
    update_flags(r0);
}

void func_LDI(const u_int16_t* instr) {

    uint16_t r0 = (*instr >> 9) & 0x7; //DR
    uint16_t pc_offset = sign_extend(*instr & 0xFF,9); //our PC reg offset

    //basically in this instruction we are haviing given an offset which points to a location which contains the
    //memory address of the data which is to be loaded into the DR 
    //it is similar to the double pointers in C 

    reg[r0] = mem_read(mem_read(reg[R_PC]+pc_offset));
    update_flags(r0);
}


void func_AND(const uint16_t* instr) {
    uint16_t r0 = (*instr>>9) & 0x7; //DR
    uint16_t r1 = (*instr>>6) & 0x7; //SR1
    uint16_t imm_flag = (*instr>>5) & 0x1; //mode selection

    if(imm_flag) {
        uint16_t imm5 = sign_extend(*instr & 0x1F,5);
        reg[r0] = reg[r1] & imm5;
    }
    else {
        uint16_t r2 = (*instr & 0x7);
        reg[r0] = reg[r1] & reg[r2];
    }
    update_flags(r0);
}

void func_NOT(const uint16_t* instr) {
    uint16_t r0 =(*instr >>9) & 0x7;
    uint16_t r1 = (*instr >>6) & 0x7;
    reg[r0] = ~reg[r1];
    update_flags(r0);
}

//basically a branching function mostly used while looping
void func_BR(const u_int16_t* instr) {
    uint16_t pc_offset = sign_extend(*instr & 0x1FF,9);
    uint16_t cond_flag = (*instr >> 9) & 0x7;
    if(cond_flag & reg[R_COND]) {
        reg[R_PC] += pc_offset;
    }
}

void func_JMP(const uint16_t* instr) {
    uint16_t r1 = (*instr >> 6) & 0x7;
    reg[R_PC] = reg[r1];
}

void func_JSR(const uint16_t* instr) {
    uint16_t flag = (*instr >> 11) & 1;
    reg[R_R7] = reg[R_PC];
    if(flag) {
        uint16_t pc_offset = sign_extend(*instr & 0x7FF ,11);
        reg[R_PC] += pc_offset;
    }
    else {
        uint16_t r1 = (*instr >> 6) & 0x7;
        reg[R_PC] = reg[r1];
    }
}

void func_LD(const uint16_t* instr) {
    uint16_t r0 = (*instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(*instr & 0x1FF,9);
    reg[r0] = mem_read(reg[R_PC] + pc_offset);
    update_flags(r0);
}

//*
void func_LDR(const uint16_t* instr) {
    uint16_t r0 = (*instr >> 9) & 0x7;
    uint16_t r1 = (*instr >> 6) & 0x7;
    uint16_t offset = sign_extend(*instr & 0x3f,6);
    reg[r0] = mem_read(reg[r1]+offset);
    update_flags(r0); 
}

void func_LEA(const uint16_t* instr) {
    uint16_t r0 = (*instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(*instr & 0x1FF,9);
    reg[r0] = reg[R_PC] + pc_offset;
    update_flags(r0);
}

//*
void func_ST(const uint16_t* instr) {
    uint16_t r0 = (*instr>>9)&0x7;
    uint16_t pc_offset = sign_extend(*instr & 0x1FF,9);
    mem_write(reg[R_PC] + pc_offset , reg[r0]);
}

void func_STI(const uint16_t* instr) {
    uint16_t r0 = (*instr >>9) &0x7;
    uint16_t pc_offset = sign_extend(*instr & 0x1FF,9);
    mem_write(mem_read(reg[R_PC]+pc_offset),reg[r0]);
}

void func_STR(const uint16_t* instr) {
    uint16_t r0 = (*instr >> 9) & 0x7;
    uint16_t r1 = (*instr >> 6) & 0x7;
    uint16_t offset = sign_extend(*instr & 0x3F,6);
    mem_write(reg[r1]+offset,reg[r0]);
}

//TRAP SUBROUTINES 

void func_TRAP_PUTS () {
    uint16_t* c = memory + reg[R_R0];
    while(*c) {
        putc((char)*c,stdout);
        ++c;
    }
    //there is the need to flush entire thing into the stdout because putc stores everything into the buffer first 
    //untill it encounters a new line or the end of the file stuff but in our case it not might be ending with this 
    //so ultimately we are just flushing it out into the stdout regardless
    fflush(stdout); 
}

void func_TRAP_GETC () {
    reg[R_R0] = (uint16_t)getchar();
    update_flags(R_R0);
}

void func_TRAP_OUT () {
    putc((char)reg[R_R0],stdout);
    fflush(stdout);
}

void func_TRAP_IN () {
    printf("Enter a character: ");
    char c = getchar();
    putc(c,stdout);
    fflush(stdout);
    reg[R_R0] = (uint16_t)c;
    update_flags(R_R0);
}

void func_TRAP_PUTSP () {
    //instead of storing the character which requires only 8 bytes at max we are storing it into 
    //the 16 bit space which is obviously the waste of space and in order to handle this thing 
    //what we are doing is we are using a word space to store 2 characters instead 
    uint16_t* c = memory + reg[R_R0];
    while(*c) {
        char char1 = (*c) & 0xFF;
        putc(char1,stdout);
        char char2 = (*c) >>8;
        if(char2) putc(char2,stdout);
        ++c; 
    }
    fflush(stdout);
}

void func_TRAP_HALT (int * running) {
    puts("HALT");
    fflush(stdout);
    *running = 0;
}

uint16_t swap16(uint16_t x) {
    return (x << 8) | (x>>8);
}

//reading the image file 

void read_image_file(FILE* file) {
    uint16_t origin; //this gonna store the base address for the machine code to be stored
    fread(&origin,sizeof(origin),1,file);//this is for reading the file for only first 2 bytes 
    origin = swap16(origin);//this is basically swapping 2 bytes so that the innate big-endian of LC assembler gets to convert into the little endian as specified by modern microprocessors 
    reg[R_PC] = origin;
    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t* p = memory + origin;
    size_t read = fread(p,sizeof(uint16_t),max_read,file);

    while(read-->0) {
        *p  = swap16(*p);
        ++p;
    }
}

int read_image(const char* image_path) {
    FILE* file  = fopen(image_path,"rb");
    if(!file) {return 0;}
    read_image_file(file);
    fclose(file);
    return 1;
}
void handle_interrupt(int signal)
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}