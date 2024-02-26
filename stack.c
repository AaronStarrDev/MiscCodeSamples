
#ifndef STACK_C
#define STACK_C

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stack.h"

//Global Variable, I understand it is frowned on, hence why I made it static, but it simplifies code.
//Particuluarlly because it's shown that I can't make a Stack structure and can only use StackNodes.
//Which ultimately doesn't allow for the easy tracking of size. Thus I needed a variable.
static int OverAllSize = 0;

//typedef struct Stack{
//        int size;
//        StackNode *stackElements;
//}Stack;
/// Create stack struct
/// takes nothing, returns stack ptr
StackNode* createStack(){
	return makeStackNode("0", NULL);
}

/// Add an element to the top of the stack (stack is changed).
/// This routine should dynamically allocate a new node.
/// @param stack Points to the top of the stack
/// @param data The token (C String)
void push(StackNode** stack, char* data){
	
	if (OverAllSize == 0){
		
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
		
		StackNode* newnode = makeStackNode(data, NULL);
		
		int temp = atoi((*stack)->data);
		temp = temp + 1;
		char tempstr[10];
		sprintf(tempstr, "%d", temp);
		((*stack)->data) = tempstr;
		//(*stack)->data = atoi((*stack)->data)++;
		OverAllSize++;
		
		(*stack)->next = newnode;
	}
	else{
		
		//printf( "%s\n", "ELSE" );
		
		StackNode* temp = (*stack)->next;
		StackNode* newnode = makeStackNode(data, temp);
		((*stack)->next) = newnode;
		OverAllSize++;
	}
    return;
}

/// Removes the top element from the stack (stack is changed).
/// This routine should free the memory associated with the top node.
/// @param stack Points to the top of the stack
/// @exception If the stack is empty, the program should 
///	exit with EXIT_FAILURE
void pop(StackNode** stack){
	if(OverAllSize == 0){
		free(stack);
	}
	else if (OverAllSize == 1){
		//printf( "%s\n", "ifELSE" );
		free((*stack)->next);
		(*stack)->next = NULL;
		OverAllSize--;
	}
	else{
		//printf( "%s\n", "ELSE" );
		//printf( "%s\n", ((*stack)->next)->data );
		//printf( "%s\n", ((*stack)->next)->next->data );
		StackNode* temp = ((*stack)->next)->next;
		free((*stack)->next);
		(*stack)->next = temp;
		OverAllSize--;
	}
}

/// Return the top element from the stack (stack is unchanged)
/// @param stack Points to the top of the stack
/// @return the top element on the stack
/// @exception If the stack is empty, the program should 
///	exit with EXIT_FAILURE
char* top(StackNode* stack){
	if(OverAllSize == 0){
		exit(EXIT_FAILURE);
	}
	else{
		return (stack->next)->data;
	}
}

/// Tells whether the stack is empty or not (stack is unchanged)
/// @param stack Points to the top of the stack
/// @return 0 if not empty, any other value otherwise
int emptyStack(StackNode* stack){
	if (OverAllSize != 0){
		return 0; // NOT EMPTY
	}
	else{
		return 1; // EMPTY
	}
}

/* int main(){
	
	//printf( "%s\n", "test 1" ); //test
	
	StackNode* stack = createStack();
	
	//printf( "%s\n", stack->data ); //test
	
	//StackNode* stackptr = &stack;
	StackNode** stackptr = &stack;
	
	//printf( "%s\n", "test 2" ); //test
	
	push(stackptr, "o");
	printf( "%s\n", stack->next->data ); //test
	
	push(stackptr, "l");
	printf( "%s\n", stack->next->data ); //test
	
	push(stackptr, "l");
	printf( "%s\n", stack->next->data ); //test
	
	push(stackptr, "e");
	printf( "%s\n", stack->next->data ); //test
	
	push(stackptr, "h");
	printf( "%s\n", stack->next->data ); //test
	
	//printf( "%s\n", "test 3" ); //test
	
	printf( "%s\n", top(stack) );
	
	//printf( "%s\n", "test 4" ); //test
	
	pop(stackptr);
	printf( "%s\n", top(stack) );
	pop(stackptr);
	printf( "%s\n", top(stack) );
	pop(stackptr);
	printf( "%s\n", top(stack) );
	pop(stackptr);
	printf( "%s\n", top(stack) );
	pop(stackptr);
	
	printf( "%s\n", "test 5" ); //test
	
	exit(EXIT_SUCCESS);
}
 */
 
 #endif