/*
Implement a Linked List ADT using a struct and four functions. A list is a resizing collection that we can use to store elements. 
Although a list is an OOP concept, we can still implement it in C with a clever design. 
We will even aim to implement it such that any datatype may be stored in it. 
The struct contains variables to represent an internal data structure, and the size of the collection. 
Create three functions: list_destroy, list_add, and list_size. list_create is already completed as an example. 

Descriptions of each are shown next.

The ADT will be structured with three files: main.c, LinkedList.h, and LinkedList.c. 
The first two files (main.c, LinkedList.h) are provided below, while you will need to implement LinkedList.c. 
The main.c file contains testing code that uses the list you will be implementing - it indirectly shows the syntax for the functions.
*/



main.c

#include <stdio.h>
#include "LinkedList.h"
int main() {
	list* lst;
	lst = list_create();

	list_add(lst, 10);
	list_add(lst, 15);
	list_add(lst, 20);

	printf("Size: %d\n", list_size(lst));

	list_destroy(&lst);
	return 0;
}