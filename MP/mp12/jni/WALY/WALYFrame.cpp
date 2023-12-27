//									tab:8
//
// WALYFrame.cpp - source file for the WALY library frame class
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
// Version:	    1.03
// Creation Date:   27 July 2011
// Filename:	    WALYFrame.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	27 December 2012
//		Migrated/removed a couple of functions.
//	SL	1.02	3 January 2013
//		Removed some dead code.
//	SL	1.03	5 January 2013
//		Added requiresCallback and fixed a long-lived bug with
//                counting callbacks.
//

#include "WALY.h"

using namespace std;

namespace WALY_1_0_0 {
    
void 
Frame::attachImage (Image* img) 
{
    // cannot attach image to root...
    if (NULL == parent) { return; }

    useImage = true;
    surface = img->imageData;
    markExposure ();
    markForRedraw ();
}

void 
Frame::adjustCallbackCount (callback_id_t cid, int32_t amt) 
{
    for (Frame* ancestor = parent; NULL != ancestor; 
	 ancestor = ancestor->parent) {
	ancestor->childCallbackCnt[cid] += amt;
    }
}

void 
Frame::clearCallbackFunc (callback_id_t cid) 
{
    if (NULL != callback[cid]) {
	callback[cid] = NULL;
	adjustCallbackCount (cid, -1);
	if (NULL != screen) {
	    screen->withdrawCallback (this);
	}
    }
}

bool 
Frame::requiresCallback (callback_id_t cid) 
{
    // Used only to check whether a connection/network event
    // should be pulled from the queue--if there's no one to
    // listen to such a message, we leave it in the queue.
    return (active && (NULL != callback[cid] || 0 < childCallbackCnt[cid]));
}

void 
Frame::registerForCallback (callback_id_t cid) 
{
    if (!active) { return; }
    if (0 < childCallbackCnt[cid]) {
	for (std::list<Frame*>::iterator it = children.begin ();
	     children.end () != it; it++) {
	    (*it)->registerForCallback (cid);
	}
    }
    if (NULL != callback[cid]) {
	screen->addCallback (this);
    }
}

void 
Frame::removeFromParent (bool clearCallback)
{
    for (callback_id_t cid = FIRST_CALLBACK; NUM_CALLBACKS > cid; 
	 cid = (callback_id_t)(1 + (int32_t)cid)) { 
	adjustCallbackCount (cid, -childCallbackCnt[cid] +
			     (!clearCallback && NULL != callback[cid]));

	// clearing also removes from current activations...
	if (clearCallback) {
	    clearCallbackFunc (cid);
	}
    }
    if (NULL != parent) {
	parent->children.remove (this);
        parent = NULL;
    }
}

void 
Frame::setParent (Frame* newPar)
{
    markExposure ();
    removeFromParent (false);
    newPar->addChild (this);
}

void Frame::setCallbackFunc (callback_id_t cid, 
			     void (*newFunc) (Frame* f, const Event* e)) 
{
    clearCallbackFunc (cid);
    if (NULL != newFunc) {
	callback[cid] = newFunc;
	adjustCallbackCount (cid, 1);
    }
}

ostream& operator<< (ostream& s, const Rect& r) {
    return (s << "(" << r.x << "," << r.y << "," << r.w << "," << r.h << ")");
}

void Frame::recalculateExtent ()
{
    int32_t right;
    int32_t bottom;

    prepareToRedraw ();

    if (useImage) {
        extent.x = 0;
        extent.y = 0;
	right = surface->w;
	bottom = surface->h;
    } else {
	extent.x = 32767;
	extent.y = 32767;
	right = -32768;
	bottom = -32768;
    }
    for (list<Frame*>::iterator it = children.begin (); 
	 children.end () != it; it++) {
	Frame* child = *it;

	if (!child->visible) {
	    continue;
	}
	if (!child->extentValid) { child->recalculateExtent (); }
	if (0 == child->extent.w || 0 == child->extent.h) {
	    continue;
	}
	if (extent.x > child->x + child->extent.x) {
	    extent.x = child->x + child->extent.x;
	}
	if (right < child->x + child->extent.x + child->extent.w) {
	    right = child->x + child->extent.x + child->extent.w;
	}
	if (extent.y > child->y + child->extent.y) {
	    extent.y = child->y + child->extent.y;
	}
	if (bottom < child->y + child->extent.y + child->extent.h) {
	    bottom = child->y + child->extent.y + child->extent.h;
	}
    }
    if (extent.x >= right || extent.y >= bottom) {
	extent.x = extent.y = extent.w = extent.h = 0;
    } else {
	extent.w = right - extent.x;
	extent.h = bottom - extent.y;
    }

    // override children (which still needed to be recalculated) ...
    if (NULL == parent) {
	extent.x = extent.y = 0;
	extent.w = surface->w;
	extent.h = surface->h;
    } else if (0 != scrollRect.h && 0 != scrollRect.w) {
        extent = scrollRect;
    }

//    Screen::debug () << "extent for " << hex << this << " is " << dec << extent << endl;
    extentValid = true;

    // now reposition the frame based on its extent and alignment
    align_loc_t horiz = (align_loc_t)(align & (ALIGN_CENTER | ALIGN_RIGHT));
    switch (horiz) {
        case ALIGN_LEFT:   x = unalignedX; break;
	case ALIGN_CENTER: x = unalignedX - extent.w / 2; break;
	default:           x = unalignedX - extent.w; break;
    }
    align_loc_t vert = (align_loc_t)(align & (ALIGN_MIDDLE | ALIGN_BOTTOM));
    switch (vert) {
        case ALIGN_TOP:    y = unalignedY; break;
	case ALIGN_MIDDLE: y = unalignedY - extent.h / 2; break;
	default:           y = unalignedY - extent.h; break;
    }

    // now move the frame so that its scroll rectangle falls in the right spot
    x -= scrollRect.x;
    y -= scrollRect.y;
}

void
Frame::notOnScreen ()
{
    if (wasOnScreen) {
        wasOnScreen = false;
	for (list<Frame*>::iterator it = children.begin (); 
	     children.end () != it; it++) {
	    (*it)->notOnScreen ();
	}
    }
}

void 
Frame::render (Surface* on, int32_t atX, int32_t atY)
{
    Rect origClip;
    Rect newClip;
    Rect exposeClip;
    Rect drawRegion;

//    Screen::debug () << "starting render of " << hex << this << " on "
//                     << on << " at (" << atX << "," << atY << ")" << endl;
    if (!visible) {
//	Screen::debug () << "--- not visible" << endl;
	notOnScreen ();
        return; 
    }
    if (!extentValid) { recalculateExtent (); }
//    Screen::debug () << "--- extent is " << extent << endl;

    if (NULL == on) {
    	on = surface;
	if (NULL == on) { 
//	    Screen::debug () << "--- no surface available" << endl;
	    return;
	}
	Rect r;
	r.x = r.y = 0;
	r.w = on->w;
	r.h = on->h;
	SDL_SetClipRect (on, &r);
//	Screen::debug () << "--- initialized clip rect to " << r << endl;
    }

    atX += x;
    atY += y;
//    Screen::debug () << "--- ready to render " << hex << this << " on "
//                     << on << " at (" << atX << "," << atY << ")" << endl;

    SDL_GetClipRect (on, &origClip);
    newClip = extent;
    newClip.x += atX;
    newClip.y += atY;
    drawRegion = newClip;
    if (!intersect (&origClip, &newClip)) {
//	Screen::debug () << "--- clip rect is empty--nothing to draw" << endl;
	notOnScreen ();
	return;
    }

    if (needRedraw) {
	// redraw fully
	exposeClip = newClip;
	wasOnScreen = true;
	screenRect = drawRegion;
    } else if (NULL != screen) {
	// redraw any exposed or redrawn section
	exposeClip = newClip;
	screen->restrictToExposedOrRedrawn (&exposeClip);
    } else {
	// no screen to draw on...
        exposeClip.w = 0;
    }
    if (0 != exposeClip.w) {
	SDL_SetClipRect (on, &exposeClip);
//	Screen::debug () << "--- shrank clip rect to " << exposeClip << endl;
	if (useImage) {
	    SDL_BlitSurface (surface, &extent, on, &drawRegion);
	    if (NULL != screen) {
		// Note that SDL_BlitSurface changes drawRegion to the part
		// actually drawn on.
		screen->drawRect (drawRegion);
	    }
//	    Screen::debug () << "--- blitted from " << extent << " to "
//			     << drawRegion << endl;
	} else if (opaqueBackground) {
	    SDL_FillRect (on, &drawRegion, bgColor);
	    if (NULL != screen) {
		// Note that SDL_FillRect changes drawRegion to the part
		// actually drawn on.
		screen->drawRect (drawRegion);
	    }
//	    Screen::debug () << "--- drew background " << bgColor << " at "
//			     << drawRegion << endl;
	}
    }
    if (!needRedraw) {
	// reset clip rectangle for children
	SDL_SetClipRect (on, &newClip);
//	Screen::debug () << "--- set clip rect to " << newClip << endl;
    } else {
	needRedraw = false;
    }

    for (list<Frame*>::iterator it = children.begin (); 
    	 children.end () != it; it++) {
	Frame* child = *it;
//	Screen::debug () << "--- considering child " << child << " of " 
//	                 << this << endl;
	if (!child->visible) {
//	    Screen::debug () << "--- child not visible; ignoring " << endl;
	    continue;
	}
	if (extent.x >= child->x + child->extent.x + child->extent.w ||
	    extent.y >= child->y + child->extent.y + child->extent.h ||
	    extent.x + extent.w < child->x + child->extent.x ||
	    extent.y + extent.h < child->y + child->extent.y) {
//	    Screen::debug () << "--- child out of my extent; ignoring " << endl;
	    continue;
	}
	child->render (on, atX, atY);
    }

    SDL_SetClipRect (on, &origClip);

    if (NULL == parent) {
//	int32_t t = SDL_GetTicks ();
#if 0
	// for 3.0 emulator, screen size is touchpad size, so we need
	// to specify Pre 2 size for updates to avoid slowness of 
	// constant updates to unchanged parts of screen...
	SDL_UpdateRect (surface, 0, 0, 320, 480);
#else
	SDL_UpdateRect (surface, 0, 0, 0, 0);
#endif
//	Screen::debug () << "update took " << SDL_GetTicks () - t 
//		     	 << " msec" << endl;
	if (NULL != screen) { screen->resetExposed (); }
    }

//    Screen::debug () << dec;
}

void
Frame::addCallbacksToParent ()
{
    if (NULL != parent) {
	for (callback_id_t cid = FIRST_CALLBACK; NUM_CALLBACKS > cid; 
	     cid = (callback_id_t)(1 + (int32_t)cid)) { 
	    adjustCallbackCount (cid, childCallbackCnt[cid] +
				 (NULL != callback[cid]));
	}
    }
}

bool 
Frame::addChild (Frame* child)
{
    if (NULL != child->parent) { return false; }
    if (child == Screen::getRootFrame ()) { return false; }
    child->screen = screen;
    child->parent = this;
    children.push_back (child);
    child->addCallbacksToParent ();
    child->markForRedraw ();
    return true;
}

bool
Frame::withinBoundary (int32_t xCoord, int32_t yCoord)
{
    if (!extentValid) { return false; }
    mapToLocal (&xCoord, &yCoord);
    return (extent.x <= xCoord && extent.x + extent.w > xCoord &&
	    extent.y <= yCoord && extent.y + extent.h > yCoord);
}

void 
Frame::markExposure ()
{
    if (NULL != screen && wasOnScreen) {
	screen->expose (screenRect);
    }
}

void
Frame::doMarkRedraw ()
{
    needRedraw = true;
    for (list<Frame*>::iterator it = children.begin ();
         children.end () != it; it++) {
        (*it)->doMarkRedraw ();
    }
}

void
Frame::markSiblingsForRedraw ()
{
    // extents need to be recalculated for frame and all ancestors
    extentValid = false;

    if (NULL != parent) {
	parent->markSiblingsForRedraw ();

        list<Frame*>::iterator it;

	for (it = parent->children.begin (); this != (*it); it++);
	for (it++; parent->children.end () != it; it++) {
	    (*it)->doMarkRedraw ();
	}
    }
}

void 
Frame::markForRedraw ()
{
    doMarkRedraw ();

    for (Frame* p = this; NULL != p; p = p->parent) {
        p->extentValid = false;
    }

    // everything at or above this frame on the screen must be redrawn,
    // including later siblings and their children
    //markSiblingsForRedraw ();
}

}
