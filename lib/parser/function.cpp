/*
 * function.cpp
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

// Headers
#include "function.h"
#include <sstream>



////////////////////
// CLASS ROUTINES //
////////////////////


//
// Construction and destruction
//

// Parameterisized constructor
Function::Function(Value (*iPointer)(std::vector<Value>)) {
    mPointer = iPointer;
    mReturnType = Type(VOID);
}

// Parameterisized constructor -- only return value
Function::Function(Value (*iPointer)(std::vector<Value>), const Type& iReturn) {
    mPointer = iPointer;
    mReturnType = iReturn;
}

// Parameterisized constructor -- only parameters
Function::Function(Value (*iPointer)(std::vector<Value>), const std::vector<Type>& iArguments) {
    mPointer = iPointer;
    mParameterTypes = iArguments;
}

// Parameterisized constructor -- return value and parameters
Function::Function(Value (*iPointer)(std::vector<Value>), const Type& iReturn, const std::vector<Type>& iArguments) {
    mPointer = iPointer;
    mReturnType = iReturn;
    mParameterTypes = iArguments;
}


//
// Function execution
//

// Call without parameters
Value Function::call() const {
    // Check parameters
    if (mParameterTypes.size() != 0)
        throw Exception(FUNCTION, "function needs parameters");

    // Call function
    std::vector<Value> tEmpty;
    Value tReturn = (*mPointer)(tEmpty);

    // Check return value
    if (tReturn.getType() != mReturnType) {
        std::stringstream error;
        error << "function returned " << tReturn << ", while I expected something of type " << mReturnType;
        throw Exception(FUNCTION, error.str());
    }
    return tReturn;
}

// Call with parameters
Value Function::call(const std::vector<Value>& iParameters) const {
    if (mParameterTypes.size() != iParameters.size())
        throw Exception(FUNCTION, "incorrect amount of parameters");
    for (int i = 0; i < iParameters.size(); i++) {
        if (iParameters[i].getType() != mParameterTypes[i]) {
            std::stringstream error;
            error << "parameter " << i << " is of type " << iParameters[i].getType() << ", while I expected " << mParameterTypes[i];
            throw Exception(FUNCTION, error.str());
        }
    }

    // Call function
    Value tReturn = (*mPointer)(iParameters);

    // Check return value
    if (tReturn.getType() != mReturnType)
        throw Exception(FUNCTION, "function returned invalid value");
    return tReturn;
}