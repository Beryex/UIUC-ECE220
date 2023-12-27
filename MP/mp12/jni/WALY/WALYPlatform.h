//									tab:8
//
// WALYPlatform.h - header file for most platform-dependent WALY code
//
// "Copyright (c) 2012-2013 by Steven S. Lumetta."
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
// Version:	    1.06
// Creation Date:   23 December 2012
// Filename:	    WALYPlatform.h
// History:
//	SL	1.00	23 December 2012
//		First written.
//	SL	1.01	1 January 2013
//		Working on several platforms.
//	SL	1.02	3 January 2013
//		Added orientation-handling.
//	SL	1.03	4 January 2013
//		Fixed minor bug with SDL mouse limits.
//	SL	1.04	5 January 2013
//		Added network events.
//	SL	1.05	12 January 2013
//		Fixed cut-and-paste bug with keyboard hiding.
//	SL	1.06	2 February 2013
//		Added support for allowing application to size the window.
//

#if !defined(WALY_PLATFORM_H)
#define WALY_PLATFORM_H

//
// platform-specific includes for WALY
//
#if !defined(WALY_TARGET_ANDROID)
#    include <SDL/SDL.h>
#    include <SDL/SDL_ttf.h>
#    if defined(WALY_TARGET_WEBOS)
#        include <PDL.h>
#    endif // defined(WALY_TARGET_WEBOS)
#else // defined(WALY_TARGET_ANDROID)
#    include <WALYSDLStubs.h>
#    include <android_native_app_glue.h>
#endif // defined(WALY_TARGET_*)

namespace WALY_1_0_0 {

//
// Now that we have SDL event definitions (either from SDL or from our
// own stubs), we can define extra events in those terms...
//
#define WALY_CONNECTION_EVENT (SDL_USEREVENT + 0)
#define WALY_NETWORK_EVENT    (SDL_USEREVENT + 1)

//
// platform-specific elements for WALY
//
#if !defined(WALY_TARGET_ANDROID)
#define WALY_TOP_PIXEL   0
#define WALY_KBD_IS_SOFT 0
#define WALY_KBD_PIXEL   0
#if defined(WALY_TARGET_WEBOS)
#define WALY_FONT_DIR  "/usr/share/fonts/"
#define WALY_IMAGE_DIR "images/"
#define WALY_SOUND_DIR "sounds/"
#else // defined(WALY_TARGET_CYGWIN)
// SDL device count unknown...rather than limit, we just size by type in
// the SDL mouse events...
#define SDL_MAXMOUSE 256
#define PDLK_GESTURE_BACK    0x1B    // ESC key
#define PDLK_GESTURE_FORWARD 0x10000 // won't match a key
#define WALY_FONT_DIR  "assets/fonts/"
#define WALY_IMAGE_DIR "assets/images/"
#define WALY_SOUND_DIR "assets/sounds/"
#endif // !defined(WALY_TARGET_ANDROID)
#else // defined(WALY_TARGET_ANDROID)
#define WALY_TOP_PIXEL   0   // rescaled to avoid status bar
#define WALY_KBD_IS_SOFT 1
#define WALY_KBD_PIXEL   320
#define WALY_FONT_DIR  ""
#define WALY_IMAGE_DIR "images/"
#define WALY_SOUND_DIR "sounds/"
#endif // defined(WALY_TARGET_*)

    typedef SDL_Rect Rect;
    typedef SDL_Surface Surface;
    typedef SDL_Event Event;

    class Frame;
    class Widget;

    class Button;
    class DigitDraw;
    class EntryBox;
    class EntryBoxIP;
    class Filter;
    class Font;
    class Image;
    class Label;
    class Screen;
    class Sound;


// the Platform class definition; static methods only

class Platform {
    public:
	// initialize platform-specific interfaces
	static Surface* init (int32_t width, int32_t height);
	// clean up platform-specific interfaces for shutdown
	static void terminate ();

	// load an image from a file/asset
	static Surface* IMG_Load (const char* fname);

	// get the application name
	static char* appName;
	static const char* getAppName () { return appName; }

	// get a local directory for storage
	static char* dirName;
	static const char* getDirName () { return dirName; }

	// get a visible local directory for debug files
	static char* visDirName;
	static const char* getVisibleDir () { return visDirName; }

	// minimize the window
	static void minimize ();

