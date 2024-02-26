// File: $Id: stackNode.h,v 1.1 2013/10/13 04:49:51 csci243 Exp $
// Description: A stack node.
// Author: sps
#ifndef STACK_NODE_EXP_H
#define STACK_NODE_EXP_H

#include "expNode.h"
#include "expNode.c"

// Represents a single node in the stack
typedef struct StackNodeExp {
	ExpNode* data;		// data associated with the node	
	struct StackNodeExp* next;	// pointer to next node (NULL if none)
} StackNodeExp;

/// Dynamically allocate a new stack node
/// @param data The data value
/// @param next Pointer to next node (NULL if none)
/// @return The new stack node
StackNodeExp* makeStackNodeExp(ExpNode* data, StackNodeExp* next);

#endif