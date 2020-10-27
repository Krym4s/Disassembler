//
// Created by egolg on 20.10.2020.
//

#include "IsE_Disassembler.h"

int readIsE_ByteCode (const char* filename, char** storage, int* nMembers)
{
    readToStorage (filename, storage);
    *nMembers = *(int*)(*storage + 8);
    *storage = (*storage + 12);
    return NO_ERRORS;
}

int translateIsE_ByteCodeToASM (char* IsE_Bytecode, const int Members, char** assemblerCode, size_t* nChars)
{
    *assemblerCode = (char*)calloc(40*sizeof(char), Members);

    for (int i = 0; i < Members;)
    {
        int commandCode = IsE_Bytecode[i++];

        switch (commandCode)
        {
            #define DEF_CPU(name,value,nParams,instructions,readArgInstruction,writeDisAsmInstruction) case value: \
                                                                strcat(*assemblerCode, #name); \
                                                                strcat(*assemblerCode, " ");   \
                                                                if (nParams == 1)              \
                                                                {                              \
                                                                    writeDisAsmInstruction     \
                                                                }strcat (*assemblerCode, "\n");\
                                                                break;


            #include "C:\Users\egolg\CLionProjects\DED_ASM\cmake-build-debug\data\processor_commit.IsCPU"

            default:
                printf("problems %x %x", i, commandCode);
            #undef DEF_CPU
        }
    }
    *nChars = strchr (*assemblerCode, '\0') - *assemblerCode;
    return NO_ERRORS;
}

int findReg (char** answerLine, int regN)
{
#define DEF_REG(name,value) case value: strcat (*answerLine, #name); break;
    switch (regN)
    {
#include "C:\Users\egolg\CLionProjects\DED_ASM\cmake-build-debug\data\registers.IsCPU"

        default: strcat (*answerLine, "There is no such register\n");
    }
}

