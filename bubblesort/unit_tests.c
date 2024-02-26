// unit_tests.c
// Larry L. Kiser Oct. 22, 2015
// Copyright 2015 All Rights Reserved

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "bubble_sort.h"

// used by stdout switching functions
static int stdout_file_descriptor = 0 ;
static fpos_t stdout_position ;

// Redirect printf's to the specified text file (redirect stdout).
static void switch_stdout_to_file( char *filename )
{
	fflush( stdout ) ;
	fgetpos( stdout, &stdout_position ) ;
	stdout_file_descriptor = dup( fileno( stdout ) ) ;
	freopen( filename, "w", stdout ) ;
}

// Restore normal printf behavior (restore stdout).
// MUST be called only after a prior call to switch_stdout_to_file!
static void restore_stdout( void )
{
	if ( stdout_file_descriptor )
	{
		fflush( stdout ) ;
		dup2( stdout_file_descriptor, fileno( stdout ) ) ;
		close( stdout_file_descriptor ) ;
		clearerr( stdout ) ;
		fsetpos( stdout, &stdout_position ) ;
	}
}

// Simple boolean assert function for unit testing
int assert( int test_result, char error_format[], ... ) {
	va_list arguments ;
	static int test_number = 1 ;
	int result = 1 ;	// return 1 for test passed or 0 if failed
	
	if ( ! test_result ) {
		va_start( arguments, error_format ) ;
		printf( "Test # %d failed: ", test_number ) ;
		vprintf( error_format, arguments ) ;
		printf( "\n" ) ;
		result = 0 ;
	}
	test_number++ ;
	return result ;
}

//////////////////////////////////////////////////////////////////////////
// Begin unit tests //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// do the unit tests
int test()
{
	int passcount = 0 ;
	int failcount = 0 ;
	char empty_string[] = "" ;
	char string2[] = "ab" ;
	char string3[] = "testCAB" ;
	char string4[] = "bac1" ;
	char bogus_string[] = { '\0', 'a', 'b' } ;	// NOT a valid string
	char bogus_string_copy[] = { '\0', 'a', 'b' } ;	// NOT a valid string
	int positions_swapped ;
	
	swap_characters_in_array( bogus_string, 0 ) ;	// This must do nothing since we have an End of String in the 0 position
	assert( memcmp( bogus_string, bogus_string_copy, sizeof(bogus_string) ) == 0,
		"1. swap_characters_in_array failed -- it incorrectly swapped characters" )
		? passcount++ : failcount++ ;

	swap_characters_in_array( string2, 0 ) ;
	assert( strcmp( string2, "ba" ) == 0,
		"2. swap_characters_in_array failed -- it incorrectly swapped characters. Got %s: ", string2 )
		? passcount++ : failcount++ ;
		
	swap_characters_in_array( string4, 0 ) ;
	assert( strcmp( string4, "abc1" ) == 0,
		"3. swap_characters_in_array failed -- it failed to swap characters. Got %s: ", string4 )
		? passcount++ : failcount++ ;
	
	printf( "\nSummary of unit tests:\n%d tests passed\n%d tests failed\n\n", passcount, failcount ) ;
	
	return failcount ;
}