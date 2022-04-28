#ifndef LIST_H
#define LIST_H
#endif

int foo();

typedef struct nlist {
    char *data;
    struct nlist *next;
} nlist;

/* Frees the linked list pointed to by the head argument */
void free_nlist(struct nlist *);

/* Adds a node to the linked list. */
/* Accepts two arguments. The first is the head of the list. The second is the data to add */
/*                                         ^        */
/*                                         tail???? <- could be a better & faster option. However, we would have to keep track of the tail*/
void add_node(struct nlist *, char *);

/* Searches the a list for a value */
/* Accepts two arguments. The first is a pointer of the head of the list. The second is data being searched for. */
/* Returns a pointer to the node being searched for. Returns null if not found */
struct nlist *get_node(struct nlist *, char *);
