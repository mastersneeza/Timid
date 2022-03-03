#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_NOP,
    OP_CONSTANT, OP_CONSTANT_LONG,
    OP_TRUE, OP_FALSE, OP_NULL,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW, OP_MOD,
    OP_EQ, OP_LT, OP_GT,
    OP_NEG, OP_NOT,
    OP_RET,
    OP_DUMP
} OpCode;

typedef struct {
    int count;
    int capacity;
    int* indices;
    uint8_t* code;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int index);
void freeChunk(Chunk* chunk);
void writeConstant(Chunk* chunk, Value value, int index);
int addConstant(Chunk* chunk, Value value);

#endif