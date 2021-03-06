//
// Created by egolg on 05.10.2020.
//

#include "IsE_Stack.h"
#include <cstring>


struct Stack
{
    unsigned long long sufferer1;

    size_t capacity;
    size_t sz;
    size_t minimumCpct;

    int errCode;
    long long stackHash;
    FILE* stackLogs;

    void* data;

    unsigned long long sufferer2;
};


//FILE* logs = fopen ("logs_stack.txt","w");

int stackConstructor (struct Stack* thou, size_t capacity, const FILE* stackLogs)
{
    assert(thou);

    thou->data = (stackType*) calloc (1, (capacity) * sizeof(stackType) + 2 * sizeof(unsigned long long) );

    if (!thou->data)
        return NO_FREE_MEMORY;

    *((unsigned long long *) thou->data) = CANARY_VALUE;
    thou->data = ((unsigned long long *) thou->data + 1);
    *((unsigned long long *)((stackType*) thou->data + capacity)) = CANARY_VALUE;

    if (!thou->data)
        return NO_FREE_MEMORY;

    thou->stackLogs = (FILE*) stackLogs;

    if (!thou->stackLogs)
        return NO_LOGS;

    thou->minimumCpct = (capacity < MINIMUM_STACK_SIZE) ? capacity : MINIMUM_STACK_SIZE;
    thou->capacity = capacity;
    thou->sz = 0;
    thou->errCode = NO_STACK_ERRORS;
    thou->sufferer1 = CANARY_VALUE;
    thou->sufferer2 = CANARY_VALUE;

    fillStackWithPoison (thou, 0,(int) capacity - 1);

    thou->stackHash = makeHash (thou);

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError(thou) asserted (numOfErrorCode (stackError (thou))));

    return 0;
}

//------------------------------------------------------------

int stackInit (struct Stack* thou)
{
    assert(thou);

    thou->data = (stackType*) calloc (1, (MINIMUM_STACK_SIZE) * sizeof(stackType) + 2 * sizeof(unsigned long long) );

    if (!thou->data)
        return NO_FREE_MEMORY;

    *((unsigned long long *) thou->data) = CANARY_VALUE;
    thou->data = ((unsigned long long *) thou->data + 1);
    *((unsigned long long *)((stackType*) thou->data + MINIMUM_STACK_SIZE)) = CANARY_VALUE;

    if (!thou->data)
        return NO_FREE_MEMORY;

    thou->stackLogs = fopen ("stack_logs", "w");

    if (!thou->stackLogs)
        return NO_LOGS;

    thou->minimumCpct = MINIMUM_STACK_SIZE;
    thou->capacity = MINIMUM_STACK_SIZE;
    thou->sz = 0;
    thou->errCode = NO_STACK_ERRORS;
    thou->sufferer1 = CANARY_VALUE;
    thou->sufferer2 = CANARY_VALUE;

    fillStackWithPoison (thou, 0,(int) MINIMUM_STACK_SIZE - 1);

    thou->stackHash = makeHash (thou);

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError(thou) asserted (numOfErrorCode (stackError (thou))));

    return 0;
}

//-----------------------------------------------------------

void stackDestructor (struct Stack* thou)
{
    assert(thou);
    free ((stackType*)((unsigned long long*) thou->data - 1));
    thou->data = NULL;

    free (thou);
    thou = NULL;

}

//------------------------------------------------------------

int pushIntoStack (struct Stack* thou, const stackType newMember)
{
    assert (thou);

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError(thou) asserted (numOfErrorCode (stackError (thou))));

    if (thou->sz == thou->capacity)
        if (enlargeStack(thou))
        {
            stackDump (thou,ARGNAME (NO_FREE_MEMORY), __LINE__);
            return NO_FREE_MEMORY;
        }


    *((stackType*) thou->data + (thou->sz++)) = newMember;
    //printf("%lg\n",*((stackType*)thou->data + (thou->sz-1)));
    thou->stackHash = makeHash (thou);

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

    return NO_FUNCTION_ERRORS;
}

//-------------------------------------------------------

int enlargeStack (struct Stack* thou)
{
    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError(thou) asserted(numOfErrorCode (stackError (thou))));

    if (resizeConfirm(!stackResize (thou, (size_t) (thou->capacity * ENLARGE_BIG_COEFFICIENT)), thou))
        return NO_FUNCTION_ERRORS;

    if (resizeConfirm(!stackResize (thou, (size_t) (thou->capacity * ENLARGE_SMALL_COEFFICIENT)), thou))
        return NO_FUNCTION_ERRORS;

    if (resizeConfirm(!stackResize (thou, (size_t) (thou->capacity) + 100), thou))
        return NO_FUNCTION_ERRORS;

    if (resizeConfirm(!stackResize (thou, (size_t) (thou->capacity) + 1), thou))
        return NO_FUNCTION_ERRORS;

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError (thou) asserted (numOfErrorCode (stackError (thou))));

    return NO_FREE_MEMORY;
}

