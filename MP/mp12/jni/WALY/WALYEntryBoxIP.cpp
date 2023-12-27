//									tab:8
//
// WALYEntryBoxIP.cpp - source file for the WALY library IP address
//                      entry box widget
//
// "Copyright (c) 2011-2013 by Steven S. Lumetta."
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
// Version:	    1.02
// Creation Date:   27 July 2011
// Filename:	    WALYEntryBoxIP.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	28 December 2012
//		Fixed bug, added text focus concept, and hid WebOS-specific
//                behavior from other platforms.
//	SL	1.02	6 January 2013
//		Enabled use of TAB and keypad digits.
//

#include "WALY.h"

#if !defined(WALY_TARGET_ANDROID)
#include "SDL/SDL_net.h"
#else // defined(WALY_TARGET_ANDROID)
#include <linux/in.h>
#endif // defined(WALY_TARGET_*)

using namespace std;

namespace WALY_1_0_0 {
    
void 
EntryBoxIP::prepareToRedraw () 
{
    if (NULL != surface) { return; }

    uint32_t height = font->lineSkip ();
    surface = SDL_CreateRGBSurface 
		(0, width + xPadding * 2, height + yPadding * 2, 32, 
		 0xFF0000, 0xFF00, 0xFF, 0xFF000000);
    if (NULL == surface) {
    	useImage = false;
	return;
    }
    useImage = true;

    SDL_FillRect (surface, NULL, bgColor);

    for (int32_t i = 0; 4 > i; i++) {
	Surface* writing;
	Rect src;
	Rect dst;

	if (0 < i) {
	    dst.x = xPadding + partWidth * i + dotWidth * (i - 1);
	    dst.y = yPadding;
	    SDL_BlitSurface (dotImg, NULL, surface, &dst);
	}

	if (0 < part[i].length ()) {
	    writing = font->renderText (part[i].c_str (), fgColor);
	    if (NULL == writing) {
		lastDraw[i] = 0;
		continue;
	    }
	    src.x = 0;
	    src.y = 0;
	    src.w = ((int32_t)partWidth >= writing->w ? writing->w : partWidth);
	    src.h = height;
	    dst.x = xPadding + (partWidth + dotWidth) * i + partWidth - src.w;
	    dst.y = yPadding;
	    lastDraw[i] = dst.x;
	    if (highlighted && haveFocus ()) {
		if (enabled && i == whichHigh) {
		    dst.w = src.w;
		    dst.h = height;
		    SDL_FillRect (surface, &dst, highColor);
		}
	    }
	    SDL_BlitSurface (writing, &src, surface, &dst);
	    SDL_FreeSurface (writing);
	} else {
	    dst.x = xPadding + (partWidth + dotWidth) * i + partWidth;
	}
	if (enabled && !highlighted && haveFocus () && cursorOn && 
	    i == whichCurs) {
	    Rect cursor;
	    int h;
	    int w;

	    font->getSize (part[i].substr (0, cursorPos).c_str (), &w, &h);
	    cursor.x = w + dst.x;
	    cursor.y = yPadding;
	    cursor.w = 1;
	    cursor.h = height;
	    SDL_FillRect (surface, &cursor, fgColor);
	}
    }

    if (!enabled) {
        Filter::greyOut (surface);
    } else {
	Filter::drawDarkBorder (surface, border);
    }
}

void
EntryBoxIP::setText (const char* newText)
{
    string s (newText);
    size_t pos = 0;
    
    whichCurs = 3;
    Widget::setText (""); // replace extra code from EntryBox version
    for (int32_t i = 0; 4 > i; i++) {
	size_t next = s.find_first_of ('.', pos);
	if (s.npos == next) { 
	    part[i] = s.substr (pos);
	    whichCurs = i;
	    while (4 > ++i) { part[i] = ""; }
	    break;
	}
	part[i] = s.substr (pos, next - pos);
	pos = next + 1;
    }
    cursorPos = part[whichCurs].length ();
}

void
EntryBoxIP::setAddr (uint32_t ipAddr)
{
    char buf[32];

    sprintf (buf, "%d.%d.%d.%d", (ipAddr >> 24), ((ipAddr >> 16) & 0xFF),
	     ((ipAddr >> 8) & 0xFF), (ipAddr & 0xFF));
    setText (buf);
}

uint32_t
EntryBoxIP::getAddr ()
{
    uint32_t p1, p2, p3, p4;

    // ignore trailing garbage, e.g., "1a"
    p1 = p2 = p3 = p4 = 0;
    if (1 != sscanf (part[0].c_str (), "%u", &p1) ||
        1 != sscanf (part[1].c_str (), "%u", &p2) ||
	1 != sscanf (part[2].c_str (), "%u", &p3) ||
	1 != sscanf (part[3].c_str (), "%u", &p4)) {
	return INADDR_ANY;
    }
    return (((p1 & 0xFF) << 24) | ((p2 & 0xFF) << 16) | ((p3 & 0xFF) << 8) | 
    	    (p4 & 0xFF));
}

void
EntryBoxIP::mouseDown (Frame* f, const Event* e)
{
    EntryBoxIP* eb = (EntryBoxIP*)f;
    if (!eb->enabled || 0 != e->button.which) { return; }
    if (eb->needRedraw) {
	// need to know the boundary and the cursor positioning
    	eb->recalculateExtent ();
    }
    if (!eb->withinBoundary (e->button.x, e->button.y)) { 
        eb->lastMouseDown = -1000; // can't double-click with outside hit
	return; 
    }

    int32_t locX, trash, which;
    locX = e->button.x - eb->xPadding + eb->dotWidth / 2;
    trash = 0;
    eb->mapToLocal (&locX, &trash);
    which = (locX + eb->partWidth + eb->dotWidth) / 
	    (eb->partWidth + eb->dotWidth) - 1;
    if (0 > which) { which = 0; }
    else if (3 < which) { which = 3; }

    if (250 > SDL_GetTicks () - eb->lastMouseDown) {
        // double-click
	eb->highlighted = true;
	eb->whichHigh = which;
	eb->makeVisibleChange ();
    } else {
	int32_t left, right, mid, w;

        // place cursor...
	locX -= eb->lastDraw[which] + eb->dotWidth / 2;
	left = 0;
	right = eb->part[which].length ();
	while (right > left) {
	    mid = (left + right) / 2;
	    eb->font->getSize (eb->part[which].substr (0, mid).c_str (),
			       &w, &trash);
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
	eb->whichCurs = which;
    }
    eb->lastMouseDown = SDL_GetTicks ();
}

void
EntryBoxIP::jumpToNextPart ()
{
    if (highlighted) {
	whichCurs = whichHigh = (whichHigh + 1) % 4;
    } else {
	whichCurs = whichHigh = (whichCurs + 1) % 4;
    }
    cursorPos = part[whichCurs].length ();
    highlighted = (0 < cursorPos);
    makeVisibleChange ();
}

void
EntryBoxIP::keystroke (Frame* f, const Event* e)
{
    EntryBoxIP* eb = (EntryBoxIP*)f;

    if (!eb->enabled || !eb->haveFocus ()) { return; }
    if (SDLK_BACKSPACE == e->key.keysym.sym) {
	if (eb->highlighted) {
	    eb->part[eb->whichHigh].clear ();
	    eb->cursorPos = 0;
	    eb->whichCurs = eb->whichHigh;
	    eb->makeVisibleChange ();
	    eb->highlighted = false;
	} else if (0 < eb->cursorPos) {
	    eb->part[eb->whichCurs].erase (eb->cursorPos - 1, 1);
	    eb->cursorPos--;
	    eb->makeVisibleChange ();
	} else if (0 < eb->whichCurs) {
	    int32_t fld  = --eb->whichCurs;
	    int32_t posn = eb->cursorPos = eb->part[fld].length ();
	    if (0 < posn) {
		eb->part[fld].erase (posn - 1, 1);
		eb->cursorPos--;
	    }
	    eb->makeVisibleChange ();
	}
    } else if (SDLK_RETURN == e->key.keysym.sym) {
	if (NULL != eb->action) {
	    eb->action (eb);
	}
    } else if ((-1 == eb->maxChars || eb->highlighted ||
    		3 > (int32_t)eb->part[eb->whichCurs].length ())) {
	int32_t key = e->key.keysym.sym;

#if defined(WALY_TARGET_WEBOS) // painful Palm keyboard translation...
	// Palm Pre numeric keypad conversion
	static const char convertFrom[10] = {
	    '@', 'e', 'r', 't', 'd', 'f', 'g', 'x', 'c', 'v'
	};
	for (int32_t i = 0; 10 > i; i++) {
	    if (convertFrom[i] == key) {
		key = '0' + i;
	    }
	}
#endif // defined(WALY_TARGET_WEBOS)

	// translate keypad presses to digits
	if (SDLK_KP0 <= key && SDLK_KP9 >= key) {
	    key = key + '0' - SDLK_KP0;
	}
	if ('0' <= key && '9' >= key) {
	    if (eb->highlighted) {
		eb->part[eb->whichHigh].clear ();
		eb->cursorPos = 0;
		eb->whichCurs = eb->whichHigh;
		eb->highlighted = false;
	    }
	    eb->part[eb->whichCurs].insert (eb->cursorPos, 1, key);
	    if (3 > eb->whichCurs && 3 == eb->part[eb->whichCurs].length ()) {
	        eb->jumpToNextPart ();
	    } else {
		eb->cursorPos++;
		eb->makeVisibleChange ();
	    }
	} else if (' ' == key || '.' == key || SDLK_TAB == key) {
	    eb->jumpToNextPart ();
	}
    }
}

void EntryBoxIP::construct (const char* txt)
{
    int32_t w;
    int32_t trash;

    font->getSize ("888", &w, &trash); // "MMM" is unrealistic...
    partWidth = w;
    font->getSize (".", &w, &trash);
    dotWidth = w;
    width = 4 * partWidth + 3 * dotWidth;
    for (int32_t i = 0; 4 > i; i++) { lastDraw[i] = 0; }

    dotImg = font->renderText (".", fgColor);
    if (NULL == dotImg) {
        throw exception ();
    }

    // virtual function won't be called properly during base construction
    setText (txt);

    // override these functions from EntryBox
    setCallbackFunc (MOUSE_DOWN, mouseDown);
    setCallbackFunc (KEY_DOWN, keystroke);
}

EntryBoxIP::EntryBoxIP (Frame* par, int32_t xCoord, int32_t yCoord,
		        const char* txt, uint32_t fg, uint32_t bg, Font* fnt) :
	EntryBox (par, xCoord, yCoord, 0, txt, fg, bg, fnt)
{
    construct (txt);
}

EntryBoxIP::EntryBoxIP (Frame* par, int32_t xCoord, int32_t yCoord,
			const char* txt, uint32_t fg, uint32_t bg,
			uint32_t highCol, Font* fnt) :
	EntryBox (par, xCoord, yCoord, 0, txt, fg, bg, highCol, fnt)
{
    construct (txt);
}

}
