/*
 * tetris.cpp
 * Evolve - Tetris environment
 *
 * Copyright (c) 2009 Tim Besard <tim.besard@gmail.com>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Headers
#include "tetris.h"



/////////////
// GENERIC //
/////////////

//
// Construction and destruction
//

EnvTetris::EnvTetris() {
    // Configure grammar and parser
    setup();
    mParser = new Parser(this, LIMIT_INSTRUCTIONS);

    // Configure tetris
    mTetrisBoard = new Board(&mTetrisPieces, mTetrisOutput.GetScreenHeight());
    mTetrisGame = new Game(mTetrisBoard, &mTetrisPieces, &mTetrisOutput, mTetrisOutput.GetScreenHeight());
}

EnvTetris::~EnvTetris() {
    delete(mParser);
}



/////////////////
// ENVIRONMENT //
/////////////////


// Alphabet (maximal amount of instructions)
int EnvTetris::alphabet() const {
    return 254;
}

// Fitness function
double EnvTetris::fitness(const DNA* inputDNA) {
    // Validate the syntax
    try {
        mParser->validate(*inputDNA);
    } catch (Exception e) {
        return 0;
    }

    // Reset tetris
    mTetrisGame->Reset();

    // Execute the code
    double tScore = 0;
    unsigned long tScoreCurrent;
    try {
        for (unsigned int i = 0; i < RUNS; i++) {

            // Reset counters and gamestate
            unsigned long tCountUnchanged = 0;
            unsigned int tScorePrevious = 0;
            unsigned long tTime1 = SDL_GetTicks();
            mTetrisGame->Reset();

            // Play a game
            while (!mTetrisBoard->IsGameOver() && tCountUnchanged <= LIMIT_RUNS) {
                // Poll for events
                SDL_Event event;
                while ( SDL_PollEvent(&event) ) {
                        switch (event.type) {
                                case SDL_QUIT:
                                        exit(3);
                        }
                }
                // Evaluate
                mParser->evaluate(*inputDNA);
                
                // Manage counters
                SDL_Delay(GAME_USERDELAY/GAME_SPEED);

                // Calculate score
                tScoreCurrent = mTetrisBoard->Score();
                if (tScoreCurrent == tScorePrevious)
                    tCountUnchanged++;
                tScorePrevious = tScoreCurrent;

		// Move downwards if the wait time is elapsed
		unsigned long tTime2 = SDL_GetTicks();
		if ((tTime2 - tTime1) > GAME_DROPDELAY/GAME_SPEED) {
			mTetrisGame->down();
                        mTetrisGame->DrawScene();
			tTime1 = tTime2;
		}
            }
        }
        tScore += ((double)tScoreCurrent) / LIMIT_RUNS;
    } catch (Exception e) {
        return 0;
    }

    // Get the score
    // TODO: genetic multi-parameter support (e.g. positive score, negative time of death)
    explain(inputDNA);
    return tScore;
}

// Condition (mutate 10 times
bool EnvTetris::condition() {
    return true;
}

// Update function (does nothing)
void EnvTetris::update(const DNA* inputDNA) {
}

// Expain the DNA
void EnvTetris::explain(const DNA* iDNA) {
    try {
        mParser->validate(*iDNA);
        mParser->evaluate(*iDNA);
    }
    catch (Exception e) {
        std::cout << "! Initial DNA invalid" << std::endl << e << std::endl;
    }
    mParser->print(std::cout, *iDNA);
}


/////////////
// GRAMMAR //
/////////////

//
// Board control
//

unsigned char ROTATE;
Value EnvTetris::rotate(std::vector<Value>) {
    mTetrisGame->rotate();
    mTetrisGame->DrawScene();
    return Value();
}

unsigned char LEFT;
Value EnvTetris::left(std::vector<Value>) {
    mTetrisGame->left();
    mTetrisGame->DrawScene();
    return Value();
}

unsigned char RIGHT;
Value EnvTetris::right(std::vector<Value>) {
    mTetrisGame->right();
    mTetrisGame->DrawScene();
    return Value();
}

unsigned char DOWN;
Value EnvTetris::down(std::vector<Value>) {
    mTetrisGame->down();
    mTetrisGame->DrawScene();
    return Value();
}

unsigned char DROP;
Value EnvTetris::drop(std::vector<Value>) {
    mTetrisGame->drop();
    mTetrisGame->DrawScene();
    return Value();
}


//
// Informational
//

unsigned char BLOCK_CURRENT;
Value EnvTetris::block_current(std::vector<Value>) {
    return mTetrisGame->getPieceCurrent();
}

unsigned char BLOCK_NEXT;
Value EnvTetris::block_next(std::vector<Value>) {
    return mTetrisGame->getPieceNext();
}

unsigned char POS_X;
Value EnvTetris::pos_x(std::vector<Value>) {
    return mTetrisGame->getX();
}

unsigned char POS_Y;
Value EnvTetris::pos_y(std::vector<Value>) {
    return mTetrisGame->getY();
}

unsigned char ROTATION;
Value EnvTetris::rotation(std::vector<Value>) {
    return mTetrisGame->getRotation();
}

unsigned char SIZE_X;
Value EnvTetris::size_x(std::vector<Value>) {
    return BOARD_WIDTH;
}

unsigned char SIZE_Y;
Value EnvTetris::size_y(std::vector<Value>) {
    return BOARD_HEIGHT;
}


//
// Tests
//


unsigned char IS_BLOCK;
Value EnvTetris::is_block(std::vector<Value> p) {
    int x = p[0].getInt();
    if (x < 0 || x >= BOARD_WIDTH)
        throw Exception(GENERIC, "x-coordinate invalid");

    int y = p[1].getInt();
    if (y < 0 || y >= BOARD_HEIGHT)
        throw Exception(GENERIC, "y-coordinate invalid");

    return (!mTetrisBoard->IsFreeBlock(x, y));
}

unsigned char IS_FREE;
Value EnvTetris::is_free(std::vector<Value> p) {
    int x = p[0].getInt();
    if (x < 0 || x >= BOARD_WIDTH)
        throw Exception(GENERIC, "x-coordinate invalid");

    int y = p[1].getInt();
    if (y < 0 || y >= BOARD_HEIGHT)
        throw Exception(GENERIC, "y-coordinate invalid");

    return mTetrisBoard->IsFreeBlock(x, y);
}


//
// Configuration
//

void EnvTetris::setup() {
    // Call parent
    SimpleGrammar::setup();

    // Board control
    ROTATE = setPointer(&EnvTetris::rotate, "rotate", {}, VOID);
    LEFT = setPointer(&EnvTetris::left, "left", {}, VOID);
    RIGHT = setPointer(&EnvTetris::right, "right", {}, VOID);
    DOWN = setPointer(&EnvTetris::down, "down", {}, VOID);
    DROP = setPointer(&EnvTetris::drop, "drop", {}, VOID);

    // Informational
    BLOCK_CURRENT = setPointer(&EnvTetris::block_current, "block_current", {}, INT);
    BLOCK_NEXT = setPointer(&EnvTetris::block_next, "block_next", {}, INT);
    POS_X = setPointer(&EnvTetris::pos_x, "pos_x", {}, INT);
    POS_Y = setPointer(&EnvTetris::pos_y, "pos_y", {}, INT);
    ROTATION = setPointer(&EnvTetris::rotation, "rotation", {}, INT);
    SIZE_X = setPointer(&EnvTetris::size_x, "size_x", {}, INT);
    SIZE_Y = setPointer(&EnvTetris::size_y, "size_y", {}, INT);

    // Tests
    IS_BLOCK = setPointer(&EnvTetris::is_block, "is_block", {INT, INT}, BOOL);
    IS_FREE = setPointer(&EnvTetris::is_block, "is_free", {INT, INT}, BOOL);

}

void EnvTetris::block() {
    // Call parent
    SimpleGrammar::block();
}

// Save the function locally
unsigned char EnvTetris::setPointer(Value (EnvTetris::*iPointer)(std::vector<Value>), std::string iName, std::initializer_list<Type> iParameters, Type iReturn) {
    unsigned char tByte;
    tByte = SimpleGrammar::createFunction(iName, std::vector<Type>(iParameters), iReturn);
    mPointers[tByte] = iPointer;
    return tByte;
}

// Execute a function
Value EnvTetris::executeFunction(unsigned char iByte, const std::vector<Value>& iParameters) {
    // Have we got a function definition?
    std::map<unsigned char, Value (EnvTetris::*)(std::vector<Value>)>::iterator it = mPointers.find(iByte);
    if (it != mPointers.end())
        return ((this)->*(mPointers[iByte]))(iParameters);
    else
        return SimpleGrammar::executeFunction(iByte, iParameters);
}


//////////
// MAIN //
//////////
// TODO: explain, per environment
int main() {
    // Create an environment
    EnvTetris tEnvironment;

    // Set-up an initial DNA string
    std::cout << "* Initial construct" << std::endl;
    DNA tDNA({
        INSTR_OPEN,
            DOWN, ARG_OPEN, ARG_CLOSE,
        INSTR_SEP,
            COND_IF,
            ARG_OPEN,
                RAND_BOOL, ARG_OPEN, ARG_CLOSE,
            ARG_CLOSE,
            INSTR_OPEN,
                LEFT, ARG_OPEN, ARG_CLOSE,
            INSTR_CLOSE,
            COND_ELSE,
            INSTR_OPEN,
                RIGHT, ARG_OPEN, ARG_CLOSE,
            INSTR_CLOSE,
        INSTR_SEP,
            COND_IF,
            ARG_OPEN,
                RAND_BOOL, ARG_OPEN, ARG_CLOSE,
            ARG_CLOSE,
            INSTR_OPEN,
                ROTATE, ARG_OPEN, ARG_CLOSE,
            INSTR_CLOSE,
        INSTR_CLOSE
    });
    tEnvironment.explain(&tDNA);
    
    
    // Create a population with initial DNA
    Population* tPopulation = new PopSingleStraight(&tEnvironment, tDNA);
    std::cout << "* Evolving" << std::endl;

    // Simulate
    try {
        tPopulation->evolve();
        tEnvironment.explain(tPopulation->get());
    }
    catch (std::string error) {
        std::cout << "Caught error: " << error << std::endl;
    }
    catch (Exception e) {
        std::cout << "FATAL EXCEPTION: " << e.what() << std::endl;
    }

    delete tPopulation;
    return 0;
}


