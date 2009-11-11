/*
 * parser.h
 * Evolve - Code parser (main class)
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
#ifndef __PARSER
#define __PARSER

// Headers
#include <iostream>
#include <stack>
#include <valarray>
#include <utility>
#include <initializer_list>
#include "grammar.h"
#include "../dna.h"


//////////////////////
// CLASS DEFINITION //
//////////////////////

class Parser {
public:
    // Construction and destruction
    Parser(Grammar*);

    // Main functionality
    void validate(const DNA&);
    void evaluate(const DNA&);
    void print(std::ostream&, const DNA&);

private:
    // Validation helpers
    void validate_block(unsigned char*, unsigned int);
    void validate_instruction(unsigned char*, unsigned int, unsigned int&);
    void validate_conditional(unsigned char*, unsigned int, unsigned int&);
    void validate_function(unsigned char*, unsigned int, unsigned int&);
    void validate_data(unsigned char*, unsigned int, unsigned int&);

    // Evaluation helpers
    void evaluate_block(unsigned char*, unsigned int);
    Value evaluate_instruction(unsigned char*, unsigned int, unsigned int&);
    void evaluate_conditional(unsigned char*, unsigned int, unsigned int&);
    Value evaluate_function(unsigned char*, unsigned int, unsigned int&);
    Value evaluate_data(unsigned char*, unsigned int, unsigned int&);

    // Output helpers
    void print_block(std::ostream&, unsigned char*, unsigned int);
    void print_indentation(std::ostream&, unsigned int);
    void print_newline(std::ostream&, unsigned int);
    
    // Auxiliary functions
    std::vector<std::pair<unsigned int, unsigned int> > extract_syntax(std::initializer_list<unsigned char>, unsigned char*, unsigned int, unsigned int&);
    std::vector<std::pair<unsigned int, unsigned int> > extract_arguments(unsigned char*, unsigned int, unsigned int&);
    std::vector<std::pair<unsigned int, unsigned int> > extract_instructions(unsigned char*, unsigned int, unsigned int&);
    
    // Byte conversion
    bool toBool(unsigned char);
    int toInt(unsigned char);

    // Data members
    Grammar* mGrammar;
};


// Include guard
#endif
