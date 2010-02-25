/*
 * tetris.h
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

// Include guard
#ifndef __TETRIS
#define __TETRIS

// Headers -- Genetic
#include "../../dna.h"
#include "../../population.h"
#include "../../populations/singlestraight.h"
#include "../../environment.h"

// Headers -- Parser
#include "../../parser/grammars/simple.h"
#include "../../parser/parser.h"
#include "../../parser/value.h"
#include "../../parser/type.h"
#include "../../parser/exception.h"

// Headers -- Tetris
#include "game.h"
#include "pieces.h"
#include "output.h"
#include "board.h"

// Headers -- System
#include <initializer_list>
#include <vector>
#include <map>


//
// Constants
//

// Per-gene instruction limit
const unsigned long LIMIT_INSTRUCTIONS = 10000;
const unsigned long LIMIT_RUNS = 10000;
const unsigned long RUNS = 100;
const unsigned int GAME_DROPDELAY = 1000;
const unsigned int GAME_USERDELAY = 1000;
const unsigned int GAME_SPEED = 5;



//////////////////////
// CLASS DEFINITION //
//////////////////////

class EnvTetris : public Environment, public SimpleGrammar {
public:
    // Constructor
    EnvTetris();
    ~EnvTetris();

    // Environment functionality
    double fitness(const DNA&);
    int alphabet() const;
    void update(const DNA&);
    bool condition();
    void explain(const DNA&);

    // Grammar functionality
    void setup();
    void block();
    Value executeFunction(unsigned char, const std::vector<Value>&);

protected:
    // Board control
    Value rotate(std::vector<Value>);
    Value left(std::vector<Value>);
    Value right(std::vector<Value>);
    Value down(std::vector<Value>);
    Value drop(std::vector<Value>);

    // Informational
    Value block_current(std::vector<Value>);
    Value block_next(std::vector<Value>);
    Value pos_x(std::vector<Value>);
    Value pos_y(std::vector<Value>);
    Value rotation(std::vector<Value>);
    Value size_x(std::vector<Value>);
    Value size_y(std::vector<Value>);

    // Tests
    Value is_block(std::vector<Value>);
    Value is_free(std::vector<Value>);

private:    
    // Grammar functionality
    Parser* mParser;
    std::map<unsigned char, Value (EnvTetris::*)(std::vector<Value>)> mPointers;
    unsigned char setPointer(Value (EnvTetris::*)(std::vector<Value>), std::string, std::initializer_list<Type>, Type);


    // Tetris functionality
    Output mTetrisOutput;
    Pieces mTetrisPieces;
    Board* mTetrisBoard;
    Game* mTetrisGame;
};


// Include guard
#endif
