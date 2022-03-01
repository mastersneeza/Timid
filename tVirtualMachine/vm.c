#include "vm.h"
#include "stdio.h"
#include "debug.h"
#include "memory.h"
#include "math.h"
#include "chunk.h"

void resetStack() {
    vm.stackCount = 0;
}

void initVm() {
    vm.stack = NULL;
    vm.stackCapacity = 0;
    resetStack();
}

void freeVm() {

}

Value pop() {
    vm.stackCount--;
    return vm.stack[vm.stackCount];
}

void push(Value value) {
    if (vm.stackCapacity < vm.stackCount + 1) {
        int oldCapacity = vm.stackCapacity;
        vm.stackCapacity = GROW_CAPACITY(oldCapacity);
        vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
    }

    vm.stack[vm.stackCount] = value;
    vm.stackCount++;
}

InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    #define READ_CONSTANT_LONG() (vm.chunk->constants.values[READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16)])

    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stack + vm.stackCount; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                push(constant);
                break;
            }
            case OP_ADD: {
                Value a = pop();
                Value b = pop();
                push(a + b);
                break;
            }
            case OP_SUB: {
                Value a = pop();
                Value b = pop();
                push(b - a);
                break;
            }
            case OP_MUL: {
                Value a = pop();
                Value b = pop();
                push(a * b);
                break;
            }
            case OP_DIV: {
                Value a = pop();
                Value b = pop();
                push(b / a);
                break;
            }
            case OP_POW: {
                Value a = pop();
                Value b = pop();
                push(pow(b, a));
                break;
            }
            case OP_MOD: {
                Value a = pop();
                Value b = pop();
                push(fmod(b, a));
                break;
            }
            case OP_NEG:
                push(-pop()); //Negate top value of stack
                break;
            case OP_DUMP:
                printValue(pop()); //Pops top value and prints it, acts as a temporary end-of-program
                printf("\n");
                return INTERPRET_OK;
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef READ_CONSTANT_LONG
}

void readBytecode(uint8_t* bytecode, size_t length) {
    for (int i = 0; i < length; i++) {
        uint8_t instruction = bytecode[i];
        switch (instruction) {
            case 0:
                writeChunk(vm.chunk, OP_NOP, i);
                break;
            case 1: {
                union {
                    uint8_t bytes[4];
                    float f;
                } u; //Bytes are based on machine's endianness

                for (int j = 1; j < 5; j++) { //Copy bytes into union
                    u.bytes[j - 1] = bytecode[i + j];
                }

                writeConstant(vm.chunk, u.f, i); //Add value to constant pool

                i += 4; //Skip past float values otherwise garbage values will be used
                break;
            }
            case 2:
                writeChunk(vm.chunk, OP_ADD, i);
                break;
            case 3:
                writeChunk(vm.chunk, OP_SUB, i);
                break;
            case 4:
                writeChunk(vm.chunk, OP_MUL, i);
                break;
            case 5:
                writeChunk(vm.chunk, OP_DIV, i);
                break;
            case 6:
                writeChunk(vm.chunk, OP_POW, i);
                break;
            case 7:
                writeChunk(vm.chunk, OP_MOD, i);
                break;
            case 8:
                writeChunk(vm.chunk, OP_NEG, i);
                break;
            case 9:
                writeChunk(vm.chunk, OP_DUMP, i);
                break;
            default:
                printf("Unkown opcode %d\n", instruction);
                break;
        }
    }
}

InterpretResult interpret(uint8_t* bytecode, size_t length) {
    Chunk chunk;
    initChunk(&chunk);

    vm.chunk = &chunk;

    readBytecode(bytecode, length); //Convert bytecode into instructions

    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}