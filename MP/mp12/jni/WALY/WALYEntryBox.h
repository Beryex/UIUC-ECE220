//									tab:8
//
// WALYEntryBox.h - header file for the WALY library entry box widget
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
// Filename:	    WALYEntryBox.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	28 December 2012
//		Added text focus concept.
//

#if !defined(__WALY_ENTRY_BOX_H)
#define __WALY_ENTRY_BOX_H

#include <string>

#include "WALY.h"

namespace WALY_1_0_0 {
    
class EntryBox : public Widget {
    protected:
	int32_t     maxChars;
	int32_t     cursorPos;
	bool	    cursorOn;
	int32_t     cursorTicks;
	int32_t     cursorTickCnt;
	void        (*action) (EntryBox* eb);
	int32_t     lastMouseDown;

	void prepareToRedraw ();
	void construct ();

	static void mouseDown (Frame* f, const Event* e);
	static void blink (Frame* f, const Event* e);
	static void keystroke (Frame* f, const Event* e);

    public:
        EntryBox (Frame* par, int32_t xCoord, int32_t yCoord, int32_t pixWidth,
		  const char* txt, uint32_t fg, uint32_t bg, Font* fnt = NULL);
        EntryBox (Frame* par, int32_t xCoord, int32_t yCoord, int32_t pixWidth,
		  const char* txt, uint32_t fg, uint32_t bg, uint32_t highCol,
		  Font* fnt = NULL);
	
	void setText (const char* newText);
	void setActionFunc (void (*newFunc) (EntryBox* b)) { action = newFunc; }
	void setMaxChars (int32_t newVal = -1);

	void setFocus (bool isFocus);
};

}

#endif /* __WALY_ENTRY_BOX_H */
