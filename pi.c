/*
File Name:

Author: Aaron Starr

Class: Computational Problem Solving for Engineers

Section:  01

Program Intent:To write a program that calculates an approximation pi from three different infinite series. The program asks for a non-negative integer. If the input is not a non-negative integer, the program
prints a message and terminates the program. If the input is zero, the program terminates. If the input is correct it  outputs an approximation pi.

Input Data: Non-zero, non-negative integer value. (User entered)

Output Data: Approximation of pi, to first, second, and third series of input terms. Terminate message.
*/

/*---------------
Include Section
----------------*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>


/*--------------------------- main function -------------------------------
Purpose: To prompt the user and display the calculations formed from the other three functions. Mainly used to set defined limits to what the user enters and checks for validity in following instructions. When true, the program calls the other functions for values to pi.

Returns: return (EXIT_SUCCESS) to the OS.
---------------------------------------------------------------------------*/
long double approx1(int a);
long double approx2(int b);
long double approx3(int c);
long double main()
{
	int x;

	puts(" ");
	printf("Enter terms in which to approximate pi to: ");
	scanf("%d", &x);
	puts(" ");
	while (x != 0)
	{
		if (x == 1)
		{
			printf("The value of pi approximated to %d term is %.8Lf.\n", x, approx1(x));
			break;
		}
		else if (x > 1)
		{
			printf("The value of pi approximated to %d terms in function one is %.8Lf.\n", x, approx1(x));
			printf("The value of pi approximated to %d terms in function two is %.8Lf.\n", x, approx2(x));
			printf("The value of pi approximated to %d terms in function three is %.8Lf.\n", x, approx3(x));
			break;
		}
		else
		{
			printf("Negative value entered, try again.\n");
			main();
		}
	}
	puts(" ");
	printf("***** Program Terminated *****\n");
	puts(" ");


	return (EXIT_SUCCESS);;
}

long double approx1(int x)
{
	long double pi = 0;
	int i = 1, num, denom;
	num = 4;
	denom = 1;

	while (i <= x)
	{
		if (i % 2 != 0)
		{
			pi = pi + (num / denom);
		}
		else
		{
			pi = pi - (num / denom);
		}
		denom = denom + 2;
		i = i + 1;
	}
	return pi;
}

long double approx2(int x)
{
	long double pi = 1, num = 1, denom = 1;
	int i = 1;

	for (i = 1; i <= x; i = i + 1)
	{
		if (i % 2 != 0)
		{
			pi = pi + num / (denom * denom);
		}
		else
		{
			pi = pi - num / (denom * denom);
		}
		denom = denom + 1;
	}
	return sqrt(12 * pi);
}

long double approx3(int x)
{
	long double pi = 1, num = 1, denom = 1, den = 3, exp = 0; // denom = first term, den = 3, exp = exp++
	int i = 1;

	for (i = 1; i <= x; i = i + 1)
	{
		if (i % 2 != 0)
		{
			pi = pi + num / (denom * pow(den, exp));
		}
		else
		{
			pi = pi - num / (denom * pow(den, exp));
		}
		denom = denom + 2;
		exp = exp + 1;
	}
	return pow(12, .5) * pi;
}
