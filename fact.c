/*
File Name:fact.c

Author:Aaron Starr

Class: Computational Problem Solving for Engineers

Section: 01

Program Intent: The program intent is to provide the factorial of any nonnegative integer value. The program shows all values of the three approximations up to 14, 
at which it will then show values at intervals of 5, followed by the exact integer value should it not be a factor of 5.

Input Data: Nonnegative integer

Output Data:Exact factorial, approximation using one formula, and a second approximation using a second formula.
*/

/*---------------
Include Section
----------------*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define PI 3.141592653589793
#define TRUE 1
#define FALSE 0


/*--------------------------- main function -------------------------------
Purpose: To provide the values of approximations and list them in an orderly fashion. Main function also checks for the input to be nonnegative and an integer value.
After all this done it then asks the user if they would like to input another approximation value.

Returns: Approximation values, Exact value (x <= 14), and EXIT_SUCCESS to OS.
---------------------------------------------------------------------------*/
long long fact(int g);
double stirling1(int g);
double stirling2(int g);
void clearbuffer();
char ch;
int flag, again = 1, i = 1, n;
int main()
{


while (again == TRUE)
{
	puts(" ");
	printf("Please input a nonnegative integer: ");
	flag = scanf("%d", &n);
	clearbuffer();

	if (flag != 1)
	{
		puts(" ");
		printf("Input must be an integer.\n");
		continue;
	}

	else if (n <= 0)
	{
		puts(" ");
		printf("The factorial is undefined.\n");
		continue;
	}
	else if (n <= 14)
	{
		puts(" ");
		printf("Number     Factorial      Approximation1     Approximation2\n");
		printf("-------------------------------------------------------\n");
		for (i = 1; i <= n; i++)
		{
			printf("    %2d%14lld      %e      %e\n", i, fact(n), stirling1(n), stirling2(n));
		}
	}
	else
	{
		puts(" ");
		printf("  Number    Approximation1    Approximation2\n");
		printf("--------------------------------------------\n");
		printf("        1    %e     %e\n", stirling1(n), stirling2(n));
		for (i = 5; i <= n; (i++))
		{
			if ((i % 5) == 0)
			{
				printf("     %4d    %e     %e\n", i, stirling1(n), stirling2(n));
			}
			else if (i == n)
			{
				printf("     %4d    %e     %e\n", i, stirling1(n), stirling2(n));
			}
			else
			{
				
			}
		}

	}
	printf("\nDo you want to try another integer? (y/n): ");
	scanf(" %c", &ch);
	if (ch == 'n')
	{
	11		break;
	}
	else
	{
		puts(" ");
	}
	clearbuffer();
}
puts(" ");
printf("***** Program Terminated *****\n");

   return (EXIT_SUCCESS);
}
/*--------------------------- fact function -------------------------------
Purpose: To provide the first approximation to an input nonnegative integer factorial.

Returns: m
---------------------------------------------------------------------------*/
long long fact(int m)
{
	long result = 1;

	for (m = 1; m <= i; m++)
		result = result * m;

	return (result);
}


/*--------------------------- stirling1 function -------------------------------
Purpose: To provide the first approximation to an input nonnegative integer factorial. (Using stirling approximation)

Returns: p
---------------------------------------------------------------------------*/
double stirling1(int p)
{
	p = pow(i, i) * exp(0 - i) * sqrt(2 * PI * i);

	return (p);
}

/*--------------------------- stirling2 function -------------------------------
Purpose: To provide the second approximation to an input nonnegative integer factorial. (Using stirling approximation)

Returns: q
---------------------------------------------------------------------------*/
double stirling2(int q)
{
	q = pow(i, i) * exp(0 - i) * sqrt(2 * PI * i) * (1 + (1 / (12 * i)));

	return (q);
}


/*--------------------------- clearbuffer function -------------------------------
Purpose: To clear the input buffer.

Returns: cleared input values
---------------------------------------------------------------------------*/
void clearbuffer()
{
	{
		int ch;

		while ((ch = getchar()) != EOF && ch != '\n');
	}
}