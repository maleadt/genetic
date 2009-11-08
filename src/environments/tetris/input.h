/*
 * input.h
 * Evolve - Tetris environment (input interface)
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
#ifndef __TETRIS_INPUT
#define __TETRIS_INPUT

// Headers
#include <SDL/SDL.h>							
#include <SDL/SDL_gfxPrimitives.h>



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Input {
	public:

		Input						();
		
		int Pollkey				();
		int Getkey				();
		int IsKeyDown			(int pKey);
};

// Include guard
#endif
