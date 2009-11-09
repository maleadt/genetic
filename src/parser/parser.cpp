/*
 * parser.cpp
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

// Headers
#include "parser.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Parameterized constructor
Parser::Parser(Grammar* iGrammar) {
    mGrammar = iGrammar;
}



//
// Parsing
//

// Execute DNA
void Parser::execute(const DNA& iDNA) {
    // Execute all instructions
    for (unsigned int i = 0; i < iDNA.genes(); i++) {
        // Extract the gene
        unsigned char* tGene;
        unsigned int tSize;
        iDNA.extract_gene(i, tGene, tSize);

        // Execute the block
        evaluate_block(tGene, tSize);

        // Free the block
        free(tGene);
    }
}

void Parser::evaluate_block(unsigned char* iBlock, unsigned int iSize) {
    // Extract all instructions
    unsigned int tLoc = 0;
    std::vector<unsigned int> tInstructionBytecode = extract_instructions(iBlock, iSize, tLoc);
    
    // Give the grammar a chance to do some stuff (variable handling, ...)
    mGrammar->block();

    // Evaluate all instructions
    for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
        evaluate_instruction(iBlock, iSize, tInstructionBytecode[i]);
    }
}


//
// Auxiliary
//

void Parser::evaluate_conditional(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Save conditional for later evaluation
    unsigned char tConditional = iBlock[tLoc];

    // Extract the arguments
    tLoc++;
    std::vector<unsigned int> tParameterBytecode = extract_arguments(iBlock, iSize, tLoc);

    // Extract the (conditional) instructions
    std::vector<unsigned int> tInstructionBytecode = extract_instructions(iBlock, iSize, tLoc);

    // Check the evaluation type
    bool tConditionalExecution;
    switch (tConditional) {
        case COND_IF:
        {
            // Check amount of parameters
            if (tParameterBytecode.size() != 1)
                throw Exception(SYNTAX, "if-conditional only accepts one parameter");

            // Evaluate the parameter
            Value tTest = evaluate_instruction(iBlock, iSize, tParameterBytecode[0]);
            if (tTest.getType() != BOOL)
                throw Exception(SYNTAX, "test passed to if-conditional did not produce boolean value");

            // Evaluate the instructions
            if (tTest.getBool()) {
                for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
                    evaluate_instruction(iBlock, iSize, tInstructionBytecode[i]);
                }
            }
            break;
        }

        case COND_UNLESS:
        {
            // Check amount of parameters
            if (tParameterBytecode.size() != 1)
                throw Exception(SYNTAX, "if-conditional only accepts one parameter");

            // Evaluate the parameter
            Value tTest = evaluate_instruction(iBlock, iSize, tParameterBytecode[0]);
            if (tTest.getType() != BOOL)
                throw Exception(SYNTAX, "test passed to if-conditional did not produce boolean value");

            tConditionalExecution = !(tTest.getBool());
            break;
        }

        case COND_WHILE:
        {
            // Check amount of parameters
            if (tParameterBytecode.size() != 1)
                throw Exception(SYNTAX, "while-conditional only accepts one parameter");

            while (1) {
                // Evaluate the parameter
                unsigned int tLocCond = tParameterBytecode[0];
                Value tTest = evaluate_instruction(iBlock, iSize, tLocCond);
                if (tTest.getType() != BOOL)
                    throw Exception(SYNTAX, "test passed to if-conditional did not produce boolean value");

                // Evaluate the instructions
                if (tTest.getBool()) {
                    for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
                        unsigned int tLocInst = tInstructionBytecode[i];
                        evaluate_instruction(iBlock, iSize, tLocInst);
                    }
                } else {
                    break;
                }
            }
            break;
        }

        default:
        {
            throw Exception(GENERIC, "conditional clause not implemented");
        }
    }
}

Value Parser::evaluate_function(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Fetch the function
    const Function* tFunction = mGrammar->getFunction(iBlock[tLoc]);

    // Extract the arguments
    tLoc++;
    std::vector<unsigned int> tParameterBytecode = extract_arguments(iBlock, iSize, tLoc);

    // Evaluate all arguments
    std::vector<Value> tParameters;
    for (unsigned int i = 0; i < tParameterBytecode.size(); i++) {
        Value tParameter = evaluate_instruction(iBlock, iSize, tParameterBytecode[i]);
        if (tParameter.getType() == VOID)
            throw Exception(FUNCTION, "function parameter returned void");
        tParameters.push_back(tParameter);
    }

    // Call the function
    return tFunction->call(tParameters);
}

Value Parser::evaluate_data(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    unsigned char tDataType = iBlock[tLoc++];
    switch (tDataType) {
        case DATA_VOID:
        {
            return VOID;
            break;
        }
        case DATA_BOOL:
        {
            if (tLoc >= iSize)
                throw Exception(SYNTAX, "boolean type needs 1 byte of data");
            return toBool(iBlock[tLoc++]);
            break;
        }
        case DATA_INT:
        {
            if (tLoc >= iSize)
                throw Exception(SYNTAX, "integer type needs 1 byte of data");
            return toInt(iBlock[tLoc++]);
            break;
        }
        default:
        {
            throw Exception(SYNTAX, "unknown datatype");
        }
    }
}

Value Parser::evaluate_instruction(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Conditional
    if (mGrammar->isConditional(iBlock[tLoc])) {
        evaluate_conditional(iBlock, iSize, tLoc);
        return Value();
    }

    // Function
    else if (mGrammar->isFunction(iBlock[tLoc])) {
        return evaluate_function(iBlock, iSize, tLoc);
    }

    // Data
    else if (mGrammar->isData(iBlock[tLoc])) {
        return evaluate_data(iBlock, iSize, tLoc);
    } else {
        throw Exception(SYNTAX, "unknown byte identifier");
    }

    
    return Value();
}

std::vector<unsigned int> Parser::extract_syntax(std::initializer_list<unsigned char> iList, unsigned char* iBlock, unsigned int iSize, unsigned int& iLoc) {
    // Read syntaxis data
    if (iList.size() != 3)
        throw Exception(GENERIC, "syntaxis extraction needs exactly three parameters");
    unsigned char OPEN = *(iList.begin());
    unsigned char SEPARATE = *(iList.begin()+1);
    unsigned char CLOSE = *(iList.begin()+2);

    // Allocate new container
    std::vector<unsigned int> tItems;

    // Find opening arguments bracket
    if (iLoc >= iSize || iBlock[iLoc] != OPEN)
        throw Exception(SYNTAX, "could not find opening arguments bracket");

    // Extract all arguments
    unsigned int tLocItem = 0;
    unsigned int tBracketBalance = 0;
    while (iLoc < iSize) {
        // Start of argument
        if (iBlock[iLoc] == OPEN) {
            tBracketBalance++;
            if (tBracketBalance == 1) {
                tLocItem = iLoc+1;
            }
        }

        // Argument separator
        else if (iBlock[iLoc] == SEPARATE) {
            if (tBracketBalance == 1) {
                if (tLocItem == 0)
                    throw Exception(SYNTAX, "item seperator cannot occur before any item");
                tItems.push_back(tLocItem);
                tLocItem = iLoc+1;
            }
        }

        // End of argument
        else if (iBlock[iLoc] == CLOSE) {
            if (tBracketBalance == 1) {
                if (tLocItem != 0)
                    tItems.push_back(tLocItem);
            }
            tBracketBalance--;
        }

        // Data
        else if (mGrammar->isData(iBlock[iLoc])) {
            iLoc++;
        }

        // Move pointer
        iLoc++;

        // End of argument parsing
        if (tBracketBalance == 0)
            break;
    }

    // Check if the brackets did match
    if (tBracketBalance != 0)
        throw Exception(SYNTAX, "could not find end of syntaxis list");

    return tItems;
}

// Extract arguments
std::vector<unsigned int> Parser::extract_arguments(unsigned char* iBlock, unsigned int iSize, unsigned int& iLoc) {
    return extract_syntax({ARG_OPEN, ARG_SEP, ARG_CLOSE}, iBlock, iSize, iLoc);
}

// Extract instructions
std::vector<unsigned int> Parser::extract_instructions(unsigned char* iBlock, unsigned int iSize, unsigned int& iLoc) {
    return extract_syntax({INSTR_OPEN, INSTR_SEP, INSTR_CLOSE}, iBlock, iSize, iLoc);
}


//
// Byte conversion
//

// Byte to bool
bool Parser::toBool(unsigned char iByte) {
    return iByte >= 128;
}

// Byte to int
int Parser::toInt(unsigned char iByte) {
    return (int) iByte;
}