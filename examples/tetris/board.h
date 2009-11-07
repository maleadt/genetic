/*
 * tetris_board.h
 * Evolve - Tetris environment (board definition)
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
#ifndef __TETRIS_BOARD
#define __TETRIS_BOARD

// Headers
#include "pieces.h"


//
// Constants
//

#define BOARD_LINE_WIDTH 6			// Width of each of the two lines that delimit the board
#define BLOCK_SIZE 16				// Width and Height of each block of a piece
#define BOARD_POSITION 320			// Center position of the board from the left of the screen
#define BOARD_WIDTH 10				// Board width in blocks 
#define BOARD_HEIGHT 20				// Board height in blocks
#define MIN_VERTICAL_MARGIN 20		// Minimum vertical margin for the board limit 		
#define MIN_HORIZONTAL_MARGIN 20	// Minimum horizontal margin for the board limit
#define PIECE_BLOCKS 5				// Number of horizontal and vertical blocks of a matrix piece



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Board {
	public:

		Board						(Pieces *pPieces, int pScreenHeight);

		int GetXPosInPixels			(int pPos);
		int GetYPosInPixels			(int pPos);
		bool IsFreeBlock			(int pX, int pY);
		bool IsPossibleMovement		(int pX, int pY, int pPiece, int pRotation);
		void StorePiece				(int pX, int pY, int pPiece, int pRotation);
		void DeletePossibleLines	();
		bool IsGameOver				();

	private:

		enum { POS_FREE, POS_FILLED };			// POS_FREE = free position of the board; POS_FILLED = filled position of the board
		int mBoard [BOARD_WIDTH][BOARD_HEIGHT];	// Board that contains the pieces
		Pieces *mPieces;
		int mScreenHeight;

		void InitBoard();
		void DeleteLine (int pY);
};

// Include guard
#endif
