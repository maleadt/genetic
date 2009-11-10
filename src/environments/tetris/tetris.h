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

// Headers
#include "../../dna.h"
#include "../../population.h"
#include "../../environment.h"
#include "../../parser/grammars/simple.h"
#include "../../parser/parser.h"
#include "../../parser/value.h"
#include "../../parser/type.h"
#include "../../parser/exception.h"
#include <initializer_list>
#include <vector>
#include <map>



//////////////////////
// CLASS DEFINITION //
//////////////////////

class EnvTetris : public Environment, public SimpleGrammar {
public:
    // Constructor
    EnvTetris();
    ~EnvTetris();

    // Environment functionality
    double fitness(const DNA*);
    int alphabet() const;
    void update(const DNA*);
    bool condition();
    void explain(const DNA*);

    // Grammar functionality
    void setup();
    void block();
    Value executeFunction(unsigned char, const std::vector<Value>&);

private:
    unsigned int mCounter;
    
    // Grammar functionality
    Parser* mParser;
    std::map<unsigned char, Value (EnvTetris::*)(std::vector<Value>)> mPointers;
    unsigned char setPointer(Value (EnvTetris::*)(std::vector<Value>), std::string, std::initializer_list<Type>, Type);
};


// Include guard
#endif
