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

    /* Parse the file and create rule list */
	rules = parse_file(fp); 
    
    /* Print out the rule list */
    rule_list *cur_rule = rules;
    while (cur_rule != NULL) {
        printf("Target: %s\n", cur_rule->target);
        nlist *cur_dep = cur_rule->depen;
        printf("Dependencies:");
        while (cur_dep != NULL) {
            printf(" %s ", cur_dep->data);
            cur_dep = cur_dep->next;
        }
        printf("\n");
        nlist *cur_act = cur_rule->actions;
        printf("Actions:");
        while (cur_act != NULL) {
            printf(" %s ", cur_act->data);
            cur_act = cur_act->next;
        }
        printf("\n");
        cur_rule = cur_rule->next;
    }  
    return 0;
}
