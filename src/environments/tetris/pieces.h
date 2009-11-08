/*
 * pieces.h
 * Evolve - Tetris environment (pieces with their rotational and displacement
 *          details)
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
#ifndef __TETRIS_PIECES
#define __TETRIS_PIECES



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Pieces {
	public:

		int GetBlockType		(int pPiece, int pRotation, int pX, int pY);
		int GetXInitialPosition (int pPiece, int pRotation);
		int GetYInitialPosition (int pPiece, int pRotation);
};

// Include guard
#endif
