#include "rule.h"
#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;
	rule_list *rules;

    /* open the makefile based on command line arguments */
    fp = openMakefile(argc, argv);
	printf("Makefile opened!\n");
    /* Parse the file for rules, dependancies, and actions */
    
	rules = parse_file(fp);	    
	print_rules(rules);
	printf("%s\n", rules->target);
	printf("%s\n", rules->actions->data);
	return 0;
}
