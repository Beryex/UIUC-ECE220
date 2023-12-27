//									tab:8
//
// WALYSound.cpp - source file for the WALY library sound class
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
// Filename:	    WALYSound.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	25 July 2012
//		Added counting to control destruction order and Mix_Quit.
//	SL	1.02	26 December 2012
//		Revised to support Android and Cygwin.
//	SL	1.03	6 January 2013
//		Added destruction ordering to Android sound management.
//		Cleared channel array when stopping all sounds with SDL.
//

#include "WALY.h"

#if defined(WALY_TARGET_ANDROID)
#include <android/log.h>
#endif // defined(WALY_TARGET_ANDROID)

namespace WALY_1_0_0 {

//
// Why bother to count sounds?  While it's easy enough to ensure
// that we call Mix_Init before we try to open any sounds, it's
// a pain to try to order destructors when we quit.  On some
// platforms, the program is just killed, so it doesn't matter.
// On others, we need to make sure that the sounds are closed
// before we call Mix_Quit to avoid the program 'crashing' when
// a user tries to quit.
//
Sound* Sound::beingPlayed[channelLimit];
int32_t Sound::soundCount = 0;
bool Sound::doQuit = true;

#if !defined(WALY_TARGET_ANDROID)

void 
Sound::channelDone (int channel) 
{
    if (NULL != beingPlayed[channel]) {
	beingPlayed[channel]->chanNum = -1;
	beingPlayed[channel] = NULL;
    }
}

void 
Sound::init () 
{
    int32_t flags = 0;
    //
    // We are not pre-loading any libraries for the mixer, so
    // the call to Mix_Init is just a placeholder in case we 
    // need to do so in the future.
    //
    if (!doQuit || 0 != soundCount || flags != Mix_Init (flags)) {
	throw std::exception ();
    }
    // default is 22.05kHz
    if (0 != Mix_OpenAudio (44100, // MIX_DEFAULT_FREQUENCY, 
			    MIX_DEFAULT_FORMAT, 2, 1024)) {
	Mix_Quit ();
	throw std::exception ();
    }
    (void)Mix_AllocateChannels (channelLimit);
    Mix_ChannelFinished (channelDone);
    doQuit = false;
}

void 
Sound::quit () 
{
    if (doQuit) {
        throw std::exception ();
    }
    doQuit = true;
    if (0 == soundCount) {
	(void)Mix_CloseAudio ();
	Mix_Quit ();
    }
}

Sound::Sound (const char* fileName) : chanNum (-1) 
{
    char fName[1024];

    sprintf (fName, "%s%.800s", WALY_SOUND_DIR, fileName);
    sound = Mix_LoadWAV (fName);
    if (NULL == sound) {
	throw std::exception ();
    }
    soundCount++;
}

Sound::~Sound () 
{
    // should I be locking here?  not needed if sounds are stopped
    // before destroyed, but ... FIXME?
    if (-1 != chanNum) {
	Mix_HaltChannel (chanNum);
	beingPlayed[chanNum] = NULL;
    }
    Mix_FreeChunk (sound);
    if (0 == --soundCount && doQuit) {
        (void)Mix_CloseAudio ();
	Mix_Quit ();
    }
}

bool 
Sound::play (int32_t iter) 
{
    bool retVal = true;

    SDL_LockAudio ();
    if (-1 != chanNum && this == beingPlayed[chanNum]) { 
	retVal = false;
    } else {
	chanNum = Mix_PlayChannel (-1, sound, iter);
	if (-1 == chanNum) { 
	    retVal = false;
	} else {
	    beingPlayed[chanNum] = this; 
	}
    }
    SDL_UnlockAudio ();
    //
    // Note that callback may change chanNum here, thus we
    // record the return value inside the critical section.
    //
    return retVal;
}

bool 
Sound::stop () 
{
    SDL_LockAudio ();
    if (-1 == chanNum || this != beingPlayed[chanNum]) {
	SDL_UnlockAudio ();
	return false;
    }
    Mix_HaltChannel (chanNum);
    beingPlayed[chanNum] = NULL;
    chanNum = -1;
    SDL_UnlockAudio ();
    return true;
}

void 
Sound::stopAll () 
{
    SDL_LockAudio ();
    (void)Mix_HaltChannel (-1);
    for (int32_t i = 0; channelLimit > i; i++) {
        if (NULL != beingPlayed[i]) {
	    beingPlayed[i]->chanNum = -1;
	    beingPlayed[i] = NULL;
	}
    }
    SDL_UnlockAudio ();
}

#else // defined(WALY_TARGET_ANDROID)

SLObjectItf Sound::engine = NULL; // engine object
SLEngineItf Sound::engineItf;     // engine creation interface
SLObjectItf Sound::mixer;         // output mixer (sink for all sounds)

void
Sound::playEnded (SLPlayItf ignored, void* soundPtr, SLuint32 evt)
{
    Sound* s = (Sound*)soundPtr;

    //
    // This callback is NOT MADE if the sound is looping infinitely,
    // even if the callback is registered.
    //
    // Maybe we'll just check it anyway, though?
    //

    //
    // WARNING: This kind of looping will be rough.  OpenSLES supports
    // infinite looping (but not finite).  Should use that instead,
    // at least for infinite looping...
    //
    (void)(*s->playItf)->SetPlayState (s->playItf, SL_PLAYSTATE_STOPPED);
    if (-1 == s->chanNum || s != beingPlayed[s->chanNum]) {
        s->chanNum = -1;
	// no associated channel...something odd
	return;
    }
    if (-1 == s->count || 0 < s->count--) {
        // play again using the same channel
	(void)(*s->playItf)->SetPlayState (s->playItf, SL_PLAYSTATE_PLAYING);
	return;
    }
    beingPlayed[s->chanNum] = NULL;
    s->chanNum = -1;
}

void 
Sound::init () 
{
    if (!doQuit || 0 != soundCount ||
	SL_RESULT_SUCCESS != slCreateEngine (&engine, 0, NULL, 0, NULL, NULL)) {
        throw std::exception ();
    }
    if (SL_RESULT_SUCCESS != (*engine)->Realize (engine, SL_BOOLEAN_FALSE) ||
        SL_RESULT_SUCCESS != (*engine)->GetInterface
		(engine, SL_IID_ENGINE, &engineItf) ||
	SL_RESULT_SUCCESS != (*engineItf)->CreateOutputMix
		(engineItf, &mixer, 0, NULL, NULL)) {
        (*engine)->Destroy (engine);
	engine = NULL;
        throw std::exception ();
    }
    if (SL_RESULT_SUCCESS != (*mixer)->Realize (mixer, SL_BOOLEAN_FALSE)) {
	(*mixer)->Destroy (mixer);
        (*engine)->Destroy (engine);
	engine = NULL;
        throw std::exception ();
    }
    doQuit = false;
}

void 
Sound::quit () 
{
    if (doQuit) {
        throw std::exception ();
    }
    doQuit = true;
    if (0 == soundCount) {
	(*mixer)->Destroy (mixer);
	(*engine)->Destroy (engine);
	engine = NULL;
    }
}

Sound::Sound (const char* fileName) : chanNum (-1) 
{
    char fName[1024];

    sprintf (fName, "%s%.800s", WALY_SOUND_DIR, fileName);
    AAsset* asset = AAssetManager_open 
    		(Platform::getApp ()->activity->assetManager, fName, 
		 AASSET_MODE_STREAMING);

    //
    // Android provides an SLES extension to use an fd as a source, and
    // we can open an asset as an fd, but ... it's unclear that the system
    // won't leave it open while we use the sound and then only close it
    // later...  be nice to check... supposedly has trace/strace somewhere,
    // but I don't see it...
    //
    
    //
    // SLES specs don't seem to say whether we need to keep memory fixed
    // or not after we use as a source...  not at all clear how we get the
    // handle back to free it later, but that's easy enough...
    //

    if (NULL == asset) {
	__android_log_print (ANDROID_LOG_INFO, "asset", "open %s failed",
			     fileName);
	throw std::exception ();
    }

    off_t os, ol;
    int ofd = AAsset_openFileDescriptor (asset, &os, &ol);
    // sample code (native-audio) handles this way... I guess it's safe?
    AAsset_close (asset);
    if (0 > ofd) {
	throw std::exception ();
    }

    // unspecified type is ok, since file formats we care about
    // have unique format ids at the start
    SLDataLocator_AndroidFD srcloc = {SL_DATALOCATOR_ANDROIDFD, ofd, os, ol};
    SLDataFormat_MIME srcfmt = {SL_DATAFORMAT_MIME, NULL, 
    			        SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource src = {&srcloc, &srcfmt};

    SLDataLocator_OutputMix snkloc = {SL_DATALOCATOR_OUTPUTMIX, mixer};
    SLDataSink snk = {&snkloc, NULL};

    // need seek interface used for infinite looping
    // (could still support it, but sounds bad)
    const SLInterfaceID itfID[1] = {SL_IID_SEEK};
    const SLboolean reqd[1] = {SL_BOOLEAN_TRUE};

    if (SL_RESULT_SUCCESS != (*engineItf)->CreateAudioPlayer 
    	(engineItf, &player, &src, &snk, 1, itfID, reqd)) {
	throw std::exception ();
    }
    if (SL_RESULT_SUCCESS != 
		(*player)->Realize (player, SL_BOOLEAN_FALSE) ||
	SL_RESULT_SUCCESS != 
		(*player)->GetInterface (player, SL_IID_PLAY, &playItf) ||
	SL_RESULT_SUCCESS !=
		(*playItf)->RegisterCallback (playItf, playEnded, this) ||
	SL_RESULT_SUCCESS != (*playItf)->SetCallbackEventsMask
		(playItf, SL_PLAYEVENT_HEADATEND) ||
	SL_RESULT_SUCCESS != 
		(*player)->GetInterface (player, SL_IID_SEEK, &seekItf)) {
        (*player)->Destroy (player);
	throw std::exception ();
    }
    soundCount++;
}

Sound::~Sound () 
{
    (void)(*playItf)->SetPlayState (playItf, SL_PLAYSTATE_STOPPED);
    (*player)->Destroy (player);
    if (0 == --soundCount && doQuit) {
	(*mixer)->Destroy (mixer);
	(*engine)->Destroy (engine);
	engine = NULL;
    }
}

bool 
Sound::play (int32_t iter) 
{
    // make sure that sound is stopped
    (void)(*playItf)->SetPlayState (playItf, SL_PLAYSTATE_STOPPED);
    if (-1 != chanNum && this == beingPlayed[chanNum]) {
        // reuse the channel we have--we have stopped the sound already,
	// so callback should not fire
	count = iter;
    } else {
	// find a free channel
	for (int32_t i = 0; ; i++) {
	    if (channelLimit == i) {
	        return false;
	    }
	    if (NULL == beingPlayed[i]) {
		beingPlayed[i] = this;
		chanNum = i;
		count = iter;
		break;
	    }
	}
    }
    if (-1 == iter) {
	(void)(*seekItf)->SetLoop
		(seekItf, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
    } else {
	//
	// You might wonder why we need to 'valid' pass position markers
	// when turning looping OFF.  The reason is that the Android
	// OpenSLES implementation checks the position values 
	// ***regardless of the state*** and spits out a whiny error 
	// message to the log if you, say, pass a 0 as the end marker.  
	// So...right.
	//
	// (Let's avoid making their 'debugging tools' more painful.)
	//
	(void)(*seekItf)->SetLoop (seekItf, SL_BOOLEAN_FALSE, 0,
				   SL_TIME_UNKNOWN);
    }
    (void)(*playItf)->SetPlayState (playItf, SL_PLAYSTATE_PLAYING);
    return true;
}

bool 
Sound::stop () 
{
    (void)(*playItf)->SetPlayState (playItf, SL_PLAYSTATE_STOPPED);
    if (-1 != chanNum && this == beingPlayed[chanNum]) {
        beingPlayed[chanNum] = NULL;
	chanNum = -1;
	return true;
    }
    return false;
}

void 
Sound::stopAll () 
{
    for (int32_t i = 0; channelLimit > i; i++) {
	Sound* s = beingPlayed[i];

	if (NULL != s) {
	    (void)(*s->playItf)->SetPlayState 
		    (s->playItf, SL_PLAYSTATE_STOPPED);
	    beingPlayed[i] = NULL;
	    s->chanNum = -1;
	}
    }
}

#endif // defined(WALY_TARGET_*)

}