//----------------------------------------------------------------

int reduceStack (struct Stack* thou)
{
    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError(thou) asserted (numOfErrorCode (stackError (thou))));

    if (!stackResize(thou, thou->capacity * REDUCE_COEFFICIENT))
    {
        thou->stackHash = makeHash (thou);
        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );

        return  NO_FUNCTION_ERRORS;
    }

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError(thou) asserted(numOfErrorCode(stackError(thou))));

    return NO_FREE_MEMORY;
}

//----------------------------------------------------------------

stackType popFromStack (struct  Stack* thou)
{
    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

    if (stackError (thou))
    {
        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
        assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

        return poison;
    }

    if (thou->sz > 0)
    {
        stackType returnMember;
        thou->sz--;
        returnMember = *((stackType*)thou->data + thou->sz);
        fillStackWithPoison(thou, thou->sz, thou->sz);
        thou->stackHash = makeHash (thou);

        if ( thou->sz <= thou->capacity * REDUCE_COEFFICIENT && thou->capacity * REDUCE_COEFFICIENT > thou->minimumCpct)
            reduceStack(thou);

        thou->stackHash = makeHash (thou);

        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
        assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

        return returnMember;
    }

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

    return poison;
}

//-----------------------------------------------------------------------

int stackError (struct Stack* thou)
{
    if (!thou)
        return NULL_POINTER_IN_ARGUMENT;

    if (thou->sz > thou->capacity)
    {
        thou->errCode = BAD_SIZE;
        return BAD_ARGUMENT;
    }

    for (size_t ind = thou->sz; ind < thou->capacity; ++ind )
        if (isfinite (*((stackType*) thou->data + ind)))
        {
            thou->errCode = UNACCEPTABLE_Member;
            return BAD_ARGUMENT;
        }

    if (thou->minimumCpct == 0 || thou->minimumCpct > thou->capacity || thou->capacity == 0)
    {
        thou->errCode = BAD_MINIMUM_SIZE;
        return BAD_MINIMUM_SIZE;
    }


    if (NCAN (thou->sufferer1,thou->sufferer2, *((unsigned long long*) thou->data - 1),*((unsigned long long*) thou->data + thou->capacity)))
    {
        thou->errCode = CANARY_VALUE_CHANGED;
        return CANARY_VALUE_CHANGED;
    }


    if (thou->stackHash != makeHash (thou))
    {
        thou->errCode = BAD_HASH;
        return BAD_HASH;
    }

    if (!thou->data)
        return NO_DATA;   // mistake for conqueror

    return  NO_STACK_ERRORS;
}

//-------------------------------------------------------------------------

int stackResize (struct  Stack* thou, const size_t newCapacity)
{
    void* temp = ((unsigned long long*)thou->data) - 1;
    temp = (void*) realloc (temp , sizeof(stackType) * newCapacity + 2 * sizeof(unsigned long long));

    if (!temp)
    {
        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
        return NO_FREE_MEMORY;
    }
    else
    {
        *(unsigned long long*)temp = CANARY_VALUE;
        thou->data = (unsigned long long*) temp + 1;
        *(unsigned long long*) ((stackType*) thou->data + newCapacity) = CANARY_VALUE;

        thou->capacity = newCapacity;
        thou->stackHash = makeHash(thou);
        

        return NO_FUNCTION_ERRORS;
    }
}

//-----------------------------------------------------------------------------

void fillStackWithPoison ( struct Stack* thou, const int beginInd, const int endInd)
{
    for (int ind = beginInd; ind <= endInd; ++ind)
        *((stackType*) thou->data + ind) = poison;
}

//--------------------------------------------------------------------------

