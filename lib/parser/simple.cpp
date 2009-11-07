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

void test1() {
    std::cout << "This is test1" << std::endl;
}


//////////////
// WRAPPERS //
//////////////

Value test1_wrapper(std::vector<Value> param) {
    test1();
    return Value();
}


//////////
// MAIN //
//////////

int main() {
    try {
        Grammar tGrammar;
        tGrammar.setFunction(1, new Function(&test1_wrapper));

        const Function* tFunction = tGrammar.getFunction(1);
        tFunction->call();
    }
    catch (Exception e) {
        std::cout << e << std::endl;
    }
}
