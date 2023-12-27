//									tab:8
//
// WALYWidget.h - header file for the WALY library widget class
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
// Version:	    1.01
// Creation Date:   27 July 2011
// Filename:	    WALYWidget.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	28 December 2012
//		Added text focus concept.
//

#if !defined(__WALY_WIDGET_H)
#define __WALY_WIDGET_H

#include <string>

#include "WALY.h"

namespace WALY_1_0_0 {
    
class Widget : public Frame {
    public:
	typedef enum {
	    TEXT_ENTRY,
	    TEXT_LEFT,
	    TEXT_CENTER,
	    TEXT_RIGHT
	} text_align_t;

    protected:

	//
	// If non-NULL, this widget has the keyboard focus.  WALY widgets
	// such as entry boxes are designed to have one focus, and to only
	// have one cursor showing (for example).  If you build other
	// widgets, behavior is up to you...
	//
	static Widget* textFocus;
	// called when another widget wants the focus
	bool haveFocus () { return (this == textFocus); }
	virtual void loseFocus () { this->makeVisibleChange (); }
	virtual void claimFocus () {
	    Widget* prevFocus = textFocus;
	    textFocus = this;
	    if (NULL != prevFocus) {
	        prevFocus->loseFocus ();
	    }
	    this->makeVisibleChange ();
	    Platform::showKeyboard ();
	}
	virtual void giveUpFocus () {
	    if (haveFocus ()) {
		textFocus = NULL;
	        loseFocus ();
	    }
	    Platform::hideKeyboard ();
	}

	typedef enum {
	    FILTER_NONE,
	    FILTER_RAISED,
	    FILTER_LOWERED,
	    FILTER_SUNKEN
	} filter_type_t;

	std::string   text;  
	Font*         font;

	uint32_t      width;
	uint32_t      xPadding;
	uint32_t      yPadding;
	text_align_t  align;

	filter_type_t filter;
	uint32_t      border;
	uint32_t      fgColor;
	uint32_t      highColor;

	bool          enabled;
	bool	      highlighted;

	int32_t       startOfText;
	int32_t       lastDrawOffset;

	void prepareToRedraw ();
	void makeVisibleChange ();

	void construct (uint32_t width, uint32_t padX, uint32_t padY,
			const char* txt, text_align_t aln, filter_type_t ftr,
			uint32_t bdr, uint32_t fg, uint32_t highCol,
			Font* fnt);

	void setDepressed (bool newVal);

    public:
        Widget (Frame* par, int32_t xCoord, int32_t yCoord, uint32_t pixWidth,
		uint32_t padX, uint32_t padY, const char* txt,
		text_align_t aln, filter_type_t ftr, uint32_t bdr, uint32_t fg, 
		Font* fnt = NULL);
        Widget (Frame* par, int32_t xCoord, int32_t yCoord, uint32_t pixWidth,
		uint32_t padX, uint32_t padY, const char* txt, 
		text_align_t aln, filter_type_t ftr, uint32_t bdr, uint32_t fg,
		uint32_t bg, Font* fnt = NULL);
        Widget (Frame* par, int32_t xCoord, int32_t yCoord, uint32_t pixWidth,
		uint32_t padX, uint32_t padY, const char* txt, 
		text_align_t aln, filter_type_t ftr, uint32_t bdr, uint32_t fg,
		uint32_t bg, uint32_t highCol, Font* fnt = NULL);

	~Widget () { 
	    giveUpFocus ();
	    if (NULL != surface) {
	        SDL_FreeSurface (surface); 
	    }
	}
	
	bool addChild (Frame* f) { return false; }
	bool isEnabled () { return enabled; }
	void setEnabled (bool newVal);
	virtual void setText (const char* newText);
	void setFgColor (uint32_t newVal);
	const char* getText () { return text.c_str (); }
	void setWidth (int32_t newVal = 0) {
	    width = newVal;
	    makeVisibleChange ();
	}
	void setTextAlign (text_align_t newVal) {
	    align = newVal;
	    makeVisibleChange ();
	}
};

}

#endif /* __WALY_WIDGET_H */
