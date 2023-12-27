//									tab:8
//
// mp12.h - header file for Spring 2018 MP12 student code
//
// "Copyright (c) 2018 by Steven S. Lumetta."
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without written agreement is
// hereby granted, provided that the above copyright notice and the following
// two paragraphs appear in all copies of this software.
//
// IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO
// ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
// PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
// THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE,
// SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
//
// Author:	    Steve Lumetta
// Version:	    1.00
// Creation Date:   20 April 2018
// Filename:	    mp12.h
// History:
//	SL	1.00	20 April 2018
//		First written.
//

#if !defined(MP12_H)
#define MP12_H

#include "WALYmain.h"

#include "TurtleScreen.h"

#include "mp5.h"
#include "mp12.h"

//-------------------------------------------------------------------------
// THESE STRUCTURES AND THE ENUMERATION ARE HERE TO SIMPLIFY GRADING
// THEY SHOULD ONLY BE USED IN mp12.cpp

typedef struct segment_t segment_t;
struct segment_t
{
    int32_t color; // color of line segment
    int32_t x1;    // x position 1
    int32_t y1;    // y position 1
    int32_t x2;    // x position 2
    int32_t y2;    // y position 2
};

typedef enum
{
    CMD_NONE = -1,
    CMD_TURN, // turn right or left once per frame
    CMD_MOVE, // move forward
    CMD_WAIT, // wait
} command_type_t;

typedef struct command_t command_t;
struct command_t
{
    command_type_t type; // type of command
    int32_t cmdFrames;   // frames needed for command
    int32_t framesLeft;  // frames remaining until done
    int32_t dir;         // for turns: 1 is left, -1 is right
    int32_t startX;      // for moves
    int32_t startY;
    int32_t endX;
    int32_t endY;
};

//-------------------------------------------------------------------------

// Open the input stream for reading.  Returns 1 on success, 0 on failure.
extern int32_t openInputStream(const char *fname);

// Close the input stream.
extern void closeInputStream(void);

// Draw all lines in the list of lines using set_color and draw_line from
// MP5.
extern void drawEverything(void);

// Read commands and execute them until a command that needs animation
// is read.  Return it in the cmd variable (in mp12.cpp).
void readNewCommand(TurtleScreen *ts);

// Called 25 times per second to redraw the scene.  Call readNewCommand
// if new command is needed (cmd.type is CMD_NONE).  Note that
// readNewCommand has no way to return a failure, so check cmd.type
// again and handle CMD_NONE appropriately.
extern void frameUpdate(TurtleScreen *ts);

#endif // MP12_H
