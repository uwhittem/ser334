//////////////////////////////////////////////////////////////////////
//Include Files
#include <stdio.h> 


//////////////////////////////////////////////////////////////////////
//Type Definitions
typedef struct list list;


//////////////////////////////////////////////////////////////////////
//Function Declarations
//borrowed from docs.oracle.com/javase/9/docs/api/java/util/LinkedList.html

//purpose: creates a new list and returns it.
//return: error value (zero for success, non-zero for error)
list* list_create();


//purpose: destroys a list.
//return: n/a
void list_destroy(list** lst);


//purpose: appends the specified element to the end of this list.
//return: n/a
void list_add(list* lst, void* element);


//purpose: returns the size of the size.
//return: number of elements in the stack
int list_size(list* lst);