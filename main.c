#include "rule.h"
#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;
	rule_list *rules, *arg;

    /* open the makefile based on command line arguments */
    fp = openMakefile(argc, argv);
	printf("Makefile opened!\n");
    /* Parse the file for rules, dependancies, and actions */
	rules = parse_file(fp);	    
	
	/* print out rules list .. just for debuggin */
	print_rules(rules);
	/* delete! */

	/* based on arguments provided, apply proper rule */
	if (argc == 1) {
		apply_rule(&rules);
	} else if (argc == 2) {
		arg = get_rule(rules, argv[1]);
		if (arg == NULL) {
			perror("Argument not found.");
			exit(-1);
		}

		apply_rule(&arg);
	}

	return 0;
}
