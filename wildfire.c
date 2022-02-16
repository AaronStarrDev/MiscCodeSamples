/*
 * File:    		wildfire.c
 * Description:		Implements the spread of fire in a tree square
 * Author:  		Aaron Starr
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "display.c"
 
#define NEIGHBORS_BURNING_PERCENT .25
#define CURSOR_NUMBER 5 // the magic number of arguments required to possibly designate a cursor-based program run
#define PRINT_NUMBER 6 // the magic number of arguments required to possibly designate a print program run
#define BURNING_TREE '*'
#define TREE 'Y'
#define BURNT_TREE '_'
#define EMPTY ' '

void initialize_values(int arguments, char *argv[], int *size, int *probability, double *density, double *init_burning);
void initialize_oneD_field(char *oneD_Field, int *size, double *density, double *init_burning);
void shuffle(char *field, int limit);
void dimensional_rift(int *size, char *oneD_Field, char (*twoD_Field)[*size]);
void print_2D_array(int *size, char (*twoD_Field)[*size]);
void update(int *size, int *probability, char (*twoD_Field)[*size], int *changes, int *cum_changes);
int number_burning(int *size, char (*twoD_Field)[*size]);

/*
main(int, char*)
takes in command line arguments
main function of program, returns -1 on exit failure
*/
int main( int argc, char *argv[] ){
	
	static int size = 0; // the width of the grid in characters and the height in rows. 
	// minimum size is 5. maximum is 40.
	int *size_PTR = &size;
	
	static int cycles = 0; // only used in -p# flag usage
	int *cycles_PTR = &cycles;
	
	static int probability = 0; // chance of a tree catching fire in percentage. 
	// minimum is 0. maximum is 100. EX:: prob = 60, means 60%
	int *probability_PTR = &probability;
	
	static double density = 0; // proportion of the simulation space that is occupied expressed as an integer. 
	// Interprets it as the percentage of the whole space (size X size) that is to be occupied by trees. minimum is 0. maximum is 100.
	double *density_PTR = &density;
	
	static double initial_burning = 0; // proportion of the population that is burning at the start of the simulation. 
	// The proportion burning is an initial value, which will vary as the simulation proceeds. minimum is 0.  maximum is 100. EX:: prob = 60, means 60%
	double *init_burning_PTR = &initial_burning;
	
	static int changes = 0; // number of changes in a time frame. A change is something catching fire or extinguishing.
	int *changes_PTR = &changes;
	
	static int cum_changes = 0; // cumulative changes
	int *cum_changes_PTR = &cum_changes;
	
	
	if( (argc != CURSOR_NUMBER) && (argc != PRINT_NUMBER) ){
		fprintf(stderr, "usage error: An incorrect number of arguments were entered, %d arguments were entered. \
		\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
		\nThe -pN option tells the simulation to print N cycles and stop. \
		\nThe probability is the probability a tree will catch fire.", argc);
		exit(EXIT_FAILURE);
	}
	
	else if (argc == CURSOR_NUMBER){ // cursor mode possibly
		//char field[*size_PTR][*size_PTR];
		//printf("%s\n",argv[2]);
		initialize_values(argc, argv, size_PTR, probability_PTR, density_PTR, init_burning_PTR);
		char oneD_Field[(*size_PTR) * (*size_PTR)];
		initialize_oneD_field(oneD_Field, size_PTR, density_PTR, init_burning_PTR);
		char twoD_Field[(*size_PTR)][(*size_PTR)];
		dimensional_rift(size_PTR, oneD_Field, twoD_Field);
		system("clear"); // see notes in while loop clear usage
		print_2D_array(size_PTR, twoD_Field);
		printf("cycle %d, size %d, probCatch %.2f, density %.2f, proportion %.2f, changes %d\n", *cycles_PTR, *size_PTR, (((float)*probability_PTR)/100), (*density_PTR/100), (*init_burning_PTR/100), *changes_PTR);
		sleep(2);
		while(number_burning(size_PTR, twoD_Field) > 0){
			update(size_PTR, probability_PTR, twoD_Field, changes_PTR, cum_changes_PTR);
			//clear(); // this doesn't work, reads the ESC key "\033" but doesn't know what "[2J" is.
			//system("cls"); // this is for windows
			system("clear"); // I would love to have used clear(); if it actually worked, this seems to work
			print_2D_array(size_PTR, twoD_Field);
			cycles++;
			printf("cycle %d, size %d, probCatch %.2f, density %.2f, proportion %.2f, changes %d\n", *cycles_PTR, *size_PTR, (((float)*probability_PTR)/100), (*density_PTR/100), (*init_burning_PTR/100), *changes_PTR);
			sleep(2);
		}
		printf("fire is out after %d cumulative changes", *cum_changes_PTR);
		//printf("%c 1d[0], %c 1d[20], %c 1d[60], %c 1d[300]\n", oneD_Field[0], oneD_Field[20], oneD_Field[60], oneD_Field[300]);
	}
	
	else { // print argument possibly
		if ( isdigit(*argv[1]) ){ // this is redundant to the while loop condition, however it offers more information on error
			fprintf(stderr, "usage error: %d arguments but not designated as print mode.\nuse -p# flag, where # is cycles \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argc);
			exit(EXIT_FAILURE);
		}
		
		int flag;
		while ((flag = getopt(argc, argv, "p:")) != -1){
			switch (flag) {
				
				case 'p':
					if(atoi(optarg) < 0){
						fprintf(stderr, "usage error: The -pN option was negative. \
						\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
						\nThe -pN option tells the simulation to print N cycles and stop. \
						\nThe probability is the probability a tree will catch fire.");
						exit(EXIT_FAILURE);
						}
					*cycles_PTR = atoi(optarg); // value after p is entered as the cycles
					//char field[*size_PTR][*size_PTR];
					initialize_values(argc, argv, size_PTR, probability_PTR, density_PTR, init_burning_PTR);
					char oneD_Field[(*size_PTR) * (*size_PTR)];
					initialize_oneD_field(oneD_Field, size_PTR, density_PTR, init_burning_PTR);
					char twoD_Field[(*size_PTR)][(*size_PTR)];
					dimensional_rift(size_PTR, oneD_Field, twoD_Field);
					print_2D_array(size_PTR, twoD_Field);
					//printf("%d", *cycles_PTR);
					int cycles_start = 0;
					while((number_burning(size_PTR, twoD_Field) > 0) && (cycles_start < *cycles_PTR)){
						update(size_PTR, probability_PTR, twoD_Field, changes_PTR, cum_changes_PTR);
						print_2D_array(size_PTR, twoD_Field);
						cycles_start++;
						printf("cycle %d, size %d, probCatch %.2f, density %.2f, proportion %.2f, changes %d\n", cycles_start, *size_PTR, (((float)*probability_PTR)/100), (*density_PTR/100), (*init_burning_PTR/100), *changes_PTR);
					}
					printf("fire is out after %d cumulative changes", *cum_changes_PTR);
					break;
			}
		}
	}
	//printf("%d size, %d cycles, %d prob, %f den, %f init\n", *size_PTR, *cycles_PTR, *probability_PTR, *density_PTR, *init_burning_PTR);
	exit(EXIT_SUCCESS);
}

/*
Finds how many trees are burning
takes size and a 2d array
returns int, number of burning trees
*/
int number_burning(int *size, char (*twoD_Field)[*size]){
	int i;
	int j;
	int burning = 0;
	for(i = 0; i < *size; i++){
		for(j = 0; j < *size; j++){
			if (twoD_Field[i][j] == BURNING_TREE){
				burning++;
			}
		}
	}
	return burning;
}

/*
2 iterations. first to extinguish trees, second checks for spreading.
takes a 2d array, size, and probability of spread/extinguish
returns nothing
*/
void update(int *size, int *probability, char (*twoD_Field)[*size], int *changes, int *cum_changes){
	*changes = 0;
	//FIRST ITERATION (EXTINGUISH)
	int i;
	int j;
	srand(time(NULL));
	for(i = 0; i < *size; i++){
		for(j = 0; j < *size; j++){
			if (twoD_Field[i][j] == BURNING_TREE){
				float test = rand() % 100; // 0-99 :: SPREAD FUNCTIONALITY
				//printf("%f test\n", test);
				if ((test) < *probability){
					twoD_Field[i][j] = BURNT_TREE;
					*changes = *changes + 1;
					*cum_changes = *cum_changes + 1;
				}
			}
		}
	}
	//SECOND ITERATION (CATCH FIRE)
	srand(time(NULL));
	for(i = 0; i < *size; i++){
		for(j = 0; j < *size; j++){
			if (twoD_Field[i][j] == TREE){
				float num_of_neighbors = 0;
				float neighbors_burning = 0;
				
				if (!((i - 1) < 0) ){ // NORTH -i j
					if (twoD_Field[i - 1][j] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i - 1][j] == TREE){
						num_of_neighbors++;
					}
				}
				if ( (!((i - 1) < 0)) && (!((j + 1) == *size)) ){ // NORTH EAST -i +j
					if (twoD_Field[i - 1][j + 1] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i - 1][j + 1] == TREE){
						num_of_neighbors++;
					}
				}
				if (!((j + 1) == *size) ){ // EAST i +j
					if (twoD_Field[i][j + 1] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i][j + 1] == TREE){
						num_of_neighbors++;
					}
				}
				if ( (!((i + 1) == *size)) && (!((j + 1) == *size)) ){ // SOUTH EAST +i +j
					if (twoD_Field[i + 1][j + 1] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i - 1][j] == TREE){
						num_of_neighbors++;
					}
				}
				if (!((i + 1) == *size) ){ // SOUTH +i j
					if (twoD_Field[i + 1][j] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i + 1][j] == TREE){
						num_of_neighbors++;
					}
				}
				if ( (!((i + 1) == *size)) && (!((j - 1) < 0)) ){ // SOUTH WEST +i -j
					if (twoD_Field[i + 1][j - 1] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i + 1][j - 1] == TREE){
						num_of_neighbors++;
					}
				}
				if (!((j - 1) < 0) ){ // WEST i -j
					if (twoD_Field[i][j - 1] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i][j - 1] == TREE){
						num_of_neighbors++;
					}
				}
				if ( (!((i - 1) < 0)) && (!((j - 1) < 0)) ){ // NORTH WEST -i -j
					if (twoD_Field[i - 1][j - 1] == BURNING_TREE){
						num_of_neighbors++;
						neighbors_burning++;
					}
					else if (twoD_Field[i - 1][j - 1] == TREE){
						num_of_neighbors++;
					}
				}
				//printf("-------");
				//printf("%f NUM, %f BURN\n", num_of_neighbors, neighbors_burning);
				if ((neighbors_burning >= (num_of_neighbors * NEIGHBORS_BURNING_PERCENT)) && (neighbors_burning != 0) && (num_of_neighbors != 0)){ // at least 25% of neightbors are burning
					float test = rand() % 100; // 0-99 :: SPREAD FUNCTIONALITY
					if ((test) < *probability){
						twoD_Field[i][j] = BURNING_TREE;
						*changes = *changes + 1;
						*cum_changes = *cum_changes + 1;
					}
				}
			}
		}
	}
	//printf("-------\n");
}

