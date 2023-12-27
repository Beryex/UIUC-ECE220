//									tab:8
//
// WALYFilter.cpp - source file for the WALY library image filter class
//
// "Copyright (c) 2011 by Steven S. Lumetta."
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
// Version:	    1.00
// Creation Date:   27 July 2011
// Filename:	    WALYFilter.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//

#include "WALY.h"

using namespace std;

namespace WALY_1_0_0 {
    
uint32_t 
Filter::colorDarken (uint32_t ARGB, uint8_t amt)
{
    uint8_t r, g, b;

    r = ((ARGB & 0xFF0000) >> 16);
    g = ((ARGB & 0xFF00) >> 8);
    b = (ARGB & 0xFF);
    r = (amt > r ? 0x00 : r - amt);
    g = (amt > g ? 0x00 : g - amt);
    b = (amt > b ? 0x00 : b - amt);

    return ((ARGB & 0xFF000000) | (r << 16) | (g << 8) | b);
}

uint32_t 
Filter::colorLighten (uint32_t ARGB, uint8_t amt)
{
    uint8_t r, g, b;

    r = ((ARGB & 0xFF0000) >> 16);
    g = ((ARGB & 0xFF00) >> 8);
    b = (ARGB & 0xFF);
    r = ((amt ^ 0xFF) <= r ? 0xFF : r + amt);
    g = ((amt ^ 0xFF) <= g ? 0xFF : g + amt);
    b = ((amt ^ 0xFF) <= b ? 0xFF : b + amt);

    return ((ARGB & 0xFF000000) | (r << 16) | (g << 8) | b);
}

void 
Filter::roundCorners (Surface* s, int32_t radius)
{
    int32_t   square;
    int32_t   j;
    uint32_t* pixHi;
    uint32_t* pixLo;
    int32_t   sqDist;
    int32_t   i;

    if (SDL_MUSTLOCK (s)) { SDL_LockSurface (s); }
    square = radius * radius;
    for (j = 0; radius > j && s->h > j; j++) {
	pixHi = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	pixLo = (uint32_t*)(((uint8_t*)s->pixels) + 
		s->pitch * (s->h - 1 - j));
	sqDist = square - (radius - j) * (radius - j);
	for (i = 0; radius > i && s->w > i; i++) {
	    if (sqDist < (radius - i) * (radius - i)) {
	        pixHi[i] = 0x00000000;
		pixHi[s->w - i - 1] = 0x00000000;
	        pixLo[i] = 0x00000000;
		pixLo[s->w - i - 1] = 0x00000000;
	    }
	}
    }
    if (SDL_MUSTLOCK (s)) { SDL_UnlockSurface (s); }
}

void 
Filter::greyOut (Surface* s)
{
    int32_t   j;
    uint32_t* pix;
    int32_t   i;

    if (SDL_MUSTLOCK (s)) { SDL_LockSurface (s); }
    for (j = 0; s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	for (i = 0; s->w > i; i++) {
	    pix[i] = (pix[i] & 0xFF000000) |
		    ((((pix[i] & 0xFF0000) + 3 * 0x800000) / 4) & 0xFF0000) |
		    ((((pix[i] & 0xFF00) + 3 * 0x8000) / 4) & 0xFF00) |
		    (((pix[i] & 0xFF) + 3 * 0x80) / 4);
	}
    }
    if (SDL_MUSTLOCK (s)) { SDL_UnlockSurface (s); }
}

void 
Filter::drawLowered (Surface* s, int32_t border)
{
    int32_t   j;
    uint32_t* pix;
    int32_t   i;
    uint32_t amt;

    if (SDL_MUSTLOCK (s)) { SDL_LockSurface (s); }
    for (j = 0; border > j && s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	amt = (0x20 * (border - j)) / border;
	for (i = 0; s->w > i; i++) {
	    pix[i] = colorDarken (pix[i], amt);
	}
    }
    //for (j = border; s->h - border > j; j++) {
    for (j = 0; s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	for (i = 0; border > i && s->w > i; i++) {
	    pix[i] = colorDarken (pix[i], (0x20 * (border - i)) / border);
	}
	for (i = (border <= s->w ? s->w - border : 0); s->w > i; i++) {
	    pix[i] = colorLighten 
		    (pix[i], (0x20 * (border - (s->w - 1 - i))) / border);
	}
    }
    for (j = (border <= s->h ? s->h - border : 0); s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	amt = (0x20 * (border - (s->h - 1 - j))) / border;
	for (i = 0; s->w > i; i++) {
	    pix[i] = colorLighten (pix[i], amt);
	}
    }
    if (SDL_MUSTLOCK (s)) { SDL_UnlockSurface (s); }
}

void 
Filter::drawRaised (Surface* s, int32_t border)
{
    int32_t   j;
    uint32_t* pix;
    int32_t   i;
    uint32_t amt;

    if (SDL_MUSTLOCK (s)) { SDL_LockSurface (s); }
    for (j = 0; border > j && s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	amt = (0x20 * (border - j)) / border;
	for (i = 0; s->w > i; i++) {
	    pix[i] = colorLighten (pix[i], amt);
	}
    }
    //for (j = border; s->h - border > j; j++) {
    for (j = 0; s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	for (i = 0; border > i && s->w > i; i++) {
	    pix[i] = colorLighten (pix[i], (0x20 * (border - i)) / border);
	}
	for (i = (border <= s->w ? s->w - border : 0); s->w > i; i++) {
	    pix[i] = colorDarken 
		    (pix[i], (0x20 * (border - (s->w - 1 - i))) / border);
	}
    }
    for (j = (border <= s->h ? s->h - border : 0); s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	amt = (0x20 * (border - (s->h - 1 - j))) / border;
	for (i = 0; s->w > i; i++) {
	    pix[i] = colorDarken (pix[i], amt);
	}
    }
    if (SDL_MUSTLOCK (s)) { SDL_UnlockSurface (s); }
}

void 
Filter::drawDarkBorder (Surface* s, int32_t border)
{
    int32_t   j;
    uint32_t* pix;
    int32_t   i;
    uint32_t amt;

    if (SDL_MUSTLOCK (s)) { SDL_LockSurface (s); }
    for (j = 0; border > j && s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	amt = (0x20 * (border - j)) / border;
	for (i = 0; s->w > i; i++) {
	    pix[i] = colorDarken (pix[i], amt);
	}
    }
    //for (j = border; s->h - border > j; j++) {
    for (j = 0; s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	for (i = 0; border > i && s->w > i; i++) {
	    pix[i] = colorDarken (pix[i], (0x20 * (border - i)) / border);
	}
	for (i = (border <= s->w ? s->w - border : 0); s->w > i; i++) {
	    pix[i] = colorDarken 
		    (pix[i], (0x20 * (border - (s->w - 1 - i))) / border);
	}
    }
    for (j = (border <= s->h ? s->h - border : 0); s->h > j; j++) {
	pix = (uint32_t*)(((uint8_t*)s->pixels) + s->pitch * j);
	amt = (0x20 * (border - (s->h - 1 - j))) / border;
	for (i = 0; s->w > i; i++) {
	    pix[i] = colorDarken (pix[i], amt);
	}
    }
    if (SDL_MUSTLOCK (s)) { SDL_UnlockSurface (s); }
}

}

