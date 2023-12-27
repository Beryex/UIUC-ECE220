#include <stdlib.h>
#include <string.h>

#include "mp8.h"

/*
	This project can implement several varieties of flood fill opreations with different stop conditions including
	stop at white pixels, stop at near-white pixels, stop at any color far enough from the color at the initial flood
	point and is limited to a 35-pixel range. This file includes 3 wrapper functions basicFlood, greyFlood, limitedRecurse.
	These operations use recursion. This file also include a function colorsWithinDistSq that compares if two colors 
	are within a given distance_square. 
*/


/*
 * basicFlood -- wrapper for flood filling recursively from a point until 
 *               reaching white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicFlood (int32_t width, int32_t height,
	    const uint8_t* inRed, const uint8_t* inGreen, 
	    const uint8_t* inBlue, 
	    int32_t startX, int32_t startY, 
	    uint8_t floodR, uint8_t floodG, uint8_t floodB,
	    uint8_t* outRed, uint8_t* outGreen, 
	    uint8_t* outBlue)
{
	int32_t index;
	uint8_t color, floodcolor;
	uint8_t* marking;
	const uint8_t* origin;
	for (color = 1; color <= 3; color++) {					// color = 1,2,3 represent red, green, blue respectively
		switch(color){
			case 1:
			origin = inRed;
			floodcolor = floodR;
			marking = outRed;
			break;
			case 2:
			origin = inGreen;
			floodcolor = floodG;
			marking = outGreen;
			break;
			case 3:
			origin = inBlue;
			floodcolor = floodB;
			marking = outBlue;
			break; 
		}
		for (index = 0; index <= (width * height - 1); index++){	// initialize the channel array with 0
			*(marking + index) = 0;
		}
		if (!((*(inRed + startX + startY * width) == 255) && (*(inGreen + startX + startY * width) == 255) && (*(inBlue + startX + startY * width) == 255))){
			basicRecurse (width, height, inRed, inGreen, inBlue, startX, startY, marking);	// if start point is not white, call basicRecurse to recur
		}
		for (index = 0; index <= (width * height - 1); index++){
			if (*(marking + index) == 0 ) {								// overwrite "out" channel to fill in the final image
				*(marking + index) = *(origin + index);					// mark with 0 means not be flooded, fill in original color
				continue;
			}
			if (*(marking + index) == 1 ) {								// mark with 1 means be flooded, fill in flood color
				*(marking + index) = floodcolor;
				continue;
			}
		}
	}					
}


/*
 * colorsWithinDistSq -- returns 1 iff two colors are within Euclidean
 *                       distance squared of one another in RGB space
 * INPUTS: r1 -- red component of color 1
 *         g1 -- green component of color 1
 *         b1 -- blue component of color 1
 *         r2 -- red component of color 2
 *         g2 -- green component of color 2
 *         b2 -- blue component of color 2
 *         distSq -- maximum distance squared for the check
 * RETURN VALUE: 1 if the sum of the squares of the differences in the 
 *               three components is less or equal to distSq; 0 otherwise
 * SIDE EFFECTS: none
 */
int32_t
colorsWithinDistSq (uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t r2, uint8_t g2, uint8_t b2, uint32_t distSq)
{
	int32_t R = r1 - r2, G = g1 - g2, B = b1 - b2,D;
	R = R * R;												// if color 1 and color 2 are close enough, return 1
	G = G * G;
	B = B * B;
	D = distSq;
	if (R + G + B <= D){											
		return 1;
	}
    return 0;
}


/*
 * greyFlood -- wrapper for flood filling recursively from a point until 
 *              reaching near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyFlood (int32_t width, int32_t height,
	   const uint8_t* inRed, const uint8_t* inGreen, 
	   const uint8_t* inBlue, 
	   int32_t startX, int32_t startY, 
	   uint8_t floodR, uint8_t floodG, uint8_t floodB, uint32_t distSq,
	   uint8_t* outRed, uint8_t* outGreen, 
	   uint8_t* outBlue)
{
	int32_t index;
	uint8_t color, floodcolor;
	uint8_t* marking;
	const uint8_t* origin;
	for (color = 1; color <= 3; color++) {					// color = 1,2,3 represent red, green, blue respectively
		switch(color){
			case 1:
			origin = inRed;
			marking = outRed;
			floodcolor = floodR;
			break;
			case 2:
			origin = inGreen;
			marking = outGreen;
			floodcolor = floodG;
			break;
			case 3:
			origin = inBlue;
			marking = outBlue;
			floodcolor = floodB;
			break; 
		}
		for (index = 0; index <= (width * height - 1); index++){	// initialize the channel array with 0
			*(marking + index) = 0;
		}
		greyRecurse (width, height, inRed, inGreen, inBlue, startX, startY, distSq, marking);	// call the function greyRecurse
		for (index = 0; index <= (width * height - 1); index++){
			if (*(marking + index) == 0 ) {								// overwrite "out" channel to fill in the final image
				*(marking + index) = *(origin + index);					// mark with 0 means not be flooded, fill in original color
				continue;
			}
			if (*(marking + index) == 1 ) {								// mark with 1 means be flooded, fill in flood color
				*(marking + index) = floodcolor;
				continue;
			}
		}
	}		
}


/*
 * limitedFlood -- wrapper for flood filling recursively from a point until 
 *                 reaching pixels too different (in RGB color) from the 
 *                 color at the flood start point, too far away 
 *                 (> 35 pixels), or beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedFlood (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t startX, int32_t startY, 
	      uint8_t floodR, uint8_t floodG, uint8_t floodB, uint32_t distSq,
	      uint8_t* outRed, uint8_t* outGreen, 
	      uint8_t* outBlue)
{
	int32_t index;
	uint8_t color, floodcolor;
	uint8_t* marking;
	const uint8_t* origin;
	for (color = 1; color <= 3; color++) {					// color = 1,2,3 represent red, green, blue respectively
		switch(color){
			case 1:
			origin = inRed;
			marking = outRed;
			floodcolor = floodR;
			break;
			case 2:
			origin = inGreen;
			marking = outGreen;
			floodcolor = floodG;
			break;
			case 3:
			origin = inBlue;
			marking = outBlue;
			floodcolor = floodB;
			break; 
		}
		for (index = 0; index <= (width * height - 1); index++){	// initialize the channel array with 0
			*(marking + index) = 0;
		}
		limitedRecurse (width, height, inRed, inGreen, inBlue, startX, startY, startX, startY, distSq, marking);	// call the function greyRecurse
		for (index = 0; index <= (width * height - 1); index++){
			if (*(marking + index) == 0 ) {								// overwrite "out" channel to fill in the final image
				*(marking + index) = *(origin + index);					// mark with 0 means not be flooded, fill in original color
				continue;
			}
			if (*(marking + index) == 1 ) {								// mark with 1 means be flooded, fill in flood color
				*(marking + index) = floodcolor;
				continue;
			}
		}
	}		
}

