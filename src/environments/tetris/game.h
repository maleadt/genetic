/*
 * game.h
 * Evolve - Tetris environment (general game class)
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
#ifndef __TETRIS_GAME
#define __TETRIS_GAME

// Headers
#include "output.h"
#include "input.h"
#include "board.h"
#include "pieces.h"
#include <time.h>


//
// Constants
//

#define WAIT_TIME 700			// Number of milliseconds that the piece remains before going 1 block down */



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Game {
	public:
		// Construction and destruction
		Game(Board *pBoard, Pieces *pPieces, Output *pOutput, int pScreenHeight);
		
		// Game routines
		void DrawScene();
		void CreateNewPiece();
		
		// Action routines
		bool left();
		bool right();
		bool down();
		bool rotate();
		void drop();

	private:
		// Current block details
		int mPosX, mPosY;
		int mPiece, mRotation;
		
		// Next block details
		int mNextPosX, mNextPosY;
		int mNextPiece, mNextRotation;
		
		// Screen height (pixels)
		int mScreenHeight;
		
		// Game pointers
		Board *mBoard;
		Pieces *mPieces;
		Output *mOutput;
		
		// Other routines
		int GetRand (int pA, int pB);
		void InitGame();
		void DrawPiece (int pX, int pY, int pPiece, int pRotation);
		void DrawBoard ();
};

// Include guard
#endif
