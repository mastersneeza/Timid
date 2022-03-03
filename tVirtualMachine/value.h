#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef enum {
    V_NUMBER,
    V_BOOL,
    V_NULL
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        float number;
    } as;

} Value;

#define IS_NUMBER(value)    ((value).type == V_NUMBER)
#define IS_BOOL(value)      ((value).type == V_BOOL)
#define IS_NULL(value)      ((value).type == V_NULL)

#define BOOL_VAL(value)     ((Value){V_BOOL, {.boolean = value}})
#define NUMBER_VAL(value)   ((Value){V_NUMBER, {.number = value}})
#define NULL_VAL            ((Value){V_NULL, {.boolean = 0}})

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)

typedef struct {
    int count;
    int capacity;
    Value* values;
} ValueArray;

bool equals(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif