//									tab:8
//
// WALYWidget.cpp - source file for the WALY library widget class
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
// Version:	    1.01
// Creation Date:   27 July 2011
// Filename:	    WALYWidget.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	28 December 2012
//		Added text focus concept.
//	SL	1.02	8 February 2013
//		Fixed bug with use of empty text.
//

#include "WALY.h"

using namespace std;

namespace WALY_1_0_0 {
    
Widget* Widget::textFocus = NULL;

void 
Widget::prepareToRedraw () 
{
    Rect pad;
    Rect src;
    int32_t drawWidth;

    if (NULL != surface) { return; }

    Surface* writing;
    if (0 < text.length ()) {
	writing = font->renderText (text.c_str (), fgColor);
	// may also fail...then writing just won't show up...
	drawWidth = (0 < width ? width : writing->w);
    } else {
        writing = NULL;
	drawWidth = width;
    }
    uint32_t height = font->lineSkip ();
    pad.x = xPadding;
    pad.y = yPadding;
    surface = SDL_CreateRGBSurface 
		(0, drawWidth + pad.x * 2, height + pad.y * 2, 32, 
		 0xFF0000, 0xFF00, 0xFF, 0xFF000000);
    if (NULL == surface) {
    	useImage = false;
	if (NULL != writing) {
	    SDL_FreeSurface (writing);
	}
	return;
    }
    useImage = true;
    if (opaqueBackground) {
	SDL_FillRect (surface, NULL, bgColor);
    } else if (NULL != writing) {
        // need to make sure the writing shows up...
	SDL_SetAlpha (writing, 0, 0);
    }
    if (NULL != writing) {
	src.w = (drawWidth >= writing->w ? writing->w : drawWidth);
	switch (align) {
	    case TEXT_ENTRY:
		startOfText = pad.x + src.w - writing->w;
		lastDrawOffset = src.x = writing->w - src.w;
		break;
	    case TEXT_LEFT:
		startOfText = pad.x;
		lastDrawOffset = src.x = 0;
		break;
	    case TEXT_CENTER:
		startOfText = pad.x + (drawWidth - writing->w) / 2;
		lastDrawOffset = (writing->w - drawWidth) / 2;
		if (drawWidth > writing->w) {
		    src.x = 0;
		    pad.x -= lastDrawOffset;
		} else {
		    src.x = lastDrawOffset;
		}
		break;
	    case TEXT_RIGHT:
		startOfText = pad.x + (drawWidth - writing->w);
		lastDrawOffset = writing->w - drawWidth;
		if (drawWidth > writing->w) {
		    src.x = 0;
		    pad.x -= lastDrawOffset;
		} else {
		    src.x = lastDrawOffset;
		}
		break;
	}
	src.y = 0;
	src.h = height;
	if (enabled && highlighted && haveFocus ()) {
	    pad.w = src.w;
	    pad.h = height;
	    SDL_FillRect (surface, &pad, highColor);
	}
	SDL_BlitSurface (writing, &src, surface, &pad);
	SDL_FreeSurface (writing);
    } else {
	startOfText = pad.x;
	highlighted = false;
	src.w = 0;
	lastDrawOffset = 0;
    }

    if (!enabled) {
        Filter::greyOut (surface);
    } else {
    	switch (filter) {
	    case FILTER_RAISED:
	    	Filter::drawRaised (surface, border);
		break;
	    case FILTER_LOWERED:
	    	Filter::drawLowered (surface, border);
		break;
	    case FILTER_SUNKEN:
	    	Filter::drawDarkBorder (surface, border);
		break;
	    case FILTER_NONE:
	    default:
	        break;
	}
    }
}

void
Widget::makeVisibleChange ()
{
    if (NULL != surface) {
	SDL_FreeSurface (surface);
	surface = NULL;
    }
    // need to mark exposure here: even though in theory nothing
    // should be exposed by things like changing the color of the text,
    // in practice we find some anti-aliasing effects around edges
    // (for example) that require redrawing the background pixels...
    markExposure ();
    markForRedraw ();
}

void
Widget::setEnabled (bool newVal)
{
    enabled = newVal;
    makeVisibleChange ();
}

void 
Widget::setText (const char* newText) 
{
    text = string (newText);
    makeVisibleChange ();
}

void 
Widget::setFgColor (uint32_t newColor)
{
    fgColor = (0xFF000000 | newColor);
    makeVisibleChange ();
}

void Widget::construct (uint32_t pixWidth, uint32_t padX, uint32_t padY,
			const char* txt, text_align_t aln, filter_type_t ftr,
			uint32_t bdr, uint32_t fg, uint32_t highCol, Font* fnt)
{
    if (NULL != fnt) {
        font = fnt;
    } else {
        font = Font::getDefaultFont ();
    }
    width = pixWidth;
    xPadding = padX;
    yPadding = padY;
    align = aln;

    filter = ftr;
    border = bdr;
    fgColor = (0xFF000000 | fg);
    highColor = (0xFF000000 | highCol);
    useImage = true;

    enabled = true;
    highlighted = false;

    lastDrawOffset = 0;
    startOfText = 0;

    setText (txt);
}

Widget::Widget (Frame* par, int32_t xCoord, int32_t yCoord, uint32_t pixWidth,
		uint32_t padX, uint32_t padY, const char* txt, 
		text_align_t aln, filter_type_t ftr, uint32_t bdr, uint32_t fg,
		Font* fnt) : Frame (par, xCoord, yCoord)
{
    construct (pixWidth, padX, padY, txt, aln, ftr, bdr, fg, 0xFFFF00, fnt);
}

Widget::Widget (Frame* par, int32_t xCoord, int32_t yCoord, uint32_t pixWidth,
		uint32_t padX, uint32_t padY, const char* txt,
		text_align_t aln, filter_type_t ftr, uint32_t bdr, uint32_t fg,
		uint32_t bg, Font* fnt) : Frame (par, xCoord, yCoord)
{
    useSolidBackground (bg);
    construct (pixWidth, padX, padY, txt, aln, ftr, bdr, fg, 0xFFFF00, fnt);
}

Widget::Widget (Frame* par, int32_t xCoord, int32_t yCoord, uint32_t pixWidth,
		uint32_t padX, uint32_t padY, const char* txt,
		text_align_t aln, filter_type_t ftr, uint32_t bdr, uint32_t fg,
		uint32_t bg, uint32_t highCol, Font* fnt) :
		Frame (par, xCoord, yCoord)
{
    useSolidBackground (bg);
    construct (pixWidth, padX, padY, txt, aln, ftr, bdr, fg, highCol, fnt);
}

}

