//									tab:8
//
// WALYFrame.h - header file for the WALY library frame class
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
// Version:	    1.04
// Creation Date:   27 July 2011
// Filename:	    WALYFrame.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	27 December 2012
//		Migrated a function from WALYFrame.cpp; specialized
//		  background RGB for Android.
//	SL	1.02	3 January 2013
//		Removed some dead code.
//	SL	1.03	5 January 2013
//		Added network callbacks, checking for need, and explanations.
//	SL	1.04	2 February 2013
//		Changed no-argument constructor to protected (used by Screen).
//

#if !defined(__WALY_FRAME_H)
#define __WALY_FRAME_H

#include <list>

namespace WALY_1_0_0 {
    
class Frame {
    friend class Screen;

    public:
	typedef enum {
	    FIRST_CALLBACK,
	    ACTIVATE = FIRST_CALLBACK,	// app (in)active and/or mouse/key focus
	    FRAME_UPDATE,		// (mostly) periodic frame redraw
	    KEY_DOWN,			// key pressed
	    KEY_UP,			// key released
	    MOUSE_MOTION,		// mouse pointer moved
	    MOUSE_DOWN,			// button down
	    MOUSE_UP,			// button up
	    CONNECTION,			// created or failed
	    NETWORK,			// data has arrived
	    QUIT_PROGRAM,		// end the program
	    NUM_CALLBACKS
	} callback_id_t;

	typedef enum {
	    ALIGN_TOP    = 0,
	    ALIGN_MIDDLE = 1,
	    ALIGN_BOTTOM = 2,
	    ALIGN_LEFT   = 0,
	    ALIGN_CENTER = 4,
	    ALIGN_RIGHT  = 8,
	    ALIGN_NW = ALIGN_TOP + ALIGN_LEFT,
	    ALIGN_N  = ALIGN_TOP + ALIGN_CENTER,
	    ALIGN_NE = ALIGN_TOP + ALIGN_RIGHT,
	    ALIGN_W  = ALIGN_MIDDLE + ALIGN_LEFT,
	    ALIGN_C  = ALIGN_MIDDLE + ALIGN_CENTER,
	    ALIGN_E  = ALIGN_MIDDLE + ALIGN_RIGHT,
	    ALIGN_SW = ALIGN_BOTTOM + ALIGN_LEFT,
	    ALIGN_S  = ALIGN_BOTTOM + ALIGN_CENTER,
	    ALIGN_SE = ALIGN_BOTTOM + ALIGN_RIGHT
	} align_loc_t;

    protected:
	Screen*      screen;      // screen on which this Frame is drawn

	Frame*       parent;

	int32_t      x;		  // position within parent
	int32_t      y;

	int32_t      unalignedX;  // alignment position
	int32_t      unalignedY;  // alignment position
	align_loc_t  align;

	bool         needRedraw;  // has content changed?

	bool	     visible;	  // drawn (if not, no children...)
	bool	     active;	  // receive callbacks? (if not, no children...)

	bool	     useImage;    // frame is a single image

	bool	     opaqueBackground;  // handled for all types...
	uint32_t     bgColor;

	bool	     cacheBitmap;

	// the actual drawing surface for the root window...
	Surface*     surface; // used only if cacheBitmap is true

	Rect         scrollRect;  // ignored for root window

	bool         extentValid;
	Rect         extent;      // bounding box for children

	bool	     wasOnScreen; // on screen during last rendering?
	Rect         screenRect;  // where?

	std::list<Frame*> children;

	// callbacks...
	void (*callback[NUM_CALLBACKS]) (Frame* f, const Event* e);
	int32_t childCallbackCnt[NUM_CALLBACKS];

	void recalculateExtent ();
	void notOnScreen ();
	void render (Surface* on, int32_t atX, int32_t atY);

	void adjustCallbackCount (callback_id_t cid, int32_t amt);
	void clearCallbackFunc (callback_id_t cid);
	bool requiresCallback (callback_id_t cid);

	// only the screen should call this recursive function...
	void registerForCallback (callback_id_t cid);

	// when changing/removing from parent
	void removeFromParent (bool clearCallback);

	void markExposure ();
	void doMarkRedraw ();
	void markSiblingsForRedraw ();
	void markForRedraw ();

	// called just before recalculating extent...
	virtual void prepareToRedraw () {}

