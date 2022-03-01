#include "chunk.h"
#include "memory.h"
#include "stdio.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->indices = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int index) {
    if (chunk->capacity < chunk->count + 1) {

        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->indices = GROW_ARRAY(int, chunk->indices, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->indices[chunk->count] = index;
    chunk->count++;
}

void freeChunk(Chunk* chunk) {
    freeValueArray(&chunk->constants);
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->indices, chunk->capacity);
    initChunk(chunk);
}

void writeConstant(Chunk* chunk, Value value, int index) {
    int constIndex = addConstant(chunk, value);

    if (constIndex < 256) {
        writeChunk(chunk, OP_CONSTANT, index);
        writeChunk(chunk, constIndex, index);
    } else {
        writeChunk(chunk, OP_CONSTANT_LONG, index);
        writeChunk(chunk, (uint8_t) (constIndex & 0xff), index);
        writeChunk(chunk, (uint8_t) (constIndex >> 8 & 0xff), index);
        writeChunk(chunk, (uint8_t) (constIndex >> 16 & 0xff), index);
    }
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}