/*
 * tetris.cpp
 * Evolve - Tetris environment
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
#include "tetris.h"

/////////////
// GENERIC //
/////////////

//
// Construction and destruction
//

EnvTetris::EnvTetris() {
    // Set everything up
    setup();
    mCounter = 0;

    // Create a new parser
    mParser = new Parser(this);
}

EnvTetris::~EnvTetris() {
    delete(mParser);
}



/////////////////
// ENVIRONMENT //
/////////////////


// Alphabet (maximal amount of instructions)
int EnvTetris::alphabet() const {
    return 254;
}

// Fitness function
double EnvTetris::fitness(const DNA* inputDNA) {
    try {
        mParser->evaluate(*inputDNA);
    } catch (Exception e) {
        return 0;
    }
    std::cout << std::endl << std::endl << "-- FOUND VALID MUTATION --" << std::endl;
    explain(inputDNA);
    return ++mCounter;
}

// Condition (mutate 10 times
bool EnvTetris::condition() {
    return true;
}

// Update function (does nothing)
void EnvTetris::update(const DNA* inputDNA) {
}

// Expain the DNA
void EnvTetris::explain(const DNA* iDNA) {
    mParser->print(std::cout, *iDNA);
}


/////////////
// GRAMMAR //
/////////////

void EnvTetris::setup() {
    // Call parent
    SimpleGrammar::setup();
}

void EnvTetris::block() {
    // Call parent
    SimpleGrammar::block();
}

// Save the function locally
unsigned char EnvTetris::setPointer(Value (EnvTetris::*iPointer)(std::vector<Value>), std::string iName, std::initializer_list<Type> iParameters, Type iReturn) {
    unsigned char tByte;
    tByte = SimpleGrammar::createFunction(iName, std::vector<Type>(iParameters), iReturn);
    mPointers[tByte] = iPointer;
    return tByte;
}

// Execute a function
Value EnvTetris::executeFunction(unsigned char iByte, const std::vector<Value>& iParameters) {
    // Have we got a function definition?
    std::map<unsigned char, Value (EnvTetris::*)(std::vector<Value>)>::iterator it = mPointers.find(iByte);
    if (it != mPointers.end())
        return ((this)->*(mPointers[iByte]))(iParameters);
    else
        return SimpleGrammar::executeFunction(iByte, iParameters);
}


//////////
// MAIN //
//////////
// TODO: explain, per environment
int main() {
    // Create an environment
    EnvTetris tEnvironment;

    // Set-up an initial DNA string
    std::cout << "* Initial construct" << std::endl;
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
    tEnvironment.explain(&tDNA);

    // Create a population with initial DNA
    Population tPopulation(&tEnvironment, tDNA);
    std::cout << "* Evolving" << std::endl;

    // Simulate
    try {
        std::cout << "a" << std::endl;
        tPopulation.evolve_single_straight();
        std::cout << "b" << std::endl;
        tEnvironment.explain(tPopulation.get());
    }
    catch (std::string error) {
        std::cout << "Caught error: " << error << std::endl;
    }
    catch (Exception e) {
        std::cout << "FATAL EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}


