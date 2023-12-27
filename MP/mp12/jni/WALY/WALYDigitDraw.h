//									tab:8
//
// WALYDigitDraw.h - header file for the WALY library digit drawing frame
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
// Filename:	    WALYDigitDraw.h
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//

#if !defined(__WALY_DIGIT_DRAW_H)
#define WALY_DIGIT_DRAW_H

#include "math.h"

#include <vector>
#include <iostream>
#include <string>

#include "WALY.h"

namespace WALY_1_0_0 {

class DigitDraw : public Frame {
    public:
	static constexpr int32_t unknown = -1;

    private:

        static constexpr double largeError = 1E20;

        class Point {
	    public:
		int32_t x;
		int32_t y;
		Point () : x (0), y (0) { }
	        Point (int32_t xPos, int32_t yPos) : x (xPos), y (yPos) { }
	};

	class Line {
	    public:
		uint32_t ID;
		double slope;
		double intercept;
		double frac;
		int32_t dir;
	};

	class Circle {
	    public:
		uint32_t ID;
		Point center;
		double radius;
	        double frac;
		int32_t start;
		int32_t end;
		bool clockwise;
		int32_t len;
	};

	class Stroke {
	    public:
		double               totalLength;
		std::vector<Point>   point;
		std::vector<Line>    lineElt;
		std::vector<Circle>  circleElt;
		std::string          code;
	};


	friend std::ostream& operator<< (std::ostream& s, const Point& p);
	friend std::ostream& operator<< (std::ostream& s, const Line& line);
	friend std::ostream& operator<< (std::ostream& s, const Circle& c);
	friend std::ostream& operator<< (std::ostream& s, const Stroke& str);

	static double distSq (const Point& p1, const Point& p2) {
	    double dx = p1.x - p2.x;
	    double dy = p1.y - p2.y;
	    return (dx * dx + dy * dy);
	}
	static double dist (const Point& p1, const Point& p2) {
	    return sqrt (distSq (p1, p2));
	}

	// multiplier for line fit error when compared with circle fit error
	static constexpr double relError = 3.0;

	Rect     drawingArea;
	uint32_t numDigits;
	bool     mouseIsDown;
	void     (*notifyFunc) (DigitDraw* dd, int32_t answer);

	double          totalLength;
        std::vector<Point>   point;
	Point           lastPoint;

	int32_t              nElts;
	std::vector<Line>    lineElt;
	std::vector<Circle>  circleElt;
	std::string          code;

	std::vector<Stroke*>  stroke;

	double fitCircle (int32_t start = 0, int32_t end = -1,
			  bool addElt = false);
	double fitLine (int32_t start = 0, int32_t end = -1,
			bool addElt = false);

	static int32_t slopeToDir (double sl, double dx, double dy);
	static int32_t pointsToDir (const Point& p1, const Point& p2);

	void findPieces ();

	void fitStroke (Stroke* s);
	void doFitting (uint32_t first, uint32_t last);

	bool inDrawingArea (int32_t xPos, int32_t yPos) {
	    mapToLocal (&xPos, &yPos);
	    return (drawingArea.x <= xPos &&
		    drawingArea.x + drawingArea.w > xPos &&
		    drawingArea.y <= yPos &&
		    drawingArea.y + drawingArea.h > yPos);
	}


	void releaseMouse ();
	static void mouseDown (Frame* f, const Event* e);
	static void mouseMotion (Frame* f, const Event* e);
	static void mouseUp (Frame* f, const Event* e);

        bool addPoint (int32_t xPos, int32_t yPos) {
	    if (80 <= point.size ()) { return false; }
	    if (0 != point.size ()) {
		double segLen = dist (Point (xPos, yPos), lastPoint);
		if (10 > segLen) { return false; }
		totalLength += segLen;
	    }
	    lastPoint.x = xPos;
	    lastPoint.y = yPos;
	    point.push_back (Point (xPos, yPos));
	    return true;
	}

	bool endStroke () {
	    if (20 > totalLength) {
	        reset ();
		return false;
	    } else {
		Stroke* s = new Stroke ();
		fitStroke (s);
		stroke.push_back (s);
		return true;
	    }
	}

	int32_t classify ();

	void removeStrokes () {
	    for (uint32_t idx = 0; stroke.size () > idx; idx++) {
	        delete stroke[idx];
	    }
	    stroke.clear ();
	}

    public:

	DigitDraw (Frame* par, const Rect& dArea, uint32_t nDig,
		   void (*nFunc) (DigitDraw* dd, int32_t answer)) :
		Frame (par, 0, 0), drawingArea (dArea), numDigits (nDig),
		mouseIsDown (false), notifyFunc (nFunc), totalLength (0) { 
	    setCallbackFunc (MOUSE_DOWN, mouseDown);
	    setCallbackFunc (MOUSE_MOTION, mouseMotion);
	    setCallbackFunc (MOUSE_UP, mouseUp);
	    setVisible (false); // never visible
	    setActive (false);
	}
	~DigitDraw () { removeStrokes (); }

	int32_t getAnswer (uint32_t maxDigits);
	void reset () {
	    totalLength = 0;
	    point.clear ();
	    mouseIsDown = false;
	    removeStrokes ();
	}

	void activate ()   { setActive (true); }
	void deactivate () { setActive (false); }

	bool addChild (Frame* child) { return false; }
};

}

#endif /* __WALY_DIGIT_DRAW_H */
