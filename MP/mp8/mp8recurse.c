#include <stdlib.h>
#include <string.h>

#include "mp8.h"

/*
	This project can implement several varieties of flood fill opreations with different stop conditions including
	stop at white pixels, stop at near-white pixels, stop at any color far enough from the color at the initial flood
	point and is limited to a 35-pixel range. This file includes 3 utility recursion functions basicRecurse, greyRecurse,
	limitedRecurse, which are called by basicFlood, greyFlood, limitedRecurse respectively.
*/

/*
 * basicRecurse -- flood fill recursively from a point until reaching
 *                 white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicRecurse (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t x, int32_t y, 
	      uint8_t* marking)
{
	int8_t direction;
	if ((x < 0) || (x > width -1) || (y < 0) || (y > height - 1)){	// if out of border, stop
		return;
	}
	*(marking + x + y * width) = 1;									// mark 1 at this pixel means been flooded
	for (direction = 1; direction <= 4; direction++){				// direction = 1,2,3,4 means direction is up,right,down,left
		switch(direction){											// check and recure in order: up,right,down,left
			case 1:													// check if upward is marked, if yes, recure
			if (*(marking + x + (y-1) * width) == 0){
				if (!((*(inRed + x + (y - 1) * width) == 255) && (*(inGreen + x + (y - 1) * width) == 255) && (*(inBlue + x + (y - 1) * width) == 255))){
					basicRecurse(width,height,inRed,inGreen,inBlue,x,y - 1,marking);	// if next point is not white, call basicRecurse to recur
				}
			}
			break;
			case 2:													// similiar to up
			if (*(marking + x + 1 + y * width) == 0){
				if (!((*(inRed + x + 1 + y * width) == 255) && (*(inGreen + x + 1 + y * width) == 255) && (*(inBlue + x + 1 + y * width) == 255))){
					basicRecurse(width,height,inRed,inGreen,inBlue,x + 1,y,marking);
				}
			}
			break;
			case 3:													// similiar to up
			if (*(marking + x + (y+1) * width) == 0){
				if (!((*(inRed + x + (y + 1) * width) == 255) && (*(inGreen + x + (y + 1) * width) == 255) && (*(inBlue + x + (y + 1) * width) == 255))){
					basicRecurse(width,height,inRed,inGreen,inBlue,x,y + 1,marking);	
				}
			}
			break;													
			case 4:													// similiar to up
			if (*(marking + x  - 1 + y * width) == 0){
				if (!((*(inRed + x - 1 + y * width) == 255) && (*(inGreen + x - 1 + y * width) == 255) && (*(inBlue + x - 1 + y * width) == 255))){
					basicRecurse(width,height,inRed,inGreen,inBlue,x - 1,y,marking);	
				}
			}
			break;
		}
	}
}


/*
 * greyRecurse -- flood fill recursively from a point until reaching
 *                near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyRecurse (int32_t width, int32_t height,
	     const uint8_t* inRed, const uint8_t* inGreen, 
	     const uint8_t* inBlue, 
	     int32_t x, int32_t y, uint32_t distSq, 
	     uint8_t* marking)
{
	int8_t direction;
	if ((x < 0) || (x > width -1) || (y < 0) || (y > height - 1)){	
		return;														// if out of border, stop
	}
	if (colorsWithinDistSq(*(inRed + x + y * width), *(inGreen + x + y * width), *(inBlue + x + y * width), 0xFF, 0xFF, 0xFF, distSq)){
		return;														// if the distance between this pixel's color and white is within the given value, stop
	}	
	*(marking + x + y * width) = 1;									// mark 1 at this pixel means been flooded
	for (direction = 1; direction <= 4; direction++){				// direction = 1,2,3,4 means direction is up,right,down,left
		switch(direction){											// check and recure in order: up,right,down,left
			case 1:													// check if upward is marked, if yes, recure
			if (*(marking + x + (y-1) * width) == 0){
				greyRecurse(width,height,inRed,inGreen,inBlue,x,y - 1, distSq, marking);
			}
			break;
			case 2:													// similiar to up
			if (*(marking + x + 1 + y * width) == 0){
				greyRecurse(width,height,inRed,inGreen,inBlue,x + 1,y, distSq, marking);
			}
			break;
			case 3:													// similiar to up
			if (*(marking + x + (y+1) * width) == 0){
				greyRecurse(width,height,inRed,inGreen,inBlue,x,y + 1, distSq, marking);
			}
			break;													
			case 4:													// similiar to up
			if (*(marking + x  - 1 + y * width) == 0){
				greyRecurse(width,height,inRed,inGreen,inBlue,x - 1,y, distSq, marking);
			}
			break;
		}
	}
}


/*
 * limitedRecurse -- flood fill recursively from a point until reaching
 *                   pixels too different (in RGB color) from the color at
 *                   the flood start point, too far away (> 35 pixels), or
 *                   beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         origX -- starting x position of the flood
 *         origY -- starting y position of the flood
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedRecurse (int32_t width, int32_t height,
	        const uint8_t* inRed, const uint8_t* inGreen, 
	        const uint8_t* inBlue, 
	        int32_t origX, int32_t origY, int32_t x, int32_t y, 
		uint32_t distSq, uint8_t* marking)
{
	int8_t direction;
	if((x-origX)*(x-origX)+(y-origY)*(y-origY) > 35 * 35){
		return;														// if further than 35 pixels from the origin point, stop
	}
	if ((x < 0) || (x > width -1) || (y < 0) || (y > height - 1)){	
		return;														// if out of border, stop
	}	
	if (!(colorsWithinDistSq(*(inRed + x + y * width), *(inGreen + x + y * width), *(inBlue + x + y * width), *(inRed + origX + origY * width), *(inGreen + origX + origY * width), *(inBlue + origX + origY * width), distSq))){
		return;														// if the distance between this pixel's color and original color is out of the given value, stop
	}
	*(marking + x + y * width) = 1;									// mark 1 at this pixel means been flooded
	for (direction = 1; direction <= 4; direction++){				// direction = 1,2,3,4 means direction is up,right,down,left
		switch(direction){											// check and recure in order: up,right,down,left
			case 1:													// check if upward is marked, if yes, recure
			if (*(marking + x + (y-1) * width) == 0){
				limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x,y - 1, distSq, marking);
			}
			break;
			case 2:													// similiar to up
			if (*(marking + x + 1 + y * width) == 0){
				limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x + 1,y, distSq, marking);
			}
			break;
			case 3:													// similiar to up
			if (*(marking + x + (y+1) * width) == 0){
				limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x,y + 1, distSq, marking);
			}
			break;													
			case 4:													// similiar to up
			if (*(marking + x  - 1 + y * width) == 0){
				limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x - 1,y, distSq, marking);
			}
			break;
		}
	}
}

