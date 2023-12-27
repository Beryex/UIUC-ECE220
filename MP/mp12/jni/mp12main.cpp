//									tab:8
//
// mp12.cpp - source file for Spring 2018 MP12 main program
//
// "Copyright (c) 2018 by Steven S. Lumetta."
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
// Creation Date:   20 April 2018
// Filename:	    mp12.cpp
// History:
//	SL	1.00	20 April 2018
//		First written.
//

#include <iostream>

#include "WALYmain.h"
#include "TurtleScreen.h"
#include "mp12.h"

using namespace std;
using namespace WALY;

int WALY_main()
{
    if (!openInputStream("commands"))
    {
        cerr << "failed to open commands file." << endl;
        return 3;
    }

    Frame *rootFrame = Screen::createRootFrame(640, 640);

    srand(time(NULL));

    // If you turn this back on, files will appear in the debug subdirectory.
    // You can use Screen::debug () to get the output stream.
    // Be sure to turn on the termination and deletion at the end of this
    // function.
    //
    // create the output file for debugging...
    // DebugFile* dbg = DebugFile::createDebugFile ();
    // dbg->getStream () << "----- START OF DEBUG OUTPUT -----" << endl;
    // Screen::setDebugOutput (&dbg->getStream ());

    // 25 fps (40 millisecond per frame)
    Screen::setMSBetweenFrames(40);

    TurtleScreen *turtleScreen = new TurtleScreen(rootFrame);

    turtleScreen->activate();

    Screen::run();

    // dbg->getStream () << "----- END OF DEBUG OUTPUT -----" << endl;
    // delete dbg;

    closeInputStream();

    return 0;
}