int stackDump (struct Stack* thou, char* problem, const int line)
{
    if (!thou)
    {
        fprintf(thou->stackLogs, "ERROR_Stack(NULL_PTR");
        return NULL_POINTER_IN_ARGUMENT;
    }
    char marker = '*';
    int sz = thou->sz;
    fprintf (thou->stackLogs, "executed on line %d\n",   line);
    fprintf (thou->stackLogs, "Stack(%s) [%p] \n { \n",  problem, thou);
    fprintf (thou->stackLogs, "\t" "sufferer1 = %llx\n", thou->sufferer1);
    fprintf (thou->stackLogs, "\t" "minimumSz = %zu\n",  thou->minimumCpct);
    fprintf (thou->stackLogs, "\t" "errcode   = %d\n",   thou->errCode);
    fprintf (thou->stackLogs, "\t" "stackHash = %llu\n", thou->stackHash);
    fprintf (thou->stackLogs, "\t" "sz        = %zu\n",  thou->sz);
    fprintf (thou->stackLogs, "\t" "capacity  = %zu\n",  thou->capacity);
    fprintf (thou->stackLogs, "\t" "data [%p]\n",        thou->data);
    fprintf (thou->stackLogs, "\t\t" "{\n");
    fprintf (thou->stackLogs, "\t\t" "canary1 = %llx\n", ((unsigned long long*) thou->data)[-1]);

    for (int ind = 0; ind < thou->capacity; ++ind)
    {
        if (ind >= thou->sz)
            marker = ' ';
        fprintf (thou->stackLogs, "\t\t" "%c[%d] %lg\n",marker, ind, ((stackType*) thou->data)[ind]);
    }
    fprintf (thou->stackLogs, "\t\t" "canary2 = %llx\n", ((unsigned long long*) thou->data)[thou->capacity]);
    fprintf (thou->stackLogs, "\t\t" "}\n");
    fprintf (thou->stackLogs, "\t" "sufferer2 = %llx\n", thou->sufferer2);
    fprintf (thou->stackLogs, "}\n");
    fflush (thou->stackLogs);

    return NO_FUNCTION_ERRORS;
}

//------------------------------------------------------------------------

long long makeHash (struct Stack* thou)
{
    long long oldHash = thou->stackHash;
    thou->stackHash = 0;
    long long newHash = 0;

    char* stackBuff = (char*) thou;
    for (int ind = 0; ind < sizeof(*thou); ++ind)
        newHash = ROR (newHash) + stackBuff[ind];

    char* dataBuff = (char*) thou->data;
    for (int ind = 0; ind < sizeof(*(stackType*) thou->data); ++ind)
        newHash = ROR (newHash) + dataBuff[ind];

    thou->stackHash = oldHash;

    return newHash;
}

//-----------------------------------------------------------

long long ROL (long long value)
{
    return (value << 1 | value >> 63);
}

//--------------------------------------------------

long long ROR (long long value)
{
    return (value >> 1 | value << 63);
}

//-------------------------------------------------

char* numOfErrorCode (const int errorCode)
{
    switch (errorCode)
    {
        case NO_STACK_ERRORS:      return ARGNAME(NO_STACK_ERRORS);
        case BAD_SIZE:             return ARGNAME(BAD_SIZE);
        case UNACCEPTABLE_Member:  return ARGNAME(UNACCEPTABLE_Member);
        case BAD_MINIMUM_SIZE:     return ARGNAME(BAD_MINIMUM_SIZE);
        case CANARY_VALUE_CHANGED: return ARGNAME(CANARY_VALUE_CHANGED);
        case BAD_HASH:             return ARGNAME(BAD_HASH);

        default:
            char* str;
            sprintf(str, "Error function returned code %d. Stack can`t have this status\n", errorCode);
            return str;
    }
}

//-------------------------------------------------

struct Stack* newStack()
{
    struct Stack* temp = (struct Stack*) calloc(1, sizeof(*temp));
    if (!temp)
        return NULL;

    return temp;
}

//--------------------------------------------------

stackType topOfStack (struct Stack* thou)
{
    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert(!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

    if (stackError(thou))
    {
        assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));
        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
        return poison;
    }


    if (thou->sz > 0){
        stackType returnMember;
        thou->sz;
        returnMember = *((stackType*) thou->data + thou->sz - 1);

        assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));
        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );

        return returnMember;
    }

    !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );
    assert (!stackError (thou) asserted(numOfErrorCode (stackError (thou))));

    return poison;
}

//-----------------------------------------------------------------------

int resizeConfirm (const int condition, struct Stack* thou)
{
    size_t oldCapacity = thou->capacity;
    if (condition)
    {
        fillStackWithPoison (thou, oldCapacity, thou->capacity - 1);
        thou->stackHash = makeHash (thou);
        !stackError(thou) || stackDump (thou, numOfErrorCode (stackError(thou)), __LINE__ );

        return  NO_FUNCTION_ERRORS;
    }

    return  NO_FREE_MEMORY;
}