/*
print a 2d array (assumed square)
takes a 2d array, and size
returns nothing
*/
void print_2D_array(int *size, char (*twoD_Field)[*size]){
	int i;
	for(i = 0; i < *size; i++){
		int j;
		for(j = 0; j < *size; j++){
			printf("%c ", twoD_Field[i][j]);
		}
		printf("\n");
	}
}

/*
change the 1D array into a 2D array
takes in a 1D array, 2D array, and size dimension.
returns nothing
*/
void dimensional_rift(int *size, char *oneD_Field, char (*twoD_Field)[*size]){
	int i;
	for (i = 0; i < *size; i++) {
		int j;
		for (j = 0; j < *size; j++) {
			twoD_Field[i][j] = oneD_Field[(i * (*size)) + j];  
		}
	}
}

/*
places values into 1d array
takes in a 1d array, size, density, and init burning
returns nothing
*/
void initialize_oneD_field(char *oneD_Field, int *size, double *density, double *init_burning){
	
	//char oneD_Field[(*size) * (*size)];
	
	int number_trees = (int)(( (*size) * (*size) ) * (*density / 100.00));
	int trees_burning = (int)(( number_trees ) * (*init_burning / 100.00));
	number_trees -= trees_burning;
	//printf("%d number of trees, %d trees burning, %d size\n", number_trees, trees_burning, *size);
	
	int i;
	for (i = 0; i < ((*size) * (*size)); i++){
		if (trees_burning > 0){
			oneD_Field[i] = BURNING_TREE;
			trees_burning--;
		}
		else if (number_trees > 0){
			oneD_Field[i] = TREE;
			number_trees--;
		}
		else{
			oneD_Field[i] = EMPTY;
		}
	}
	
	shuffle(oneD_Field, ( (*size) * (*size) )); // shuffle the 1D array
	//printf("%c 1d[0], %c 1d[20], %c 1d[60], %c 1d[300]\n", oneD_Field[0], oneD_Field[20], oneD_Field[60], oneD_Field[300]);
}

