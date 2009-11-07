/*
 * value.h
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

// Include guard
#ifndef __VALUE
#define __VALUE

// Headers
#include <fstream>
#include "type.h"



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Value {
public:
    // Construction
    Value();
    Value(bool);
    Value(int);
    Value(const Value&);

    // Getters
    const Type& getType() const;
    const bool& getBool() const;
    const int& getInt() const;

    // Operators
    bool operator==(const Value&) const;
    bool operator!=(const Value&) const;
    friend std::ostream& operator<<(std::ostream& out, const Value& v);

private:
    Type mType;
    bool mBool;
    int mInt;
};


// Include guard
#endif