	// poll with timeout (as usual, pass -1 to msec for unlimited waiting)
	static bool pollEvent (int32_t msec, Event* evt);

#if !defined(WALY_TARGET_ANDROID)
	// hard keyboards...
	static void showKeyboard () { }
	static void hideKeyboard () { }
#if 0 // these are not used outside of Android-specific code (for events)
	static bool keyboardShown () { return true; }
	static void getScreenDims (int32_t* widthPtr, int32_t* heightPtr,
				   int32_t* topPtr, int32_t* bottomPtr) {
	    *widthPtr  = 320;
	    *heightPtr = 480;
	    *topPtr    = 0;
	    *bottomPtr = 480;
	}
	static void getDrawingOffset (int32_t* xPtr, int32_t* yPtr) {
	    *xPtr = 0;
	    *yPtr = 0;
	}
	static void setDrawingOffset (int32_t offX, int32_t offY) { }
#endif
	// done in Platform::init ()
#else // defined(WALY_TARGET_ANDROID)
	static struct  android_app* app;
	static JNIEnv* env;
	static jmethodID showKbdMethod;
	static jmethodID hideKbdMethod;
	static jmethodID minimizeMethod;
	static jmethodID getTopVisibleMethod;
	static jmethodID getBottomVisibleMethod;
	static bool kbdShown;
	static int32_t physScreenW;
	static int32_t physScreenH;
	static int32_t drawOffX;
	static int32_t drawOffY;
	static int32_t appRequestW;
	static int32_t appRequestH;
    public:
    	static void setAppPointer (struct android_app* newApp) { 
	    app = newApp;
	    app->activity->vm->AttachCurrentThread (&env, NULL);
	    jclass ourClass = env->GetObjectClass (app->activity->clazz);
	    showKbdMethod = env->GetMethodID (ourClass, "showKeyboard", "()V");
	    hideKbdMethod = env->GetMethodID (ourClass, "hideKeyboard", "()V");
	    minimizeMethod = env->GetMethodID (ourClass, "minimize", "()V");
	    getTopVisibleMethod = env->GetMethodID 
	    			     	(ourClass, "getTopVisible", "()I");
	    getBottomVisibleMethod = env->GetMethodID 
	    			     	(ourClass, "getBottomVisible", "()I");

	    jmethodID getNameMethod = env->GetMethodID 
		    (ourClass, "getAppName", "()Ljava/lang/String;");
	    jstring name = (jstring)env->CallObjectMethod 
		    (app->activity->clazz, getNameMethod);
	    int32_t len = env->GetStringUTFLength (name);
	    appName = new char[len + 1];
	    strncpy (appName, env->GetStringUTFChars (name, NULL), len);
	    appName[len] = '\0';

	    jmethodID getDirMethod = env->GetMethodID 
		    (ourClass, "getAppDir", "()Ljava/lang/String;");
	    jstring dir = (jstring)env->CallObjectMethod 
		    (app->activity->clazz, getDirMethod);
	    int32_t dirLen = env->GetStringUTFLength (dir);
	    dirName = new char[dirLen + 1];
	    strncpy (dirName, env->GetStringUTFChars (dir, NULL), dirLen);
	    dirName[dirLen] = '\0';

	    const char* vDir = "/sdcard/Android/data/debug/";
	    int32_t vDirLen = strlen (vDir);
	    visDirName = new char[vDirLen + 1];
	    strncpy (visDirName, vDir, vDirLen);
	    visDirName[vDirLen] = '\0';

	    // find the physical screen dimensions
	    //   (NOT including the 'hard' buttons)
	    jmethodID getWidthMethod = env->GetMethodID 
		    (ourClass, "getWidth", "()I");
	    physScreenW = env->CallIntMethod 
		    (app->activity->clazz, getWidthMethod);
	    jmethodID getHeightMethod = env->GetMethodID 
		    (ourClass, "getHeight", "()I");
	    physScreenH = env->CallIntMethod 
		    (app->activity->clazz, getHeightMethod);
	    if (physScreenW > physScreenH) {
	    	int32_t tmp = physScreenW; // swap them
		physScreenW = physScreenH;
		physScreenH = tmp;
	    }
	}
	static void detach () {
	    if (NULL != env) {
		app->activity->vm->DetachCurrentThread ();
		env = NULL;
	    }
	}
	static struct android_app* getApp () { return app; }
	static void showKeyboard () {
	    if (!kbdShown) {
		env->CallVoidMethod (app->activity->clazz, showKbdMethod);
		kbdShown = true;
	    }
	}
	static void hideKeyboard () {
	    if (kbdShown) {
		env->CallVoidMethod (app->activity->clazz, hideKbdMethod);
		kbdShown = false;
	    }
	}
	static bool keyboardShown () { return kbdShown; }
	static void getScreenDims (int32_t* widthPtr, int32_t* heightPtr,
				   int32_t* topPtr, int32_t* bottomPtr) {
	    int32_t top = env->CallIntMethod (app->activity->clazz,
					      getTopVisibleMethod);
	    int32_t bottom = env->CallIntMethod (app->activity->clazz,
						 getBottomVisibleMethod);
	    *topPtr    = top;
	    *bottomPtr = bottom;
	    *widthPtr  = physScreenW;
	    *heightPtr = physScreenH;
	}
	static void getRequestedDims (int32_t* widthPtr, int32_t* heightPtr) {
	    *widthPtr  = appRequestW;
	    *heightPtr = appRequestH;
	}
	static void getDrawingOffset (int32_t* xPtr, int32_t* yPtr) {
	    *xPtr = drawOffX;
	    *yPtr = drawOffY;
	}
	static void setDrawingOffset (int32_t offX, int32_t offY) { 
	    drawOffX = offX;
	    drawOffY = offY;
	}
#endif // defined(WALY_TARGET_*) 
};

}

#endif /* WALY_PLATFORM_H */

