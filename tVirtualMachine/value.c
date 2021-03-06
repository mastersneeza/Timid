#include "value.h"
#include "memory.h"
#include <stdio.h>

bool equals(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case V_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case V_NULL:
            return true;
        case V_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        default:
            return false;
    }
}

void initValueArray(ValueArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray* array, Value value) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

void printValue(Value value) {
    switch (value.type) {
        case V_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case V_NULL:
            printf("null");
            break;
        case V_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
    }
}