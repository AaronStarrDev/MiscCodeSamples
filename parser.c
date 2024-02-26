#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "symTbl.h"
#include "symTbl.c"
#include "stack.h"
#include "stackNode.h"
#include "stackNode.c"
#include "stack.c"
#include "expNode.h"
#include "expNode.c"
#include "stackExp.h"
#include "stackNodeExp.h"
#include "stackNodeExp.c"
#include "stackExp.c"

static ParserError parse_error = NONE;

// takes in a string, returns whether it is an operator or not.
// 1 = is operator, 0 = is NOT operator.
int isOp(char* word){
	char* operators[] = {"+", "-", "*", "/", "%", "="};
	int i = 0;
	int isOp = 0;
	// check if the word being looked at is an operator (requires popping and a push)
	for(i = 0; i < 6; i++){ // 6 comes from the operators array, will always be size 6
		if(strcmp(word, operators[i]) == 0){
			isOp = 1;
			break;
		}
	}
	return isOp;
}

/// The main parse routine that handles the entire parsing
/// process, using the rest of the routines defined here.
/// @param exp The expression as a string
void parse(char exp[]){
	ExpNode* node = parseTree(exp);// error catch!
	if(node->type == UNKNOWN){
		return;
	}
	Value eval = evalTree(node);
	infixTree(node);
	if(eval.type == TYPE_DOUBLE){
		printf(" = %6f", eval.value.dVal);
	}
	else if(eval.type == TYPE_INT){
		printf(" = %d", eval.value.iVal);
	}
	free(node);

}

/// Constructs the expression tree from the expression.  It
/// must use the stack to order the tokens.  It must also
/// deallocate the memory associated with the stack in all cases.
/// If a symbol is encountered, it should be stored in the node
/// without checking if it is in the symbol table - evaluation will
/// resolve that issue.
/// @param expr the postfix expression as a C string
/// @return the root of the expression tree
/// @exception There are 2 error conditions that you must deal
/// 	with.  In each case, the memory associated with the
///	tree must be cleaned up before returning.  Neither 
///	stops execution of the program. 
///
///	1. If there are too few tokens, set the parser error
///	to TOO_FEW_TOKENS and display the message to standard error:
///
///	Invalid expression, not enough tokens
///
///	2. If there are many tokens (stack is not empty after building),
///	set the parser error to TOO_MANY_TOKENS and display the message
///	to standard error:
///
/// 	Invalid expression, too many tokens
ExpNode* parseTree(char expr[]){
	
	ExpNode* ERROR = makeExpNode("ERROR", NULL, NULL);
	
	if(sizeof(expr)/sizeof(expr[0]) == 2){
		printf("Invalid expression, not enough tokens");
		parse_error = TOO_FEW_TOKENS;
		return ERROR;
	}
	
	StackNode* stack = createStack();
	StackNode** stackptr = &stack;
	StackNodeExp* stackExp = createStackExp();
	StackNodeExp** stackExpptr = &stackExp;
	
	char* word;
	word = strtok(expr, " ");
	while(word != NULL){
		
		int isOper = isOp(word);
		
		if(isOper == 0){ // is NOT and operator
			push(stackptr, word);
		}
		else{ //has to be, isOp = 1 (It is an Operator)
			
			ExpNode* OpExpNode = makeExpNode(word, NULL, NULL);
			//ExpNode** OpExpNode_PTR = &OpExpNode;
			
			// FIRST POP - RIGHT SIDE
			
			if(emptyStack(stack) == 1){
				printf("Invalid expression, not enough tokens");
				parse_error = TOO_FEW_TOKENS;
				free(stack);
				free(stackExp);
				free(OpExpNode);
				return ERROR;
			}
			
			char* newExpNodeDataRight = top(stack);
			pop(stackptr);
			
			if(isOp(newExpNodeDataRight)){//you popped an expression that you added earlier, now attach it's lower bits.
				ExpNode* OpNode_Right = topExp(stackExp);
				OpExpNode->left = OpNode_Right;
				popExp(stackExpptr);
			}
			else{
				ExpNode* newExpNodeRight = makeExpNode(newExpNodeDataRight, NULL, NULL);
				OpExpNode->right = newExpNodeRight;
			}
			
			// SECOND POP - LEFT SIDE
			
			if(emptyStack(stack) == 1){
				printf("Invalid expression, not enough tokens");
				parse_error = TOO_FEW_TOKENS;
				free(stack);
				free(stackExp);
				free(OpExpNode);
				return ERROR;
			}
			
			char* newExpNodeDataLeft = top(stack);
			pop(stackptr);
			
			if(isOp(newExpNodeDataLeft)){//you popped an expression that you added earlier, now attach it's lower bits.
				ExpNode* OpNode_Left = topExp(stackExp);
				OpExpNode->left = OpNode_Left;
				popExp(stackExpptr);
			}
			else{
				ExpNode* newExpNodeLeft = makeExpNode(newExpNodeDataLeft, NULL, NULL);
				OpExpNode->left = newExpNodeLeft;
			}
			
			push(stackptr, word);//add the operator to the stack for later construction
			pushExp(stackExpptr, OpExpNode);//add the operator to the stack for later construction
			
			
		}
		
		word = strtok(NULL, " ");
	}
	if(emptyStack(stack) == 0){
		printf("Invalid expression, too many tokens");
		parse_error = TOO_MANY_TOKENS;
		free(stack);
		free(stackExp);
		return ERROR;
	}
	free(stack);
	ExpNode* tree_root = topExp(stackExp);
	popExp(stackExpptr);
	
	return tree_root;
}

