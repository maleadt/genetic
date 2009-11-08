/*
 * tetris.cpp
 * Evolve - Tetris environment
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



/////////////////
// APPLICATION //
/////////////////

int main() {
	//
	// Setup
	//
	
	// Create game handlers
	Input mInput;
	Output mOutput;
	Pieces mPieces;

	// Start the game
	int mScreenHeight = mOutput.GetScreenHeight();
	Board mBoard (&mPieces, mScreenHeight);
	Game mGame (&mBoard, &mPieces, &mOutput, mScreenHeight);

	// Get the actual clock milliseconds (SDL)
	unsigned long mTime1 = SDL_GetTicks();
	
	
	//
	// Main loop
	//
	
	while (1) {
		// Draw the game
		mGame.DrawScene();

		// Let input handler tick
		int mKey = mInput.Pollkey();
		switch (mKey) {
			case (SDLK_RIGHT): 
				mGame.right();
				break;

			case (SDLK_LEFT): 
				mGame.left();
				break;

			case (SDLK_DOWN):
				mGame.down();
				break;

			case (SDLK_SPACE):
				mGame.drop();
				break;

			case (SDLK_UP):
				mGame.rotate();
				break;
			
			case (SDLK_ESCAPE):
				exit(0);
				break;
		}

		// Move downwards if the wait time is elapsed
		unsigned long mTime2 = SDL_GetTicks();
		if ((mTime2 - mTime1) > WAIT_TIME) {
			mGame.down();
			mTime1 = mTime2;
		}
		
		// Check if the game isn't over yet
		if (mBoard.IsGameOver()) {
			mInput.Getkey();
			exit(0);
		}
	}

	return 0;
}
