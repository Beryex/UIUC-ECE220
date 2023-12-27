//									tab:8
//
// WALYButton.cpp - source file for the WALY library button widget
//
// "Copyright (c) 2011 by Steven S. Lumetta."
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
// Creation Date:   27 July 2011
// Filename:	    WALYButton.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//

#include "WALY.h"

namespace WALY_1_0_0 {
    
void
Button::setDepressed (bool newVal)
{
    depressed = newVal;
    filter = (depressed ? FILTER_LOWERED : FILTER_RAISED);
    makeVisibleChange ();
}

void
Button::mouseMove (Frame* f, const Event* e)
{
    if (0 != e->motion.which) { return; }
    Button* b = (Button*)f;
    if (b->depressed && b->extentValid && 
        !b->withinBoundary (e->motion.x, e->motion.y)) {
	b->setDepressed (false);
    }
}

void
Button::mouseDown (Frame* f, const Event* e)
{
    if (0 != e->button.which) { return; }
    Button* b = (Button*)f;
    if (b->enabled && b->withinBoundary (e->button.x, e->button.y)) {
	b->setDepressed (true);
    }
}

void
Button::mouseUp (Frame* f, const Event* e)
{
    Button* b = (Button*)f;
    if (0 != e->button.which) { return; }
    if (b->depressed) { 
	if (b->withinBoundary (e->button.x, e->button.y)) {
	    // button pressed: make callback
	    if (NULL != b->action) { b->action (b); }
	}
	b->setDepressed (false);
    }
}

Button::Button (Frame* par, int32_t xCoord, int32_t yCoord, const char* txt, 
	uint32_t fg, uint32_t bg, Font* fnt) : 
	Widget (par, xCoord, yCoord, 0, 10, 5, txt, TEXT_CENTER, FILTER_RAISED,
		8, fg, bg, fnt)
{
    setCallbackFunc (MOUSE_MOTION, mouseMove);
    setCallbackFunc (MOUSE_DOWN, mouseDown);
    setCallbackFunc (MOUSE_UP, mouseUp);

    depressed = false;
    action = NULL;
}

}
