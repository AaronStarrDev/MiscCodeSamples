
#ifndef EXPNODE_C
#define EXPNODE_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "expNode.h"
#include "symTbl.h"
#include "stack.h"

#define FALSE 1
#define TRUE 0

int isInteger(double val);

/// Construct an expression node dynamically on the heap.
/// Assume the following order when determining which
/// type to create:
///	1. the operators
///  	2. literal
///	3. integer literal
///	4. symbol
/// @param token The next C string token in the expression
/// @param left Pointer to left node (NULL if none)
/// @param right Pointer to right node (NULL if none)
/// @return The new node
ExpNode* makeExpNode(char token[], ExpNode* left, ExpNode* right){
    ExpNode *XpNode;
	XpNode = (ExpNode *)malloc(sizeof(ExpNode));
	XpNode->left = left;
	XpNode->right = right;
	
	
	//Enter if literal (ADD_OP_STR)
	if(strcmp(token, ADD_OP_STR) == 0){
		XpNode->type = ADD_OP;
	}
	//Enter if literal (SUB_OP_STR)
	else if(strcmp(token, SUB_OP_STR) == 0){
		XpNode->type = SUB_OP;
	}
	//Enter if literal (MUL_OP_STR)
	else if(strcmp(token, MUL_OP_STR) == 0){
		XpNode->type = MUL_OP;
	}
	//Enter if literal (DIV_OP_STR)
	else if(strcmp(token, DIV_OP_STR) == 0){
		XpNode->type = DIV_OP;
	}
	//Enter if literal (MOD_OP_STR)
	else if(strcmp(token, MOD_OP_STR) == 0){
		XpNode->type = MOD_OP;
	}
	//Enter if literal (ASSIGN_OP_STR)
	else if(strcmp(token, ASSIGN_OP_STR) == 0){
		XpNode->type = ASSIGN_OP;
	}
	
	//Enter if literal (DOUBLE) or Enter if literal (INTEGER)
	else if(isInteger(atof(token)) == 1){
		Value tempValue;
		if(strstr(token, ".") != NULL) {
			XpNode->type = DOUBLE;
			tempValue.type = TYPE_DOUBLE;
			tempValue.value.dVal = atof(token);
			
			XpNode->value = tempValue;
		}
	}
	//Enter if literal (INTEGER)
	else if(isInteger(atof(token)) == 0){
		Value tempValue;
		XpNode->type = INTEGER;
		tempValue.type = TYPE_INT;
		tempValue.value.iVal = atoi(token);
			
		XpNode->value = tempValue;
	}

	//Enter if SYMBOL
	else if(lookupTable(token) != NULL){

		XpNode->type = SYMBOL;
		strcpy(XpNode->symbol, (*(lookupTable(token))).varName);
		
		//if(strcmp(*lookupTable.varName, "int") == 0){
		XpNode->value = (*(lookupTable(token))).value;//.iVal;
		//}
		//else{
		//	XpNode->value = *lookSym_PTR.value.value.dVal;
		//}
	}
	// Enter if UNKNOWN
	else{
		XpNode->type = UNKNOWN;
	}
	return XpNode;
}
// checks if number is int
// returns true if is int, false if not.
int isInteger(double val){
    int truncated = (int)val;
	if(val == truncated){
		return 0;
	}
    return 1;
}

/*int main(){
	ExpNode* test = makeExpNode("+", NULL, NULL);
	printf("\tType: , Name: %s, Value: %d\n", testSym.varName, testSym.value.value.iVal);
	//free(test);
	exit(EXIT_SUCCESS);
}*/

#endif
