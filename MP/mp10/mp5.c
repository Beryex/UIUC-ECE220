/*									tab:8
 *
 * main.c - skeleton source file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 */




#include <stdio.h>
#include "mp5.h"

/*
	This function will draw a 624 * 320 pixels PNG picture about a symbol in the game WATCH_DOG.
	This function will use some defined colors and a combination of subfunctions including 
	near_horizontal, near_vertical, draw_lines, draw_rect, draw_triangle, draw_parallelogram,
	draw_circle, rect_gradient.
*/



/* 
 *  near_horizontal
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_horizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end) {
	int32_t x1, x2, y1, y2;		
	int32_t x,y;													
	int32_t counter;												
	int32_t sign;
	int32_t tem_result = 1;													
	int32_t subresult = 1;
	if (x_end - x_start > 0) {										// use (x1,y1) denote the point with smaller x and (x2,y2) denote the other
		x1 = x_start;
		y1 = y_start;
		x2 = x_end;
		y2 = y_end;
	} else {
		x1 = x_end;
		y1 = y_end;
		x2 = x_start;
		y2 = y_start;
	}
	sign = y2 - y1;
	if (sign > 0) {													// sign is the sign of (y2-y1)
		sign = 1;
	} else if(sign == 0){
		sign = 0;
	} else if(sign < 0){
		sign = -1;
	}
	for (counter = x2 - x1 + 1; counter > 0; counter--) {			// let counter holds the value of the pixels needed to draw
		x = x2 + 1 - counter;										// use (x,y) denote an arbitrary point
		if (x == x1){
			y = y1;
		} else if (x == x2){
			y = y2;
		} else{
			y = (2*(y2-y1)*(x-x1)+(x2-x1)*sign) / (2*(x2-x1)) + y1;
		}
		tem_result = draw_dot(x,y);
		subresult = subresult && tem_result;						// use subresult to check if all called functions works successfully
	}
	return subresult;
}


/* 
 *  near_vertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_vertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end) {
	int32_t x1, x2, y1, y2;		
	int32_t x,y;													
	int32_t counter;												
	int32_t sign;
	int32_t tem_result = 1;													
	int32_t subresult = 1;
	if (y_end - y_start > 0) {										// use (x1,y1) denote the point with smaller y and (x2,y2) denote the other
		x1 = x_start;
		y1 = y_start;
		x2 = x_end;
		y2 = y_end;
	} else if (y_end - y_start < 0) {
		x1 = x_end;
		y1 = y_end;
		x2 = x_start;
		y2 = y_start;
	} else if (y_end - y_start == 0) {								// if y_end == y_start, there is only one point, print nothing and return 0
		return 0;
	}
	sign = x2 - x1;
	if (sign > 0) {													// sign is the sign of (x2-x1)
		sign = 1;
	} else if(sign == 0){
		sign = 0;
	} else if(sign < 0){
		sign = -1;
	}
	for (counter = y2 - y1 + 1; counter > 0; counter--) {			// let counter holds the value of the pixels needed to draw
		y = y2 + 1 - counter;										// use (x,y) denote an arbitrary point
		if (y == y1){
			x = x1;
		} else if(y == y2){
			x = x2;
		} else{
			x = (2*(x2-x1)*(y-y1)+(y2-y1)*sign) / (2*(y2-y1)) + x1;
		}
		tem_result = draw_dot(x,y);
		subresult = subresult && tem_result;						// use subresult to check if all called functions works successfully
	}
	return subresult;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_line(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	int32_t result = 1;
	int32_t tem_result = 1;
	double slope;
	if (x_end == x_start) {														// if slope not exist or < -1, or > 1, call near_vertical
		tem_result = near_vertical(x_start, y_start, x_end, y_end);
		result = result && tem_result;											// use result to check if all called functions works successfully
	} else {
		slope = (double)(y_end - y_start) / (double)(x_end - x_start);
		if ( (slope > 1) || (slope < -1) ) {
			tem_result = near_vertical(x_start, y_start, x_end, y_end);
			result = result && tem_result;	
		} else {																// if -1 <= slope <= 1, call near horizontal 
			tem_result = near_horizontal(x_start, y_start, x_end, y_end);	
			result = result && tem_result;	
		}
	}
	return result;
}


/* 
 *  draw_rect
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 * OUTPUTS: draws a pixel to every point of the edges of the rectangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_rect(int32_t x, int32_t y, int32_t w, int32_t h){
	int32_t x1, y1, x2, y2, counter, result = 1;
	int32_t tem_result = 1;
	if ( (w < 0) || (h < 0) || ( w == 0 && h == 0 ) ) {				// if w or h is negative, or w == h = 0,print nothing and return 0
		return 0;
	} else {
		if ( (w == 0) && (h >0) ) {									// if w == 0 and h > 0, print a line but return 0
			x1 = x2 = x;
			y1 = y;
			y2 = y + h;
			draw_line(x1, y1, x2, y2);
			return 0;
		} else if ( (w > 0) && (h ==0) ) {							// if w > 0 and h == 0, print a line but return 0
			x1 = x;
			x2 = x + w;
			y1 = y2 = y;
			draw_line(x1, y1, x2, y2);
			return 0;
		} else {													// if w > 0 and h > 0, let (x1,y1) and (x2,y2) denote two vertices
			for (counter = 4; counter > 0 ; counter--) {
				switch (counter){
					case 4: 
					x1 = x;
					y1 = y;
					x2 = x + w;
					y2 = y;
					break;
					case 3:
					x1 = x;
					y1 = y;
					x2 = x;
					y2 = y + h;
					break;
					case 2:
					x1 = x + w;
					y1 = y;
					x2 = x + w;
					y2 = y + h;
					break;
					case 1:
					x1 = x;
					y1 = y + h;
					x2 = x + w;
					y2 = y + h;
					break;
				}
				tem_result = draw_line(x1, y1, x2, y2);
				result = result && tem_result;						// print sides of the rectangle
			}														// use result to check if all called functions works successfully
			return result;
		}
	}
}


/* 
 *  draw_triangle
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the triangle
 *         x_B,y_B -- the coordinates of another of the vertices of the triangle
 *         x_C,y_C -- the coordinates of the final of the vertices of the triangle
 * OUTPUTS: draws a pixel to every point of the edges of the triangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_triangle(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C) {
	int32_t x1, y1, x2, y2, counter, result = 1;
	int32_t tem_result;
	for (counter = 3; counter > 0; counter--) {						// let (x1,y1) and (x2,y2) denote two vertices
		switch(counter) {
			case 3:
			x1 = x_A;
			y1 = y_A;
			x2 = x_B;
			y2 = y_B;
			break;
			case 2:
			x1 = x_A;
			y1 = y_A;
			x2 = x_C;
			y2 = y_C;
			break;
			case 1:
			x1 = x_B;
			y1 = y_B;
			x2 = x_C;
			y2 = y_C;
			break;
		}
		tem_result = draw_line(x1, y1, x2, y2);
		result = result && tem_result;								// print sides of the triangle
	}																// use result to check if all called functions works successfully
	return result;
}

/* 
 *  draw_parallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a pixel to every point of the edges of the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	int32_t x1, y1, x2, y2, counter, result = 1, x_D, y_D;
	int32_t tem_result;
	x_D = x_C - x_B + x_A;
	y_D = y_C - y_B + y_A;
	for (counter = 4; counter > 0; counter--) {						// let (x1,y1) and (x2,y2) denote two vertices
		switch(counter) {
			case 4:
			x1 = x_A;
			y1 = y_A;
			x2 = x_B;
			y2 = y_B;
			break;
			case 3:
			x1 = x_B;
			y1 = y_B;
			x2 = x_C;
			y2 = y_C;
			break;
			case 2:
			x1 = x_D;
			y1 = y_D;
			x2 = x_C;
			y2 = y_C;
			break;
			case 1:
			x1 = x_A;
			y1 = y_A;
			x2 = x_D;
			y2 = y_D;
			break;
		}
		tem_result = draw_line(x1, y1, x2, y2);
		result = result && tem_result;								// print sides of the parallelogram
	}																// use result to check if all called functions works successfully
	return result;
}


/* 
 *  draw_circle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_circle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
	int32_t x1, y1, distance, result = 1;
	int32_t tem_result;
	if ((inner_r < 0) || (outer_r < inner_r)){													// if inner_r < 0 or outer_r < inner_r, print nothing and return 0
		return 0;
	}
	for (x1 = x - outer_r; x1 <= x + outer_r; x1++) {											// let (x1,y1) denote a point in a square around the circle
		for (y1 = y - outer_r; y1 <= y + outer_r; y1++) {
			distance = (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y);								// if between inner_r and outer_r, draw it
			if ( (distance <= (outer_r * outer_r)) && (distance >= (inner_r * inner_r)) ){
				tem_result = draw_dot(x1,y1);	
				result = result && tem_result;													// use result to check if all called functions works successfully
			}
		}
	}
	return result;
}


/* 
 *  rect_gradient
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 *         start_color -- the color of the far left side of the rectangle
 *         end_color -- the color of the far right side of the rectangle
 * OUTPUTS: fills every pixel within the bounds of the rectangle with a color
 *	    based on its position within the rectangle and the difference in
 *          color between start_color and end_color
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
rect_gradient(int32_t x, int32_t y, int32_t w, int32_t h, int32_t start_color, int32_t end_color){
	int32_t x1, y1, sign, counter, level, level_sep, level1, level2, level1_R, level1_G, level1_B, level2_R, level2_G, level2_B, result = 1;
	int32_t tem_result;
	int32_t byte_R = 0x00FF0000;									// use these data to & level1 and level2 to gain their separated color level 
	int32_t byte_G = 0x0000FF00;
	int32_t byte_B = 0x000000FF;
	level1_R = (start_color & byte_R) >> 16;						// gain start_color and end_color separated color level 
	level1_G = (start_color & byte_G) >> 8;
	level1_B = start_color & byte_B;
	level2_R = (end_color & byte_R) >> 16;
	level2_G = (end_color & byte_G) >> 8;
	level2_B = end_color & byte_B;
	if ( (w < 1) || (h < 0) ) {										// if w < 1 or h is < 0, print nothing and return 0
		return 0;
	} else {
		for(x1 = x; x1 <= x + w; x1++){
			if (x1 == x){											// calculate a vertical line's color level
				level = start_color;
			} else if(x1 == x + w){
				level = end_color;
			} else {
				level = 0;											//reset level
				for (counter = 3; counter > 0; counter--){
					switch(counter){								// let level1 and level2 denote start_color and end_color's separated color
						case 3:
						level1 = level1_R;
						level2 = level2_R;
						break;
						case 2:
						level1 = level1_G;
						level2 = level2_G;
						break;
						case 1:
						level1 = level1_B;
						level2 = level2_B;
						break;
					}
					if (level2 - level1 > 0){						// sign = the sign of (level2 - level1)
						sign = 1;
					} else if (level2 - level1 < 0){
						sign = -1;
					} else {
						sign = 0;
					}
					level_sep = ((2*(x1 - x)*(level2 - level1) + w*sign) / (2*w) + level1);			// calculate the separated color level and OR it to level
					switch(counter){
						case 3:
						level_sep = level_sep << 16;
						break;
						case 2:
						level_sep = level_sep << 8;
						break;
						case 1:
						break;
					}
					level = level | level_sep;
				}
			}
			set_color(level);
			for(y1 = y; y1 <= y + h; y1++){
				tem_result = draw_dot(x1,y1);
				result = result && tem_result;						// draw the point
			}														// use result to check if all called functions works successfully
		}
	}
	return result;
}


/* 
 *  draw_picture
 *	 
 *	 
 *	
 *	
 * INPUTS: none
 * OUTPUTS: alters the image by calling any of the other functions in the file
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */


