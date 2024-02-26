#ifndef STACKNODEEXP_C
#define STACKNODEEXP_C

#include <stdio.h>
#include <stdlib.h>
#include "stackNodeExp.h"

/// Dynamically allocate a new stack node
/// @param data The data value
/// @param next Pointer to next node (NULL if none)
/// @return The new stack node
StackNodeExp* makeStackNodeExp(ExpNode* data, StackNodeExp* next){
	
    StackNodeExp *Stack;
    Stack = (StackNodeExp *)malloc(sizeof(StackNodeExp));
    Stack->next = next;
    Stack->data = data;
	
    return Stack;
}

#endif