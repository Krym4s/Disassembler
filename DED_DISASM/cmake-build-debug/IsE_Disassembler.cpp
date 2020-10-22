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
            #define DEF_CPU(name,value,nParams,instructions) case value: \
                                                                strcat(*assemblerCode, #name); \
                                                                strcat(*assemblerCode, " ");\
                                                                if (nParams == 1)  \
                                                                {        \
                                                                    char mode = IsE_Bytecode[i++]; \
                                                                    if (mode == 1)            \
                                                                    {\
                                                                        char reg = IsE_Bytecode[i++]; \
                                                                        switch (reg) {\
                                                                            case rax:         \
                                                                                strcat(*assemblerCode, "rax"); break; \
                                                                            case rbx:             \
                                                                                strcat(*assemblerCode, "rbx"); break;\
                                                                            case rcx:             \
                                                                                strcat(*assemblerCode, "rcx"); break;\
                                                                            case rdx:             \
                                                                                strcat(*assemblerCode, "rdx"); break;\
                                                                        }\
                                                                    } else if (!strcmp(#name, "PUSH"))\
                                                                    {    \
                                                                        double* dValue =(double*) (IsE_Bytecode + i);     \
                                                                        i += 8;                \
                                                                        char strValue[20] = {};\
                                                                        sprintf (strValue, "%lg", *dValue);            \
                                                                        strcat (*assemblerCode, strValue);\
                                                                        \
                                                                    }    \
                                                                    \
                                                                    \
                                                                }strcat (*assemblerCode, "\n");\
                                                                break;


            #include "C:\Users\egolg\CLionProjects\DED_ASM\cmake-build-debug\data\processor_commit.IsCPU"

            default:
                printf("problems %d %d", i, commandCode);
            #undef DEF_CPU
        }
    }
    *nChars = strchr (*assemblerCode, '\0') - *assemblerCode;
    return NO_ERRORS;
}

