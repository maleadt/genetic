/*
 * value.cpp
 * Evolve - Code parser (value container)
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
#include "value.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction
//

// Default constructor
Value::Value() {
    mType = Type(VOID);
}

// Parameterized constructor -- boolean
Value::Value(bool iBool) {
    mType = Type(BOOL);
    mBool = iBool;
}

// Parameterized constructor -- integer
Value::Value(int iInt) {
    mType = Type(INT);
    mInt = iInt;
}

// Copy constructor
Value::Value(const Value& iValue) {
    mType = Type(iValue.mType);
    if (mType == BOOL)
        mBool = iValue.getBool();
    else if (mType == INT)
        mInt = iValue.getInt();
}


//
// Getters
//

// Get the value type
const Type& Value::getType() const {
    return mType;
}

// Get the boolean value
const bool& Value::getBool() const {
    return mBool;
}

// Get the integer value
const int& Value::getInt() const {
    return mInt;
}



//
// Operators
//

// Equality
bool Value::operator==(const Value& v) const {
    if (v.getType() != mType)
        return false;

    switch (mType.getEnum()) {
        case VOID:
            return true;
            break;
        case BOOL:
            return mBool == v.getBool();
            break;
        case INT:
            return mInt == v.getInt();
            break;
    }
}

// Inequality
bool Value::operator!=(const Value& v) const {
    return !(*this != v);
}

// Output
std::ostream& operator<<(std::ostream& out, const Value& v) {
    switch (v.getType().getEnum()) {
        case VOID:
            out << "(void)";
            break;
        case BOOL:
            out << "(bool) " << (v.getBool()?"true":"false");
            break;
        case INT:
            out << "(int) " << v.getInt();
            break;
    }
    return out;
}