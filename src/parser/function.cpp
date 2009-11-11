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
Function::Function(std::string iName, std::vector<Type> iParameters, const Type& iReturn) {
    mName = iName;
    mReturnType = iReturn;
    mParameterTypes = iParameters;
}


//
// Data verification
//

// Check the parameters
void Function::checkParameters(const std::vector<Value>& iParameters) const {
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
}

// Check the return value
void Function::checkReturn(const Value& iReturn) const {
    // Check return value
    if (iReturn.getType() != mReturnType) {
        std::stringstream details;
        details << "function returned " << iReturn.getType() << " while function definition was " << mReturnType;
        throw Exception(FUNCTION, "function returned invalid value", details.str());
    }
}


//
// Data IO
//

std::string Function::getName() const {
    return mName;
}

unsigned int Function::getParameterCount() const {
    return mParameterTypes.size();
}