#include <stdlib.h>
#include <string.h>
#include "func.h"

FILE *openMakefile(int argc, char **argv) {
    FILE *fp;
    if (argc > 2) {
        printf("Error. Too many arguments for command 'make'");
        exit(-1);
    }
    else if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            perror(argv[1]);
            exit(-1);
        }
    }
    else {
        fp = fopen("smakefile", "r");
        if (fp == NULL) {
            perror("smakefile");
            exit(-1);
        }
    }
    return fp;
}
