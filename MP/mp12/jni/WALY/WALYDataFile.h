//									tab:8
//
// WALYDataFile.h - header file for the WALY storage management class
//
// "Copyright (c) 2012 by Steven S. Lumetta."
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
// Creation Date:   28 December 2012
// Filename:	    WALYDataFile.h
// History:
//	SL	1.00	28 December 2012
//		First written.
//	SL	1.01	31 December 2012
//		Moved platform-dependent code into WALYPlatform.
//

#if !defined(__WALY_DATA_FILE_H)
#define __WALY_DATA_FILE_H

//
// I am going to adopt the Palm-style API for getting a filename in
// the right place.  Platform has the application name.
//

#include "WALY.h"

namespace WALY_1_0_0 {
    
class DataFile {
    public:
        static bool getFileName (const char* suffix, char* buf, 
				 uint32_t bufLen) {
	    buf[bufLen - 1] = '\0'; // not clear that strncpy always padded...
	    strncpy (buf, Platform::getDirName (), bufLen);
	    if (buf[bufLen - 1] != '\0') {
	        return false;
	    }
	    uint32_t len = strlen (buf);
	    strncpy (buf + len, suffix, bufLen - len);
	    return (buf[bufLen - 1] == '\0');
	}
};

}

#endif /* __WALY_DATA_FILE_H */

