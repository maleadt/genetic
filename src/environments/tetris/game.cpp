/*
 * game.cpp
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

// Headers
#include "game.h"



//////////////////////////
// CLASS IMPLEMENTATION //
//////////////////////////

//
// Construction and destruction
//

Game::Game(Board *pBoard, Pieces *pPieces, Output *pOutput, int pScreenHeight) 
{
	mScreenHeight = pScreenHeight;

	// Get the pointer to the Board and Pieces classes
	mBoard = pBoard;
	mPieces = pPieces;
	mOutput = pOutput;

	// Game initialization
	InitGame ();
}


//
// Informational
//

int Game::getPieceCurrent() {
    return mPiece;
}

int Game::getPieceNext() {
    return mNextPiece;
}

int Game::getX() {
    return mPosX;
}

int Game::getY() {
    return mPosY;
}

int Game::getRotation() {
    return mRotation;
}


//
// Auxiliary
//

void Game::Reset() {
    mBoard->Reset();
    InitGame();
}


// Get a random integer from pA to pB (inclusive)
int Game::GetRand (int pA, int pB) {
	return rand () % (pB - pA + 1) + pA;
}

// Initialise the game parameters
void Game::InitGame() {
	// Init random numbers
	srand ((unsigned int) time(NULL));

	// First piece
	mPiece			= GetRand (0, 6);
	mRotation		= GetRand (0, 3);
	mPosX 			= (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
	mPosY 			= mPieces->GetYInitialPosition (mPiece, mRotation);

	//  Next piece
	mNextPiece 		= GetRand (0, 6);
	mNextRotation 	= GetRand (0, 3);
	mNextPosX 		= BOARD_WIDTH + 5;
	mNextPosY 		= 5;	
}

// Generate a new random piece
void Game::CreateNewPiece() {
	// The new piece
	mPiece			= mNextPiece;
	mRotation		= mNextRotation;
	mPosX 			= (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
	mPosY 			= mPieces->GetYInitialPosition (mPiece, mRotation);

	// Random next piece
	mNextPiece 		= GetRand (0, 6);
	mNextRotation 	= GetRand (0, 3);
}

// Draw a piece
//   pX:		Horizontal position in blocks
//   pY:		Vertical position in blocks
//   pPiece:	Piece to draw
//   pRotation:	1 of the 4 possible rotations
void Game::DrawPiece (int pX, int pY, int pPiece, int pRotation) {
	color mColor;				// Color of the block 

	// Obtain the position in pixel in the screen of the block we want to draw
	int mPixelsX = mBoard->GetXPosInPixels (pX);
	int mPixelsY = mBoard->GetYPosInPixels (pY);

	// Travel the matrix of blocks of the piece and draw the blocks that are filled
	for (int i = 0; i < PIECE_BLOCKS; i++) 	{
		for (int j = 0; j < PIECE_BLOCKS; j++) 		{
			// Get the type of the block and draw it with the correct color
			switch (mPieces->GetBlockType (pPiece, pRotation, j, i))
			{
				case 1: mColor = GREEN; break;	// For each block of the piece except the pivot
				case 2: mColor = BLUE; break;	// For the pivot
			}
			
			if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0)
				mOutput->DrawRectangle	(mPixelsX + i * BLOCK_SIZE, 
									mPixelsY + j * BLOCK_SIZE, 
									(mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
									(mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
									mColor);
		}
	}
}


/* 
======================================									
Draw board

Draw the two lines that delimit the board
====================================== 
*/
void Game::DrawBoard ()
{
	// Calculate the limits of the board in pixels	
	int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
	int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
	int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);
	
	// Check that the vertical margin is not to small
	//assert (mY > MIN_VERTICAL_MARGIN);

	// Rectangles that delimits the board
	mOutput->DrawRectangle (mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, BLUE);
	mOutput->DrawRectangle (mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, BLUE);
	
	// Check that the horizontal margin is not to small
	//assert (mX1 > MIN_HORIZONTAL_MARGIN);

	// Drawing the blocks that are already stored in the board
	mX1 += 1;
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{	
			// Check if the block is filled, if so, draw it
			if (!mBoard->IsFreeBlock(i, j))	
				mOutput->DrawRectangle (	mX1 + i * BLOCK_SIZE, 
										mY + j * BLOCK_SIZE, 
										(mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
										(mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
										RED);
		}
	}	
}


/* 
======================================									
Draw scene

Draw all the objects of the scene
====================================== 
*/
void Game::DrawScene ()
{

	mOutput->ClearScreen (); 		// Clear screen		
	DrawBoard ();													// Draw the delimitation lines and blocks stored in the board
	DrawPiece (mPosX, mPosY, mPiece, mRotation);					// Draw the playing piece
	DrawPiece (mNextPosX, mNextPosY, mNextPiece, mNextRotation);	// Draw the next piece
	mOutput->UpdateScreen ();		// Put the graphic context in the screen
}


//
// Action routines
//

// Move the piece to the left
bool Game::left() {
	if (mBoard->IsPossibleMovement (mPosX - 1, mPosY, mPiece, mRotation)) {
		mPosX--;
		return true;
	}
	return false;
}

// Move the piece to the right
bool Game::right() {
	if (mBoard->IsPossibleMovement (mPosX + 1, mPosY, mPiece, mRotation)) {
		mPosX++;
		return true;
	}
	return false;
}

// Move the piece downwards
bool Game::down() {
	if (mBoard->IsPossibleMovement (mPosX, mPosY + 1, mPiece, mRotation)) {
		mPosY++;
		return true;
	} else {
		mBoard->StorePiece(mPosX, mPosY, mPiece, mRotation);
		mBoard->DeletePossibleLines();
		if (!mBoard->IsGameOver())
			CreateNewPiece();
		return false;
	}
}

// Rotate the piece
bool Game::rotate() {
	if (mBoard->IsPossibleMovement(mPosX, mPosY, mPiece, (mRotation + 1) % 4)) {
		mRotation = (mRotation + 1) % 4;
		return true;
	}
	return false;
}

// Drop the piece
void Game::drop() {
	// Move down as much as we can
	while (down()) {
	}
}
