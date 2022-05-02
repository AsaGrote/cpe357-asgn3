#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* Frees the linked list pointed to by the head argument */
void free_nlist(struct nlist *head) {
    nlist *temp = NULL;

	if (head == NULL) { /* base case */
        return;
    }
    temp = head->next; /* keep a pointer to the head's next */
    free(head); 
    head = NULL;
    free_nlist(temp); /* recursively free list */
}

/* Creates a new node with the data passed in to create_node */
/* Arguments: String that contains the data for the new node */
/* Returns a pointer to the new node */
struct nlist *create_node(char *data) {
    struct nlist * new = (struct nlist *) malloc(sizeof(struct nlist));
    if (new == NULL) {
        printf("Error. Out of memory\n");
        exit(-1);
    }
    new->data = data;
    new->next = NULL;
    return new;
}

/* adds a new node to the nlist */
void add_node(nlist **headptr, char *data) {
	nlist *new = create_node(data); /* create new node with data passed in */
    nlist *tail = *headptr; /* Used to traverse list */
    
	if (*headptr == NULL) { /* If head is null, set head to new node */
        *headptr = new;
        return;
    }
    while (tail->next != NULL) { /* Traverse list until tail is reached */
        tail = tail->next;
    }
    tail->next = new; /* Set the tail's next to the new node */

    /* Set pointers to null */
    tail = NULL;
    new = NULL;
}

/* search the node list for a value
 * returns a pointer to the node  */
struct nlist *get_node(struct nlist *head, char *data) {
	nlist *cur = NULL;
	cur = head;

	while (cur != NULL && cur->data != data) {
		cur = cur->next;
	}

	return cur;
}
