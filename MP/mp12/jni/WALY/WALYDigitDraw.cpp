//									tab:8
//
// WALYDigitDraw.cpp - source file for the WALY library digit drawing frame
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
// Version:	    1.02
// Creation Date:   27 July 2011
// Filename:	    WALYDigitDraw.cpp
// History:
//	SL	1.00	27 July 2011
//		Header added for distribution.
//	SL	1.01	26 July 2012
//		Fixed warnings in Cygwin SDL build.
//	SL	1.02	26 December 2012
//		Fixed more warnings for Android.
//

#include <list>

#include "WALY.h"

using namespace std;

namespace WALY_1_0_0 {

int32_t
DigitDraw::slopeToDir (double sl, double dx, double dy)
{
    if (0.414 > fabs (sl)) { return (0 < dx ? 0 : 4); } 
    if (2.414 < fabs (sl)) { return (0 < dy ? 6 : 2); }
    if (0 < sl) { return (0 < dx ? 7 : 3); } 
    return (0 < dx ? 1 : 5);
}

int32_t
DigitDraw::pointsToDir (const Point& to, const Point& from)
{
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    double sl = (0 == dx ? 1E9 : dy / dx);
    return slopeToDir (sl, dx, dy);
}

double
DigitDraw::fitCircle (int32_t start, int32_t end, bool addElt)
{
    // Adjust and check arguments.
    if (-1 == end) { end = point.size (); }
    end--;
    if (end - 1 <= start) { return largeError; }

    double A;
    double B;
    double C;
    double D;
    double E;
    double len;

    A = B = C = D = E = 0;

    len = 0;
    for (int32_t idx = start; end > idx; idx++) {

	double dx = point[idx + 1].x - point[idx].x;
	double dy = point[idx + 1].y - point[idx].y;
	double segLenSq = dx * dx + dy * dy;
	if (0.0 == segLenSq) { continue; }

	double sqDiff = point[idx].x * point[idx].x - 
			point[idx + 1].x * point[idx + 1].x +
			point[idx].y * point[idx].y - 
			point[idx + 1].y * point[idx + 1].y;

	A += (2 * dx * dx) / segLenSq;
	B += (dx * sqDiff) / segLenSq;
	C += (2 * dx * dy) / segLenSq;
	D += (2 * dy * dy) / segLenSq;
	E += (dy * sqDiff) / segLenSq;

	len += sqrt (segLenSq);
    }

    //
    // If all line segments are parallel to one another, the
    // circle's center is at infinity (i.e., A * D == C * C).
    //
    if (A * D == C * C) { return largeError; }

    Point center ((int32_t)((C * E - B * D) / (A * D - C * C)),
		  (int32_t)((B * C - A * E) / (A * D - C * C)));

    double radius = 0;
    for (int32_t idx = start; end >= idx; idx++) {
	radius += distSq (point[idx], center);
    }
    radius = sqrt (radius / (double)(end - start + 1));

    // We disallow 'circles' with huge radii.
    if (200 < radius) { return largeError; }

    double cError = 0;
    for (int32_t idx = start; end > idx; idx++) {
	double dx = point[idx + 1].x - point[idx].x;
	double dy = point[idx + 1].y - point[idx].y;
	double num = (2 * center.x - point[idx].x - point[idx + 1].x) * dx +
		     (2 * center.y - point[idx].y - point[idx + 1].y) * dy;
	cError += (num * num) / (4 * (dx * dx + dy * dy));
    }
    cError /= len;

    int32_t lastDir = pointsToDir (point[start], center);
    int32_t change = 0;
    int32_t numChanges = 0;
    for (int32_t idx = start + 1; end >= idx; idx++) {
	int32_t val = pointsToDir (point[idx], center);
	// Circle segments must not loop back on themselves.
	if (val != lastDir) {
	    if (0 == change) {
	        change = ((val + 8 - lastDir) % 8);
	    } else if (((lastDir + change) % 8) != val) {
		return largeError;
	    }
	    numChanges++;
	    lastDir = val;
	}
    }

    if (addElt) {
	Circle c;

	c.frac = len / totalLength;
	if (0.1 <= c.frac) {
	    c.ID = nElts++;
	    c.center = center;
	    c.radius = radius;
	    c.start = pointsToDir (point[start], center);
	    c.end = pointsToDir (point[end], center);
	    c.clockwise = (7 == change);
	    c.len = numChanges;
	    circleElt.push_back (c); 
	    code += "C";
	}
    }

    return cError;
}

double
DigitDraw::fitLine (int32_t start, int32_t end, bool addElt)
{
    // Adjust and check arguments.
    if (-1 == end) { end = point.size (); }
    if (end <= start) { return largeError; }

    double avgX;
    double avgY;
    double A;
    double B;
    double C;
    double len;

    avgX = avgY = 0;
    for (int32_t idx = start; end > idx; idx++) {
	avgX += point[idx].x; 
	avgY += point[idx].y; 
    }
    avgX /= end - start;
    avgY /= end - start;

    A = B = C = 0;
    len = 0;
    for (int32_t idx = start; end > idx; idx++) {
	if (end - 1 > idx) {
	    len += dist (point[idx], point[idx + 1]);
	}
	A += point[idx].y * (point[idx].x - avgX);
	B += point[idx].x * (point[idx].x - avgX) - 
	     point[idx].y * (point[idx].y - avgY);
	C += point[idx].x * (avgY - point[idx].y);
    }

    // we've implicitly assumed that the best fit line is not 
    // vertical...
    double slope;
    if (1E-5 >= fabs (A)) {
	slope = 1000000;
    } else {
	slope = (-B + sqrt (B * B - 4 * A * C)) / (2 * A);
    }
    double intercept = avgY - slope * avgX;

    double lError = 0;
    for (int32_t idx = start; end > idx; idx++) {
	double num = slope * point[idx].x + intercept - point[idx].y;
	lError += num * num;
    }
    lError /= len * (1 + slope * slope);
    lError *= relError;

    if (addElt) {
	Line line;

	line.frac = len / totalLength;
	if (0.1 <= line.frac) {
	    line.ID = nElts++;

	    line.slope = slope;
	    line.intercept = intercept;
	    line.dir = slopeToDir (slope, avgX - point[start].x,
				   avgY - point[start].y);
	    lineElt.push_back (line); 
	    code += "L";
	}
    }

    return lError;
}

void
DigitDraw::doFitting (uint32_t first, uint32_t last)
{
    double  oneError;
    double  twoError;
    int32_t twoCut;
    double  tryError;

    // 0 = circle 0 to N
    // 1 = line N to end
    double* cCost[2];
    double* lCost[2];

    cCost[0] = new double[last - first + 1];
    cCost[1] = new double[last - first + 1];
    lCost[0] = new double[last - first + 1];
    lCost[1] = new double[last - first + 1];

    for (uint32_t delta = 0; last >= first + delta; delta++) {
	cCost[0][delta] = fitCircle (first, first + delta);
//	Screen::debug () << "C0," << delta << " " << cCost[0][delta] << std::endl;
	lCost[0][delta] = fitLine (first, first + delta);
//	Screen::debug () << "L0," << delta << " " << lCost[0][delta] << std::endl;
	cCost[1][delta] = fitCircle (first + delta, last);
//	Screen::debug () << "C1," << delta << " " << cCost[1][delta] << std::endl;
	lCost[1][delta] = fitLine (first + delta, last);
//	Screen::debug () << "L1," << delta << " " << lCost[1][delta] << std::endl;
    }

    oneError = (cCost[1][0] < lCost[1][0] ? cCost[1][0] : lCost[1][0]);
    twoError = oneError;
    twoCut = 0;
    for (uint32_t delta = first; last >= first + delta; delta++) {
        tryError = (cCost[0][delta] < lCost[0][delta] ?
		    cCost[0][delta] : lCost[0][delta]);
        tryError += (cCost[1][delta] < lCost[1][delta] ?
		     cCost[1][delta] : lCost[1][delta]);
        if (twoError > tryError) {
	    twoError = tryError;
	    twoCut = delta;
	}
    }

    Screen::debug () << "oneError = " << oneError << std::endl;
    Screen::debug () << "twoError = " << twoError << std::endl;

    if (40 < oneError && 40 < twoError + 20) {
        uint32_t p1, p2;
	double min = (oneError < twoError + 20 ? oneError : twoError + 20);
	double opt;
	int32_t opt1 = 0;
	int32_t opt2 = 0;
	bool aLine;
	bool optLine = false;

	opt = min - 40;
	for (p1 = first + 1; last > p1; p1++) {
	    for (p2 = p1 + 1; last >= p2; p2++) {
	        double once;
		
		once = (cCost[0][p1 - first] < lCost[0][p1 - first] ?
			cCost[0][p1 - first] : lCost[0][p1 - first]);
		once += (cCost[1][p2 - first] < lCost[1][p2 - first] ?
			 cCost[1][p2 - first] : lCost[1][p2 - first]);
		if (opt > once) {
		    double cError = fitCircle (p1, p2);
		    double lError = fitLine (p1, p2);
		    if (cError > lError) {
			once += lError;
			aLine = true;
		    } else {
			once += cError;
			aLine = false;
		    }
		    if (opt > once) {
			opt1 = p1;
			opt2 = p2;
			opt = once;
			optLine = aLine;
		    }
		}
	    }
	}

	Screen::debug () << "threeError = " << opt << std::endl;

	if (min - 40 > opt) {
	    if (cCost[0][opt1] > lCost[0][opt1]) {
		fitLine (first, opt1, true);
	    } else {
	        fitCircle (first, opt1, true);
	    }
	    if (optLine) {
		fitLine (opt1, opt2, true);
	    } else {
	        fitCircle (opt1, opt2, true);
	    }
	    if (cCost[1][opt2] > lCost[1][opt2]) {
		fitLine (opt2, last, true);
	    } else {
	        fitCircle (opt2, last, true);
	    }
	    return;
	}
    }

    if (twoError + 20 > oneError) {
	if (lCost[1][0] == oneError) {
	    fitLine (first, last, true);
	} else {
	    fitCircle (first, last, true);
	}
    } else {
	if (cCost[0][first + twoCut] > lCost[0][first + twoCut]) {
	    fitLine (first, first + twoCut, true);
	} else {
	    fitCircle (first, first + twoCut, true);
	}
	if (cCost[1][first + twoCut] > lCost[1][first + twoCut]) {
	    fitLine (first + twoCut, last, true);
	} else {
	    fitCircle (first + twoCut, last, true);
	}
    }
}

void
DigitDraw::fitStroke (Stroke* s)
{
    Screen::debug () << std::endl << "START STROKE FITTING" << std::endl;
    Screen::debug () << "total length is " << totalLength << std::endl;

    lineElt.clear ();
    circleElt.clear ();
    nElts = 0;
    code = "";

    doFitting (0, point.size ());

    Screen::debug () << "# of elements = " << nElts << std::endl;
    for (uint32_t idx = 0; lineElt.size () > idx; idx++) {
        Screen::debug () << lineElt[idx] << std::endl;
    }
    for (uint32_t idx = 0; circleElt.size () > idx; idx++) {
        Screen::debug () << circleElt[idx] << std::endl;
    }

    Screen::debug () << "STROKE ALONE GIVES " << classify () << std::endl;

    s->totalLength = totalLength;
    s->point = point;
    s->lineElt = lineElt;
    s->circleElt = circleElt;
    s->code = code;

    lineElt.clear ();
    circleElt.clear ();
    nElts = 0;
    code = "";
    point.clear ();
    totalLength = 0;
}

int32_t
DigitDraw::classify ()
{
    if (0 == nElts || 40 > totalLength) { 
        return unknown; 
    }

    switch (nElts) {
        case 1: {
	    if (1 == circleElt.size ()) {
		Circle& c = circleElt[0];

		if (3 >= c.start && !c.clockwise && 11 <= c.len) { return 6; }
		if (2 >= c.start && c.clockwise && 11 <= c.len) { return 9; }
		if (6 <= c.len) { return 0; }
	    } else {
		Line& line = lineElt[0];

		if (5 <= line.dir && 7 >= line.dir) { return 1; }
	    }
	    break;
	}
        case 2: {
	    if (2 == lineElt.size ()) {
	        Line& l1 = lineElt[0];
	        Line& l2 = lineElt[1];

		if (1 >= l1.dir && 5 <= l2.dir && 6 >= l2.dir) { return 7; }
		if (6 == l1.dir && 2 == l2.dir) { return 0; }
	    } else if (1 == lineElt.size ()) {
	        Circle& c = circleElt[0];
	        Line& line = lineElt[0];

		if (1 == c.ID) {
		    if (!c.clockwise && 5 <= line.dir && 6 >= line.dir && 
		        7 <= c.len) {
		        return 6;
		    }
		    if ((6 == line.dir && 5 <= c.start && 1 <= c.end && 
		         3 >= c.end) ||
			(2 == line.dir && 1 <= c.start && 3 >= c.start &&
			 5 <= c.end)) {
		        return 0;
		    }
		    if (!c.clockwise && (2 == line.dir || 6 == line.dir) && 
		        1 <= c.start && 3 >= c.start && 
			(7 == c.end || 0 == c.end)) {
			return 4;
		    }
		    if (0 == line.dir && !c.clockwise && 1 <= c.start && 
			3 >= c.start && 4 <= c.len && 6 >= c.len && 
			(1 >= c.end || 7 == c.end)) {
			return 2;
		    }
		} else {
		    if (5 <= line.dir && 6 >= line.dir && 5 <= c.len && 
		        (2 >= c.start || 7 == c.start) &&
			(1 >= c.end || 6 <= c.end)) {
			return 9;
		    }
		    if ((2 == line.dir && 1 <= c.start && 3 >= c.start &&
		         5 <= c.end) ||
			(6 == line.dir && 5 <= c.start && 1 <= c.end &&
			 3 >= c.end)) {
		        return 0;
		    }
		    if (c.clockwise && 3 <= c.start && 5 >= c.start &&
			5 <= c.len && 6 >= c.len && 5 <= c.end && 7 >= c.end &&
			0 == line.dir) {
			return 2;
		    }
		    if (c.clockwise && 80 <= c.radius &&
		        5 <= line.dir && 6 <= line.dir) {
		        return 7;
		    }
		}
	    } else {
		Circle& c1 = circleElt[0];
		Circle& c2 = circleElt[1];

		if (c1.clockwise && c2.clockwise && 5 <= c1.len &&
		    4 <= c2.len && 4 <= c1.start && 5 >= c1.start &&
		    6 <= c1.end && 1 <= c2.start && 2 >= c2.start &&
		    3 <= c2.end && 6 >= c2.end) {
		    return 3;
		}
		if (!c1.clockwise && !c2.clockwise &&
		    (3 >= c1.start && 4 <= c1.end && 5 >= c1.end &&
		     7 <= c2.len && 80 <= c1.radius)) {
		    return 6;
		}
		if (7 <= c1.len && 9 <= c1.len + c2.len && c2.clockwise && 
		    (1 >= c2.start || 7 == c2.start)) {
		    return 9;
		}
		if (c1.clockwise == c2.clockwise) {
		    return 0;
		}
	    }
	    break;
	}
        case 3: {
	    if (3 == lineElt.size ()) {
	        Line& l1 = lineElt[0];
	        Line& l2 = lineElt[1];
	        Line& l3 = lineElt[2];

		if (1 >= l1.dir && 5 == l2.dir && 0 == l3.dir) {
		    return 2;
		}
		if ((0 == l1.dir || 1 == l1.dir) && 
		    ((0 == l2.dir && 4 <= l3.dir && 5 >= l3.dir) ||
		     (5 == l2.dir && (0 == l3.dir || 5 == l3.dir)))) {
		    return 3;
		}
		//
		// We need to differentiate 4 from 17.  When two strokes
		// are put together, we can get the same LLL (down, right,
		// down) pattern.  With 17, the first stroke has frac 
		// close to 1...
		//
		// Similarly, 14 and 41 are indistinguishable if all types
		// of 4's are allowed...
		//    (6 == l1.dir && 5 <= l2.dir && 6 >= l2.dir && 
		//     0 == l3.dir) ||
		//
		if ((5 <= l1.dir && 6 >= l1.dir && 0 == l2.dir && 
		     6 == l3.dir && 0.95 > l1.frac) ||
		    (2 == l1.dir && 5 == l2.dir && 0 == l3.dir) ||
		    (4 == l1.dir && 1 == l2.dir && 5 <= l3.dir && 
		     6 >= l3.dir)) {
		    return 4;
		}
		if (4 <= l1.dir && 5 >= l1.dir &&
		    (0 == l2.dir || 7 == l2.dir) &&
		    4 <= l3.dir && 5 >= l3.dir) {
		    return 5;
		}
	    } else if (2 == lineElt.size ()) {
	        Circle& c = circleElt[0];
	        Line& l1 = lineElt[0];
	        Line& l2 = lineElt[1];

		if (2 == c.ID) {
		    if ((4 <= l1.dir && 5 >= l1.dir && 7 == l2.dir &&
		         c.clockwise) ||
			((0 == l1.dir || 7 == l1.dir) && 5 == l2.dir &&
		         !c.clockwise)) {
			return 8;
		    }
		    if ((0 == l1.dir || 7 == l1.dir) && 4 <= l2.dir &&
		        5 >= l2.dir && 5 <= c.len && 1 <= c.start &&
			4 >= c.start && 4 <= c.end && 6 >= c.end &&
			c.clockwise) {
			return 3;
		    }
		    if (3 <= l1.dir && 5 >= l1.dir && 5 <= l2.dir &&
		        c.clockwise && 4 <= c.len) {
			return 5;
		    }
		} else if (1 == c.ID) {
		    if ((4 <= l1.dir && 5 >= l1.dir && 1 == l2.dir && 
		    	 c.clockwise) ||
			((0 == l1.dir || 7 == l1.dir) && 3 == l2.dir &&
			 !c.clockwise)) {
			return 8;
		    }
		    if (5 <= l1.dir && 6 >= l1.dir && 
			4 <= l2.dir && 6 >= l2.dir && !c.clockwise) {
			return 6;
		    }
		    if (4 >= l1.dir && 5 <= l1.dir && 4 <= c.len &&
			5 <= l2.dir && 6 >= l2.dir && !c.clockwise) {
			return 9;
		    }
		    if (5 <= l1.dir && 6 >= l1.dir && 0 == l2.dir &&
		        !c.clockwise) {
			return 4;
		    }
		    // Avoid turning an initial '1' into part of a 5...
		    if (5 <= l1.dir && 6 >= l1.dir && 0 == l2.dir &&
		        c.clockwise && 0.95 >= l1.frac) {
			return 5;
		    }
		} else { // 0 == c.ID
		    if (c.clockwise && 3 <= c.start && 4 >= c.start && 
			((6 <= c.len && 7 >= c.len && 4 <= l2.dir && 
			  5 >= l2.dir && (0 == l1.dir || 7 == l1.dir)) ||
			 (3 >= c.len && 5 == l1.dir && 5 == l2.dir))) {
		        return 3;
		    }
		    if (!c.clockwise && 2 >= c.start && 2 >= c.end &&
		        6 <= c.len && 5 <= l1.dir && 3 <= l2.dir &&
			5 >= l2.dir) {
		        return 9;
		    }
		    if (c.clockwise && 3 <= c.start && 5 >= c.start &&
			(6 >= c.len || 9 <= c.len) && 
			(2 >= c.end || 6 <= c.end) &&
			5 == l1.dir && 0 == l2.dir) {
			return 2;
		    }
		    if (!c.clockwise && 4 >= c.len && 
		        (1 >= l1.dir || 7 == l1.dir) &&
			(4 <= l2.dir && 5 >= l2.dir)) {
			return 5;
		    }
		}
	    } else if (1 == lineElt.size ()) {
	        Circle& c1 = circleElt[0];
	        Circle& c2 = circleElt[1];
	        Line& line = lineElt[0];

		if (2 == line.ID) {
		    if (3 <= c1.len && 4 <= c2.len && 9 <= c1.len + c2.len &&
			((!c1.clockwise && c2.clockwise && 2 >= c1.start && 
			  4 <= c1.end && 3 >= c2.start && 2 <= c2.end && 
			  5 >= c2.end && 1 <= line.dir && 2 >= line.dir) ||
			 (c1.clockwise && !c2.clockwise && 2 <= c1.start && 
			  4 >= c1.start && (5 <= c1.end || 0 == c1.end) && 
		    	  1 <= c2.start && 4 >= c2.start && 
			  (2 >= c2.end || 7 == c2.end) && 2 <= line.dir &&
			  3 >= line.dir))) {
			return 8;
		    }
		    if (5 <= c1.len && 2 <= c2.len && 8 <= c1.len + c2.len &&
		    	c1.clockwise && c2.clockwise && 4 <= c1.start && 
			5 >= c1.start && 6 <= c1.end && 2 <= c2.start && 
			3 >= c2.start && (6 <= c2.end || 0 == c2.end) &&
			4 <= line.dir && 5 >= line.dir) {
		        return 3;
		    }
		    if (!c1.clockwise && !c2.clockwise &&
		        (5 <= line.dir && 6 >= line.dir) && 
			8 <= c1.len + c2.len) {
			return 6;
		    }
		    if (c1.clockwise == c2.clockwise && 5 <= line.dir) {
			return 9;
		    }
		} else if (1 == line.ID) {
		    if (c2.clockwise && 2 >= c1.end && 6 == line.dir &&
			((c1.clockwise && 5 <= c1.start) ||
			 (!c1.clockwise && 2 >= c1.start))) {
			return 9;
		    }
		} else if (0 == line.ID) {
		    if (5 <= c1.len && 2 <= c2.len && 1 <= c2.end && 
		        3 >= c2.end &&
			((c1.clockwise && !c2.clockwise && 2 <= c1.start && 
			  3 >= c1.start && 2 <= c1.end && 6 >= c1.end &&
			  6 <= c2.start && 5 == line.dir) ||
			 (!c1.clockwise && c2.clockwise && 1 <= c1.start && 
			  2 >= c1.start && (2 >= c1.end || 6 <= c1.end) &&
			  5 <= c2.start && 6 >= c2.start && 7 == line.dir))) {
			return 8;
		    }
		    if (!c1.clockwise && !c2.clockwise && 
		        (5 <= line.dir && 6 >= line.dir) && 
		        7 <= c1.len + c2.len) {
		        return 6;
		    }
		    if (c1.clockwise == c2.clockwise && 5 <= line.dir) {
		        return 0;
		    }
		}
	    } else {
	        Circle& c1 = circleElt[0];
	        Circle& c2 = circleElt[1];
	        Circle& c3 = circleElt[2];

	        if (1 <= c1.start && 3 >= c1.start && 1 <= c3.end && 
		    3 >= c3.end && c1.clockwise == !c2.clockwise &&
		    c2.clockwise == !c3.clockwise) {
		    return 8;
		}
		if (c1.clockwise && c2.clockwise && c3.clockwise &&
		    ((c1.end == c2.start && 1 == c1.len) ||
		     (c2.end == c3.start && 1 == c3.len))) {
		    return 3;
		}
		if (!c1.clockwise && !c2.clockwise && !c3.clockwise) {
		    // could not generate 3 circles with 0
		    return 6;
		}
		if (c1.clockwise == c2.clockwise && c3.clockwise) {
		    // could not generate 3 circles with 0
		    return 9;
		}
	    }
	    break;
	}
	default:
	    break;
    }
    return unknown;
}

int32_t
DigitDraw::getAnswer (uint32_t maxDigits)
{
    uint32_t s;
    int32_t digit;
    list<int32_t> num;

    s = 0;
    Screen::debug () << "called getAnswer ... " << std::endl;
    while (stroke.size () > s) {
	Screen::debug () << "#" << s << " " << *stroke[s] << std::endl;
	int32_t s1Elts = stroke[s]->lineElt.size () +
			 stroke[s]->circleElt.size ();
        if (stroke.size () - 1 > s) {
	    // try two strokes...
	    int32_t s2Elts = stroke[s + 1]->lineElt.size () +
			     stroke[s + 1]->circleElt.size ();
	    if (3 >= s1Elts + s2Elts) {
		nElts = s1Elts + s2Elts;
		lineElt = stroke[s]->lineElt;
		circleElt = stroke[s]->circleElt;
		for (uint32_t idx = 0; stroke[s + 1]->lineElt.size () > idx;
		     idx++) {
		    lineElt.push_back (stroke[s + 1]->lineElt[idx]);
		    lineElt[lineElt.size () - 1].ID += s1Elts;
		}
		for (uint32_t idx = 0; stroke[s + 1]->circleElt.size () > idx;
		     idx++) {
		    circleElt.push_back (stroke[s + 1]->circleElt[idx]);
		    circleElt[circleElt.size () - 1].ID += s1Elts;
		}
		totalLength = stroke[s]->totalLength +
			      stroke[s + 1]->totalLength;
		digit = classify ();
		Screen::debug () << "strokes " << s << "+" << s+1 << " gave "
			 	 << digit << std::endl;
		if (4 == digit || 5 == digit) {
		    // consume two strokes
		    if (maxDigits == num.size ()) { num.pop_front (); }
		    num.push_back (digit);
		    s += 2;
		    continue;
		}
	    }
	}
	nElts = s1Elts;
	lineElt = stroke[s]->lineElt;
	circleElt = stroke[s]->circleElt;
	totalLength = stroke[s]->totalLength;
	digit = classify ();
	Screen::debug () << "stroke " << s << " gave " << digit << std::endl;
	if (unknown != digit) {
	    // consume one stroke
	    if (maxDigits == num.size ()) { num.pop_front (); }
	    num.push_back (digit);
	} else {
	    // discard a stroke
	}
	s += 1;
    }

    int32_t val = 0;
    for (list<int32_t>::iterator it = num.begin (); num.end () != it; it++) {
        val = 10 * val + (*it);
    }

    lineElt.clear ();
    circleElt.clear ();
    nElts = 0;
    code = "";
    point.clear ();
    totalLength = 0;

    Screen::debug () << "returning " << val << std::endl;
    return val;
}

void 
DigitDraw::mouseDown (Frame* f, const Event* e) 
{
    if (0 == e->button.which) {
	DigitDraw* dd = (DigitDraw*)f;
	if (dd->inDrawingArea (e->button.x, e->button.y)) {
	    dd->mouseIsDown = true;
	    dd->addPoint (e->button.x, e->button.y);
	}
    }
}

void 
DigitDraw::releaseMouse () 
{
    mouseIsDown = false;
    bool goodStroke = endStroke ();

    if (NULL != notifyFunc) {
	if (!goodStroke) {
	    notifyFunc (this, unknown);
	} else {
	    int32_t answer = getAnswer (numDigits);

	    if (unknown != answer) {
		notifyFunc (this, answer);
	    }
	}
    }
}

void 
DigitDraw::mouseMotion (Frame* f, const Event* e) 
{
    if (0 == e->motion.which) {
	DigitDraw* dd = (DigitDraw*)f;
	if (dd->mouseIsDown) { 
	    if (dd->inDrawingArea (e->motion.x, e->motion.y)) {
		dd->addPoint (e->motion.x, e->motion.y);
	    } else {
		dd->releaseMouse ();
	    }
	}
    }
}

void 
DigitDraw::mouseUp (Frame* f, const Event* e) 
{
    if (0 == e->button.which) {
	DigitDraw* dd = (DigitDraw*)f;
	if (dd->mouseIsDown) { 
	    dd->addPoint (e->button.x, e->button.y);
	    dd->releaseMouse ();
	}
    }
}

ostream&
operator<< (ostream& s, const DigitDraw::Point& p)
{
    return s << "(" << p.x << "," << p.y << ")";
}

ostream&
operator<< (ostream& s, const DigitDraw::Line& line)
{
    return s << "L" << line.ID << " " << line.frac << " " << line.dir << " " 
	     << line.slope << " " << line.intercept;
}

ostream&
operator<< (ostream& s, const DigitDraw::Circle& c)
{
    return s << "C" << c.ID << " " << c.frac << " " << c.start << "-" << c.end
	     << (c.clockwise ? " (clockwise) " : " (counterclockwise) ")
	     << c.center << " " << c.radius << " " << c.len;
}

ostream&
operator<< (ostream& s, const DigitDraw::Stroke& str)
{
    s << "stroke " << str.code << " length " << str.totalLength << std::endl;
    for (uint32_t idx = 0; str.lineElt.size () > idx; idx++) {
        s << "  " << str.lineElt[idx] << std::endl;
    }
    for (uint32_t idx = 0; str.circleElt.size () > idx; idx++) {
        s << "  " << str.circleElt[idx] << std::endl;
    }
    return s;
}

}

