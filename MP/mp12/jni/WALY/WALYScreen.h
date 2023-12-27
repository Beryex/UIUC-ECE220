//									tab:8
//
// WALYScreen.h - header file for the WALY library screen class
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
// Version:	    1.03
// Creation Date:   27 July 2011
// Filename:	    WALYScreen.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	24 December 2012
//		Revised to support Android and Cygwin.
//	SL	1.02	27 December 2012
//		Working on several platforms.
//	SL	1.03	2 February 2013
//		Changed rootFrame allocation model for variable window sizing.
//

#if !defined(__WALY_SCREEN_H)
#define __WALY_SCREEN_H

#include <ostream>
#include <list>

//#include "SDL.h"
#include "WALYFrame.h"

namespace WALY_1_0_0 {
    
// constructed automatically when library is used

class Screen : public Frame {

#if defined(WALY_TARGET_ANDROID)
    public:
    	static void setWindow (ANativeWindow* win);
	static void clearWindow () { rootFrame->appWindow = NULL; }
    private:
    	void postRender ();
	ANativeWindow* appWindow;
#else // !defined(WALY_TARGET_ANDROID)
    private:
    	void postRender () { }
	void* appWindow; // placeholder...used only with Android
#endif // defined(WALY_TARGET_*)

    private:
	static std::ostream* debugOut;
	static int32_t       msBetweenFrames;

	Rect exposed;
	std::list<Rect> drawn; // portions drawn during call to render
	void renderScreen () {
	    drawn.clear ();
	    render (NULL, 0, 0);
	    postRender ();
	}

        Screen (int32_t width, int32_t height);

	bool doingCallback;
	std::list<Frame*> needCallback;    // list of callbacks for an event
	std::list<Frame*>::iterator cbWalk;
	std::list<Frame*>::iterator cbNext;
	void executeCallback (callback_id_t cid, const Event* e) {
	    if (doingCallback) { return; }
	    doingCallback = true;

	    // generate a list of frames, then call each one in list
	    // (avoid having deletions cause problems...)
	    registerForCallback (cid);
	    for (cbWalk = needCallback.begin (), cbNext = cbWalk, cbNext++;
	         cbWalk != needCallback.end (); cbWalk = cbNext, cbNext++) {
	        (*cbWalk)->callback[cid] (*cbWalk, e);
	    }
	    needCallback.clear ();

	    doingCallback = false;
	}

	static bool pause (int32_t* remainingTicksPtr);

	static Screen* rootFrame;

    public:
	static Frame* createRootFrame (int32_t width = 320,
				       int32_t height = 480) {
	    if (NULL != rootFrame) {
	    	throw std::exception ();
	    }
	    rootFrame = new Screen (width, height);
	    return rootFrame; 
	}
	static Frame* getRootFrame () { return rootFrame; }
	virtual ~Screen ();
	static void setDebugOutput (std::ostream* out) { debugOut = out; }
	static std::ostream& debug () { return *debugOut; }
	static void setMSBetweenFrames (int32_t ms) { msBetweenFrames = ms; }
	static void update ();
	
	static void run ();

	void expose (const Rect& r);
	const Rect& getExposed () { return exposed; }
	void resetExposed () { exposed.w = 0; }

	void drawRect (const Rect& rect);
	void restrictToExposedOrRedrawn (Rect* r);

	void addCallback (Frame* f) {
	    // add frame to list for callback
	    needCallback.push_back (f);
	}
	void withdrawCallback (Frame* f) {
	    // remove frame from list (usually because it's being deleted)
	    if (doingCallback) {
	        if (f == *cbNext) { cbNext++; }
		needCallback.remove (f);
	    }
	}

};

}

#endif /* __WALY_SCREEN_H */

