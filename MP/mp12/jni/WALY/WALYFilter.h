//									tab:8
//
// WALYFilter.h - header file for the WALY library image filter class
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
// Filename:	    WALYFilter.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//

#if !defined(__WALY_FILTER_H)
#define __WALY_FILTER_H

#include "WALY.h"

namespace WALY_1_0_0 {
    
class Filter {
    public:
	static uint32_t colorDarken (uint32_t ARGB, uint8_t amt);
	static uint32_t colorLighten (uint32_t ARGB, uint8_t amt);
	static void roundCorners (Surface* s, int32_t radius);
	static void greyOut (Surface* s);
	static void drawLowered (Surface* s, int32_t border);
	static void drawRaised (Surface* s, int32_t border);
	static void drawDarkBorder (Surface* s, int32_t border);
};

}

#endif /* __WALY_FILTER_H */
