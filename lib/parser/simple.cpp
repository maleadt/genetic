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

Value plus(std::vector<Value> p) {
    return p[0].getInt() + p[1].getInt();
}

Value min(std::vector<Value> p) {
    return p[0].getInt() - p[1].getInt();
}

Value mult(std::vector<Value> p) {
    return p[0].getInt() * p[1].getInt();
}

Value div(std::vector<Value> p) {
    return p[0].getInt() / p[1].getInt();
}


//
// Conditional
//

Value equals(std::vector<Value> p) {
    return p[0].getInt() == p[1].getInt();
}

Value lesser(std::vector<Value> p) {
    return p[0].getInt() < p[1].getInt();
}

Value greater(std::vector<Value> p) {
    return p[0].getInt() > p[1].getInt();
}


//
// Other
//

Value print(std::vector<Value> p) {
    std::cout << "Print: " << p[0].getInt();
    return VOID;
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
        tGrammar.createFunction(1, &plus, {INT, INT}, INT);
        tGrammar.createFunction(2, &min, {INT, INT}, INT);
        tGrammar.createFunction(3, &mult, {INT, INT}, INT);
        tGrammar.createFunction(4, &div, {INT, INT}, INT);

        // Conditional functions
        tGrammar.createFunction(5, &equals, {INT, INT}, BOOL);
        tGrammar.createFunction(6, &lesser, {INT, INT}, BOOL);
        tGrammar.createFunction(7, &greater, {INT, INT}, BOOL);

        // Other
        tGrammar.createFunction(8, &print, {INT}, VOID);
    }
    catch (Exception e) {
        std::cout << e << std::endl;
    }
}
