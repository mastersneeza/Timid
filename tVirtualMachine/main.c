#include "stdio.h"
#include "vm.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "common.h"
#include "getopt.h"

static bool hasError = false;
static char* version = "1";

size_t sizeOfFile(FILE* file) {
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

FILE* openFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) { //If file not found
        fprintf(stderr, "File Not Found Error: File \"%s\" either does not exist or has been moved to another location\n", path);
        hasError = true;
        return NULL;
    }

    return file;
}

uint8_t* readFile(const char* path) {
    FILE* file = openFile(path);

    size_t fileSize = sizeOfFile(file);

    uint8_t* buffer = (uint8_t*) malloc(fileSize + 1); //Allocate extra byte for null character

    if (buffer == NULL) { //If not enough RAM available
        fprintf(stderr, "Memory Error: Not enough RAM to open file \"%s\"\n", path);
        hasError = true;
        return NULL;
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    if (bytesRead < fileSize) { //If read fails
        fprintf(stderr, "Error: Could not read file \"%s\"\n", path);
        hasError = true;
        return NULL;
    }

    buffer[bytesRead] = '\0'; //Append null character

    fclose(file);
    return buffer;
}

void runFile(const char* path) {
    FILE* file = openFile(path);
    if (hasError) {
        hasError = false;
        return;
    }
    uint8_t* bytecode = readFile(path);
    if (hasError) {
        hasError = false;
        return;
    }

    InterpretResult result = interpret(bytecode, sizeOfFile(file));
    if (result == INTERPRET_RUNTIME_ERROR) {
        hasError = true;
    }
}

void printUsage() {
    fprintf(stderr, "Timid Runtime version %s\n", version);
    fprintf(stderr, "Usage: TimidRuntime [-d | --dev] [-h | --help] [-v | --version] <.timb files>\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "-d, --dev:\tenables debug messages\n");
    fprintf(stderr, "-h, --help:\tprints this help message\n");
    fprintf(stderr, "-v, --version:\tprints Timid Runtime's version\n");
}

int main(int argc, char* const argv[]) {
    initVm();

    static const struct option longopts[] = {
        {.name = "dev", .has_arg = no_argument, .val = 'd'},
        {.name = "help", .has_arg = no_argument, .val = 'h'},
        {.name = "version", .has_arg = no_argument, .val = 'v'},
    };

    int option;

    if (argc < 2) {
        printUsage();
    }

    while ((option = getopt_long(argc, argv, "dhv", longopts, NULL)) != -1) {
        switch (option) {
            case 'd':
                #define DEBUG_TRACE_EXECUTION
                break;
            case 'h':
                printUsage();
                exit(EXIT_SUCCESS);
            case 'v':
                printf("Timid Runtime version %s\n", version);
                exit(EXIT_SUCCESS);
            case '?':
                fprintf(stderr, "Unknown option: %c\n", option);
                break;
            default:
                printUsage();
                exit(EXIT_FAILURE);
        }
    }

    for(; optind < argc; optind++){ //when some extra arguments are passed
        runFile(argv[optind]);
        hasError = false;
    }

    freeVm();
    return 0;
}