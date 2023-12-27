#include <stdio.h>
#include <stdint.h>

/* This function can determine which pixels in a row must be black with r1,r2,r3,r4 descriping
black region sizes and width descriping the width of the row in pixels. The function will print
"X" to represent known black pixels and "." to represent other pixels. If the specified can be
fit into the given width, the function will print "\n" in the end and return 1 to indicate 
success. Otherwise, the function will return 0 to indicate failure and print nothing*/

int32_t print_row (int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t width)
{
	int counter = 0;										
	int result;												
	int offset;												
	int checker;											
	int length;												
	int i;													
	int j;
	if (r2 > 0) {											// let counter holds the numbers how many r1,r2,r3,r4 are positive
		counter = counter + 1;}
	if (r3 > 0) {
		counter = counter + 1;}
	if (r4 > 0) {
		counter = counter + 1;}
	if (width < r1 + r2 + r3 + r4 + counter) {				// if yes, width is too small to hold all regions
		result = 0;											// let result holds 0 to indicate failure
	} else { 
		offset = width - ( r1 + r2 + r3 + r4 + counter );	// let offset holds the number of "." in each region should be printed
		for ( checker = 4 ; checker > 0 ; checker-- ) {		// let checker holds from 4 to 1 to let length hold r1,r2,r3,r4's value
			switch(checker) {
				case 4: length = r1;
				break;
				case 3: length = r2;
				break;
				case 2: length = r3;
				break;
				case 1: length = r4;
				break;
			}
			if ( length > 0 && (checker == 3 || checker == 2 || checker == 1)) {	// only print "." between regions when the region has length > 0
				printf(".");
			}
			if ( offset >= length) {						// if offset >= length, print "." all in this region
				for ( i = length ; i >0 ; i-- ) {
					printf(".");
				}
			} else {										// if not, print offset * "." and (length - offset) * "X"
				for ( j = offset ; j > 0 ; j--) {
					printf(".");
				}
				for ( i = length - offset ; i > 0 ; i--) {
					printf("X"); 
				}	
			}
			if (checker == 1) {								// after r4 has been printed, print the rest of the row and a newline character
				for ( i = offset ; i > 0 ; i--){			// print the rest of row, which are all "."
					printf(".");
				}	
				printf("\n");
				result = 1;									// result holds 1 to indicate success
			}
		}
	}
	return result;
}