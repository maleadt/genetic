/*
 * type.cpp
 * Evolve - Code parser (type definition)
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
#include "type.h"



////////////////////
// CLASS ROUTINES //
////////////////////


//
// Construction and destruction
//

// Empty constructor -- void type
Type::Type() {
    mTypeVal = VOID;
}

// Parameterized constructor -- given type
Type::Type(TYPEVAL iTypeVal) {
    mTypeVal = iTypeVal;
}

// Copy constructor
Type::Type(const Type& iType) {
    mTypeVal = iType.mTypeVal;
}


//
// Operators
//

// Equality
bool Type::operator==(const Type& iType) const {
    return iType.mTypeVal == mTypeVal;
}
bool Type::operator==(const TYPEVAL& iTypeVal) const {
    return iTypeVal == mTypeVal;
}

// Inequality
bool Type::operator!=(const Type& iType) const {
    return iType.mTypeVal != mTypeVal;
}
bool Type::operator!=(const TYPEVAL& iTypeVal) const {
    return iTypeVal != mTypeVal;
}

// Output
std::ostream& operator<<(std::ostream& out, const Type& t) {
    switch (t.mTypeVal) {
        case VOID:
            out << "(void)";
            break;
        case BOOL:
            out << "(bool)";
            break;
        case INT:
            out << "(int)";
            break;
    }
    return out;
}


//
// Getters
//

// Get the enumeration type
TYPEVAL Type::getEnum() const {
    return mTypeVal;
}