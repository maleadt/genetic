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

//////////////
// ROUTINES //
//////////////

//
// Mathematical
//

unsigned char MATH_PLUS;
Value plus(std::vector<Value> p) {
    return p[0].getInt() + p[1].getInt();
}

unsigned char MATH_MIN;
Value min(std::vector<Value> p) {
    return p[0].getInt() - p[1].getInt();
}

unsigned char MATH_MULT;
Value mult(std::vector<Value> p) {
    return p[0].getInt() * p[1].getInt();
}

unsigned char MATH_DIV;
Value div(std::vector<Value> p) {
    return p[0].getInt() / p[1].getInt();
}

unsigned char MATH_MOD;
Value mod(std::vector<Value> p) {
    return p[0].getInt() % p[1].getInt();
}


//
// Tests
//

unsigned char TEST_EQUALS;
Value equals(std::vector<Value> p) {
    return p[0].getInt() == p[1].getInt();
}

unsigned char TEST_INEQUALS;
Value inequals(std::vector<Value> p) {
    return p[0].getInt() != p[1].getInt();
}

unsigned char TEST_LESSER;
Value lesser(std::vector<Value> p) {
    return p[0].getInt() <= p[1].getInt();
}

unsigned char TEST_STRICTLESSER;
Value strictlesser(std::vector<Value> p) {
    return p[0].getInt() < p[1].getInt();
}

unsigned char TEST_GREATER;
Value greater(std::vector<Value> p) {
    return p[0].getInt() >= p[1].getInt();
}

unsigned char TEST_STRICTGREATER;
Value strictgreater(std::vector<Value> p) {
    return p[0].getInt() > p[1].getInt();
}


//
// Other
//

unsigned char OTHER_PRINT;
Value print(std::vector<Value> p) {
    std::cout << "Print: " << p[0].getInt() << std::endl;
    return Value();
}



//////////////////////
// CLASS DEFINITION //
//////////////////////

class SimpleGrammar : public Grammar {
public:
    // Grammar setup
    void setup();
    
};

////////////////////
// CLASS ROUTINES //
////////////////////

void SimpleGrammar::setup() {
    // Parent setup
    Grammar::setup();

    // Mathematical functions
    MATH_PLUS = createFunction(&plus, {INT, INT}, INT);
    MATH_MIN = createFunction(&min, {INT, INT}, INT);
    MATH_MULT = createFunction(&mult, {INT, INT}, INT);
    MATH_DIV = createFunction(&div, {INT, INT}, INT);

    // Test functions
    TEST_EQUALS = createFunction(&equals, {INT, INT}, BOOL);
    TEST_INEQUALS = createFunction(&inequals, {INT, INT}, BOOL);
    TEST_LESSER = createFunction(&lesser, {INT, INT}, BOOL);
    TEST_STRICTLESSER = createFunction(&strictlesser, {INT, INT}, BOOL);
    TEST_GREATER = createFunction(&greater, {INT, INT}, BOOL);
    TEST_STRICTGREATER = createFunction(&strictgreater, {INT, INT}, BOOL);

    // Other
    OTHER_PRINT = createFunction(&print, {INT}, VOID);
}


// Include guard
#endif
