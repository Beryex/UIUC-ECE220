//									tab:8
//
// TurtleScreen.cpp - source file for Spring 2018 MP12 display screen
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
// Version:	    1.01
// Creation Date:   20 April 2018
// Filename:	    TurtleScreen.cpp
// History:
//	SL	1.00	20 April 2018
//		First written.
//

#include "WALYmain.h"

#include "TurtleScreen.h"

#include "mp5.h"
#include "mp12.h"

using namespace WALY;

int32_t TurtleScreen::numInstances = 0;
Image **TurtleScreen::image = NULL;

static uint32_t ARGB = 0xFFFFFFFF;
static Surface *drawOn = NULL;

void TurtleScreen::init()
{
    char buf[20];

    image = new Image *[36];
    for (int32_t i = 0; 36 > i; i++)
    {
        sprintf(buf, "turtle%03d.png", i * 10);
        image[i] = new Image(buf);
    }
}

void TurtleScreen::cleanUp()
{
    for (int32_t i = 0; 36 > i; i++)
    {
        delete image[i];
    }
    delete[] image;
    image = NULL;
}

void TurtleScreen::doAnimation(Frame *f, const Event *e)
{
    TurtleScreen *ts = (TurtleScreen *)f;

    // ts->frameUpdate ();

    frameUpdate(ts);
}

TurtleScreen::TurtleScreen(Frame *par) : Frame(par)
{
    Rect r;

    if (0 == numInstances++)
    {
        init();
    }

    useSolidBackground(0x808050);
    r.x = r.y = 0;
    r.w = 640;
    r.h = 640;
    setScrollRect(r);

    direction = 0;

    turtle = new Frame(this, 320, 320);
    turtle->attachImage(image[direction]);
    turtle->setAlign(ALIGN_C);

    setCallbackFunc(FRAME_UPDATE, doAnimation);

    setVisible(false);
    setActive(false);
}

void TurtleScreen::prepareToRedraw()
{
    if (NULL != surface)
    {
        return;
    }

    surface = SDL_CreateRGBSurface(0, 640, 640, 32, 0xFF0000, 0xFF00, 0xFF, 0xFF000000);
    if (NULL == surface)
    {
        useImage = false;
        return;
    }
    useImage = true;

    SDL_FillRect(surface, NULL, bgColor); // from Frame

    if (SDL_MUSTLOCK(surface))
    {
        SDL_LockSurface(surface);
    }
    drawOn = surface;

    drawEverything();

    drawOn = NULL;
    if (SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface);
    }
}

void TurtleScreen::activate()
{
    setActive(true);
    setVisible(true);
}

void TurtleScreen::deactivate()
{
    setVisible(false);
    setActive(false);
}

void TurtleScreen::makeVisibleChange()
{
    if (NULL != surface)
    {
        SDL_FreeSurface(surface);
        surface = NULL;
    }
}

void set_color(int32_t new_color)
{
    ARGB = (0xFF000000UL | new_color);
}

int32_t
draw_dot(int32_t x, int32_t y)
{
    uint32_t *pix;

    if (!drawOn || 0 > x || drawOn->w <= x || 0 > y || drawOn->h <= y)
    {
        return 0;
    }
    pix = (uint32_t *)(((uint8_t *)drawOn->pixels) + drawOn->pitch * y);
    pix[x] = ARGB;
    return 1;
}
