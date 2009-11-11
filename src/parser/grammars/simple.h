/*
 * grammar.h
 * Evolve - Code parser (simple grammar)
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
#ifndef __SIMPLE
#define __SIMPLE

// Headers
#include "../grammar.h"
#include <map>



//////////////////////
// CLASS DEFINITION //
//////////////////////

class SimpleGrammar : public Grammar {
public:
    // Grammar setup
    virtual void setup();
    virtual void block();
    virtual Value executeFunction(unsigned char, const std::vector<Value>&);

protected:
    // Variable handling
    Value get(std::vector<Value> p);
    Value set(std::vector<Value> p);

    // Mathematical
    Value plus(std::vector<Value> p);
    Value min(std::vector<Value> p);
    Value mult(std::vector<Value> p);
    Value div(std::vector<Value> p);
    Value mod(std::vector<Value> p);

    // Tests
    Value equals(std::vector<Value> p);
    Value inequals(std::vector<Value> p);
    Value greater(std::vector<Value> p);
    Value strictgreater(std::vector<Value> p);
    Value lesser(std::vector<Value> p);
    Value strictlesser(std::vector<Value> p);

    // Other
    Value print(std::vector<Value> p);

private:
    unsigned char setPointer(Value (SimpleGrammar::*)(std::vector<Value>), std::string, std::initializer_list<Type>, Type);
    std::map<unsigned char, Value (SimpleGrammar::*)(std::vector<Value>)> mPointers;
};


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Variable handling
//

// Static scope
std::map<unsigned int, Value> mScope;

unsigned char GET;
Value SimpleGrammar::get(std::vector<Value> p) {
    // The variable must be defined
    std::map<unsigned int, Value>::const_iterator it = mScope.find(p[0].getInt());
    if (it == mScope.end()) {
        throw Exception(FUNCTION, "unknown variable");
    }

    return it->second;
}

unsigned char SET;
Value SimpleGrammar::set(std::vector<Value> p) {
    // Define the variable
    mScope[p[0].getInt()] = p[1];

    return Value();
}


//
// Function handling
//

// Save the function locally
unsigned char SimpleGrammar::setPointer(Value (SimpleGrammar::*iPointer)(std::vector<Value>), std::string iName, std::initializer_list<Type> iParameters, Type iReturn) {
    unsigned char tByte;
    tByte = createFunction(iName, std::vector<Type>(iParameters), iReturn);
    mPointers[tByte] = iPointer;
    return tByte;
}

// Execute a function
Value SimpleGrammar::executeFunction(unsigned char iByte, const std::vector<Value>& iParameters) {
    // Have we got a function definition?
    std::map<unsigned char, Value (SimpleGrammar::*)(std::vector<Value>)>::iterator it = mPointers.find(iByte);
    if (it != mPointers.end())
        return ((this)->*(mPointers[iByte]))(iParameters);
    else
        return SimpleGrammar::executeFunction(iByte, iParameters);
}

//
// Mathematical
//

unsigned char MATH_PLUS;
Value SimpleGrammar::plus(std::vector<Value> p) {
    return p[0].getInt() + p[1].getInt();
}

unsigned char MATH_MIN;
Value SimpleGrammar::min(std::vector<Value> p) {
    return p[0].getInt() - p[1].getInt();
}

unsigned char MATH_MULT;
Value SimpleGrammar::mult(std::vector<Value> p) {
    return p[0].getInt() * p[1].getInt();
}

unsigned char MATH_DIV;
Value SimpleGrammar::div(std::vector<Value> p) {
    return p[0].getInt() / p[1].getInt();
}

unsigned char MATH_MOD;
Value SimpleGrammar::mod(std::vector<Value> p) {
    return p[0].getInt() % p[1].getInt();
}


//
// Tests
//

unsigned char TEST_EQUALS;
Value SimpleGrammar::equals(std::vector<Value> p) {
    return p[0].getInt() == p[1].getInt();
}

unsigned char TEST_INEQUALS;
Value SimpleGrammar::inequals(std::vector<Value> p) {
    return p[0].getInt() != p[1].getInt();
}

unsigned char TEST_LESSER;
Value SimpleGrammar::lesser(std::vector<Value> p) {
    return p[0].getInt() <= p[1].getInt();
}

unsigned char TEST_STRICTLESSER;
Value SimpleGrammar::strictlesser(std::vector<Value> p) {
    return p[0].getInt() < p[1].getInt();
}

unsigned char TEST_GREATER;
Value SimpleGrammar::greater(std::vector<Value> p) {
    return p[0].getInt() >= p[1].getInt();
}

unsigned char TEST_STRICTGREATER;
Value SimpleGrammar::strictgreater(std::vector<Value> p) {
    return p[0].getInt() > p[1].getInt();
}


//
// Other
//

unsigned char OTHER_PRINT;
Value SimpleGrammar::print(std::vector<Value> p) {
    std::cout << "Print: " << p[0].getInt() << std::endl;
    return Value();
}


//
// Configuration
//


void SimpleGrammar::block() {
    // Call parent
    Grammar::block();

    // Reset the scope
    mScope.clear();
}

void SimpleGrammar::setup() {
    // Call parent
    Grammar::setup();

    // Variable handling
    GET = setPointer(&SimpleGrammar::get, "get", {INT}, INT);
    SET = setPointer(&SimpleGrammar::set, "set", {INT, INT}, VOID);

    // Mathematical functions
    MATH_PLUS = setPointer(&SimpleGrammar::plus, "plus", {INT, INT}, INT);
    MATH_MIN = setPointer(&SimpleGrammar::min, "min", {INT, INT}, INT);
    MATH_MULT = setPointer(&SimpleGrammar::mult, "mult", {INT, INT}, INT);
    MATH_DIV = setPointer(&SimpleGrammar::div, "div", {INT, INT}, INT);

    // Test functions
    TEST_EQUALS = setPointer(&SimpleGrammar::equals, "equals", {INT, INT}, BOOL);
    TEST_INEQUALS = setPointer(&SimpleGrammar::inequals, "inequals", {INT, INT}, BOOL);
    TEST_LESSER = setPointer(&SimpleGrammar::lesser, "lesser", {INT, INT}, BOOL);
    TEST_STRICTLESSER = setPointer(&SimpleGrammar::strictlesser, "strict_lesser", {INT, INT}, BOOL);
    TEST_GREATER = setPointer(&SimpleGrammar::greater, "greater", {INT, INT}, BOOL);
    TEST_STRICTGREATER = setPointer(&SimpleGrammar::strictgreater, "strict_greater", {INT, INT}, BOOL);

    // Other
    OTHER_PRINT = setPointer(&SimpleGrammar::print, "print", {INT}, VOID);
}


// Include guard
#endif
