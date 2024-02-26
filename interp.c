#ifndef INTERP_C
#define INTERP_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "symTbl.h"
#include "stack.h"
#include "stackNode.h"
#include "parser.h"

#define MAX_ARGS 2

/*
main(int, char*)
takes in command line arguments
main function of program, returns -1 on exit failure
*/
int main( int argc, char *argv[] ){
	
	if( argc > 2 ){
		fprintf(stderr, "usage: interp [sym-table]");
		exit(EXIT_FAILURE);
	}
	if(argc == 2){
		if (access(argv[1], R_OK) != 0){
			fprintf(stderr, "Error loading symbol table");
			exit(EXIT_FAILURE);
		}
		buildTable(argv[1]);
		dumpTable();
	}
	char inp[20];
	printf("Enter postfix expressions (CTRL-D to exit):\n");
	while(emptyTable() == 0){ // see symTbl.c for emptyTable()
		printf("> ");
		scanf("%s", inp);
		parse(inp);
		//printf("You entered: %s\n", inp);
	}
	/* dumpTable();
	Symbol test;
	strcpy(test.varName, "test");
	test.value.type = TYPE_INT;
	test.value.value.iVal = 5;
	addTable(test);
	dumpTable(); */
	
	exit(EXIT_SUCCESS);
}

#endif

