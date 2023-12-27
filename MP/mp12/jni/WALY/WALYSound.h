//									tab:8
//
// WALYSound.h - header file for the WALY library sound class
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
// Version:	    1.02
// Creation Date:   27 July 2011
// Filename:	    WALYSound.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	27 July 2011
//		Added counting to control destruction order and Mix_Quit.
//	SL	1.02	26 December 2012
//		Revised to support Android and Cygwin.
//

#if !defined(__WALY_SOUND_H)
#define __WALY_SOUND_H

#include <exception>

#if !defined(WALY_TARGET_ANDROID)
#include "SDL/SDL_mixer.h"
#else // defined(WALY_TARGET_ANDROID)
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#endif // defined(WALY_TARGET_*)

namespace WALY_1_0_0 {

//
// code is all platform dependent (SDL mixer for WebOS/Cygwin,
// OpenSLES for Android), so let's fix the interface and put all
// the code into the source file...
//
class Sound {
    private:
	static const int32_t channelLimit = 32;

	//
	// Why bother to count sounds?  While it's easy enough to ensure
	// that we call Mix_Init before we try to open any sounds, it's
	// a pain to try to order destructors when we quit.  On some
	// platforms, the program is just killed, so it doesn't matter.
	// On others, we need to make sure that the sounds are closed
	// before we call Mix_Quit to avoid the program 'crashing' when
	// a user tries to quit.
	//
	static Sound* beingPlayed[channelLimit];
	static int32_t soundCount;
	static bool doQuit;

	int32_t chanNum; // -1 when not playing

#if !defined(WALY_TARGET_ANDROID)
        Mix_Chunk* sound;
	static void channelDone (int channel); // callback
#else // defined(WALY_TARGET_ANDROID)
	SLObjectItf player;  // audio player object
	SLPlayItf   playItf; // play interface
	SLSeekItf   seekItf; // seek interface
	int32_t     count;   // times to loop (-1 means infinite)

	static SLObjectItf engine;	// engine object
	static SLEngineItf engineItf;   // engine creation interface
	static SLObjectItf mixer;       // output mixer (sink for all sounds)

	// callback
	static void playEnded (SLPlayItf ignored, void* soundPtr, SLuint32 evt);
#endif // defined(WALY_TARGET_*)

	static void init ();
	static void quit ();
	friend class Screen; // Screen calls init and quit

    public:
	Sound (const char* fileName);
	~Sound ();
	bool play (int32_t iter = 0);
	bool stop ();
	// add a function for fadeout?
	static void stopAll ();
};

}

#endif /* __WALY_SOUND_H */

