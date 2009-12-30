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
#include "tokenizer.h"



////////////////////
// CLASS ROUTINES //
////////////////////


//
// Construction
//

Tokenizer::Tokenizer(Grammar* iGrammar) {
    mGrammar = iGrammar;
}


//
// Functionality
//

DNA Tokenizer::tokenize(const std::string iCode) {
    // Create DNA
    std::vector<unsigned char> tVector;
    tVector.reserve((int) (iCode.length() / 5));

    // Tokenizer temporary data
    bool S_QUOTED = false;
    std::string tBuffer;

    // Read char by char
    for (int i = 0; i < iCode.length(); i++) {
        char tChar = iCode[i];

        switch (tChar) {
            case ' ':
                if (S_QUOTED)
                    tBuffer.add(' ');
                break;
            case '\'':
            case '"':
                S_QUOTED = !S_QUOTED;
                if (S_QUOTED = false) {
                    std::stringstream tStream(tBuffer);
                    int tNumeric;
                    if (!(tStream >> tNumeric).fail())
                    {
                        // TODO: as numeric
                    }
                    else {
                        // TODO: as string
                    }

                    tBuffer.clear();
                }
                break;

            case '(':
                tVector.add(ARG_OPEN);
                break;
            case ',':
                tVector.add(ARG_SEP);
                break;
            case ')':
                tVector.add(ARG_CLOSE);
                break;

            case '{':
                tVector.add(INSTR_OPEN);
                break;
            case ';':
                tVector.add(INSTR_SEP);
                break;
            case '}':
                tVector.add(INSTR_CLOSE);
                break;
        }
    }
}