/*
shuffle a 1D array
takes in an array and its size/limit
returns nothing
*/
void shuffle(char *field, int limit){
	int first_spot;
	srand(time(NULL)); // starts randomization, without it, rand() essentially doesn't work.
	for (first_spot = 0; first_spot < limit; first_spot++){
		int second_spot = rand() % limit;
		//printf("%d %d\n", first_spot, second_spot);		
		char temp = field[second_spot];
		field[second_spot] = field[first_spot];
		field[first_spot] = temp;
	}
}

/*
init(int, char*, *size)
takes in command line arguments and size_PTR
returns character array that will be used for the program
*/
void initialize_values(int arguments, char *argv[], int *size, int *probability, double *density, double *init_burning){
	
    if(arguments == CURSOR_NUMBER){
		//printf("%d\n", atoi(argv[1]));
		if (atoi(argv[1]) < 5 || atoi(argv[1]) > 40){
			fprintf(stderr, "usage error: size argument (argv[1]) is %s, must be in interval 5-40 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[1]);
			exit(EXIT_FAILURE);
		}
		else { *size = (atoi(argv[1])); }
		
		if (atoi(argv[2]) < 0 || atoi(argv[2]) > 100){
			fprintf(stderr, "usage error: probability argument (argv[2])is %s, must be in interval 0-100 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[2]);
			exit(EXIT_FAILURE);
		}
		else { *probability = atoi(argv[2]); }
		
		if (atoi(argv[3]) < 0 || atoi(argv[3]) > 100){
			fprintf(stderr, "usage error: density argument (argv[3]) is %s, must be in interval 0-100 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[3]);
			exit(EXIT_FAILURE);
		}
		else { *density = atoi(argv[3]); }
		
		if (atoi(argv[4]) < 0 || atoi(argv[4]) > 100){
			fprintf(stderr, "usage error: init_burning argument (argv[4]) is %s, must be in interval 0-100 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[4]);
			exit(EXIT_FAILURE);
		}
		else { *init_burning = atoi(argv[4]); }
	}
	else if(arguments == PRINT_NUMBER){ // else if, to run additional check inside function
		if (atoi(argv[2]) < 5 || atoi(argv[2]) > 40){
			fprintf(stderr, "usage error: size argument (argv[2]) is %s, must be in interval 5-40 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[2]);
			exit(EXIT_FAILURE);
		}
		else { *size = (atoi(argv[2])); }
		
		if (atoi(argv[3]) < 0 || atoi(argv[3]) > 100){
			fprintf(stderr, "usage error: probability argument (argv[3])is %s, must be in interval 0-100 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[3]);
			exit(EXIT_FAILURE);
		}
		else { *probability = atoi(argv[3]); }
		
		if (atoi(argv[4]) < 0 || atoi(argv[4]) > 100){
			fprintf(stderr, "usage error: density argument (argv[4]) is %s, must be in interval 0-100 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[4]);
			exit(EXIT_FAILURE);
		}
		else { *density = atoi(argv[4]); }
		
		if (atoi(argv[5]) < 0 || atoi(argv[5]) > 100){
			fprintf(stderr, "usage error: init_burning argument (argv[5]) is %s, must be in interval 0-100 \
			\nusage: wildfire [-pN] size probability treeDensity proportionBurning \
			\nThe -pN option tells the simulation to print N cycles and stop. \
			\nThe probability is the probability a tree will catch fire.", argv[5]);
			exit(EXIT_FAILURE);
		}
		else { *init_burning = atoi(argv[5]); }
	}
	
}