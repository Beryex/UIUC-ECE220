//									tab:8
//
// WALYmain.cpp - source file for WALY main entry point abstraction
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
// Version:	    1.01
// Creation Date:   23 December 2012
// Filename:	    WALYmain.cpp
// History:
//	SL	1.00	23 December 2012
//		First written.
//	SL	1.01	25 December 2012
//		Cleaned up a little.
//	SL	1.02	6 January 2013
//		More cleaning.
//

#include "WALYmain.h"

#include <unistd.h>

using namespace std;
using namespace WALY;

//
// main entry point...
//

#if defined(WALY_TARGET_WEBOS) || defined(WALY_TARGET_CYGWIN)
int main()
{
    return WALY_main();
}
#else // WALY_TARGET_ANDROID

#include <android_native_app_glue.h>
#include <android/log.h>

void android_main(struct android_app *app)
{
    // need to store a pointer to app in the Screen, then call WALY_main
    Platform::setAppPointer(app);
    (void)WALY_main();

    //
    // there's a nice visual explanation (at time of writing!) of the
    //    events that Android sends to apps at
    //    developer.android.com/reference/android/app/Activity.html
    //
    // The only event that guarantees that the app is ending is DESTROY,
    //    and DESTROY is not necessarily even delivered (can be killed
    //    without the call), so there's never a good time to actually
    //    shut down gracefully in Android.
    //
    // Thus, I don't actually send SDL_QUIT (and WALY_main never returns).
    //
    // So long as we save state on PAUSE and re-load state when the app
    //    starts, we can just let Android clobber the app if it wants...
    //    (and go to sleep on PAUSE, of course).
    //

    // finish SDL_QUIT
    WALYFinishEvent();

    Platform::detach();

    //
    // Who knows what those people were thinking?
    //
    // What they (Android developers) seem to have done is this:
    //
    // android_main returns, and the program is ... sort of dead
    //
    // but it's not really dead...it's just hibernating...
    //
    // static destructors DO NOT GET CALLED
    //
    // so you either (A) have to make sure that your program is
    // re-entrant to main (CONSTRUCTORS won't get called again
    // either...) or (B) do the following
    exit(3);
}

#endif // defined(WALY_TARGET_*)
