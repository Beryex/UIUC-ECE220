//									tab:8
//
// WALYSDLStubs.h - SDL stub functions to support WALY library on Android
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
// Version:	    1.03
// Creation Date:   24 December 2012
// Filename:	    WALYSDLStubs.h
// History:
//	SL	1.00	24 December 2012
//		First written.
//	SL	1.01	1 January 2013
//		Added extra key defines.
//	SL	1.02	5 January 2013
//		Added SDL_USEREVENT for defining network events.
//	SL	1.03	3 February 2013
//		Changed SDLK case numberings to avoid duplicate case errors.
//


#if !defined(WALY_SDL_STUBS_H)
#define WALY_SDL_STUBS_H


#if !defined(WALY_TARGET_ANDROID)
#error "WALYSDLStubs.h is only meant for targetting Android."
#endif //!defined(WALY_TARGET_ANDROID)

#include <stdint.h>

#include <android_native_app_glue.h>
#include <android/input.h>

//
// SDL stub support for Android
//
// effectively another form of platform-specific support...
//


// WARNING: ANDROID USES left,top,right,bottom for RECT--MUST CONVERT
typedef struct SDL_Rect SDL_Rect;
struct SDL_Rect {
    int16_t  x; // (x,y) of upper left corner
    int16_t  y;
    uint16_t w; // width
    uint16_t h; // height
};

typedef struct SDL_Color SDL_Color;
struct SDL_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t unused;
};

//
// going to leave this blank until I know what's needed; will need to
// translate anything anyway...
//
typedef struct wss_pixel_format_t wss_pixel_format_t;
struct wss_pixel_format_t {
    uint32_t Amask;
    uint8_t  alpha;
};
typedef struct SDL_Surface SDL_Surface;
struct SDL_Surface {
    // size of surface
    int w, h;

    // window has no alpha channel (we won't use anyway), but images do...
    wss_pixel_format_t* format; // need real type?
    // allocate a format for each surface...
    wss_pixel_format_t sfmt;

    //
    // pointer to pixel data--dynamically allocated with malloc
    //
    void* pixels;

    // number of bytes per surface row
    uint16_t pitch;

    // clipping rectangle
    SDL_Rect clipRect;
};


//
// going to leave this blank until I know what's needed; will need to
// translate anything anyway...
//
typedef enum { SDL_FALSE, SDL_TRUE } SDL_bool;
enum {
    SDL_APPMOUSEFOCUS = 1,
    SDL_APPINPUTFOCUS = 2,
    SDL_APPACTIVE     = 4
};

enum {
    // only defining what WALY uses... 
    // WARNING: NOT consistent with actual SDL definitions!
    SDL_ACTIVEEVENT,
    SDL_KEYDOWN,
    SDL_KEYUP,
    SDL_MOUSEMOTION,
    SDL_MOUSEBUTTONDOWN,
    SDL_MOUSEBUTTONUP,
    SDL_QUIT,
    SDL_VIDEOEXPOSE,

    SDL_USEREVENT  // WALY uses to define network events
                   // SDL1.2 header implicity guarantees 8 types, and
		   // WALY uses 2 (see WALYPlatform.h)
};

typedef struct wss_active_evt_t wss_active_evt_t;
struct wss_active_evt_t {
    uint8_t type;
    uint8_t gain;
    uint8_t state;
};

typedef enum {
    KMOD_NONE   = 0, // AMETA_NONE,  must be 0, even if Android goes crazy
    KMOD_LSHIFT = AMETA_SHIFT_LEFT_ON,
    KMOD_RSHIFT = AMETA_SHIFT_RIGHT_ON,
    KMOD_LCTRL  = 0, // not supported
    KMOD_RCTRL  = 0, // not supported
    KMOD_LALT   = AMETA_ALT_LEFT_ON,
    KMOD_RALT   = AMETA_ALT_RIGHT_ON,
    KMOD_LMETA  = AMETA_SYM_ON,
    KMOD_RMETA  = AMETA_SYM_ON,
    KMOD_NUM    = 0, // not supported
    KMOD_CAPS   = 0, // not supported
    KMOD_MODE   = 0  // not supported
} SDLMod;
#define KMOD_CTRL  0
#define KMOD_SHIFT (KMOD_LSHIFT|KMOD_RSHIFT|AMETA_SHIFT_ON)
#define KMOD_ALT   (KMOD_LALT|KMOD_RALT|AMETA_ALT_ON)
#define KMOD_META  (KMOD_LMETA|KMOD_RMETA)

