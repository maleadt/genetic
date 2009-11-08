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
        tGrammar.setFunction(1, new Function(&plus, {INT, INT}, INT));
        tGrammar.setFunction(2, new Function(&min, {INT, INT}, INT));
        tGrammar.setFunction(3, new Function(&mult, {INT, INT}, INT));
        tGrammar.setFunction(4, new Function(&div, {INT, INT}, INT));

        // Conditional functions
        tGrammar.setFunction(5, new Function(&equals, {INT, INT}, BOOL));
        tGrammar.setFunction(6, new Function(&lesser, {INT, INT}, BOOL));
        tGrammar.setFunction(7, new Function(&greater, {INT, INT}, BOOL));

        // Other
        tGrammar.setFunction(8, new Function(&print, {INT}));
    }
    catch (Exception e) {
        std::cout << e << std::endl;
    }
}