int32_t
draw_picture(){
	int32_t result =1;
	int32_t sky = 0x001C2F40;										// difine some color
	int32_t thin_cloud = 0x00273844;
	int32_t thick_cloud = 0x00404D56;
	int32_t building = 0x000A1921;
	int32_t light_bar = 0x00808A98;
	int32_t light = 0x00AAA190;
	int32_t symbol = 0x00FAFAFA;

	set_color(sky);
	result = result && near_horizontal(0,0,1,0);
	result = result && near_vertical(0,0,0,1);
	result = result && draw_line(0,0,1,1);
	result = result && draw_rect(0,0,2,2);
	result = result && draw_triangle(0,0,0,1,1,0);
	result = result && draw_parallelogram(2,3,2,5,3,10);
	result = result && draw_circle(1,1,0,1);
	result = result && rect_gradient(0,0,2,2,sky,sky);
	if (result == 0){												// check if there is any error
		printf("function is wrong");
	}
	
	//draw the background
	result = 1;
	result = result && rect_gradient(0, 0, 623, 319, sky, sky);		// use sky as background
	set_color(thick_cloud);											// draw some thick clouds
	result = result && draw_circle(30,140,0,30);
	result = result && draw_circle(431,168,0,24);
	result = result && draw_circle(514,154,0,14);
	result = result && rect_gradient(0,140,87,70, thick_cloud, sky);
	result = result && rect_gradient(0,153,103,52, thick_cloud, sky);
	result = result && rect_gradient(402,159,221,30, thick_cloud, sky);
	result = result && rect_gradient(181,150,221,32, thin_cloud, sky);
	result = result && rect_gradient(2,44,117,36, thin_cloud, sky);	// draw some thin clouds
	result = result && rect_gradient(237,1,75,133, sky, thin_cloud);
	result = result && rect_gradient(312,1,46,133, thin_cloud, sky);
	result = result && rect_gradient(447,72,176,49, sky, thin_cloud);
	result = result && rect_gradient(492,0,131,72, sky, thin_cloud);
	if (result == 0){												// check if there is any error
		printf("background is wrong");
	}

	// draw the building
	result = 1;
	result = result && rect_gradient(0,252,623,67, building, building);			// draw all the buildings
	result = result && rect_gradient(0,120,9,1, building, building);
	result = result && rect_gradient(0,121,18,1, building, building);
	result = result && rect_gradient(0,122,18,15, building, building);
	result = result && rect_gradient(0,137,16,15, building, building);
	result = result && rect_gradient(0,152,14,15, building, building);
	result = result && rect_gradient(0,167,12,15, building, building);
	result = result && rect_gradient(0,182,14,35, building, building);
	result = result && rect_gradient(0,217,10,35, building, building);
	result = result && rect_gradient(30,235,15,20, building, building);			
	result = result && rect_gradient(61,220,20,55, building, building);
	result = result && rect_gradient(90,197,21,100, building, building);
	result = result && rect_gradient(124,219,23,70, building, building);
	result = result && rect_gradient(159,227,20,70, building, building);
	result = result && rect_gradient(185,219,21,70, building, building);
	result = result && rect_gradient(213,215,34,70, building, building);
	result = result && rect_gradient(250,237,13,70, building, building);
	result = result && rect_gradient(271,233,39,70, building, building);
	result = result && rect_gradient(333,225,58,70, building, building);
	result = result && rect_gradient(343,218,17,70, building, building);
	result = result && rect_gradient(390,205,22,70, building, building);
	result = result && rect_gradient(411,172,10,100, building, building);
	result = result && rect_gradient(421,170,10,100, building, building);
	result = result && rect_gradient(429,145,12,150, building, building);
	result = result && rect_gradient(441,140,12,150, building, building);
	result = result && rect_gradient(453,141,10,150, building, building);
	result = result && rect_gradient(463,145,10,150, building, building);
	result = result && rect_gradient(480,154,12,130, building, building);
	result = result && rect_gradient(492,149,12,130, building, building);
	result = result && rect_gradient(504,151,10,130, building, building);
	result = result && rect_gradient(522,198,16,100, building, building);
	result = result && rect_gradient(538,196,16,100, building, building);
	result = result && rect_gradient(597,161,26,130, building, building);
	if (result == 0){												// check if there is any error
		printf("building is wrong");
	}

	//add light
	result = 1;
	set_color(light_bar);											// draw the light bar in the specific building
	result = result && draw_line(336,233,351,219);
	result = result && draw_line(336,233,348,244);
	result = result && draw_line(365,234,356,225);
	result = result && draw_line(365,234,354,244);
	set_color(light);												// draw lights on other buildings
	result = result && draw_line(1,144,9,145);
	result = result && draw_line(7,202,12,202);
	result = result && rect_gradient(0,250,7,4, light, light);
	result = result && draw_line(32,241,42,241);
	result = result && draw_line(94,201,110,201);
	result = result && draw_line(170,234,175,234);
	result = result && draw_line(417,178,428,176);
	result = result && draw_line(432,147,451,142);
	result = result && draw_line(461,199,472,200);
	result = result && draw_line(453,218,436,218);
	result = result && draw_line(484,185,504,183);
	result = result && draw_line(534,228,550,227);
	result = result && draw_line(600,161,611,160);
	if (result == 0){												// check if there is any error
		printf("light is wrong");
	}

	// draw symbol
	result = 1;
	set_color(symbol);												// draw the outer ring
	result = result && draw_circle(312,160,137,150);
	set_color(sky);
	result = result && draw_line(384,45,393,45);
	result = result && draw_line(393,38,393,44);
	result = result && draw_line(384,46,394,46);
	result = result && draw_line(394,39,394,46);
	result = result && draw_line(384,47,395,47);
	result = result && draw_line(395,40,395,47);
	result = result && draw_line(384,48,396,48);
	result = result && draw_line(396,41,396,48);
	result = result && draw_line(384,49,397,49);
	result = result && draw_line(397,42,397,49);
	result = result && draw_line(384,50,398,50);
	result = result && draw_line(398,43,398,50);
	result = result && draw_line(398,43,398,30);
	result = result && draw_line(397,42,397,30);
	result = result && draw_line(396,41,396,30);
	result = result && draw_line(395,40,395,30);
	result = result && draw_line(394,39,394,30);
	result = result && draw_line(393,38,393,30);
	set_color(symbol);													// draw the inner symbol
	result = result && draw_line(229,84,244,219);
	result = result && draw_line(238,211,412,168);
	result = result && draw_line(229,86,318,262);
	result = result && draw_line(248,218,306,98);
	result = result && draw_line(301,100,377,208);
	result = result && draw_line(376,211,378,69);
	result = result && draw_line(313,262,379,68);
	if (result == 0){												// check if there is any error
		printf("symbol is wrong");
	}
	return result;
}
