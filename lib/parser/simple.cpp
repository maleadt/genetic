/*
 * file.cpp
 * Project - Description
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
#include "../../src/dna.h"
#include "parser.h"
#include "grammar.h"
#include "value.h"
#include "function.h"
#include <vector>


//////////////
// ROUTINES //
//////////////

//
// Mathematical
//

const int MATH_PLUS = RESERVED_END;
Value plus(std::vector<Value> p) {
    return p[0].getInt() + p[1].getInt();
}

const int MATH_MIN = RESERVED_END+1;
Value min(std::vector<Value> p) {
    return p[0].getInt() - p[1].getInt();
}

const int MATH_MULT = RESERVED_END+2;
Value mult(std::vector<Value> p) {
    return p[0].getInt() * p[1].getInt();
}

const int MATH_DIV = RESERVED_END+3;
Value div(std::vector<Value> p) {
    return p[0].getInt() / p[1].getInt();
}


//
// Tests
//

const int TEST_EQUALS = RESERVED_END+10;
Value equals(std::vector<Value> p) {
    return p[0].getInt() == p[1].getInt();
}

const int TEST_LESSER = RESERVED_END+11;
Value lesser(std::vector<Value> p) {
    return p[0].getInt() < p[1].getInt();
}

const int TEST_GREATER = RESERVED_END+12;
Value greater(std::vector<Value> p) {
    return p[0].getInt() > p[1].getInt();
}


//
// Other
//

const int OTHER_PRINT = RESERVED_END+20;
Value print(std::vector<Value> p) {
    std::cout << "Print: " << p[0].getInt() << std::endl;
    // FIXME: THIS IS WRONG return VOID;
    return Value();
}


//////////
// MAIN //
//////////

int main() {
    try {
        //
        // Grammar setup
        //

        // Create grammar
        Grammar tGrammar;

        // Mathematical functions
        tGrammar.createFunction(MATH_PLUS, &plus, {INT, INT}, INT);
        tGrammar.createFunction(MATH_MIN, &min, {INT, INT}, INT);
        tGrammar.createFunction(MATH_MULT, &mult, {INT, INT}, INT);
        tGrammar.createFunction(MATH_DIV, &div, {INT, INT}, INT);

        // Test functions
        tGrammar.createFunction(TEST_EQUALS, &equals, {INT, INT}, BOOL);
        tGrammar.createFunction(TEST_LESSER, &lesser, {INT, INT}, BOOL);
        tGrammar.createFunction(TEST_GREATER, &greater, {INT, INT}, BOOL);

        // Other
        tGrammar.createFunction(OTHER_PRINT, &print, {INT}, VOID);



        //
        // Code creation
        //

        unsigned char tBytecode[] = {
        OTHER_PRINT,
            ARG_OPEN,
                DATA_INT, 5,
            ARG_CLOSE
        };
        DNA tDNA(tBytecode, 5);
        
        
        //
        // Parser setup
        //
        
        Parser tParser(&tGrammar);


        //
        // Code execution
        //

        tParser.execute(tDNA);


    }
    catch (Exception e) {
        std::cout << e << std::endl;
    }
}
