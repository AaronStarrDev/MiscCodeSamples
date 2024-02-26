
#ifndef QUEUE_C
#define QUEUE_C

#include <stdio.h>
#include <stdlib.h>
#include "expNode.h"
#include "expNode.c"
#include "symTbl.c"

//node class acts like a linked list in a way, or at least closer to a LL than a stack.
struct node{
	ExpNode* oldExpNode;
	struct node *ptr;
}*front,*back,*temp,*tempfront;

void createQueue();
ExpNode* peek();
void enqueue(ExpNode* data);
void dequeue();
int empty();
 
//create queue
void createQueue(){
	front = NULL;
	back = NULL;
}

/* Returns the front element of queue */
ExpNode* peek(){
	if ((front != NULL) && (back != NULL)){
		return(front->oldExpNode);
	}
	else{
		return 0;
	}
}

//enqueue
void enqueue(ExpNode* data){
	if (back == NULL){
		back = (struct node *)malloc(1*sizeof(struct node));
		back->ptr = NULL;
		back->oldExpNode = data;
		front = back;
	}
	else{
		temp=(struct node *)malloc(1*sizeof(struct node));
		back->ptr = temp;
		temp->oldExpNode = data;
		temp->ptr = NULL;
 
		back = temp;
	}
}

//dequeue
void dequeue(){
	tempfront = front;
 
	if (tempfront == NULL){
		printf("Error queue empty\n");
		return;
	}
	else
        if (tempfront->ptr != NULL){
			tempfront = tempfront->ptr;
			free(front);
			front = tempfront;
		}
		else{
			free(front);
			front = NULL;
			back = NULL;
		}
}

//if queue is empty or not
//0 = empty
//1 = not empty
int empty(){
	if ((front == NULL) && (back == NULL)){
		return 0;
	}
	else{
		return 1;
	}
}

#endif