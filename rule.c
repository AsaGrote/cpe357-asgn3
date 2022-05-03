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
	rule_list *head = NULL, *cur = NULL;
	if (file == NULL) {
		perror("Error: ");
		exit(-1);
	}

	size_t line_size = sizeof(char) * DEFAULT_LINE_SIZE;
	char *lineptr = malloc(line_size);
	if (lineptr == NULL) {
		perror("Error: ");
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
			const char s[] = " :\n";
			char *token;
			
			/* get the first token */
			token = strtok(lineptr, s);
			if (token == NULL) {
				continue;
			}

			/* First token is the rule */
			if (head == NULL) { /* If first rule, head = new rule with the current target name */
				head = (rule_list *) new_rule(strdup(token)); /* IS THIS CAST NECESSARY? */
				cur = head;
			}
			else { /* Else if not first rule, create new rule, set current->next to the new rule, then set current to the new rule */
				rule_list *new = (rule_list *) new_rule(strdup(token)); /* IS THIS CAST NECESSARY? */ /* Make a new rule with the target name. */ 
				cur->next = new;
				cur = new;
				new = NULL;
			}

			/* Populate current dependency list */ 

			while ( (token = strtok(NULL, s)) != NULL) {
			/* while (token != NULL) {
				token = strtok(NULL, s); */
				add_node(&(cur->depen), strdup(token));
			}
		}
	}

	return head;
	
}
