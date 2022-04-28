#include "list.h"

struct rule_list {
    char *target;
    struct nlist *depen;
    struct nlist *actions;
    struct rule_list *next;
    int updated;
};
