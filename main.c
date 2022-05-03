#include "rule.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;
	rule_list *rules, *arg;

    /* open the makefile based on command line arguments */
    fp = fopen("Smakefile", "r");
	if (fp == NULL) {
		printf("Smakefile not found.");
		exit(-1);
	} 
    /* Parse the file for rules, dependancies, and actions */
	rules = parse_file(fp);	    
	
	/* based on arguments provided, apply proper rule */
	if (argc == 1) {
		apply_rule(&rules);
	}
 
	else if (argc == 2) {
		arg = get_rule(rules, argv[1]);
		if (arg == NULL) {
			printf("Argument not found.");
			exit(-1);
		}

		apply_rule(&arg);
	}
	else {
        printf("Error. Invalid arguments for smake.");
        exit(-1);
    }

	free_rule_list(rules);
	rules = NULL;
	
	/* Close file before returning */
    fclose(fp);
	return 0;
}
