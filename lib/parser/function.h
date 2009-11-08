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
#include <vector>
#include <iostream>
#include <initializer_list>



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Function {
public:
    // Construction and destruction
    Function(Value (*)(std::vector<Value>));
    Function(Value (*)(std::vector<Value>), const Type&);
    Function(Value (*)(std::vector<Value>), std::initializer_list<Type>);
    Function(Value (*)(std::vector<Value>), std::initializer_list<Type>, const Type&);

    // Function execution
    Value call() const;
    Value call(const std::initializer_list<Value>&) const;
    Value call(const std::vector<Value>&) const;

private:
    Value (*mPointer)(std::vector<Value>);
    Type mReturnType;
    std::vector<Type> mParameterTypes;
};


// Include guard
#endif
