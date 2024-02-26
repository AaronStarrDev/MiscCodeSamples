#ifndef STACKEXP_C
#define STACKEXP_C

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stackExp.h"

//Global Variable, I understand it is frowned on, hence why I made it static, but it simplifies code.
//Particuluarlly because it's shown that I can't make a Stack structure and can only use StackNodes.
//Which ultimately doesn't allow for the easy tracking of size. Thus I needed a variable.
static int OverAllSizeExp = 0;

//typedef struct Stack{
//        int size;
//        StackNode *stackElements;
//}Stack;
/// Create stack struct
/// takes nothing, returns stack ptr
StackNodeExp* createStackExp(){
	return makeStackNodeExp(NULL, NULL);
}

/// Add an element to the top of the stack (stack is changed).
/// This routine should dynamically allocate a new node.
/// @param stack Points to the top of the stack
/// @param data The token (C String)
void pushExp(StackNodeExp** stack, ExpNode* data){
	
	if (OverAllSizeExp == 0){
		
		//printf( "%d\n", atoi((*stack)->data) );
		//int temp = atoi((*stack)->data);
		//printf( "%d\n", temp );
		//temp = temp + 1;
		//printf("%d\n", temp);
		//int temp = atoi((*stack)->data);
		//char tempstr[10];
		//sprintf(tempstr, "%d", temp);
		//((*stack)->data) = tempstr;
		
		//printf( "%d\n", atoi((*stack)->data) );
		
		StackNodeExp* newnode = makeStackNodeExp(data, NULL);
		
		/* int temp = atoi((*stack)->data);
		temp = temp + 1;
		char tempstr[10];
		sprintf(tempstr, "%d", temp); */
		//((*stack)->data) = tempstr;
		//(*stack)->data = atoi((*stack)->data)++;
		OverAllSizeExp++;
		
		(*stack)->next = newnode;
	}
	else{
		
		//printf( "%s\n", "ELSE" );
		
		StackNodeExp* temp = (*stack)->next;
		StackNodeExp* newnode = makeStackNodeExp(data, temp);
		((*stack)->next) = newnode;
		OverAllSizeExp++;
	}
    return;
}

/// Removes the top element from the stack (stack is changed).
/// This routine should free the memory associated with the top node.
/// @param stack Points to the top of the stack
/// @exception If the stack is empty, the program should 
///	exit with EXIT_FAILURE
void popExp(StackNodeExp** stack){
	if(OverAllSizeExp == 0){
		free(stack);
	}
	else if (OverAllSizeExp == 1){
		//printf( "%s\n", "ifELSE" );
		free((*stack)->next);
		(*stack)->next = NULL;
		OverAllSizeExp--;
	}
	else{
		//printf( "%s\n", "ELSE" );
		//printf( "%s\n", ((*stack)->next)->data );
		//printf( "%s\n", ((*stack)->next)->next->data );
		StackNodeExp* temp = ((*stack)->next)->next;
		free((*stack)->next);
		(*stack)->next = temp;
		OverAllSizeExp--;
	}
}

/// Return the top element from the stack (stack is unchanged)
/// @param stack Points to the top of the stack
/// @return the top element on the stack
/// @exception If the stack is empty, the program should 
///	exit with EXIT_FAILURE
ExpNode* topExp(StackNodeExp* stack){
	if(OverAllSizeExp == 0){
		exit(EXIT_FAILURE);
	}
	else{
		return (stack->next)->data;
	}
}

/// Tells whether the stack is empty or not (stack is unchanged)
/// @param stack Points to the top of the stack
/// @return 0 if not empty, any other value otherwise
int emptyStackExp(StackNodeExp* stack){
	if (OverAllSizeExp != 0){
		return 0; // NOT EMPTY
	}
	else{
		return 1; // EMPTY
	}
}
 
 #endif