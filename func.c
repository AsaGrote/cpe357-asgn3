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
        fp = fopen("Smakefile", "r");
        if (fp == NULL) {
            perror("smakefile");
            exit(-1);
        }
    }
    return fp;
}

char *read_long_line(FILE *file) {
    char *line = NULL;
    char *temp = NULL;
    int size = 0;
    char c = 0;

    while (1) {
        c = getc(file);

        temp = (char *)realloc(line, (size + 10)*sizeof(char));
        line = temp;
        temp = NULL;

        if (feof(file) || c == '\n') {
            line[size] = '\0';
            return line;
        }

        line[size] = c;
        size++;
    }

    return line;
}
