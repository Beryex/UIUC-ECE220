#include <stdio.h>
#include <stdint.h>
int32_t print_row (int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t width)
{
	int counter = 0;
	int result;
	int offset;
	int i;
	int j;
	if (r2 > 0) {
		counter = counter + 1;}
	if (r3 > 0) {
		counter = counter + 1;}
	if (r4 > 0) {
		counter = counter + 1;}
	if (width < r1 + r2 + r3 + r4 + counter) {
		result = 0;
	} else { 
		offset = width - ( r1 + r2 + r3 + r4 + counter );
		if ( offset >= r1) {
			for ( i = r1 ; i >0 ; i-- ) {
				printf(".");
			}
		} else {
			for ( j = offset ; j > 0 ; j--) {
				printf(".");
			}
			for ( i = r1 - offset ; i > 0 ; i--) {
				printf("X"); 
			}
		}
		if ( r2 > 0 ) {
			printf(".");
		}
		if ( offset >= r2) {
			for ( i = r2 ; i >0 ; i--) {
				printf(".");
			}
		} else {
			for ( j = offset ; j > 0 ; j--) {
				printf(".");
			}
			for ( i = r2 - offset ; i > 0 ; i--) {
				printf("X");
			}
		}
		if ( r3 > 0 ) {
			printf(".");
		}
		if ( offset >= r3) {
			for ( i = r3 ; i >0 ; i--) {
				printf(".");
			}
		} else {
			for ( j = offset ; j > 0 ; j--) {
				printf(".");
			}
			for ( i = r3 - offset ; i > 0 ; i--) {
				printf("X");
			}
		}
		if ( r4 > 0 ) {
			printf(".");
		}	
		if ( offset >= r4) {
			for ( i = r4 ; i >0 ; i--) {
				printf(".");
			}
		} else {
			for ( j = offset ; j > 0 ; j--) {
				printf(".");
			}
			for ( i = r4 - offset ; i > 0 ; i--) {
				printf("X");
			}
		}
		for ( i = offset ; i > 0 ; i--){
			printf(".");
		}
		printf("\n");
		result = 1;
	}
	return result;
}		