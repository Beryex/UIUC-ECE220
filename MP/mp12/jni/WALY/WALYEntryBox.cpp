//									tab:8
//
// WALYEntryBox.cpp - source file for the WALY library entry box widget
//
// "Copyright (c) 2011-2012 by Steven S. Lumetta."
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
// Version:	    1.01
// Creation Date:   27 July 2011
// Filename:	    WALYEntryBox.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	28 December 2012
//		Fixed bug and added text focus concept.
//

#include "WALY.h"

#include <ctype.h>

using namespace std;

namespace WALY_1_0_0 {
    
void 
EntryBox::prepareToRedraw () 
{
    Widget::prepareToRedraw ();
    if (enabled && !highlighted && cursorOn && haveFocus ()) {
        Rect cursor;
	int h;
	int w;

	font->getSize (text.substr (0, cursorPos).c_str (), &w, &h);
	cursor.x = w + startOfText;
	cursor.y = yPadding;
	cursor.w = 1;
	cursor.h = font->lineSkip ();
	SDL_FillRect (surface, &cursor, fgColor);
    }
}

void
EntryBox::setText (const char* newText)
{
    Widget::setText (newText);
    cursorPos = text.length ();
}

void
EntryBox::mouseDown (Frame* f, const Event* e)
{
    EntryBox* eb = (EntryBox*)f;
    if (!eb->enabled || 0 != e->button.which) { return; }
    if (eb->needRedraw) {
	// need to know the boundary and the cursor positioning
    	eb->recalculateExtent ();
    }
    if (!eb->withinBoundary (e->button.x, e->button.y)) { 
        eb->lastMouseDown = -1000; // can't double-click with outside hit
	return;
    }
    if (250 > SDL_GetTicks () - eb->lastMouseDown) {
        // double-click
	eb->highlighted = true;
	eb->makeVisibleChange ();
    } else {
	int32_t locX, trash, left, right, mid, w;

        // place cursor...
	locX = e->button.x + eb->lastDrawOffset - eb->xPadding;
	trash = 0;
	eb->mapToLocal (&locX, &trash);

	left = 0;
	right = eb->text.length ();
	while (right > left) {
	    mid = (left + right) / 2;
	    eb->font->getSize (eb->text.substr (0, mid).c_str (), &w, &trash);
	    if (w >= locX) {
	        right = mid;
	    } else if (w < locX) {
	        left = mid + 1;
	    }
	}
        eb->cursorPos = left;
	eb->cursorOn = true;
	eb->cursorTickCnt = eb->cursorTicks;
	// will call makeVisibleChange and show keyboard...
	eb->claimFocus ();  
	// eb->makeVisibleChange ();
	// Platform::showKeyboard ();
	eb->highlighted = false;
    }
    eb->lastMouseDown = SDL_GetTicks ();
}

void
EntryBox::blink (Frame* f, const Event* e)
{
    EntryBox* eb = (EntryBox*)f;

    if (!eb->enabled || 0 >= --eb->cursorTickCnt) {
        eb->cursorTickCnt = eb->cursorTicks;
	eb->cursorOn = !eb->cursorOn;
	eb->makeVisibleChange ();
    }
}

void
EntryBox::keystroke (Frame* f, const Event* e)
{
    EntryBox* eb = (EntryBox*)f;

    if (!eb->enabled || !eb->haveFocus ()) { return; }
    if (SDLK_BACKSPACE == e->key.keysym.sym) {
	if (eb->highlighted) {
	    eb->text.clear ();
	    eb->cursorPos = 0;
	    eb->makeVisibleChange ();
	    eb->highlighted = false;
	} else if (0 < eb->cursorPos) {
	    eb->text.erase (eb->cursorPos - 1, 1);
	    eb->cursorPos--;
	    eb->makeVisibleChange ();
	}
    } else if (SDLK_RETURN == e->key.keysym.sym) {
	if (NULL != eb->action) {
	    eb->action (eb);
	}
    } else if ((-1 == eb->maxChars || eb->highlighted ||
    		eb->maxChars > (int32_t)eb->text.length ()) &&
	       isprint (e->key.keysym.sym)) {
	char key = e->key.keysym.sym;

	if (eb->highlighted) {
	    eb->text.clear ();
	    eb->cursorPos = 0;
	    eb->highlighted = false;
	}
	if (0 != (KMOD_SHIFT & e->key.keysym.mod)) {
	    key = toupper (key);
	}
	eb->text.insert (eb->cursorPos, 1, key);
	eb->cursorPos++;
	eb->makeVisibleChange ();
    }
}

void 
EntryBox::setMaxChars (int32_t newVal)
{
    maxChars = (0 < newVal ? newVal : -1);
    if (0 < maxChars && maxChars < (int32_t)text.length ()) {
	text = text.substr (0, maxChars);
        makeVisibleChange ();
    }
}

void 
EntryBox::construct ()
{
    maxChars = -1;
    cursorPos = text.length ();
    cursorOn = false;
    cursorTicks = 12;
    cursorTickCnt = cursorTicks;
    action = NULL;
    lastMouseDown = 0;
    setCallbackFunc (FRAME_UPDATE, blink);
    setCallbackFunc (MOUSE_DOWN, mouseDown);
    setCallbackFunc (KEY_DOWN, keystroke);
}

EntryBox::EntryBox (Frame* par, int32_t xCoord, int32_t yCoord,
		    int32_t pixWidth, const char* txt, uint32_t fg, 
		    uint32_t bg, Font* fnt) :
		    Widget (par, xCoord, yCoord, pixWidth, 5, 4, txt,
		    	    TEXT_ENTRY, FILTER_SUNKEN, 5, fg, bg, fnt)
{
    construct ();
}

EntryBox::EntryBox (Frame* par, int32_t xCoord, int32_t yCoord,
		    int32_t pixWidth, const char* txt, uint32_t fg, 
		    uint32_t bg, uint32_t highCol, Font* fnt) :
		    Widget (par, xCoord, yCoord, pixWidth, 5, 4, txt,
		    	    TEXT_ENTRY, FILTER_SUNKEN, 5, fg, bg, highCol, fnt)
{
    construct ();
}

void
EntryBox::setFocus (bool isFocus)
{
    if (isFocus) {
        claimFocus ();
    } else {
        giveUpFocus ();
    }
}

}
