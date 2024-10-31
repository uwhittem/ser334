#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct list list;

struct node {
	void* element;
	node* next;
};
struct list {
	node* head;
	int size;
};
//    Implement list_create. This function does not take any parameters but will return
//    a new list collection.
list* list_create() {
	stack* newList = (list*)malloc(sizeof(list));
	if (newList == NULL) {
		printf("Failed to create list!");
		exit(1);
	} else {
		newList>head = NULL;
		newList>size = 0;

		return newList;
	}
}
//(a) Implement list_destroy. This function should take a list as a parameter and clean up
//    its memory allocation(s).
void list_destroy(list** lst) {
	if (lst == NULL || *lst == NULL)
		return;
	
	node* current = (*lst)->head;
	// Free all nodes
	while (current != NULL) {
		node* temp = current;
		current = current->next;
		free(temp);
	}
	
	// Free the list structure itself
	free(*lst);
	*lst = NULL;
}
//(b) Implement list_add. This function should take a list, and an element, as parameters
//    and append the element to the end of the list.
void list_add(list* lst, void* element) {
	if (lst == NULL)
		return;
	
	node* new_node = (node*)malloc(sizeof(node));
	new_node->element = element;
	new_node->next = NULL;
	
	if (lst->head == NULL) {
		// If list is empty, new node becomes head
		lst->head = new_node;
	} else {
		// Find the last node
		node* current = lst->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
	
	lst->size++;
}