#include "rule.h"
#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;
    /* open the makefile based on command line arguments */
    fp = openMakefile(argc, argv);

    /* Parse the file for rules, dependancies, and actions */
    
    
    
    return 0;
}
