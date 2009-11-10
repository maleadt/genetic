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
#include <vector>
#include "type.h"
#include "value.h"
#include "function.h"
#include "exception.h"


//
// Reserved instructions
//

// Boundaries
const unsigned char RESERVED_START = 00;
const unsigned char RESERVED_END = 24;

// Syntax
const unsigned char SYNT_START = 01;
const unsigned char ARG_OPEN = 01;
const unsigned char ARG_SEP = 02;
const unsigned char ARG_CLOSE = 03;
const unsigned char INSTR_OPEN = 04;
const unsigned char INSTR_SEP = 05;
const unsigned char INSTR_CLOSE = 06;
const unsigned char SYNT_END = 07;

// Conditionals
const unsigned char COND_START = 10;
const unsigned char COND_IF = 10;
const unsigned char COND_UNLESS = 11;
const unsigned char COND_WHILE = 12;
const unsigned char COND_END = 13;

// Data types
const unsigned char DATA_START = 20;
const unsigned char DATA_VOID = 21;
const unsigned char DATA_BOOL = 22;
const unsigned char DATA_INT = 23;
const unsigned char DATA_END = 24;



//////////////////////
// CLASS DEFINITION //
//////////////////////

// Forward function declaration
class Function;

class Grammar {
public:
    // Construction and destruction
    Grammar();
    ~Grammar();

    // Configuration
    virtual void setup();
    virtual void block();

    // Function handling
    unsigned char createFunction(const std::vector<Type>&, const Type&);
    void createFunction(unsigned char, const std::vector<Type>&, const Type&);
    void setFunction(unsigned char, const Function*);
    const Function* getFunction(unsigned char) const;
    void deleteFunction(unsigned char);
    Value callFunction(unsigned char, const std::vector<Value>&);
    virtual Value executeFunction(unsigned char, const std::vector<Value>&) = 0;

    // Test funcions
    bool isReserved(unsigned char) const;
    bool isFunction(unsigned char) const;
    bool isConditional(unsigned char) const;
    bool isData(unsigned char) const;

private:
    bool mSetup;
    std::map<unsigned char, const Function*> mFunctions;
    std::list<const Function*> mCreatedFunctions;
};


// Include guard
#endif
