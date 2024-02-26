
#ifndef STACKNODE_C
#define STACKNODE_C

#include <stdio.h>
#include <stdlib.h>
#include "stackNode.h"

/// Dynamically allocate a new stack node
/// @param data The data value
/// @param next Pointer to next node (NULL if none)
/// @return The new stack node
StackNode* makeStackNode(char* data, StackNode* next){
	
    StackNode *Stack;
    Stack = (StackNode *)malloc(sizeof(StackNode));
    Stack->next = next;
    Stack->data = data;
	
    return Stack;
}

#endif