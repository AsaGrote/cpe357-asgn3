#include <stdio.h>
#ifndef RULE_H
#define RULE_H
#endif

typedef struct rule_list {
    char *target;
    struct nlist *depen;
    struct nlist *actions;
    struct rule_list *next;
    int updated;
} rule_list;

rule_list *new_rule(char *target);
rule_list *parse_file(FILE *file);
rule_list *get_rule(rule_list *head, char *target);

void print_rules(rule_list *rule);
void apply_rule(rule_list **, rule_list **);
void free_rule_list(rule_list *);

int is_whitespace(char *);
