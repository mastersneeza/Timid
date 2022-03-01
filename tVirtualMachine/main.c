#include "stdio.h"
#include "vm.h"
#include "stdlib.h"
#include "string.h"

bool hasError = false;

size_t sizeOfFile(FILE* file) {
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

FILE* openFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) { //If file not found
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    return file;
}

uint8_t* readFile(const char* path) {
    FILE* file = openFile(path);

    size_t fileSize = sizeOfFile(file);

    uint8_t* buffer = (uint8_t*) malloc(fileSize + 1); //Allocate extra byte for null character

    if (buffer == NULL) { //If not enough RAM available
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    if (bytesRead < fileSize) { //If read fails
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0'; //Append null character

    fclose(file);
    return buffer;
}

void runFile(const char* path) {
    FILE* file = openFile(path);
    uint8_t* bytecode = readFile(path);

    interpret(bytecode, sizeOfFile(file));
}

int main(int argc, const char* argv[]) {
    initVm();

    if (argc < 2) {
        fprintf(stderr, "Usage: Timid files\n");
        exit(64);
    }

    for (int i = 1; i < argc; i++) {
        runFile(argv[i]);
        hasError = false;
    }

    freeVm();
    return 0;
}