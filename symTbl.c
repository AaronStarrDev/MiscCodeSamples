#ifndef SYMTBL_C
#define SYMTBL_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "symTbl.h"

static Symbol symTable[sizeof(Symbol)*10]; // should be the maximum size of the array (static)
int emptyTable();

/// Constructs the table by reading the file.  The format is
/// one symbol per line in the format:
///	variable-type variable-name	variable-value
void buildTable(char filename[]){
	if (access(filename, R_OK) != 0){
		fprintf(stderr, "Error loading symbol table");
		exit(EXIT_FAILURE);
	}
	else{ //unneeded due to exit failure, but is good to double check in a small program I feel
		FILE* file = fopen(filename, "r"); /* should check the result */
		//Symbol symTable[sizeof(Symbol)*10]; // should be the maximum size of the array (static)
		char line[256];

		while (fgets(line, sizeof(line), file)) {
			char* firstword;
			char* word;
			Symbol newSymbol;
			firstword = strtok(line, " "); // TYPE
			word = strtok(NULL, " "); // NAME
			if(strlen(word) < 16){
				strcpy(newSymbol.varName, word);
			}
			else{
				fprintf(stderr, "Error loading symbol table");
				exit(EXIT_FAILURE);
			}
			//printf("first word: %s --- second word: %s --- ", firstword, word);
			word = strtok(NULL, " "); // VALUE
			//printf("third word: %s\n", word);
			Value tempValue;
			
			if(strcmp(firstword, "int") == 0){
				tempValue.type = TYPE_INT;
			}
			else if(strcmp(firstword, "double") == 0){
				tempValue.type = TYPE_DOUBLE;
			}
			else{
				tempValue.type = TYPE_UNKNOWN;
			}
			
			if(tempValue.type == TYPE_INT){
				tempValue.value.iVal = atoi(word);
			}
			else if(tempValue.type == TYPE_DOUBLE){
				//printf("%4f", atof(word));
				tempValue.value.dVal = atof(word);
			}
			else{
				printf("%s", "UNKNOWN TYPE DEF symTbl");
			}
			newSymbol.value = tempValue;
			addTable(newSymbol);
			//printf("%s %d %d\n", newSymbol.varName, newSymbol.value.type, newSymbol.value.value.iVal);
			//printf("%s", line); 
		}
		fclose(file);
	}
}

/// Displays the contents of the symbol table in the following format:
/// "SYMBOL TABLE:
///	Type: variable-type Name: variable-name, Value: variable-value"
/// Each symbol should be printed one per line, tab-indented.
void dumpTable(){
	printf("SYMBOL TABLE:\n");
	int i = 0;
	for(i = 0; i < 10; i++){ // int types
		//printf("%s\n", symTable[i].varName);
		if ((symTable[i].value.type == TYPE_INT) && strcmp(symTable[i].varName, "") != 0){
			printf("\tType: int, Name: %s, Value: %d\n", symTable[i].varName, symTable[i].value.value.iVal);
		}
	}
	for(i = 0; i < 10; i++){ // double types
		//printf("%s", symTable[i].varName);
		if (symTable[i].value.type == TYPE_DOUBLE){
			printf("\tType: double, Name: %s, Value: %6f\n", symTable[i].varName, symTable[i].value.value.dVal);
		}
	}
}

/// Returns the symbol associated with variable name
/// @param symbol The name of the variable
/// @return The Symbol, if it exists, or NULL otherwise
Symbol* lookupTable(char variable[]){
	Symbol lookSymbol;
	Symbol *lookSym_PTR = &lookSymbol;
	int i;
	for(i = 0; i < 10; i++){
		//printf("%s", symTable[i].varName);
		if (strcmp(symTable[i].varName, variable) == 0){
			*lookSym_PTR = symTable[i];
			return lookSym_PTR;
		}
	}
	return NULL;
}

/// Attempt to add a new symbol to the table (if it is not
/// already full).
/// @param Symbol symbol the new symbol to add
/// @return 0 if the symbol could not be added, anything else otherwise
int addTable(Symbol symbol){
	int i = 0;
	for(i = 0; i < 10; i++){
		//printf("ADD: VarName %s\n", symTable[i].varName);
		if (strcmp(symTable[i].varName, "") == 0){
			symTable[i] = symbol;
			return 1;
		}
	}
	//if (i == 9){
	return 0;
	//}
}

/// Checks if table is full.
/// @return 0 if the table IS free (has space), 1 if not.
int emptyTable(){
	int i = 0;
	for(i = 0; i < 10; i++){
		//printf("ADD: VarName %s\n", symTable[i].varName);
		if (strcmp(symTable[i].varName, "") == 0){
			return 0;
		}
	}
	return 1;
}

/* int main(){
	char inp[20] = "symTest.txt";

	//printf("Filename: \n");
	//scanf("%s", inp);
	//printf("You entered: %s\n", inp);
	
	buildTable(inp);
	
	printf( "%s\n", "test 1" ); //test
	
	dumpTable();
	
	printf( "%s\n", "test 2" ); //test
	
	Symbol testSym;
	Symbol *testSym_PTR = &testSym;
	
	if (lookupTable("two") == NULL){exit(EXIT_FAILURE);}
	*testSym_PTR = *lookupTable("two");
	printf("\tType: int, Name: %s, Value: %d\n", testSym.varName, testSym.value.value.iVal);
	
	if (lookupTable("onefive") == NULL){exit(EXIT_FAILURE);}
	*testSym_PTR = *lookupTable("onefive");
	printf("\tType: double, Name: %s, Value: %g\n", testSym.varName, testSym.value.value.dVal);
	
	printf( "%s\n", "test 3" ); //test
	
	if (lookupTable("seven") == NULL){exit(EXIT_FAILURE);}
	*testSym_PTR = *lookupTable("seven");
	printf("\tType: int, Name: %s, Value: %d\n", testSym.varName, testSym.value.value.iVal);
	
	printf( "%s\n", "test 4" ); //test
	
	exit(EXIT_SUCCESS);
}
 */
 
 #endif

