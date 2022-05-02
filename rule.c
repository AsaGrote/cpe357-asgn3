#include "rule.h"
#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

rule_list *new_rule(char *target) {
	rule_list *rule = (rule_list *)malloc(sizeof(rule_list));
	rule->target = target;
	rule->depen = NULL;
	rule->actions = NULL;
	rule->updated = 0;
	return rule;
}

void print_rules(rule_list *rule) {
	while (rule != NULL) {
		printf("%s\n", rule->target);
		printf("dependencies:\n");
		print_nlist(rule->depen);
		printf("actions:\n");
		print_nlist(rule->actions);
		rule = rule->next;
	}
}

rule_list *get_rule(rule_list *head, char *target) {
	rule_list *cur = NULL;
	cur = head;

	while (cur != NULL && cur->target != target) {
		cur = cur->next;
	}

	return cur;
}

rule_list *parse_file(FILE *file) {
	rule_list *cur = NULL, *head = NULL;
	char *line = NULL;
	char *target = NULL;
	char *dep = NULL;

	line = read_long_line(file);	
	while (!feof(file)) {
		if (strlen(line) > 0) {
			/* if line starts with \t, it is an action */
			if (line[0] == '\t') {
				/* increment line to get rid of tab 
				 * & add action to action list */
				line++;
				add_node(&(cur->actions), line);
			/* otherwise, line is "target : dependencies" */
			} else {
				/* split line based on ':' 
				 * target = target
				 * line = dependencies */
				target = strtok(line, ":");
				dep = strtok(NULL, ":");
				/* if line is made NULL, ':' was not found */	
				if (line == NULL) {
					perror("':' not found");
					exit(-1);
				}

				/* create rule & add to list */
				if (head == NULL) {
					cur = new_rule(target);
					head = cur; 
				} else {
					cur->next = new_rule(target);
					cur = cur->next;
				}
				
				/* split dependencies & add to dependency list */
				line = dep;
				if (line != NULL) {
					while (line[0] == ' ') {
						line++;
					}
					dep = strtok(line, " ");
					while (dep != NULL) {
						/* my way of ignoring white space */ 
						add_node(&(cur->depen), dep);
						dep = strtok(NULL, " ");
					}
				}
			}
		}
		line = read_long_line(file);
	}

	return head;
}

void apply_rule(rule_list **rule) {
    struct stat dep_stat; /* holds the struct stat for the dependency */
    struct stat target_stat; /* holds the struct stat for the target */
    /* int file = 0;  file flag: 0 = file, -1 = not file */ 
    if (*rule == NULL) {
        /* TODO: THIS IS AN ERROR RIGHT? */
        perror("Error. No rule to apply.");
        exit(-1);
    }
    if ((*rule)->depen == NULL) { /* if no dependencies */
        /* execute the rule's actions, set the rule to updated, then return */
        execute_actions((*rule)->actions);
        (*rule)->updated = 1;
        return; /* Rule has been executed; return */
    }
    /* else if a file with a name matching the target does not exist */
    else if (stat((*rule)->target, &target_stat) == -1) {
        /* execute the rule's actions, set the rule to updated, then return */
        execute_actions((*rule)->actions);
        (*rule)->updated = 1;
        return; /* Rule has been executed; return*/
    }
    else { /* validate & determine the type of each dependency */
        /* For each dependency */
        nlist *cur_dep = (*rule)->depen;
        while (cur_dep != NULL) {
            /* First, check if the dependency is a target of another rule. */
            rule_list *cur_rule = (*rule);
            rule_list *target = NULL; 
            while (cur_rule != NULL) {
                if (strcmp(cur_rule->target, cur_dep->data) == 0) {
                    target = cur_rule;

                    /*break from checking if depen is target of another rule */
                    break; 
                }
                cur_rule = cur_rule->next;
            }
            /* if NOT a target of another rule, check if dependency is file */
            if (target == NULL) {
                if (stat(cur_dep->data, &dep_stat) == -1) {
                    /* current dependency is NOT a file, report an error */
                    printf("make: error. No rule to make target. Stop.");
                    exit(-1);
                }
                else { 
					/* current dependency IS a file */ 
                    /* compare dependency's timestamp with target's timestamp */

				   /* If dependency's timestamp 
                    * is more recent than target's timestamp: 
                    *      1. execute current rule's action
                    *      2. set current rule to updated */ 
                    if ((dep_stat.st_mtime) > 
						(target_stat.st_mtime))
					{
                        execute_actions((*rule)->actions);
                        (*rule)->updated = 1;
                        continue; /* Move to next dependency */
                    }
                }
            }
            else { /* else, dependency is a target of another rule
                * recursively apply this rule */
                apply_rule(&target);

                /* check if applied rule was updated 
				 * if so, the current rule should also be set to updated */
                if ( target->updated == 1) 
    				(*rule)->updated = 1;
            }

            cur_dep = cur_dep->next;
        }

        /* after executing each dependency, 
 		 * check if current rule's updated has been set */

        /* if updated has been set: 
 		 *      1. execute the actions of the current rule
	     *      2. set the current rule to updated */
        if ((*rule)->updated) {
            execute_actions((*rule)->actions);
            (*rule)->updated = 1;
        }
    }
    
    return;

}
