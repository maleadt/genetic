/*
 * grammar.h
 * Evolve - Code parser (code grammar description)
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
#ifndef __GRAMMAR
#define __GRAMMAR

// Headers
#include <map>
#include "type.h"
#include "value.h"
#include "function.h"
#include "exception.h"



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Grammar {
public:
    // Construction and destruction
    Grammar();

    // Setters
    void setFunction(unsigned char, const Function*);

    // Getters
    const Function* getFunction(unsigned char);

private:
    std::map<unsigned char, const Function*> mFunctions;
};


// Include guard
#endif
