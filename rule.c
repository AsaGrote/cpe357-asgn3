#include "rule.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEFAULT_LINE_SIZE 80

rule_list *new_rule(char *target) {
	rule_list *rule = (rule_list *)malloc(sizeof(rule_list));
	if (rule == NULL) {
		free_rule_list(rule);
		perror("Error: ");
		exit(-1);
	}
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

	while (cur != NULL && strcmp(cur->target, target) != 0) {
		cur = cur->next;
	}

	return cur;
}

rule_list *parse_file(FILE *file) {
	rule_list *head = NULL, *cur = NULL;
	size_t line_size;
	char *lineptr;

	if (file == NULL) {
		perror("null file pointer");
		exit(-1);
	}

	line_size = sizeof(char) * DEFAULT_LINE_SIZE;
	lineptr = malloc(line_size);
	if (lineptr == NULL) {
		perror("malloc");
		exit(-1);
	}

	while (getline(&lineptr, &line_size, file) != -1) { /* While end of file has not been reached */ 
		if (lineptr[0] == '\t') { /* Action line */
			if (cur == NULL) { /* If no rules have been declared, throw error */
				printf("Error. No rule declared before actions.");
				exit(-1);
			}
			add_node(&(cur->actions), strdup(lineptr)); /* Otherwise, add action to current rule_list action list */
		}
		else { /* Rule & dependency line */
			if(!is_whitespace(lineptr) && strchr(lineptr, ':') == NULL) {
				printf("Smakefile: *** missing separator.  Stop.");
			}
			const char s[] = " :\n";
			char *token;
			
			/* get the first token */
			token = strtok(lineptr, s);
			if (token == NULL) {
				continue;
			}

			/* First token is the rule */
			if (head == NULL) { /* If first rule, head = new rule with the current target name */
				head = new_rule(strdup(token)); 
				cur = head;
			}
			else { /* Else if not first rule, create new rule, set current->next to the new rule, then set current to the new rule */
				rule_list *new = new_rule(strdup(token)); /* Make a new rule with the target name. */ 
				cur->next = new;
				cur = new;
				new = NULL;
			}

			/* Populate current dependency list */ 
			while ( (token = strtok(NULL, s)) != NULL) {
				add_node(&(cur->depen), strdup(token));
			}
		}
	}
    free(lineptr);
	return head;
	
}

void apply_rule(rule_list **rule, rule_list **first_rule) {
	struct stat dep_stat; /* holds the struct stat for the dependency */
	struct stat target_stat; /* holds the struct stat for the target */
	int target_file_exists = stat((*rule)->target, &target_stat);
	nlist *cur_dep = NULL;
	rule_list *cur_rule = NULL, *target = NULL;

	if (*rule == NULL) {
		printf("Error. No rule to apply.");
		exit(-1);
	}
	if ((*rule)->depen == NULL){
		if ((*rule)->actions != NULL ) { /* if no dependencies */
			/* execute the rule's actions, set the rule to updated, then return */
			execute_actions((*rule)->actions);
			(*rule)->updated = 1;
			return; /* Rule has been executed; return */
		}
	}
	else { /* validate & determine the type of each dependency */
		/* For each dependency */
		cur_dep = (*rule)->depen;
		while (cur_dep != NULL) {
			/* First, check if the dependency is a target of another rule. */
			cur_rule = (*first_rule);
			target = NULL; 
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
					printf("smake: error. No rule to make target. Stop.");
					exit(-1);
				}
				else { /* current dependency IS a file */ 
					/* compare dependency's timestamp with target's timestamp */

					/* If dependency's timestamp 
					 * is more recent than target's timestamp: 
					 * 		1. execute current rule's actions
					 * 		2. set current rule to updated */ 
					if ( dep_stat.st_mtime > target_stat.st_mtime ) {
						(*rule)->updated = 1;

						/* Move to next dependency */
						cur_dep = cur_dep->next;
						continue; 
					}
				}
			}
			else { /* else, dependency is a target of another rule
				* recursively apply this rule */
				apply_rule(&target, first_rule);

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
		 * 		1. execute the actions of the current rule
		 * 		2. set the current rule to updated */
		if ((*rule)->updated) {
			execute_actions((*rule)->actions);
		}
		/* else if a file with a name matching the target does not exist */
		else if (target_file_exists == -1) {
			/* execute the rule's actions, set the rule to updated, then return */
			execute_actions((*rule)->actions);
			(*rule)->updated = 1;
		}
	}
	
	return;

}

void free_rule_list(rule_list *head) {
    rule_list *next;
	if (head == NULL) {
        return;
    }
    next = head->next; /* store head's next */
    if (head->depen != NULL) 
        free_nlist(head->depen);
    if (head->actions != NULL) 
        free_nlist(head->actions);
    if (head->target != NULL) 
        free(head->target);
    free(head);
    head = NULL;
    free_rule_list(next);
}

int is_whitespace(char *str) {
	int i;
	char c;
	for (i = 0; i < strlen(str); i++) {
		c = str[i];
		if (c != ' ' && c != '\t' && c != '\n') {
			return 0;
		}
	}
	return 1;
}