// keys people might want to see by name...
//  I'm just going to name the ones I've used and a few others for now...
typedef enum {
    SDLK_UNKNOWN   = 0,	// map lots of Android keys here...
    SDLK_BACKSPACE = 8,
    SDLK_TAB       = 9, 
    SDLK_RETURN    = 13,     
    SDLK_ESCAPE    = 27, // no such thing on Android...
    SDLK_SPACE     = 32,

    // common for keyboard controls...  all unknown on Android
    SDLK_LEFT  = 276,
    SDLK_RIGHT = 275,
    SDLK_UP    = 273,
    SDLK_DOWN  = 274,

    SDLK_KP0   = 256,
    SDLK_KP1   = 257,
    SDLK_KP2   = 258,
    SDLK_KP3   = 259,
    SDLK_KP4   = 260,
    SDLK_KP5   = 261,
    SDLK_KP6   = 262,
    SDLK_KP7   = 263,
    SDLK_KP8   = 264,
    SDLK_KP9   = 265,

    SDLK_KP_DIVIDE   = 267,
    SDLK_KP_ENTER    = 271,
    SDLK_KP_EQUALS   = 272,
    SDLK_KP_MINUS    = 269,
    SDLK_KP_MULTIPLY = 268,
    SDLK_KP_PERIOD   = 266,
    SDLK_KP_PLUS     = 270,

    SDLK_HOME     = 278,
    SDLK_END      = 279,
    SDLK_INSERT   = 277,
    SDLK_DELETE   = 127,
    SDLK_PAGEUP   = 280,
    SDLK_PAGEDOWN = 281,

    // these are for the Palm gestures
    PDLK_GESTURE_BACK = 0x1B,	// ASCII ESC for Cygwin equivalence
    PDLK_GESTURE_FORWARD
} SDLKey;

typedef struct wss_key_evt_t wss_key_evt_t;
struct wss_key_evt_t {
    uint8_t type;
    struct {
        SDLKey sym; // key
	SDLMod mod; // modifiers
    } keysym;
};

typedef struct wss_mouse_evt_t wss_mouse_evt_t;
struct wss_mouse_evt_t {
    uint8_t type;
    uint8_t which;
    uint16_t x;
    uint16_t y;
};

typedef struct wss_motion_evt_t wss_motion_evt_t;
struct wss_motion_evt_t {
    uint8_t type;
    uint8_t which;
    uint16_t x;
    uint16_t y;
};

typedef struct wss_user_evt_t wss_user_evt_t;
struct wss_user_evt_t {
    uint8_t type;
    int32_t code;
    void* data1;
    void* data2;
};

typedef union SDL_Event SDL_Event;
union SDL_Event {
    uint8_t          type;
    wss_active_evt_t active;
    wss_key_evt_t    key;
    wss_mouse_evt_t  button;
    wss_motion_evt_t motion;
    wss_user_evt_t   user;
};

typedef uint32_t TTF_Font; 

// max number of pointers for Android...  may be quite large, but...
// really, can you do gestures with your nose (AND all fingers)?
#define SDL_MAXMOUSE 10

// defined in WALYPlatform.cpp (if not static here)
extern "C" {
//int SDL_PollEvent (SDL_Event* evt);
//int SDL_WaitEvent (SDL_Event* evt);
uint32_t SDL_GetTicks (void);
void SDL_GetClipRect (SDL_Surface* s, SDL_Rect* r);
SDL_bool SDL_SetClipRect (SDL_Surface* s, const SDL_Rect* r);
int SDL_FillRect (SDL_Surface* s, SDL_Rect* r, uint32_t color);
void SDL_UpdateRect (SDL_Surface* s, int32_t x, int32_t y,
		     uint32_t w, uint32_t h);
void SDL_BlitSurface (SDL_Surface* src, SDL_Rect* srcrect,
		      SDL_Surface* dst, SDL_Rect* dstrect);
void SDL_FreeSurface (SDL_Surface* s);
// used for building up widgets...  NOT well-supported!
SDL_Surface* SDL_CreateRGBSurface
	(uint32_t flags, int width, int height, int depth, uint32_t Rmask, 
	 uint32_t Gmask, uint32_t Bmask, uint32_t Amask);
int SDL_SetAlpha (SDL_Surface* s, uint32_t flag, uint8_t alpha);

// not in SDL, but necessary to finish Android events!
// (made an empty stub for real SDL platforms)
void WALYFinishEvent (void);

// locking/unlocking surfaces...
SDL_bool SDL_MUSTLOCK (SDL_Surface* s);
int SDL_LockSurface (SDL_Surface* s);
void SDL_UnlockSurface (SDL_Surface* s);

// TTF calls (true type fonts) ... we simply scale one 8x16 font
static int TTF_Init () { return 0; }
static void TTF_Quit () { } // does not track open fonts!
static TTF_Font* TTF_OpenFont (const char* file, int ptsize) { 
    // no code to scale down...
    if (16 > ptsize) { ptsize = 16; }
    return new TTF_Font (ptsize);
}
static void TTF_CloseFont (TTF_Font* font) { delete font; }
int TTF_SizeText (TTF_Font* font, const char* text, int* w, int* h);
SDL_Surface* TTF_RenderText_Blended (TTF_Font* font, const char* text,
				     SDL_Color fg);
int TTF_FontLineSkip (const TTF_Font* font);
}

#endif // defined(WALY_SDL_STUBS_H)
