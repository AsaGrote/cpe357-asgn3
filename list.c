#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* Frees the linked list pointed to by the head argument */
void free_nlist(struct nlist *head) {
    if (head == NULL) { /* base case */
        return;
    }
    struct nlist *temp = head->next; /* keep a pointer to the head's next */
    free(head); 
    head = NULL;
    free_nlist(temp); /* recursively free list */
}
