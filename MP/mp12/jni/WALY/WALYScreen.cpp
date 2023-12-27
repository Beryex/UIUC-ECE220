//									tab:8
//
// WALYScreen.cpp - source file for the WALY library screen class
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
// Version:	    1.05
// Creation Date:   27 July 2011
// Filename:	    WALYScreen.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	24 December 2012
//		Revised to support Android and Cygwin.
//	SL	1.02	1 January 2013
//		Working on several platforms.
//	SL	1.03	3 January 2013
//		Added orientation-handling.
//	SL	1.04	4 January 2013
//		Added network initialization, teardown, and event checking.
//	SL	1.05	2 February 2013
//		Changed rootFrame allocation model for variable window sizing.
//

#include <exception>
#include <iostream>

#include "WALY.h"

#if !defined(WALY_TARGET_ANDROID)
static void WALYFinishEvent () { } // empty stub
#else // defined(WALY_TARGET_ANDROID)
#include <android/log.h>
#endif // defined(WALY_TARGET_ANDROID)

using namespace std;

namespace WALY_1_0_0 {

Screen*      Screen::rootFrame = NULL;
ostream*     Screen::debugOut = &cout;
int32_t      Screen::msBetweenFrames = 40;

extern ostream& operator<< (ostream& s, const Rect& r);

void
Screen::drawRect (const Rect& rect)
{
    Rect r = rect;
    list<Rect>::iterator it;
    list<Rect>::iterator next;

    // Compare r with the existing rectangles.
    for (it = drawn.begin (), next = it, next++;
         drawn.end () != it; it = next, next++) {

	if (r.x >= it->x && r.x + r.w <= it->x + it->w &&
	    r.y >= it->y && r.y + r.h <= it->y + it->h) {
	    // The old rectangle contained the new one--we're done.
	    return;
	}

        // If not overlapping, ignore...
	if (r.x + r.w <= it->x || it->x + it->w <= r.x ||
	    r.y + r.h <= it->y || it->y + it->h <= r.y) {
	    continue;
	}

	// Find the union.
	int32_t left   = (r.x < it->x ? r.x : it->x);
	int32_t right  = (r.x + r.w > it->x + it->w ?
			  r.x + r.w : it->x + it->w);
	int32_t top    = (r.y < it->y ? r.y : it->y);
	int32_t bottom = (r.y + r.h > it->y + it->h ?
			  r.y + r.h : it->y + it->h);
        r.x = left;
	r.y = top;
	r.w = right - left;
	r.h = bottom - top;

	//
	// Now erase the old rectangle and start over with the 
	// bigger rectangle...
	//
	drawn.erase (it);
	next = drawn.begin ();
    }

    drawn.push_front (r);
}

void
Screen::restrictToExposedOrRedrawn (Rect* r)
{
    //
    // Replace r with the union of the intersection of r with
    // the exposed and redrawn portions of the screen.
    //

    int32_t unionLeft   =  100000;
    int32_t unionRight  = -100000;
    int32_t unionTop    =  100000;
    int32_t unionBottom = -100000;

    int32_t left   = (r->x < exposed.x ? exposed.x : r->x);
    int32_t right  = (r->x + r->w > exposed.x + exposed.w ?
		      exposed.x + exposed.w : r->x + r->w);
    int32_t top    = (r->y < exposed.y ? exposed.y : r->y);
    int32_t bottom = (r->y + r->h > exposed.y + exposed.h ?
		      exposed.y + exposed.h : r->y + r->h);
    if (left < right && top < bottom) {
	if (unionLeft   >   left) { unionLeft   = left;   }
	if (unionRight  <  right) { unionRight  = right;  }
	if (unionTop    >    top) { unionTop    = top;    }
	if (unionBottom < bottom) { unionBottom = bottom; }
    }

    for (list<Rect>::iterator it = drawn.begin (); drawn.end () != it; it++) {
	int32_t left   = (r->x < it->x ? it->x : r->x);
	int32_t right  = (r->x + r->w > it->x + it->w ?
			  it->x + it->w : r->x + r->w);
        int32_t top    = (r->y < it->y ? it->y : r->y);
	int32_t bottom = (r->y + r->h > it->y + it->h ?
			  it->y + it->h : r->y + r->h);
        if (left < right && top < bottom) {
	    if (unionLeft   >   left) { unionLeft   = left;   }
	    if (unionRight  <  right) { unionRight  = right;  }
	    if (unionTop    >    top) { unionTop    = top;    }
	    if (unionBottom < bottom) { unionBottom = bottom; }
	}
    }

    r->x = unionLeft;
    r->y = unionTop;
    if (unionLeft < unionRight && unionTop < unionBottom) {
        r->w = unionRight - unionLeft;
	r->h = unionBottom - unionTop;
    } else {
        r->w = r->h = 0;
    }
}

Screen::Screen (int32_t width, int32_t height) : appWindow (NULL)
{
    doingCallback = false;
    
    if (NULL == (surface = Platform::init (width, height))) {
	// init should have thrown an exception if appropriate...
        return;
    }

    //
    // We need some platform-specific aspects (SDL, PDL) initialized 
    // before we try to initialize default font/sound...
    //
    try {
        Font::init ();
	Sound::init ();
	if (!Network::init ()) {
	    throw std::exception ();
	}
    } catch (exception& e) {
        // panic!
	Platform::terminate ();
	throw std::exception ();
    }

    screen = this; // from Frame
    exposed.x = 0; // fully exposed at start (needs complete redraw)
    exposed.y = 0;
    exposed.w = surface->w;
    exposed.h = surface->h;
}

Screen::~Screen () 
{
    if (rootFrame == this) {
        rootFrame = NULL;
    }
    Font::quit ();
    Sound::quit ();
    Network::quit ();

    // shut down the graphics, etc.
    Platform::terminate ();
}

bool Screen::pause (int32_t* remainingTicksPtr)
{
    SDL_Event evt;

    while (1) {
	(void)Platform::pollEvent (-1, &evt);
	switch ((int32_t)evt.type) {
	    case SDL_ACTIVEEVENT:
// debug () << "(paused) ACTIVE state = " << (int)evt.active.state << " gain = " << (int)evt.active.gain << "ticks = " << SDL_GetTicks () << endl;
		//
		// mouse activations sent when mouse pointer enters/leaves 
		// keyboard activations sent when keyboard focus changes
		// app activations sent on iconify/deiconify
		//
		// We perform the callback for all events, but only re-start
		// the program when the app becomes active...
		//
		rootFrame->executeCallback (ACTIVATE, &evt);
		if (0 != (evt.active.state & SDL_APPACTIVE)) {
		    if (evt.active.gain) {
			// must call before waiting on new events...
			WALYFinishEvent ();
			return false;
		    }
		} else {
		    // ignore other activations
		    WALYFinishEvent ();
		}
		break;
	    case SDL_QUIT:
		rootFrame->executeCallback (QUIT_PROGRAM, &evt);
		// finish after returning from WALY_main
		// WALYFinishEvent ();
		debug () << "application received SDL_QUIT" << endl;
		return true;
	    case SDL_VIDEOEXPOSE:
		// this happens when user turns phone off and back on
		// while running the app...
		// no update sent for now...
		update ();
		*remainingTicksPtr = msBetweenFrames;
		WALYFinishEvent ();
		break;
	    case SDL_KEYDOWN:
	    case SDL_KEYUP:
	    case SDL_MOUSEMOTION:
	    case SDL_MOUSEBUTTONDOWN:
	    case SDL_MOUSEBUTTONUP:
	        // Discard these silently: the window is inactive.
		// The only one I've seen here is on Cygwin, where a
		// call to minimize generates an ACTIVEEVENT that brings
		// us into this function, but the KEYUP is still 
		// delivered to this app.
		WALYFinishEvent ();
		break;
	    default:
		// this can happen in Android if we push back to pause
		// the app (back key release can arrive after pause)
		debug () << "unknown event while inactive: " 
			 << (int32_t)evt.type << endl;
		WALYFinishEvent ();
		break;
	}
    }
}

void Screen::update ()
{
    rootFrame->renderScreen ();
}

void 
Screen::run ()
{
    uint32_t  lastUpdate;
    int32_t  remainingTicks = 0;
    SDL_Event evt;

#if 0
    int32_t joyX = 0, joyY = 0, joyZ = 0;
    int32_t joyNewX = 0, joyNewY = 0, joyNewZ = 0;
    SDL_Joystick* joystick = SDL_JoystickOpen (0);
#endif

//
// 24 Dec 12: should probably wait until we have an event 
// before updating?  need to make sure we have a surface
// (from INIT_WINDOW) with Android... --SSL
//

    // will receive initial redraw request, so don't render yet...
    // update ();
    lastUpdate = SDL_GetTicks ();
    while (1) {
	int32_t delay = lastUpdate + msBetweenFrames - SDL_GetTicks ();

	if (0 >= delay) {
	    if (0 > delay) {
//		debug () << "late by" << (-delay) << "msec" << endl;
	    }
	    lastUpdate = SDL_GetTicks ();
	    rootFrame->executeCallback (FRAME_UPDATE, NULL);
	    update ();
//debug () << "render took " << SDL_GetTicks () - lastUpdate << " msec" << endl;
	}
#if 0
	joyNewX = SDL_JoystickGetAxis (joystick, 0);
	joyNewY = SDL_JoystickGetAxis (joystick, 1);
	joyNewZ = SDL_JoystickGetAxis (joystick, 2);
	if (joyX != joyNewX || joyY != joyNewY || joyZ != joyNewZ) {
	    joyX = joyNewX;
	    joyY = joyNewY;
	    joyZ = joyNewZ;
	    double jx = joyX / (double)32768;
	    double jy = joyY / (double)32768;
	    double jz = joyZ / (double)32768;
	    double sum = jx * jx + jy * jy + jz * jz;

	    joyX = joyX / 618; 
	    if (4 > joyX && -4 < joyX) {
	    	joyX = 0;
	    } else {
	    	joyX = (0 > joyX ? joyX + 3 : joyX - 3);
	    }

if (0 != joyX) {
	    debug () << "joystick changed: (" << joyX << "," << joyY << ","
	    	     << joyZ << ") -> " << sum << endl;
}
	    joyX = joyNewX;
	    joyY = joyNewY;
	    joyZ = joyNewZ;
	}
#endif

	//
	// Allowing Android to sleep makes updates late, (and SDL 
	// doesn't support short-term sleeping in poll).
	//
	while (Platform::pollEvent (0, &evt)) {
	    switch ((int32_t)evt.type) {
		case SDL_ACTIVEEVENT:
// debug () << "ACTIVE state = " << (int)evt.active.state << " gain = " << (int)evt.active.gain << "ticks = " << SDL_GetTicks () << endl;
		    //
		    // mouse activations sent when mouse pointer enters/leaves 
		    // keyboard activations sent when keyboard focus changes
		    // app activations sent on iconify/deiconify
		    //
		    // We perform the callback for all events, but only stop
		    // the program when the app is inactive...
		    //
		    rootFrame->executeCallback (ACTIVATE, &evt);
		    if (0 != (evt.active.state & SDL_APPACTIVE)) {
			if (!evt.active.gain) {
			    remainingTicks = lastUpdate + msBetweenFrames - 
					     SDL_GetTicks ();
			    // must call before waiting on new events...
			    WALYFinishEvent ();
			    if (pause (&remainingTicks)) { return; }
			    lastUpdate = SDL_GetTicks () + remainingTicks - 
					 msBetweenFrames;
			}
		    } else {
			// ignore other activations
			WALYFinishEvent ();
		    }
		    break;
		case SDL_KEYDOWN:
		    rootFrame->executeCallback (KEY_DOWN, &evt);
		    WALYFinishEvent ();
//		    *debugOut << "key down " << evt.key.keysym.sym << endl;
		    break;
		case SDL_KEYUP:
		    rootFrame->executeCallback (KEY_UP, &evt);
		    WALYFinishEvent ();
//		    *debugOut << "key up " << evt.key.keysym.sym << endl;
		    break;
		case SDL_MOUSEMOTION:
		    rootFrame->executeCallback (MOUSE_MOTION, &evt);
		    WALYFinishEvent ();
//		    *debugOut << "mouse " << (int32_t)evt.motion.which 
//				      << " moved to (" << evt.motion.x 
//				      << "," << evt.motion.y << ")" << endl;
		    break;
		case SDL_MOUSEBUTTONDOWN:
		    rootFrame->executeCallback (MOUSE_DOWN, &evt);
		    WALYFinishEvent ();
//		    *debugOut << "mouse " << (int32_t)evt.button.which 
//				      << " down at (" << evt.button.x 
//				      << "," << evt.button.y << ")" << endl;
		    break;
		case SDL_MOUSEBUTTONUP:
		    rootFrame->executeCallback (MOUSE_UP, &evt);
		    WALYFinishEvent ();
//		    *debugOut << "mouse " << (int32_t)evt.button.which 
//				      << " up at (" << evt.button.x 
//				      << "," << evt.button.y << ")" << endl;
		    break;
		case SDL_QUIT:
		    rootFrame->executeCallback (QUIT_PROGRAM, &evt);
		    // finish after returning from WALY_main
		    // WALYFinishEvent ();
		    //
		    // If you call SDL_WaitEvent after seeing an SDL_QUIT,
		    // the SDL_WaitEvent call will not return...
		    //
		    *debugOut << "application quit" << endl;
		    break;
		case SDL_VIDEOEXPOSE:
		    // no update sent for now...
		    update ();
		    remainingTicks = msBetweenFrames;
		    WALYFinishEvent ();
//		    *debugOut << "redraw request" << endl;
		    break;
#if 0
		case SDL_JOYAXISMOTION:
		    *debugOut << "joystick axis: " 
		    << (int32_t)evt.jaxis.type << "  "
		    << (int32_t)evt.jaxis.which  << "  "
		    << (int32_t)evt.jaxis.axis  << "  "
		    << (int32_t)evt.jaxis.value  << "  "
		    << endl;
		    break;
#endif
		default:
		    WALYFinishEvent ();
		    *debugOut << "unknown type " << (int32_t)evt.type << endl;
		    break;
	    }
	    if (SDL_QUIT == evt.type) {
		return;
	    }
	}

	// handle any network requests...
	while (Network::pollEvent (rootFrame->requiresCallback (CONNECTION),
				   rootFrame->requiresCallback (NETWORK),
				   &evt)) {
	    if (WALY_CONNECTION_EVENT == evt.type) {
		rootFrame->executeCallback (CONNECTION, &evt);
	    } else {
		rootFrame->executeCallback (NETWORK, &evt);
	    }
	    delete[] (uint8_t*)evt.user.data2;
	}
    }
}

void
Screen::expose (const Rect& r)
{
    if (0 == exposed.w) {
        exposed = r;
	return;
    }

    int32_t left = (exposed.x <= r.x ? exposed.x : r.x);
    int32_t right = (exposed.x + exposed.w > r.x + r.w ?
		     exposed.x + exposed.w : r.x + r.w);
    int32_t top = (exposed.y <= r.y ? exposed.y : r.y);
    int32_t bottom = (exposed.y + exposed.h > r.y + r.h ?
		      exposed.y + exposed.h : r.y + r.h);

    exposed.x = left;
    exposed.y = top;
    exposed.w = right - left;
    exposed.h = bottom - top;
}

#if defined(WALY_TARGET_ANDROID)
void
Screen::setWindow (ANativeWindow* win)
{
    rootFrame->appWindow = win;

    // new window--fully expose it for redraw...
    // this can be called >1 time with Android, so
    // don't assume that new Screen code will handle!
    rootFrame->exposed.x = rootFrame->exposed.y = 0;
    rootFrame->exposed.w = rootFrame->surface->w;
    rootFrame->exposed.h = rootFrame->surface->h;
}

void 
Screen::postRender () 
{
    ANativeWindow_Buffer buf;

    //
    // we use a private buffer and then just splat the whole new
    // screen into Android's native window; their 'dirty' region
    // is too weird to deduce; I think I'm supposed to know what
    // I want to redraw before I call it, but ... why bother?
    //
    
    //
    // Sometimes (particularly with emulator), Android is so slow that
    // the application wants another frame before the window has been
    // delivered.  So... we need to be careful... (they don't check for
    // the first arg being NULL in the call below, apparently).
    //
    if (NULL == appWindow) {
	__android_log_print (ANDROID_LOG_INFO, "WALY",
			     "update with no window!");
        return;
    }

    // We need to lock the native window before drawing on it.
    // (passing NULL for 'inoutdirtyrect' means, I believe, that 
    // we'll redraw the whole thing)
    if (0 != ANativeWindow_lock (appWindow, &buf, NULL)) {
	__android_log_print (ANDROID_LOG_INFO, "WALY", 
			     "lock native window failed");
        return;
    }

    // we want to handle orientation changes somewhat gracefully...
    // sanity check...
    if (surface->w > buf.width || surface->h > buf.height ||
        // buf.width != buf.stride ||
	WINDOW_FORMAT_RGBX_8888 != buf.format) {
	ANativeWindow_unlockAndPost (appWindow);
	__android_log_print (ANDROID_LOG_INFO, "WALY", 
			     "bad native window!");
        return;
    }

    int32_t offX;
    int32_t offY;
    Platform::getDrawingOffset (&offX, &offY);
    uint32_t* tgt = ((uint32_t*)buf.bits) + offX + offY * buf.stride;
    uint8_t* from = (uint8_t*)surface->pixels;
    for (int32_t y = 0; surface->h > y; y++) {
	memcpy (tgt, from, surface->pitch);
        tgt += buf.stride;
	from += surface->pitch;
    }

    ANativeWindow_unlockAndPost (appWindow);
}

#endif // defined(WALY_TARGET_ANDROID)

}
