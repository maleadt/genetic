/*
 * grammar.cpp
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

// Headers
#include "grammar.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Default constructor
Grammar::Grammar() {
    
}


//
// Setters
//

// Functions
void Grammar::setFunction(unsigned char iByte, const Function* iFunction) {
    // We won't overwrite an existing function
    if (mFunctions.find(iByte) != mFunctions.end()) {
        throw Exception(FUNCTION, "cannot overwrite existing function");
    }

    // Save the function
    mFunctions[iByte] = iFunction;
}


//
// Getters
//

// Functions
const Function* Grammar::getFunction(unsigned char iByte) {
    // The function must be defined
    std::map<unsigned char, const Function*>::iterator it = mFunctions.find(iByte);
    if (it == mFunctions.end()) {
        throw Exception(FUNCTION, "unknown function");
    }

    return it->second;
}