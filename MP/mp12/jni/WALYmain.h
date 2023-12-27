//									tab:8
//
// WALYmain.h - header file for WALY main entry and debug output file
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
// Creation Date:   19 June 2011
// Filename:	    WALYmain.h (formerly DebugFile.h)
// History:
//	SL	1.00	19 June 2011
//		First written.
//	SL	1.01	28 December 2012
//		Revised to support Android and Cygwin.
//	SL	1.02	8 January 2013
//		Stopped trying to delete cout when debug file points there.
//

#if !defined(WALY_MAIN_H)
#define WALY_MAIN_H

//
// Check for a valid WALY_TARGET.  These currently include
//    WALY_TARGET_ANDROID - API 17 (Android 4.2)
//    WALY_TARGET_CYGWIN  - SDL-based for execution on Windows
//    WALY_TARGET_WEBOS   - targets Palm Pre 2
//
// All graphics are currently fixed at 320x480
//

#if !defined(WALY_TARGET_WEBOS) && !defined(WALY_TARGET_CYGWIN) && !defined(WALY_TARGET_ANDROID)
#error "no target for WALY"
#endif

#include <iostream>
#include <fstream>
#include <string.h>

#include "WALY.h"

//
// main entry point in WALYmain.cpp file
//
extern int WALY_main(void);

using namespace std;
using namespace WALY;

class DebugFile
{

private:
	static const int maxAppLen = 1024;
	static const int maxNameLen = maxAppLen + 64;
	static const int maxFileCnt = 1000;

	filebuf fbuf;	 // buffer structure for output
	ostream *stream; // pointer to stream structure

	//
	// NOTE: For webOS, assumes that the application has already
	// initialized PDL before creating a debug file, so should not
	// be used with static variables.  That's why constructor is
	// private.
	//
	DebugFile()
	{
		char fileName[maxNameLen]; // debug file name
		int32_t idx;

		// assign filename and open output file
		for (idx = 0; maxFileCnt > idx; idx++)
		{
			sprintf(fileName, "%s%s-%03d.txt", Platform::getVisibleDir(),
					Platform::getAppName(), idx);
			if (NULL != fbuf.open(fileName, ios::in))
			{
				// file already exists: try another one
				(void)fbuf.close();
				continue;
			}
			if (NULL == fbuf.open(fileName, ios::out))
			{
				// file not writable: try another one
				continue;
			}

			// file is ready
			stream = new ostream(&fbuf);
			return;
		}

		// giving up; use cout instead (should throw an exception)
		stream = &cout;
	}

public:
	static DebugFile *createDebugFile() { return new DebugFile(); }

	~DebugFile()
	{
		if (NULL != stream && &cout != stream)
		{
			delete stream;
			(void)fbuf.close();
		}
	}

	ostream &getStream() const { return *stream; }
};

#endif /* WALY_MAIN_H */
