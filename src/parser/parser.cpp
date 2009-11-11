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
// Main functionality
//

// Evaluate DNA
void Parser::validate(const DNA& iDNA) {
    // Validate all instructions
    for (unsigned int i = 0; i < iDNA.genes(); i++) {
        // Extract the gene
        unsigned char* tGene;
        unsigned int tSize;
        iDNA.extract_gene(i, tGene, tSize);

        // Evaluate the block
        validate_block(tGene, tSize);

        // Free the block
        free(tGene);
    }
}



// Evaluate DNA
void Parser::evaluate(const DNA& iDNA) {
    // Evaluate all instructions
    for (unsigned int i = 0; i < iDNA.genes(); i++) {
        // Extract the gene
        unsigned char* tGene;
        unsigned int tSize;
        iDNA.extract_gene(i, tGene, tSize);

        // Evaluate the block
        evaluate_block(tGene, tSize);

        // Free the block
        free(tGene);
    }
}

// Output the DNA
void Parser::print(std::ostream& iStream, const DNA& iDNA) {
    // Print all instructions
    for (unsigned int i = 0; i < iDNA.genes(); i++) {
        // Extract the gene
        unsigned char* tGene;
        unsigned int tSize;
        iDNA.extract_gene(i, tGene, tSize);

        // Print the block
        std::cout << "* Human-readable version of block " << i << std::endl;
        print_block(iStream, tGene, tSize);

        // Free the block
        free(tGene);
    }
}

//
// Validation helpers
//

void Parser::validate_block(unsigned char* iBlock, unsigned int iSize) {
    // Extract all instructions
    unsigned int tLoc = 0;

    // Validate all instructions
    std::vector<std::pair<unsigned int, unsigned int> > tInstructionBytecode = extract_instructions(iBlock, iSize, tLoc);
    for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
        validate_instruction(iBlock, iSize, tInstructionBytecode[i].first);
        if (tInstructionBytecode[i].first != tInstructionBytecode[i].second)
            throw Exception(SYNTAX, "garbage after instruction");
    }

    if (tLoc < iSize)
        throw Exception(SYNTAX, "garbage after block");
}

void Parser::validate_instruction(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Conditional
    if (mGrammar->isConditional(iBlock[tLoc]))
        validate_conditional(iBlock, iSize, tLoc);

    // Function
    else if (mGrammar->isFunction(iBlock[tLoc]))
        validate_function(iBlock, iSize, tLoc);

    // Data
    else if (mGrammar->isData(iBlock[tLoc]))
        return validate_data(iBlock, iSize, tLoc);
    
    // Unknown
    else
        throw Exception(SYNTAX, "unknown byte identifier");
}

void Parser::validate_conditional(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Save conditional for later evaluation
    unsigned char tConditional = iBlock[tLoc++];

    // Extract tests and instructions
    std::vector<std::pair<unsigned int, unsigned int> > tTestBytecode = extract_arguments(iBlock, iSize, tLoc);
    std::vector<std::pair<unsigned int, unsigned int> > tInstructionBytecode = extract_instructions(iBlock, iSize, tLoc);

    // Check the evaluation type
    switch (tConditional) {
        case COND_IF:
        case COND_UNLESS:
        case COND_WHILE:
        {
            // Validate tests
            if (tTestBytecode.size() != 1)
                throw Exception(SYNTAX, "conditional only accepts one test");
            validate_instruction(iBlock, iSize, tTestBytecode[0].first);
            if (tTestBytecode[0].first != tTestBytecode[0].second)
                throw Exception(SYNTAX, "garbage after test");

            // Validate instruction block
            for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
                validate_instruction(iBlock, iSize, tInstructionBytecode[i].first);
                if (tInstructionBytecode[i].first != tInstructionBytecode[i].second)
                    throw Exception(SYNTAX, "garbage after instruction");
            }
            break;
        }

        default:
        {
            throw Exception(GENERIC, "conditional clause not implemented");
        }
    }

    tLoc = tInstructionBytecode.back().second + 1;
}

void Parser::validate_function(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Fetch the function
    unsigned char tFunctionBytecode = iBlock[tLoc++];

    // Validate function
    if (!mGrammar->isFunction(tFunctionBytecode))
        throw Exception(SYNTAX, "unknown function");
    const Function* tFunction = mGrammar->getFunction(tFunctionBytecode);

    // Validate parameter count
    std::vector<std::pair<unsigned int, unsigned int> > tParameterBytecode = extract_arguments(iBlock, iSize, tLoc);
    if (tParameterBytecode.size() != tFunction->getParameterCount())
        throw Exception(SYNTAX, "invalid function parameter count");

    // Validate parameter syntax
    for (unsigned int i = 0; i < tParameterBytecode.size(); i++) {
        validate_instruction(iBlock, iSize, tParameterBytecode[i].first);
        if (tParameterBytecode[i].first != tParameterBytecode[i].second)
            throw Exception(SYNTAX, "garbage after parameter");
    }

    tLoc = tParameterBytecode.back().second + 1;
}

