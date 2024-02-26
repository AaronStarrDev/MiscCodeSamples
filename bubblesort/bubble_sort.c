/*
 * bubble sort
 * Author: L. Kiser
 * Author: Aaron Starr
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bubble_sort.h"
#include "unit_tests.h"

// Swaps the characters at myarray[position] and myarray[position + 1].
// It is up to the calling program to ensure that position + 1 is a valid array location.
// If myarray[position] or myarray[position + 1] is an end of string ('\0') do not swap.
// This is an example of defensive programming.
// You are NOT allowed to use any library functions like swap().
void swap_characters_in_array( char myarray[], int position )
{
    
    int position2 = position++;
    
    char *array_ptr; 
    array_ptr = &myarray[position];
    char *array_ptr2; 
    array_ptr2 = &myarray[position2];
    
    //printf("%d -- %d\n", myarray[position], myarray[position2]);

    if ((!*array_ptr == '\0') && (!*array_ptr2 == '\0')){
        char temp;

        temp = *array_ptr ;
        *array_ptr = *array_ptr2 ;
        *array_ptr2 = temp ;
        
        //char temp = array_ptr[position++];
        //*array_ptr[position++] = array_ptr[position];
        //*array_ptr[position] = temp;
    }
//    else{
//        char temp = array_ptr[position++];
//        *array_ptr[position++] = array_ptr[position];
//        *array_ptr[position] = temp;
//    }

}

// Recursively sorts two characters in an array by their numeric value
// not a very good sort for alphabetic ordering).
// Returns the number of times a pair of characters were actually swapped.
int sort_two_positions(char myarray[], int position)
{
    int size = (sizeof(myarray) / sizeof(myarray[0]));
    int i;
    
    int swap_was_done = 0 ;
    
    for (i = position; i < size - 1; i++) // -1 removes end of line character (in theory)
    {
        int position2 = i++;
        
        char *array_ptr = &myarray[i]; 
        //array_ptr = &myarray[i];
        char *array_ptr2 = &myarray[position2]; 
        //array_ptr2 = &myarray[position2];
        
        
        if (array_ptr[i] == '\0' || array_ptr2[position2] == '\0')
    {
        return 0;
    }

    // In Line Testing
    //printf("%c -- %c\n", array_ptr, array_ptr2);
    //printf("%d -- %d\n", *array_ptr, *array_ptr2); 
    //printf("%c -- %c -- %c -- %c -- %c\n", myarray[0], myarray[1], myarray[2], myarray[3], myarray[4]);
    
    if (array_ptr[i] > array_ptr2[position2])
    {
        swap_characters_in_array(myarray, position);
        swap_was_done++;
        
        return swap_was_done + sort_two_positions(myarray, position2);
    }
    
    }

    // First see if myarray[position] or myarray[position + 1] is an end of string ('\0').
    // If so, then just return with a zero value.

    // Fill in code to decide on whether or not a swap is needed.
    // If a swap is needed use the swap_characters_in_array function you completed above
    // and set swap_was_done to a 1.

    // Now call this function recursively for the next position in the array.
    // This recursive call should look like this where you fill in the ? for the two parameters.
    // return swap_was_done + sort_two_positions( ?, ? ) ;
}

// Run the unit tests or as a "normal program".
// You can run this as a "normal program" if you want for a simple test of the sort_two_positions function.
int main( int argc, char *argv[] ) {
    char one_string[] = "test" ;
    int positions_swapped = 0 ;

    // Execute unit tests if program name contains "test".
    if ( strstr( argv[0], "test" ) )
        return test() ;

    // Simple test for the mid-level function.
    positions_swapped = sort_two_positions(one_string, 0) ;
    printf( "%d positions swapped. New string is: %s\n", positions_swapped, one_string ) ;

    return 0 ;
}

