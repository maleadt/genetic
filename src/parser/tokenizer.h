/*
 * file.h
 * Project - Description
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
#ifndef __TOKENIZER
#define __TOKENIZER

// Headers
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "grammar.h"
#include "../dna.h"



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Tokenizer {
public:
    // Construction
    Tokenizer(Grammar* iGrammar);

    // Functionality
    DNA tokenize(const std::string iCode);

private:
    Grammar* mGrammar;
};


// Include guard
#endif
