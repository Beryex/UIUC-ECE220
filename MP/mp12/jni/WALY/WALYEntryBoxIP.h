//									tab:8
//
// WALYEntryBoxIP.h - header file for the WALY library IP address 
//                    entry box widget
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
// Filename:	    WALYEntryBoxIP.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//

#if !defined(__WALY_ENTRY_BOX_IP_H)
#define __WALY_ENTRY_BOX_IP_H

#include <string>

#include "WALY.h"

namespace WALY_1_0_0 {
    
class EntryBoxIP : public EntryBox {
    protected:

	std::string part[4];
	uint32_t    partWidth;
	uint32_t    dotWidth;
	Surface*    dotImg;
	int32_t     whichHigh;
	int32_t     whichCurs;
	int32_t     lastDraw[4];

	void prepareToRedraw ();
	void construct (const char* txt);

	static void mouseDown (Frame* f, const Event* e);
	static void keystroke (Frame* f, const Event* e);

	void jumpToNextPart ();

    public:
        EntryBoxIP (Frame* par, int32_t xCoord, int32_t yCoord,
		    const char* txt, uint32_t fg, uint32_t bg,
		    Font* fnt = NULL);
        EntryBoxIP (Frame* par, int32_t xCoord, int32_t yCoord,
		    const char* txt, uint32_t fg, uint32_t bg,
		    uint32_t highCol, Font* fnt = NULL);

	~EntryBoxIP () {
	    SDL_FreeSurface (dotImg);
	}

	void setText (const char* newText);

	// these both use network byte order
	void setAddr (uint32_t ipAddr);
	uint32_t getAddr ();
};

}

#endif /* __WALY_ENTRY_BOX_IP_H */
