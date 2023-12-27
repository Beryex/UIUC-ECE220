//									tab:8
//
// mp12.cpp - source file for Spring 2018 MP12 student code
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
// Filename:	    mp12.cpp
// History:
//	SL	1.00	20 April 2018
//		First written.
//

#include <iostream>
#include <list>

#include <math.h>
#include <stdio.h>

#include "WALYmain.h"

#include "TurtleScreen.h"

#include "mp5.h"
#include "mp12.h"

using namespace std;
using namespace WALY;

// THESE ARE GLOBAL VARIABLES IN THE INTEREST OF TESTING.  THEY SHOULD
// BE FILE-SCOPE OR LOCAL TO frameUpdate.
FILE *input = NULL;           // input stream
list<segment_t> lines;        // list of lines drawn
command_t cmd = {CMD_NONE};   // current command
int32_t moveColor = 0xFFFFFF; // current movement drawing color

int32_t
openInputStream(const char *fname)
{
    input = fopen(fname, "r");                              // open the input stream for reading
    if (input == NULL){
        return 0;                                           // if open fails, return 0, otherwise, return 1
    } else {
        return 1;
    }
}

void closeInputStream(void)
{
    fclose(input);                                          // close the input stream
}

void showLines(void)
{
    // you can use 'call showLines ()' in GDB to look at the
    // list of segments.  Don't forget to call makeVisibleChange
    // when you change this list, since the background won't be
    // redrawn unless you call that function (so changes will not
    // show up.
    for (list<segment_t>::iterator it = lines.begin();
         lines.end() != it; it++)
    {
        Screen::debug() << it->color << " " << it->x1 << "," << it->y1 << "  " << it->x2 << "," << it->y2 << endl;
    }
}

void drawEverything(void)
{
    for (list<segment_t>::iterator it = lines.begin(); lines.end() != it; it++){
        set_color(it->color);                               // set color and draw line for each segment
        draw_line(it->x1, it->y1, it->x2, it->y2);
    }
}

void readNewCommand(TurtleScreen *ts)
{
    char str[201];
    char arg[10];
    char cmd1[10] = "color";
    char cmd2[10] = "move";
    char cmd3[10] = "restart";
    char cmd4[10] = "turn";
    char cmd5[10] = "wait";
    char garbage[200];
    if(fgets(str, 200, input) != NULL){
        sscanf(str, "%s", arg);
        if(strcasecmp(arg, cmd1) == 0){
            int32_t rgb;
            if(2 == sscanf(str, "%s %x %1s", arg, &rgb, garbage)){
                moveColor = rgb;
            } else {
                fprintf(stderr, "%s", str);
            }
            return;
        }
        if(strcasecmp(arg, cmd2) == 0){
            int32_t dist, frames;
            if(3 == sscanf(str, "%s %d %d %1s", arg, &dist, &frames, garbage)){
                if(dist < 0 || frames < 0){
                    return;
                }
                segment_t line;
                cmd.type = CMD_MOVE;
                cmd.cmdFrames = frames;
                cmd.framesLeft = frames;
                int32_t x = ts->getX();
                int32_t y = ts->getY();
                cmd.startX = x;
                cmd.startY = y;
                cmd.endX = x + round(cos((ts->getDirection() + 9) * acos(-1) / 18) * dist);
                cmd.endY = y - round(sin((ts->getDirection() + 9) * acos(-1) / 18) * dist);
                line.color = moveColor;
                line.x1 = x;
                line.x2 = cmd.endX;
                line.y1 = y;
                line.y2 = cmd.endY;
                lines.push_back(line);
            } else {
                fprintf(stderr, "%s", str);
            }
            return;
        }
        if(strcasecmp(arg, cmd3) == 0){
            if(1 == sscanf(str, "%s %1s", arg, garbage)){
                rewind(input);
            } else {
                fprintf(stderr, "%s", str);
            }
            return;
        }
        if(strcasecmp(arg, cmd4) == 0){
            int32_t amt;
            if(2 == sscanf(str, "%s %d %1s", arg, &amt, garbage)){
                if (amt >0){
                    cmd.type = CMD_TURN;
                    cmd.cmdFrames = amt;
                    cmd.framesLeft = amt;
                    cmd.dir = 1;
                } else if (amt<0){
                    cmd.type = CMD_TURN;
                    cmd.cmdFrames = -amt;
                    cmd.framesLeft = -amt;
                    cmd.dir = -1;
                } else {
                    cmd.type = CMD_NONE;
                }
            } else {
                fprintf(stderr, "%s", str);
            }
            return;
        }
        if(strcasecmp(arg, cmd5) == 0){
            int32_t frames;
            if(2 == sscanf(str, "%s %d %1s", arg, &frames, garbage)){
                if(frames <= 0){
                    return;
                }
                cmd.type = CMD_WAIT;
                cmd.cmdFrames = frames;
                cmd.framesLeft = frames;
            } else {
                fprintf(stderr, "%s", str);
            }
            return;
        }
    }
}

void frameUpdate(TurtleScreen *ts)
{
    int32_t x;
    int32_t y;
    int32_t offset_x;
    int32_t offset_y;
    if (cmd.framesLeft == 0){
            cmd.type = CMD_NONE;                            // when one command is done totally, reset cmd
    }
    if (cmd.type == CMD_NONE){
        readNewCommand(ts);                                 // if cmd still none, read a new command
    } else {
        switch(cmd.type){
            case CMD_TURN:                                  // turns direction
            if (cmd.dir > 0){
                ts->turnLeft();
            }
            if (cmd.dir < 0){
                ts->turnRight();
            }
            cmd.framesLeft--;
            ts->makeVisibleChange();                        // print the change
            break;
            case CMD_MOVE:                                  // calculate the move distance in a single frame and set it 
            cmd.framesLeft--;
            x = cmd.startX;
            y = cmd.startY;
            offset_x = int32_t(round((double)(cmd.endX - cmd.startX) * (double)(cmd.cmdFrames - cmd.framesLeft) / (double)(cmd.cmdFrames)));
            offset_y = int32_t(round((double)(cmd.endY - cmd.startY) * (double)(cmd.cmdFrames - cmd.framesLeft) / (double)(cmd.cmdFrames)));
            ts->setX(x + offset_x);
            ts->setY(y + offset_y);
            ts->makeVisibleChange();                        // print the change
            break;
            default:
            cmd.framesLeft--;
            ts->makeVisibleChange();                        // print the change
            break;
        }
    }
}
