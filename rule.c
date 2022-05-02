#include "rule.h"
#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rule_list *new_rule(char *target) {
	rule_list *rule = (rule_list *)malloc(sizeof(rule_list));
	rule->target = target;
	rule->depen = NULL;
	rule->actions = NULL;
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


void apply_rule(rule_list *rule) {
	

}

