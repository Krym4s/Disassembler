//
// Created by egolg on 20.10.2020.
//

#ifndef DED_DISASM_ISE_DISASSMEBLER_H
#define DED_DISASM_ISE_DISASSMEBLER_H

#include "IsE_text.h"
#include "IsE_Stack.h"

enum Registers
{
#define DEF_REG(name,value) name = value,
#include "C:\Users\egolg\CLionProjects\DED_ASM\cmake-build-debug\data\registers.IsCPU"
#undef DEF_REG
};

enum AssemblerCodeErrors
{
    NO_ASSEMBLER_ERRORS             = 0,
    NULL_POINTER_ARGUMENT_ASSEMBLER = -1,
    NO_FREE_MEMORY_ASSEMBLER        = -2,
    COMPILING_ERROR                 = -3,
};

int readIsE_ByteCode (const char* filename, char** storage, int* nMembers);

int translateIsE_ByteCodeToASM (char* IsE_Bytecode, const int Members, char** assemblerCode, size_t* nChars);

int findReg (char** answerLine, int regN);

#endif //DED_DISASM_ISE_DISASSMEBLER_H
