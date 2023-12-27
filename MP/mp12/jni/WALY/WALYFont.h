//									tab:8
//
// WALYFont.h - header file for the WALY library font class
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
// Filename:	    WALYFont.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	25 July 2012
//		Added counting to control destruction order and TTF_Quit.
//	SL	1.02	26 December 2012
//		Revised to support Android and Cygwin.
//	SL	1.03	31 December 2012
//		Added platform-independent font names.
//

#if !defined(__WALY_FONT_H)
#define __WALY_FONT_H

#include "WALY.h"

#include <exception>

//
// An attempt at approximating cross-platform fonts.  Please use these
// instead of names if possible.
//
#if defined(WALY_TARGET_WEBOS)
#define WALY_FONT_MONO_BOLD         "courbd"
#define WALY_FONT_MONO_BOLD_ITALIC  "courbi"
#define WALY_FONT_MONO_ITALIC       "couri"
#define WALY_FONT_MONO_REGULAR      "cour"
#define WALY_FONT_SANS_BOLD         "Prelude-Bold"
#define WALY_FONT_SANS_BOLD_ITALIC  "Prelude-BoldOblique"
#define WALY_FONT_SANS_ITALIC       "Prelude-MediumOblique"
#define WALY_FONT_SANS_REGULAR      "Prelude-Medium"
#define WALY_FONT_SERIF_BOLD        "timesbd"
#define WALY_FONT_SERIF_BOLD_ITALIC "timesbi"
#define WALY_FONT_SERIF_ITALIC      "timesi"
#define WALY_FONT_SERIF_REGULAR     "times"
#define WALY_FONT_DEFAULT	    "arial"
#elif defined(WALY_TARGET_CYGWIN) || defined(WALY_TARGET_ANDROID)
// we only have one scaled font on Android, so string doesn't matter...
#define WALY_FONT_MONO_BOLD         "LiberationMono-Bold"
#define WALY_FONT_MONO_BOLD_ITALIC  "LiberationMono-BoldItalic"
#define WALY_FONT_MONO_ITALIC       "LiberationMono-Italic"
#define WALY_FONT_MONO_REGULAR      "LiberationMono-Regular"
#define WALY_FONT_SANS_BOLD         "LiberationSans-Bold"
#define WALY_FONT_SANS_BOLD_ITALIC  "LiberationSans-BoldItalic"
#define WALY_FONT_SANS_ITALIC       "LiberationSans-Italic"
#define WALY_FONT_SANS_REGULAR      "LiberationSans-Regular"
#define WALY_FONT_SERIF_BOLD        "LiberationSerif-Bold"
#define WALY_FONT_SERIF_BOLD_ITALIC "LiberationSerif-BoldItalic"
#define WALY_FONT_SERIF_ITALIC      "LiberationSerif-Italic"
#define WALY_FONT_SERIF_REGULAR     "LiberationSerif-Regular"
#define WALY_FONT_DEFAULT	    "LiberationSans-Regular"
#endif // defined(WALY_TARGET_*)

namespace WALY_1_0_0 {
    
class Font {
    private:
        TTF_Font* font;

	//
	// Why bother to count fonts?  While it's easy enough to ensure
	// that we call TTF_Init before we try to open any fonts, it's
	// a pain to try to order destructors when we quit.  On some
	// platforms, the program is just killed, so it doesn't matter.
	// On others, we need to make sure that the fonts are closed
	// before we call TTF_Quit to avoid the program 'crashing' when
	// a user tries to quit.
	//
	static Font* defaultFont;
	static int32_t fontCount;
	static bool doQuit;

	static void init () {
	    if (!doQuit || 0 != fontCount || 0 != TTF_Init ()) {
		throw std::exception ();
	    }
	    defaultFont = new Font (WALY_FONT_DEFAULT, 20);
	    doQuit = false;
	}
	static void quit () {
	    delete defaultFont;
	    if (doQuit) {
	        throw std::exception ();
	    }
	    doQuit = true;
	    if (0 == fontCount) {
		TTF_Quit ();
	    }
	}
	friend class Screen;  // Screen code calls init/quit

    public:
	static Font* getDefaultFont () { return defaultFont; }

	Font (const char* fontName, int32_t ptSize) {
	    char buf[256];

	    sprintf (buf, "%s%.80s.ttf", WALY_FONT_DIR, fontName);
	    font = TTF_OpenFont (buf, ptSize);
	    if (NULL == font) {
		throw std::exception ();
	    }
	    fontCount++;
	}
	~Font () { 
	    TTF_CloseFont (font); 
	    if (0 == --fontCount && doQuit) {
	        TTF_Quit ();
	    }
	}

	void getSize (const char* txt, int32_t* wPtr, int* hPtr) {
	    TTF_SizeText (font, txt, wPtr, hPtr);
	}

	Surface* renderText (const char* txt, uint32_t RGB) {
	    SDL_Color c;

	    c.r = ((RGB & 0xFF0000) >> 16);
	    c.g = ((RGB & 0xFF00) >> 8);
	    c.b = (RGB & 0xFF);
	    return TTF_RenderText_Blended (font, txt, c);
	}

	uint32_t lineSkip () { return TTF_FontLineSkip (font); }
};

}

#endif /* __WALY_FONT_H */

