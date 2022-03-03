#include "vm.h"
#include "stdio.h"
#include "debug.h"
#include "memory.h"
#include "math.h"
#include "chunk.h"

void push(Value value);
Value pop();
static Value peek(int distance);

static void resetStack() {
    vm.stackCount = 0;
}

void initVm() {
    vm.stack = NULL;
    vm.stackCapacity = 0;
    resetStack();
}

void freeVm() {
    FREE_ARRAY(Value, vm.stack, vm.stackCount);
    initVm();
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

static Value peek(int distance) {
    return vm.stack[vm.stackCount - distance - 1];
}

static bool truth(Value value) {
    return IS_NULL(value) || (IS_BOOL(value) && !AS_BOOL(value));
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
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;
            case OP_NULL: push(NULL_VAL); break;
            case OP_ADD: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();
                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();
                push(NUMBER_VAL(AS_NUMBER(a) + AS_NUMBER(b)));
                break;
            }
            case OP_SUB: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();
                push(NUMBER_VAL(AS_NUMBER(b) - AS_NUMBER(a)));
                break;
            }
            case OP_MUL: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();
                push(NUMBER_VAL(AS_NUMBER(a) * AS_NUMBER(b)));
                break;
            }
            case OP_DIV: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();
                push(NUMBER_VAL(AS_NUMBER(b) / AS_NUMBER(a)));
                break;
            }
            case OP_POW: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();
                push(NUMBER_VAL(pow(AS_NUMBER(b), AS_NUMBER(a))));
                break;
            }
            case OP_MOD: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();
                push(NUMBER_VAL(fmod(AS_NUMBER(b), AS_NUMBER(a))));
                break;
            }
            case OP_EQ: {
                Value a = pop();
                Value b = pop();
                push(BOOL_VAL(equals(a, b)));
                break;
            }
            case OP_LT: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();

                push(BOOL_VAL(AS_NUMBER(b) < AS_NUMBER(a)));
                break;
            }
            case OP_GT: {
                if (!(IS_NUMBER(peek(0)) || IS_NUMBER(peek(1)))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();

                    return INTERPRET_RUNTIME_ERROR;
                }
                Value a = pop();
                Value b = pop();

                push(BOOL_VAL(AS_NUMBER(b) > AS_NUMBER(a)));
                break;
            }
            case OP_NEG:
                if (!IS_NUMBER(peek(0))) {
                    printf("Incompatible types on instruction %d\n", instruction);
                    resetStack();
                    return INTERPRET_RUNTIME_ERROR;
                }
                //printf("%d\n", peek(0).type);
                push(NUMBER_VAL(-AS_NUMBER(pop()))); //Negate top value of stack
                break;
            case OP_NOT:
                push(BOOL_VAL(truth(pop())));
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

void emitByte(uint8_t byte, int index) {
    writeChunk(vm.chunk, byte, index);
}

void readBytecode(uint8_t* bytecode, size_t length) {
    for (int i = 0; i < length; i++) {
        uint8_t instruction = bytecode[i];
        switch (instruction) {
            case 0:
                emitByte(OP_NOP, i);
                break;
            case 1: {
                union {
                    uint8_t bytes[4];
                    float f;
                } u; //Bytes are based on machine's endianness

                for (int j = 1; j < 5; j++) { //Copy bytes into union
                    u.bytes[j - 1] = bytecode[i + j];
                }

                writeConstant(vm.chunk, NUMBER_VAL(u.f), i); //Add value to constant pool

                i += 4; //Skip past float values otherwise garbage values will be used
                break;
            }
            case 2:
                emitByte(OP_TRUE, i);
                break;
            case 3:
                emitByte(OP_FALSE, i);
                break;
            case 4:
                emitByte(OP_NULL, i);
                break;
            case 5:
                emitByte(OP_ADD, i);
                break;
            case 6:
                emitByte(OP_SUB, i);
                break;
            case 7:
                emitByte(OP_MUL, i);
                break;
            case 8:
                emitByte(OP_DIV, i);
                break;
            case 9:
                emitByte(OP_POW, i);
                break;
            case 10:
                emitByte(OP_MOD, i);
                break;
            case 11:
                emitByte(OP_EQ, i);
                break;
            case 12:
                emitByte(OP_LT, i);
                break;
            case 13:
                emitByte(OP_GT, i);
                break;
            case 14:
                emitByte(OP_NEG, i);
                break;
            case 15:
                emitByte(OP_NOT, i);
                break;
            case 16:
                emitByte(OP_DUMP, i);
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