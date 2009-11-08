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
#include <list>
#include <initializer_list>
#include "type.h"
#include "value.h"
#include "function.h"
#include "exception.h"


//
// Reserved instructions
//

// Boundaries
const unsigned char RESERVED_START = 0x00;
const unsigned char RESERVED_END = 0x24;

// Syntax
const unsigned char SYNT_START = 0x01;
const unsigned char ARG_OPEN = 0x01;
const unsigned char ARG_SEP = 0x02;
const unsigned char ARG_CLOSE = 0x03;
const unsigned char DO = 0x04;
const unsigned char SYNT_END = 0x05;

// Conditionals
const unsigned char COND_START = 0x10;
const unsigned char COND_IF = 0x10;
const unsigned char COND_UNLESS = 0x11;
const unsigned char COND_WHILE = 0x12;
const unsigned char COND_END = 0x14;

// Data types
const unsigned char DATA_START = 0x20;
const unsigned char DATA_VOID = 0x21;
const unsigned char DATA_BOOL = 0x22;
const unsigned char DATA_INT = 0x23;
const unsigned char DATA_END = 0x24;



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Grammar {
public:
    // Construction and destruction
    Grammar();
    ~Grammar();

    // Grammar configuration
    virtual void setup()=0;

    // Functions
    unsigned char createFunction(Value (*)(std::vector<Value>), const std::initializer_list<Type>&, const Type&);
    void createFunction(unsigned char, Value (*)(std::vector<Value>), const std::initializer_list<Type>&, const Type&);
    void setFunction(unsigned char, const Function*);
    const Function* getFunction(unsigned char) const;
    void deleteFunction(unsigned char);

    // Test funcions
    bool isReserved(unsigned char) const;
    bool isFunction(unsigned char) const;
    bool isConditional(unsigned char) const;
    bool isData(unsigned char) const;

private:
    std::map<unsigned char, const Function*> mFunctions;
    std::list<const Function*> mCreatedFunctions;
};


// Include guard
#endif
