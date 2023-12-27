//									tab:8
//
// TurtleScreen.h - header file for Spring 2018 MP12 display screen
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
// Filename:	    TurtleScreen.h
// History:
//	SL	1.00	20 April 2018
//		First written.
//

#if !defined(TURTLE_SCREEN_H)
#define TURTLE_SCREEN_H

#include "WALY/WALY.h"

namespace WALY_1_0_0
{

	class TurtleScreen : public Frame
	{

	private:
		static int32_t numInstances;
		static Image **image;
		static Font *font;
		static void init();
		static void cleanUp();

		Frame *turtle;
		int32_t tx, ty;
		int32_t direction;

		static void doAnimation(Frame *f, const Event *e);

	protected:
		void prepareToRedraw();

	public:
		TurtleScreen(Frame *par);
		~TurtleScreen()
		{
			if (0 == --numInstances)
			{
				cleanUp();
			}
		}

		void activate();
		void deactivate();

		//-------------------------------------------------------------------------
		// for student use in animations

		// Read X location of the turtle.
		int32_t getX() { return turtle->getX(); }

		// Read Y location of the turtle.
		int32_t getY() { return turtle->getY(); }

		// Read current direction of the turtle (0 to 35).
		int32_t getDirection() { return direction; }

		// Set X location of the turtle.
		void setX(int32_t newVal) { return turtle->setX(newVal); }

		// Set Y location of the turtle.
		void setY(int32_t newVal) { return turtle->setY(newVal); }

		// Turn turtle left by 10 degrees.
		void turnLeft()
		{
			direction = (direction + 1) % 36;
			turtle->attachImage(image[direction]);
		}

		// Turn turtle right by 10 degrees.
		void turnRight()
		{
			direction = (direction + 35) % 36;
			turtle->attachImage(image[direction]);
		}

		// Call this function whenever the way in which the
		// background is drawn changes.
		void makeVisibleChange();
	};

}

#endif /* WELCOME_SCREEN_H */