void Parser::validate_data(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    unsigned char tDataType = iBlock[tLoc++];
    switch (tDataType) {
        case DATA_VOID:
            break;

        case DATA_BOOL:
        case DATA_INT:
            if (tLoc >= iSize)
                throw Exception(SYNTAX, "boolean type needs 1 byte of data");
            tLoc++;
            break;

        default:
            throw Exception(SYNTAX, "unknown datatype");
    }
}

//
// Evaluation helpers
//

void Parser::evaluate_block(unsigned char* iBlock, unsigned int iSize) {
    // Extract all instructions
    unsigned int tLoc = 0;
    std::vector<std::pair<unsigned int, unsigned int> > tInstructionBytecode = extract_instructions(iBlock, iSize, tLoc);

    // Give the grammar a chance to do some stuff (variable handling, ...)
    mGrammar->block();

    // Evaluate all instructions
    for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
        evaluate_instruction(iBlock, iSize, tInstructionBytecode[i].first);
    }
}

Value Parser::evaluate_instruction(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Conditional
    if (mGrammar->isConditional(iBlock[tLoc])) {
        evaluate_conditional(iBlock, iSize, tLoc);
        return Value();
    }

    // Function
    else if (mGrammar->isFunction(iBlock[tLoc]))
        return evaluate_function(iBlock, iSize, tLoc);

    // Data
    else if (mGrammar->isData(iBlock[tLoc]))
        return evaluate_data(iBlock, iSize, tLoc);

    return Value();
}

void Parser::evaluate_conditional(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Save conditional for later evaluation
    unsigned char tConditional = iBlock[tLoc++];

    // Extract tests and instructions
    std::vector<std::pair<unsigned int, unsigned int> > tTestBytecode = extract_arguments(iBlock, iSize, tLoc);
    std::vector<std::pair<unsigned int, unsigned int> > tInstructionBytecode = extract_instructions(iBlock, iSize, tLoc);

    // Check the evaluation type
    switch (tConditional) {
        case COND_IF:
        {
            // Evaluate the parameter
            Value tTest = evaluate_instruction(iBlock, iSize, tTestBytecode[0].first);
            if (tTest.getType() != BOOL)
                throw Exception(CONDITIONAL, "test passed to 'if' did not produce boolean value");

            // Evaluate the instructions
            if (tTest.getBool()) {
                for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
                    evaluate_instruction(iBlock, iSize, tInstructionBytecode[i].first);
                }
            }
            break;
        }

        case COND_UNLESS:
        {
            // Evaluate the parameter
            Value tTest = evaluate_instruction(iBlock, iSize, tTestBytecode[0].first);
            if (tTest.getType() != BOOL)
                throw Exception(CONDITIONAL, "test passed to 'unless' did not produce boolean value");

            break;
        }

        case COND_WHILE:
        {
            while (1) {
                // Evaluate the parameter
                unsigned int tLocCond = tTestBytecode[0].first;
                Value tTest = evaluate_instruction(iBlock, iSize, tLocCond);
                if (tTest.getType() != BOOL)
                    throw Exception(CONDITIONAL, "test passed to 'while' did not produce boolean value");

                // Evaluate the instructions
                if (tTest.getBool()) {
                    for (unsigned int i = 0; i < tInstructionBytecode.size(); i++) {
                        unsigned int tLocInst = tInstructionBytecode[i].first;
                        evaluate_instruction(iBlock, iSize, tLocInst);
                    }
                } else {
                    break;
                }
            }
            break;
        }
    }
    
    tLoc = tInstructionBytecode.back().second + 1;
}

Value Parser::evaluate_function(unsigned char* iBlock, unsigned int iSize, unsigned int& tLoc) {
    // Fetch the function
    unsigned char tFunction = iBlock[tLoc++];

    // Evaluate all arguments
    std::vector<std::pair<unsigned int, unsigned int> > tParameterBytecode = extract_arguments(iBlock, iSize, tLoc);
    std::vector<Value> tParameters;
    for (unsigned int i = 0; i < tParameterBytecode.size(); i++) {
        Value tParameter = evaluate_instruction(iBlock, iSize, tParameterBytecode[i].first);
        if (tParameter.getType() == VOID)
            throw Exception(FUNCTION, "function parameter returned void");
        tParameters.push_back(tParameter);
    }

    // Call the function
    tLoc = tParameterBytecode.back().second + 1;
    return mGrammar->callFunction(tFunction, tParameters);
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
            return toBool(iBlock[tLoc++]);
            break;
        }
        case DATA_INT:
        {
            return toInt(iBlock[tLoc++]);
            break;
        }
    }
}


//
// Output helpers
//