/// Evaluates the tree and returns the result.
/// @param node The node in the tree
/// @precondition:  This routine should not be called if there
/// 	is a parser error.
/// @return the evaluated value.  Note:  A symbol evaluates
///	to its stored value.  
///
///	The evaluator needs to be able to deal with the multiple
///	types our interpreter supports (TYPE_INT and TYPE_DOUBLE).
///
///	1. If a symbol is not in the table, and the table isn't full
///	It should be added to the table taking on the type of the
///	rhs (right hand side) of the assignment.  Note that
///	a symbol will evaluate to its stored value (and type).
///
///	2. When performing the math operations (except for modulus),
///	the following rule applies.  If both lhs and rhs are int's,
///	the result is an int, otherwise the result is a double.
///
/// @exceptions There are 6 error conditions that can occur.  If
///	either occurs, set the correct parser error and display
///	an error message and return a Value of TYPE_UNKNOWN.   The main program
///	should check the parser error state before using the
///	return result.
///
/// 1. If a symbol is referenced on the left hand side during an
/// 	assignment, it should be added to the symbol table with
/// 	the value being the evaluation of the right hand side.  If
/// 	there is no more room in the symbol table, it should set
/// 	a SYMBOL_TABLE_FULL parser error and display the following
/// 	message to standard error
///
/// 	Symbol table full, cannot create new symbol
///
/// 2. An assignment is made to a left hand side expression
///	node that is not a symbol node.  It should set an 
///	INVALID_ASSIGNMENT parser error and display the following
///	message to standard error:
///
///	Invalid assignment
///
/// 3. An assignment is made to a symbol with a value whose
///	type does not match the symbol's type (e.g. assigning
///	a double value to an int symbol).  It should set
///	an INVALID_ASSIGNMENT parser error, and display the following
/// 	message to standard error:
///
///	Assignment type mismatch
///
/// 4. If a math operation is being performed on a symbol that
///	that does not exist, you should set an UKNOWN_SYMBOL
/// 	parser error and display the following message to standard error
///     (where <symbol-name> is the name of the symbol):
///
///	Unknown symbol: <symbol-name>
///
/// 5. If modulus (%) is performed, both left and right hand side nodes
///	must be TYPE_INT.  If this happens, do not perform the operation,
///	instead set the parser error to INVALID_MODULUS and display
/// 	the following error to standard error:
///
///	Modulus requires both types to be int
///
/// 6. If division by zero (or modulus) is being performed, do not do it.  
///     Instead set the parser error to DIVISION_BY_ZERO and display the
///	following error to standard error:
///
///	Division by zero
///
Value evalTree(ExpNode* node){
	if(node){
		Value left;
		Value right;
		if(node->left != NULL){
			left = evalTree(node->left);
		}
		if((node->type == DOUBLE) || (node->type == INTEGER) ||(node->type == SYMBOL)){

			if(node->type == DOUBLE){
				return node->value;
			}
			else if(node->type == INTEGER){
				return node->value;
			}
			else if(node->type == SYMBOL){
				return node->value;
			}
		}
		else if(node->type == ADD_OP){
			double addition;
			right = evalTree(node->right);
			if(left.type == TYPE_INT){
				if(right.type == TYPE_INT){
					addition = (left.value.iVal + right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_INT;
					tempvalue.value.iVal = (int)(addition);
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					addition = (left.value.iVal + right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(addition) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(addition);
					}
					else{tempvalue.value.dVal = (addition);}
					return tempvalue;
				}
			}
			if(left.type == TYPE_DOUBLE){
				if(right.type == TYPE_INT){
					addition = (left.value.dVal + right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(addition) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(addition);
					}
					else{tempvalue.value.dVal = (addition);}
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					addition = (left.value.dVal + right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(addition) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(addition);
					}
					else{tempvalue.value.dVal = (addition);}
					return tempvalue;
				}
			}
		}
		else if(node->type == SUB_OP){
			double subtraction;
			right = evalTree(node->right);
			if(left.type == TYPE_INT){
				if(right.type == TYPE_INT){
					subtraction = (left.value.iVal - right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_INT;
					tempvalue.value.iVal = (int)(subtraction);
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					subtraction = (left.value.iVal - right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(subtraction) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(subtraction);
					}
					else{tempvalue.value.dVal = (subtraction);}
					return tempvalue;
				}
			}
			if(left.type == TYPE_DOUBLE){
				if(right.type == TYPE_INT){
					subtraction = (left.value.dVal - right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(subtraction) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(subtraction);
					}
					else{tempvalue.value.dVal = (subtraction);}
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					subtraction = (left.value.dVal - right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(subtraction) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(subtraction);
					}
					else{tempvalue.value.dVal = (subtraction);}
					return tempvalue;
				}
			}
		}
		else if(node->type == DIV_OP){
			double division;
			right = evalTree(node->right);
			if(left.type == TYPE_INT){
				if(right.type == TYPE_INT){
					if(right.value.iVal == 0){
						printf("Divison by zero");
						parse_error = DIVISION_BY_ZERO;
						Value tempvalue;
						tempvalue.type = TYPE_UNKNOWN;
						return tempvalue;
					}
					division = (left.value.iVal / right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(division) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(division);
					}
					else{tempvalue.value.dVal = (division);}
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					if(right.value.dVal == 0){
						printf("Divison by zero");
						parse_error = DIVISION_BY_ZERO;
						Value tempvalue;
						tempvalue.type = TYPE_UNKNOWN;
						return tempvalue;
					}
					division = (left.value.iVal / right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(division) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(division);
					}
					else{tempvalue.value.dVal = (division);}
					return tempvalue;
				}
			}
			if(left.type == TYPE_DOUBLE){
				if(right.type == TYPE_INT){
					if(right.value.iVal == 0){
						printf("Divison by zero");
						parse_error = DIVISION_BY_ZERO;
						Value tempvalue;
						tempvalue.type = TYPE_UNKNOWN;
						return tempvalue;
					}
					division = (left.value.dVal / right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(division) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(division);
					}
					else{tempvalue.value.dVal = (division);}
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					if(right.value.dVal == 0.0){
						printf("Divison by zero");
						parse_error = DIVISION_BY_ZERO;
						Value tempvalue;
						tempvalue.type = TYPE_UNKNOWN;
						return tempvalue;
					}
				division = (left.value.dVal / right.value.dVal);
				Value tempvalue;
				tempvalue.type = TYPE_DOUBLE;
				if(isInteger(division) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(division);
					}
					else{tempvalue.value.dVal = (division);}
				return tempvalue;
				}
			}
		}
		else if(node->type == MUL_OP){
			double multiplication;
			right = evalTree(node->right);
			if(left.type == TYPE_INT){
				if(right.type == TYPE_INT){
					multiplication = (left.value.iVal * right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_INT;
					if(isInteger(multiplication) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(multiplication);
					}
					else{tempvalue.value.dVal = (multiplication);}
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					multiplication = (left.value.iVal * right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(multiplication) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(multiplication);
					}
					else{tempvalue.value.dVal = (multiplication);}
					return tempvalue;
				}
			}
			if(left.type == TYPE_DOUBLE){
				if(right.type == TYPE_INT){
					multiplication = (left.value.dVal * right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(multiplication) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(multiplication);
					}
					else{tempvalue.value.dVal = (multiplication);}
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					multiplication = (left.value.dVal * right.value.dVal);
					Value tempvalue;
					tempvalue.type = TYPE_DOUBLE;
					if(isInteger(multiplication) == 0){
						tempvalue.type = TYPE_INT;
						tempvalue.value.iVal = (int)(multiplication);
					}
					else{tempvalue.value.dVal = (multiplication);}
					return tempvalue;
				}
			}
		}
		else if(node->type == MOD_OP){
			double modulus;
			right = evalTree(node->right);
			if(left.type == TYPE_INT){
				if(right.type == TYPE_INT){
					modulus = (left.value.iVal % right.value.iVal);
					Value tempvalue;
					tempvalue.type = TYPE_INT;
					tempvalue.value.iVal = (int)(modulus);
					return tempvalue;
				}
				if(right.type == TYPE_DOUBLE){
					printf("Modulus requires both types to be int");
					parse_error = INVALID_MODULUS;
					Value tempvalue;
					tempvalue.type = TYPE_UNKNOWN;
					return tempvalue;
				}
			}
			if(left.type == TYPE_DOUBLE){
				printf("Modulus requires both types to be int");
				parse_error = INVALID_MODULUS;
				Value tempvalue;
				tempvalue.type = TYPE_UNKNOWN;
				return tempvalue;
			}
		}
		}
		else if(node->type == ASSIGN_OP){
			printf("%s ", "=");
		}
		
		
		
		if(node->right != NULL){
			evalTree(node->right);
		}
}

/// Displays the infix expression for the tree, using
/// parentheses to indicate the precedence, e.g.:
///
/// expression: 10 20 + 30 *
/// infix string: ((10 + 20) * 30) 
///
/// @precondition:  This routine should not be called if there
/// 	is a parser error.
void infixTree(ExpNode* node){
	if(node){
		printf("%s ", "(");
		if(node->left != NULL){
			infixTree(node->left);
		}
		//printf("%d ", node->type);
		//printf("F %d ::: T %d", isInteger(5),isInteger(5.5));
		if((node->type == DOUBLE) || (node->type == INTEGER) ||(node->type == SYMBOL)){
			//printf("%s ", "int");
			if(node->type == DOUBLE){
				//printf("%s ", "double");
				printf("%6f ", node->value.value.dVal);
			}
			else if(node->type == INTEGER){
				//printf("%s ", "int");
				printf("%d ", node->value.value.iVal);
			}
			else if(node->type == SYMBOL){
				//printf("%s ", "sym");
				printf("%s ", node->symbol);
			}
		}
		else if(node->type == ADD_OP){
				printf("%s ", "+");
		}
		else if(node->type == SUB_OP){
				printf("%s ", "-");
		}
		else if(node->type == DIV_OP){
				printf("%s ", "/");
		}
		else if(node->type == MUL_OP){
				printf("%s ", "*");
		}
		else if(node->type == MOD_OP){
				printf("%s ", "%");
		}
		else if(node->type == ASSIGN_OP){
				printf("%s ", "=");
		}
		if(node->right != NULL){
			infixTree(node->right);
		}
		printf("%s ", ")");
	}
}

int main(){
	ExpNode* left = makeExpNode("10.5", NULL, NULL);
	ExpNode* right = makeExpNode("5", NULL, NULL);
	ExpNode* top = makeExpNode("+", left, right);
	infixTree(top);
	exit(EXIT_SUCCESS);
}

#endif