        void construct () {
	    screen = NULL;
	    align = ALIGN_NW;
	    x = unalignedX;
	    y = unalignedY;
	    visible = true;
	    active = true;
	    useImage = false;
	    opaqueBackground = false;
	    cacheBitmap = false;
	    surface = NULL;
	    wasOnScreen = false;
	    scrollRect.x = scrollRect.y = scrollRect.h = scrollRect.w = 0;
	    for (callback_id_t cid = FIRST_CALLBACK; NUM_CALLBACKS > cid; 
		 cid = (callback_id_t)(1 + (int32_t)cid)) { 
		callback[cid] = NULL;
		childCallbackCnt[cid] = 0;
	    }
	    needRedraw = true;
	}

	void addCallbacksToParent ();
	virtual bool addChild (Frame* child);

	// used by Screen
        Frame () : parent (NULL), unalignedX (0), unalignedY (0) {
	    construct ();
	}

    public:
        Frame (Frame* par, int32_t xCoord = 0, int32_t yCoord = 0) : 
		    parent (NULL), unalignedX (xCoord), unalignedY (yCoord) {
	    construct ();
	    if (!par->addChild (this)) {
	        throw std::exception ();
	    }
	}

	virtual ~Frame () {
	    // note: setting children's parent to NULL first also saves
	    // us from removing their callbacks one by one

	    // delete children...
	    for (std::list<Frame*>::iterator it = children.begin ();
	    	 children.end () != it; it++) {
		(*it)->parent = NULL; // prevent callback from child
	        delete *it;
	    }
	    children.clear ();

	    // Remove from parent's children and handle callback counting...
	    removeFromParent (true);
	}

	Frame* getParent () { return parent; }

	void setParent (Frame* newPar);

	void setVisible (bool newVal) { 
	    visible = newVal; 
	    if (visible) {
		markForRedraw ();
	    } else {
	        markExposure ();
	    }
	}
	void setActive (bool newVal) { active = newVal; }
	const Rect& getScrollRect () const { return scrollRect; }
	void setScrollRect (const Rect& newRect) { 
	    scrollRect = newRect;
	    markExposure ();
	    markForRedraw ();
	}

	void useSolidBackground (uint32_t RGB) {
	    opaqueBackground = true;
#if !defined(WALY_TARGET_ANDROID)
	    bgColor = (0xFF000000 | RGB);
#else // defined(WALY_TARGET_ANDROID)
	    bgColor = (0xFF000000 | ((RGB & 0xFF0000) >> 16) |
		       (RGB & 0xFF00) | ((RGB & 0xFF) << 16));
#endif // defined(WALY_TARGET_*)
	    markForRedraw ();
	}
	void noSolidBackground () {
	    opaqueBackground = false;
	    markExposure ();
	    markForRedraw ();
	}
	void attachImage (Image* img); // need Image class definition
	void detachImage () { 
	    useImage = false;
	    surface = NULL;
	    markExposure ();
	    markForRedraw (); // shouldn't be needed, but...
	}

	void setCallbackFunc (callback_id_t cid, 
			      void (*newFunc) (Frame* f, const Event* e));

	void setAlign (align_loc_t newVal) {
	    align = newVal;
	    // recalculating extent resets alignment...
	    extentValid = false;
	    markExposure ();
	    markForRedraw ();
	}

	int32_t getX () { return unalignedX; }
	void setX (int32_t newVal) { 
	    //x = newVal + x - unalignedX; 
	    unalignedX = newVal;
	    markExposure ();
	    markForRedraw ();
	}

	int32_t getY () { return unalignedY; }
	void setY (int32_t newVal) {
	    //y = newVal + y - unalignedY; 
	    unalignedY = newVal;
	    markExposure ();
	    markForRedraw ();
	}

	bool withinBoundary (int32_t xCoord, int32_t yCoord);

	// Note: only valid when extent is valid...
	void mapToLocal (int32_t* xCoord, int32_t* yCoord) {
	    for (Frame* f = this; NULL != f; f = f->parent) {
		*xCoord -= f->x;
		*yCoord -= f->y;
	    }
	}

	// utility function
	static bool intersect (const Rect* src, Rect* dst) {
	    int32_t left, right, top, bottom;

	    left = (src->x > dst->x ? src->x : dst->x);
	    right = (src->x + src->w < dst->x + dst->w ? 
		     src->x + src->w : dst->x + dst->w);
	    top = (src->y > dst->y ? src->y : dst->y);
	    bottom = (src->y + src->h < dst->y + dst->h ? 
		      src->y + src->h : dst->y + dst->h);
	    if (right <= left || bottom <= top) {
		dst->x = dst->y = dst->w = dst->h = 0;
		return false;
	    }
	    dst->x = left;
	    dst->y = top;
	    dst->w = right - left;
	    dst->h = bottom - top;
	    return true;
	}
};

}

#endif /* __WALY_FRAME_H */

