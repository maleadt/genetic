/*
 * tetris_render.h
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

// Include guard
#ifndef __TETRIS_OUTPUT
#define __TETRIS_OUTPUT

// Headers
#include <SDL/SDL.h>							
#include <SDL/SDL_gfxPrimitives.h>


//
// Constants
//

// Color enumeration
enum color {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX};



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Output {
	public:

		Output						();

		void DrawRectangle		(int pX1, int pY1, int pX2, int pY2, enum color pC);
		void ClearScreen		();
		int GetScreenHeight		();
		int InitGraph			();
		void UpdateScreen		();
};

// Include guard
#endif
