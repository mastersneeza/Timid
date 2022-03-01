#ifndef VM_H
#define VM_H

#include "chunk.h"

typedef enum {
    INTERPRET_OK,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

typedef struct {
    Chunk* chunk;
    uint8_t* ip; //Instruction pointer

    Value* stack;
    int stackCount;
    int stackCapacity;
} VM;

VM vm;

void initVm();
void freeVm();

InterpretResult interpret(uint8_t* bytecode, size_t length);

#endif