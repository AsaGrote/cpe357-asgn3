#include "rule.h"
#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rule_list *new_rule(char *target) {
	rule_list *rule = (rule_list *)malloc(sizeof(rule_list));
	rule->target = target;
	return rule;
}

rule_list *parse_file(FILE *file) {
	rule_list *cur = NULL, *head = NULL;
	char *line = NULL;
	char *target = NULL;
	char *dep = NULL;
	
	while (!EOF) {
		line = read_long_line(file);
		
		if (strlen(line) > 0) {
			/* if line starts with \t, it is an action */
			if (line[0] == '\t') {
				/* increment line to get rid of tab 
				 * & add action to action list */
				line++;
				add_node(cur->actions, line);
			/* otherwise, line is "target : dependencies" */
			} else {
				/* split line based on ':' 
				 * target = target
				 * line = dependencies */
				target = strsep(&line, ":");

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
				dep = strtok(line, " ");

				while (dep != NULL) {
					/* my way of ignoring white space */ 
					if (!strcmp(dep, " ") && !strcmp(dep, "")) {
						add_node(cur->depen, dep);
					}
					dep = strtok(line, " ");
				}
			}
		}
	}

	return head;
}
