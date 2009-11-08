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

// Parameterisized constructor -- return value and parameters
Function::Function(Value (*iPointer)(std::vector<Value>), std::initializer_list<Type> iParameters, const Type& iReturn) {
    mPointer = iPointer;
    mReturnType = iReturn;
    mParameterTypes = iParameters;
}


//
// Function execution
//

// Call with parameters
Value Function::call(std::initializer_list<Value> iParameters) const {
    return call(std::vector<Value>(iParameters));
}
Value Function::call(const std::vector<Value>& iParameters) const {
    // Check input parameters
    bool tParameterFailure = false;
    if (mParameterTypes.size() != iParameters.size())
        tParameterFailure = true;
    else {
        for (unsigned int i = 0; i < iParameters.size() && !tParameterFailure; i++) {
            if (iParameters[i].getType() != mParameterTypes[i])
                tParameterFailure = true;
        }
    }
    if (tParameterFailure) {
        std::stringstream details;
        details << "got called with [";
        for (unsigned int j = 0; j < iParameters.size(); j++) {
            details << iParameters[j].getType();
            if (j < iParameters.size()-1)
                details << ", ";
        }
        details << "] while function signature is [";
        for (unsigned int j = 0; j < mParameterTypes.size(); j++) {
            details << mParameterTypes[j];
            if (j < mParameterTypes.size()-1)
                details << ", ";
        }
        details << "]";
        throw Exception(FUNCTION, "input parameters did not respect function signature", details.str());
    }

    // Call function
    Value tReturn = (*mPointer)(iParameters);

    // Check return value
    if (tReturn.getType() != mReturnType) {
        std::stringstream details;
        details << "function returned " << tReturn.getType() << " while function definition was " << mReturnType;
        throw Exception(FUNCTION, "function returned invalid value", details.str());
    }
    return tReturn;
}