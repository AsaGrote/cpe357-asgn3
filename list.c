#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* Frees the linked list pointed to by the head argument */
void free_nlist(nlist *head) {
    nlist *temp = NULL;

	if (head == NULL) { /* base case */
        return;
    }
    temp = head->next; /* keep a pointer to the head's next */
    free(head); 
    head = NULL;
    free_nlist(temp); /* recursively free list */
}

void print_nlist(nlist *list) {
	nlist *cur = list;

	while (cur != NULL) {
		printf("\t%s\n", cur->data);
		cur = cur->next;
	}
}

/* Creates a new node with the data passed in to create_node */
/* Arguments: String that contains the data for the new node */
/* Returns a pointer to the new node */
nlist *create_node(char *data) {
    nlist *new = (nlist *) malloc(sizeof(nlist));
	if (new == NULL) {
        perror("Error. Out of memory\n");
        exit(-1);
    }
    new->data = data;
    new->next = NULL;
    return new;
}

/* adds a new node to the nlist */
nlist *add_node(nlist *head, char *data) {
	nlist *cur = head, *prev = NULL;

	while (cur != NULL) {
		prev = cur;
		cur = cur->next;
	}
	cur = (nlist *)malloc(sizeof(nlist));
	if (head == NULL) {
		head = cur;
	} else {
		prev->next = cur;
	}
	cur->data = data;
	cur->next = NULL;

	return head;

}

/* search the node list for a value
 * returns a pointer to the node  */
nlist *get_node(nlist *head, char *data) {
	nlist *cur = NULL;
	cur = head;

	while (cur != NULL && cur->data != data) {
		cur = cur->next;
	}

	return cur;
}
