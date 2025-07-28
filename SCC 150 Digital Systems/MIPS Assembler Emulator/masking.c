#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

//IF BYTECODE = 0 NOP
void mask(unsigned int bytecode){
    unsigned int opcode = (bytecode >> 26);
    if(opcode == 0){
        unsigned int sr = (bytecode >> 21) & 0x1f;
        unsigned int sr2 = (bytecode >> 16) & 0x1f;
        unsigned int dr = (bytecode >> 11) & 0x1f;
        unsigned int sa = (bytecode >> 6) & 0x1f;
        unsigned int fc = bytecode & 0x3f;
    }else{
        unsigned int sr = (bytecode >> 16) & 0x1f;
        unsigned int dr = (bytecode >> 11) & 0x1f;
        unsigned int constant = bytecode  & 0xffff;
    }
}

void executeRType(unsigned int sr, unsigned int sr2, unsigned int dr, unsigned int sa, unsigned int fc){
    switch (fc)
    {
    case 0x20:
        /* code */
        break;
    case 0x00:
        /* code */
        break;
    case 0x02:
        /* code */
        break;
    default:
        break;
    }

}

void executeIType(unsigned int opcode, unsigned int sr, unsigned int dr, unsigned int constant){
    switch (opcode)
    {
    case 0x08:
        /* code */
        break;
    case 0x0C:
        /* code */
        break;
    case 0x05:
        /* code */
        break;
    case 0x25:
        /* code */
        break;
    case 0x0F:
        /* code */
        break;
    case 0x23:
        /* code */
        break;
    case 0x29:
        /* code */
        break;
    case 0x2B:
        /* code */
        break;
    default:
        break;
    }

}