/*
 * tetris_render.cpp
 * Evolve - Tetris environment (output interface)
 *
 * Copyright (c) 2009 Tim Besard <tim.besard@gmail.com>
 * Copyright (c) 2009 Javier López López <javilop.com>
 * All rights reserved.
 *
 * This program is licensed under the Creative Commons - Attribution 3.0
 * Unported license.
 * 
 * You are free to:
 *   - Share: to copy, distribute and transmit the work
 *   - Remix: to adapt the work
 *
 * Under the following conditions:
 *   - Attribution: you must attribute the work in the manner specified by the 
 *                  author or licensor (but not in any way that suggests that
 *                  they endorse you or your use of the work).
 *
 */

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Headers
#include "output.h"

static SDL_Surface *mScreen;										// Screen
static Uint32 mColors [COLOR_MAX] = {0x000000ff,					// Colors
                               0xff0000ff, 0x00ff00ff, 0x0000ffff,
                               0x00ffffff, 0xff00ffff, 0xffff00ff,
                               0xffffffff};



//////////////////////////
// CLASS IMPLEMENTATION //
//////////////////////////

Output::Output() 
{
	InitGraph ();
}


/* 
======================================									
Clear the screen to black
====================================== 
*/
void Output::ClearScreen() 
{
	boxColor (mScreen, 0, 0, mScreen->w - 1, mScreen->h - 1, mColors[BLACK]);
}


/* 
======================================									
Draw a rectangle of a given color

Parameters:
>> pX1, pY1: 		Upper left corner of the rectangle
>> pX2, pY2: 		Lower right corner of the rectangle
>> pC				Rectangle color
====================================== 
*/
void Output::DrawRectangle (int pX1, int pY1, int pX2, int pY2, enum color pC)
{
	boxColor (mScreen, pX1, pY1, pX2, pY2-1, mColors[pC]);
}


/* 
======================================									
Return the screen height
====================================== 
*/
int Output::GetScreenHeight()
{
	return mScreen->h;
}


/* 
======================================									
Update screen
====================================== 
*/
void Output::UpdateScreen()
{
	SDL_Flip(mScreen);
}

/* 
======================================									
SDL Graphical Initialization
====================================== 
*/
int Output::InitGraph()
{
	const SDL_VideoInfo *info;
	Uint8  video_bpp;
	Uint32 videoflags;
        
	// Initialize SDL
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	// Alpha blending doesn't work well at 8-bit color
	info = SDL_GetVideoInfo();
	if ( info->vfmt->BitsPerPixel > 8 ) {
		video_bpp = info->vfmt->BitsPerPixel;
	} else {
		video_bpp = 16;
	}
	videoflags = SDL_SWSURFACE | SDL_DOUBLEBUF;
	
	// Set 640x480 video mode
	if ( (mScreen=SDL_SetVideoMode(640,480,video_bpp,videoflags)) == NULL ) {
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n",640,480,SDL_GetError());
		return 2;
	}
    return 0;
}
