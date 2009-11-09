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
Parser::Parser(const Grammar* iGrammar) {
    mGrammar = iGrammar;
}



//
// Parsing
//

// Execute DNA
void Parser::execute(const DNA& iDNA) {
    // Initialize variable scope
    mScope.clear();

    // Execute all instructions
    for (unsigned int i = 0; i < iDNA.genes(); i++) {
        // Extract the gene
        unsigned char* tGene;
        unsigned int tSize;
        iDNA.extract_gene(i, tGene, tSize);

        // Execute the block
        execute_instruction(tGene, tSize);

        // Free the block
        free(tGene);
    }
}

void Parser::execute_instruction(unsigned char* iBlock, unsigned int iSize) {
    unsigned int tLoc = 0;
    // TODO: tLoc member of Parser

    while (tLoc < iSize) {
        evaluate(iBlock, iSize, tLoc);
    }
}


//
// Auxiliary
//

Value Parser::evaluate(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Conditional //

    if (mGrammar->isConditional(iBlock[tLoc])) {
        // Save conditional for later evaluation
        unsigned char tConditional = iBlock[tLoc];

        // Extract the arguments
        unsigned int tStart = ++tLoc;
        unsigned int tEnd;
        std::vector<std::pair<unsigned int, unsigned int> > tParameterBytecode = extract_arguments(iBlock, iSize, tStart, tEnd);
        tLoc = tEnd;
        // TODO: let tLoc advance instead of tEnd

        // Check the evaluation type
        bool tConditionalExecution;
        switch (tConditional) {
            case COND_IF:
            {
                // Check amount of parameters
                if (tParameterBytecode.size() != 1)
                    throw Exception(SYNTAX, "if-conditional only accepts one parameter");

                // Evaluate the parameter now
                Value tTest = evaluate(iBlock, iSize, tParameterBytecode[0].first);
                // TODO: check if tLoc is at .second, if not garbage!
                if (tTest.getType() != BOOL)
                    throw Exception(SYNTAX, "test passed to if-conditional did not produce boolean value");

                tConditionalExecution = tTest.getBool();
                break;
            }

            case COND_UNLESS:
            {
                // Check amount of parameters
                if (tParameterBytecode.size() != 1)
                    throw Exception(SYNTAX, "if-conditional only accepts one parameter");

                // Evaluate the parameter now
                Value tTest = evaluate(iBlock, iSize, tParameterBytecode[0].first);
                // TODO: check if tLoc is at .second, if not garbage!
                if (tTest.getType() != BOOL)
                    throw Exception(SYNTAX, "test passed to if-conditional did not produce boolean value");

                tConditionalExecution = !(tTest.getBool());
                break;
            }

            default:
            {
                throw Exception(GENERIC, "conditional clause not implemented");
            }
        }

        // Do we need to execute code?
        if (tConditionalExecution) {
            // Extract the instructions
            unsigned int tStart = tLoc;
            unsigned int tEnd;
            std::vector<std::pair<unsigned int, unsigned int> > tInstructionBytecode = extract_instructions(iBlock, iSize, tStart, tEnd);

            // Evaluate the instructions
            for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
                // TODO: second value unused, evaluate parses untill bracket balance == 0. Instruction seperator?
                evaluate(iBlock, iSize, tInstructionBytecode[i].first);
            }
            
            // Set the pointer past the instructions
            tLoc = tEnd;
        }

        // TODO: return something here, e.v. last function RV or false if cond
        return Value();
    }

    // Function //

    else if (mGrammar->isFunction(iBlock[tLoc])) {
        // Fetch the function
        const Function* tFunction = mGrammar->getFunction(iBlock[tLoc]);

        // Extract the arguments
        unsigned int tStart = ++tLoc;
        unsigned int tEnd;
        std::vector<std::pair<unsigned int, unsigned int> > tParameterBytecode = extract_arguments(iBlock, iSize, tStart, tEnd);

        // Evaluate all arguments
        std::vector<Value> tParameters;
        for (unsigned int i = 0; i < tParameterBytecode.size(); i++) {
            Value tParameter = evaluate(iBlock, iSize, tParameterBytecode[i].first);
                // TODO: check if tLoc is at .second, if not garbage!
            tParameters.push_back(tParameter);
            // TODO: warn if void on stack, cannot be used as argument
        }
        
        // TODO 2: Conditional type. MEt function als argument. IPV recursief uitvoeren, recursief tree opstellen, dan tree uitvoeren

        // Save the pointer
        tLoc = tEnd;

        // Call the function
        return tFunction->call(tParameters);
    }

    // Data //
    
    else if (mGrammar->isData(iBlock[tLoc])) {
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
        }
    } else {
        throw Exception(SYNTAX, "unknown byte identifier");
    }

    
    return Value();
}

std::vector<std::pair<unsigned int, unsigned int> > Parser::extract_syntax(std::initializer_list<unsigned char> iList, unsigned char* iBlock, unsigned int iSize, unsigned int iStart, unsigned int& iEnd) {
    // Read syntaxis data
    if (iList.size() != 3)
        throw Exception(GENERIC, "syntaxis extraction needs exactly three parameters");
    unsigned char OPEN = *(iList.begin());
    unsigned char SEPARATE = *(iList.begin()+1);
    unsigned char CLOSE = *(iList.begin()+2);

    // Allocate new argument container
    std::vector<std::pair<unsigned int, unsigned int> > tArguments;
    unsigned int tLoc = iStart;

    // Find opening arguments bracket
    if (tLoc >= iSize || iBlock[tLoc] != OPEN)
        throw Exception(SYNTAX, "could not find opening arguments bracket");

    // Extract all arguments
    unsigned int tBracketBalance = 0;
    while (tLoc < iSize) {
        // Start of argument
        if (iBlock[tLoc] == OPEN) {
            tBracketBalance++;
            if (tBracketBalance == 1) {
                tArguments.push_back(std::pair<int, int>(tLoc+1, 0));
            }
        }

        // Argument separator
        else if (iBlock[tLoc] == SEPARATE) {
            if (tBracketBalance == 1) {
                tArguments.back().second = tLoc;
                tArguments.push_back(std::pair<int, int>(tLoc+1, 0));
            }
        }

        // End of argument
        else if (iBlock[tLoc] == CLOSE) {
            if (tBracketBalance == 1) {
                tArguments.back().second = tLoc;
            }
            tBracketBalance--;
        }

        // Data
        else if (mGrammar->isData(iBlock[tLoc])) {
            tLoc++;
        }

        // Move pointer
        tLoc++;

        // End of argument parsing
        if (tBracketBalance == 0)
            break;
    }

    // Check if the brackets did match
    if (tBracketBalance != 0)
        throw Exception(SYNTAX, "could not find closing arguments bracket");
    iEnd = tLoc;

    return tArguments;
}

// Extract arguments
std::vector<std::pair<unsigned int, unsigned int> > Parser::extract_arguments(unsigned char* iBlock, unsigned int iSize, unsigned int iStart, unsigned int& iEnd) {
    return extract_syntax({ARG_OPEN, ARG_SEP, ARG_CLOSE}, iBlock, iSize, iStart, iEnd);
}

// Extract instructions
std::vector<std::pair<unsigned int, unsigned int> > Parser::extract_instructions(unsigned char* iBlock, unsigned int iSize, unsigned int iStart, unsigned int& iEnd) {
    return extract_syntax({INSTR_OPEN, INSTR_SEP, INSTR_CLOSE}, iBlock, iSize, iStart, iEnd);
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