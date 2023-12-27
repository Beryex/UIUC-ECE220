#include <stdio.h>
#include <stdlib.h>

#include "mp4.h"

int
main (int argc, const char* const argv[])
{
    int32_t r1;    /* region 1 size */
    int32_t r2;    /* region 2 size */
    int32_t r3;    /* region 3 size */
    int32_t r4;    /* region 4 size */
    int32_t width; /* row width */

    if (6 != argc) {
        fprintf (stderr, "syntax: %s <r1> <r2> <r3> <r4> <width>\n", argv[0]);
	return 2;
    }

    r1 = atoi (argv[1]);
    r2 = atoi (argv[2]);
    r3 = atoi (argv[3]);
    r4 = atoi (argv[4]);
    width = atoi (argv[5]);

    if (1 > r1 || 50 < r1 || 0 > r2 || 50 < r2 || 0 > r3 || 50 < r3 ||
        0 > r4 || 50 < r4 || 1 > width || 50 < width) {
        fputs ("Region 1 must be between 1 and 50 pixels.\n", stderr);
        fputs ("Regions 2-4 must be between 0 and 50 pixels.\n", stderr);
        fputs ("Width must be between 1 and 50 pixels.\n", stderr);
	return 2;
    }

    if (1 != print_row (r1, r2, r3, r4, width)) {
        printf ("Impossible!\n");
	return 3;
    }

    return 0;
}