void Parser::print_block(std::ostream& iStream, unsigned char* iBlock, unsigned int iSize) {
    unsigned char tIndentation = 1;
    print_indentation(iStream, tIndentation);

    unsigned int tLoc = 0;
    bool tPrevArgEnd = false;
    while (tLoc < iSize) {

        // Syntax
        if (mGrammar->isSyntax(iBlock[tLoc])) {
            switch (iBlock[tLoc]) {
                case ARG_OPEN:
                    iStream << "(";
                    break;
                case ARG_SEP:
                    iStream << ", ";
                    break;
                case ARG_CLOSE:
                    iStream << ")";
                    tPrevArgEnd = true;
                    break;
                case INSTR_OPEN:
                    if (tPrevArgEnd) {
                        iStream << " ";
                        tPrevArgEnd = false;
                    }
                    iStream << "{";
                    tIndentation++;
                    print_newline(iStream, tIndentation);
                    break;
                case INSTR_SEP:
                    iStream << ";";
                    print_newline(iStream, tIndentation);
                    break;
                case INSTR_CLOSE:
                    tIndentation--;
                    if (tPrevArgEnd) {
                        iStream << ";";
                        tPrevArgEnd = false;
                    }
                    print_newline(iStream, tIndentation);
                    iStream << "}";
                    break;
            }
        }

        // Conditional
        else if (mGrammar->isConditional(iBlock[tLoc])) {
            switch (iBlock[tLoc]) {
                case COND_IF:
                    iStream << "if";
                    break;
                case COND_UNLESS:
                    iStream << "unless";
                    break;
                case COND_WHILE:
                    iStream << "while";
                    break;
            }
        }

        // Function
        else if (mGrammar->isFunction(iBlock[tLoc])) {
            iStream << mGrammar->nameFunction(iBlock[tLoc]);
        }

        // Data
        else if (mGrammar->isData(iBlock[tLoc])) {
            unsigned char tDataType = iBlock[tLoc++];
            switch (tDataType) {
                case DATA_VOID:
                    iStream << Value();
                    break;
                case DATA_BOOL:
                    iStream << toBool(iBlock[tLoc]);
                    break;
                case DATA_INT:
                    iStream << toInt(iBlock[tLoc]);
                    break;
            }
        }

        tLoc++;
    }
    iStream << std::endl;

}

void Parser::print_indentation(std::ostream& iStream, unsigned int iIndentation) {
    for (unsigned int i = 0; i < iIndentation; i++) {
        iStream << "    ";
    }
}

void Parser::print_newline(std::ostream& iStream, unsigned int iIndentation) {
    iStream << std::endl;
    print_indentation(iStream, iIndentation);
}



//
// Auxiliary
//

std::vector<std::pair<unsigned int, unsigned int> > Parser::extract_syntax(std::initializer_list<unsigned char> iList, unsigned char* iBlock, unsigned int iSize, unsigned int& iLoc) {
    // Read syntaxis data
    if (iList.size() != 3)
        throw Exception(GENERIC, "syntaxis extraction needs exactly three parameters");
    unsigned char OPEN = *(iList.begin());
    unsigned char SEPARATE = *(iList.begin()+1);
    unsigned char CLOSE = *(iList.begin()+2);

    // Allocate new container
    std::vector<std::pair<unsigned int, unsigned int> > tItems;

    // Find opening arguments bracket
    if (iLoc >= iSize || iBlock[iLoc] != OPEN)
        throw Exception(SYNTAX, "could not find opening arguments bracket");

    // Extract all arguments
    unsigned int tBracketBalance = 0;
    while (iLoc < iSize) {
        // Start of argument
        if (iBlock[iLoc] == OPEN) {
            tBracketBalance++;
            if (tBracketBalance == 1) {
                tItems.push_back(std::pair<unsigned int, unsigned int>(iLoc+1, 0));
            }
        }

        // Argument separator
        else if (iBlock[iLoc] == SEPARATE) {
            if (tBracketBalance == 1) {
                if (tItems.size() == 0)
                    throw Exception(SYNTAX, "item separator cannot occur before any item");
                else if (tItems.back().second != 0)
                    throw Exception(SYNTAX, "unexpected separator");
                tItems.back().second = iLoc;
                tItems.push_back(std::pair<unsigned int, unsigned int>(iLoc+1, 0));
            }
        }

        // End of argument
        else if (iBlock[iLoc] == CLOSE) {
            if (tBracketBalance == 1) {
                if (tItems.size() != 0) {
                    if (tItems.back().second != 0)
                        throw Exception(SYNTAX, "unexpected list end");
                    tItems.back().second = iLoc;
                }
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
std::vector<std::pair<unsigned int, unsigned int> > Parser::extract_arguments(unsigned char* iBlock, unsigned int iSize, unsigned int& iLoc) {
    return extract_syntax({ARG_OPEN, ARG_SEP, ARG_CLOSE}, iBlock, iSize, iLoc);
}

// Extract instructions
std::vector<std::pair<unsigned int, unsigned int> > Parser::extract_instructions(unsigned char* iBlock, unsigned int iSize, unsigned int& iLoc) {
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