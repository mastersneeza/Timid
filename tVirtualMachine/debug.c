#include "stdio.h"
#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];

    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int longConstantInstruction(const char* name, Chunk* chunk, int offset) {
    uint32_t constant = (chunk->code[offset + 1]) | (chunk->code[offset + 2] << 8) | (chunk->code[offset + 3] << 16);
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d index %d ", offset, chunk->indices[offset]);

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUB:
            return simpleInstruction("OP_SUB", offset);
        case OP_MUL:
            return simpleInstruction("OP_MUL", offset);
        case OP_DIV:
            return simpleInstruction("OP_DIV", offset);
        case OP_POW:
            return simpleInstruction("OP_POW", offset);
        case OP_MOD:
            return simpleInstruction("OP_MOD", offset);
        case OP_NEG:
            return simpleInstruction("OP_NEG", offset);
        case OP_DUMP:
            return simpleInstruction("OP_DUMP", offset);
        case OP_RET:
            return simpleInstruction("OP_RET", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}