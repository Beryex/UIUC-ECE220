//									tab:8
//
// WALYImage.cpp - source file for the WALY library image class
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
// Filename:	    WALYImage.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//

#include "WALY.h"

using namespace std;

namespace WALY_1_0_0 {
    
void 
Image::setAlpha (uint8_t alpha)
{
    int32_t   j;
    uint32_t* pix;
    int32_t   i;

    if (SDL_MUSTLOCK (imageData)) { SDL_LockSurface (imageData); }
    if (imageData->format->Amask == 0) {
        imageData->format->alpha = alpha;
    } else {
	for (j = 0; imageData->h > j; j++) {
	    pix = (uint32_t*)(((uint8_t*)imageData->pixels) + 
	    	  imageData->pitch * j);
	    for (i = 0; imageData->w > i; i++) {
// ENDIANNESS
		pix[i] = (alpha << 24) | (pix[i] & 0xFFFFFF);
	    }
	}
    }
    if (SDL_MUSTLOCK (imageData)) { SDL_UnlockSurface (imageData); }
}

}
