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
#include "simple.h"
#include "../parser.h"
#include "../../dna.h"





//////////
// MAIN //
//////////

int main() {
    try {
        //
        // Grammar setup
        //

        // Create grammar
        SimpleGrammar tGrammar;
        tGrammar.setup();


        //
        // Code creation
        //
// TODO: lege block
        DNA tDNA({
            INSTR_OPEN,
                // Set variable 1 to 0
                SET,
                    ARG_OPEN,
                        DATA_INT, 1,
                    ARG_SEP,
                        DATA_INT, 1,
                    ARG_CLOSE,
            INSTR_SEP,

                // While variable 1 < 10
                COND_WHILE,
                    ARG_OPEN,
                        TEST_LESSER,
                            ARG_OPEN,
                                GET,
                                    ARG_OPEN,
                                        DATA_INT, 1,
                                    ARG_CLOSE,
                            ARG_SEP,
                                DATA_INT, 10,
                            ARG_CLOSE,
                    ARG_CLOSE,
                INSTR_OPEN,
                    // Print value
                    OTHER_PRINT,
                        ARG_OPEN,
                            GET,
                                ARG_OPEN,
                                    DATA_INT, 1,
                                ARG_CLOSE,
                        ARG_CLOSE,
                INSTR_SEP,
                    // Increase with 1
                    SET,
                        ARG_OPEN,
                            DATA_INT, 1,
                        ARG_SEP,
                            MATH_PLUS,
                                ARG_OPEN,
                                    GET,
                                        ARG_OPEN,
                                            DATA_INT, 1,
                                        ARG_CLOSE,
                                ARG_SEP,
                                    DATA_INT, 1,
                                ARG_CLOSE,
                        ARG_CLOSE,
                INSTR_CLOSE,
            INSTR_CLOSE
        });
        
        
        //
        // Parser setup
        //
        
        Parser tParser(&tGrammar);


        //
        // Code execution
        //

        tParser.validate(tDNA);
        tParser.evaluate(tDNA);


        //
        // Code printing
        //

        tParser.print(std::cout, tDNA);


    }
    catch (Exception e) {
        std::cout << e << std::endl;
    }
}
