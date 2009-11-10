/*
 * function.h
 * Evolve - Code parser (function datastructure)
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
#ifndef __FUNCTION
#define __FUNCTION

// Headers
#include "type.h"
#include "value.h"
#include "exception.h"
#include "grammar.h"
#include <vector>
#include <iostream>
#include <initializer_list>
#include <string>



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Grammar;

class Function {
public:
    // Construction and destruction
    Function(std::string, std::vector<Type>, const Type&);

    // Data verification
    void checkParameters(const std::vector<Value>&) const;
    void checkReturn(const Value&) const;

    // Data IO
    std::string getName() const;

private:
    std::string mName;
    Type mReturnType;
    std::vector<Type> mParameterTypes;
};


// Include guard
#endif
