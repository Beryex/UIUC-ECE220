//									tab:8
//
// WALYImage.h - header file for the WALY library image class
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
// Filename:	    WALYImage.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	25 December 2012
//		Revised to support Android and Cygwin.
//

#if !defined(__WALY_IMAGE_H)
#define __WALY_IMAGE_H

namespace WALY_1_0_0 {
    
class Image {
    private:
        Surface* imageData;

    public:
	Image (const char* fileName) {
	    char fName[1024];

	    sprintf (fName, "%s%.800s", WALY_IMAGE_DIR, fileName);
	    // fail silently...
	    imageData = Platform::IMG_Load (fName);
	    if (NULL == imageData) {
	        throw std::exception ();
	    }
	}
	~Image () { 
	    if (NULL != imageData) {
		SDL_FreeSurface (imageData); 
	    }
	}

	void setAlpha (uint8_t alpha);

	friend void Frame::attachImage (Image* img);
};

}

#endif /* __WALY_IMAGE_H */

