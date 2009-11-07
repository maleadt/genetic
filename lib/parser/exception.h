/*
 * exception.h
 * Evolve - Code parser (exception definitions)
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
#ifndef __EXCEPTION
#define __EXCEPTION

// Headers
#include <string>
#include <exception>
#include <fstream>


//
// Constants
//

enum EXCEPTIONTYPE {
    GENERIC,
    SYNTAX,
    ARGUMENT,
    FUNCTION
};



///////////
// CLASS //
///////////

class Exception {
public:
    Exception(EXCEPTIONTYPE iType, const std::string& iMessage) {
        mType = iType;
        mMessage = iMessage;
    }
    Exception(const std::string& iMessage) {
        mType = GENERIC;
        mMessage = iMessage;
    }
    virtual std::string type() const throw() {
        switch(mType) {
            case GENERIC:
                return "generic error";
                break;
            case SYNTAX:
                return "syntax error";
                break;
            case ARGUMENT:
                return "argument error";
                break;
            case FUNCTION:
                return "function error";
                break;
        }
    }
    const std::string& what() const throw() {
        return mMessage;
    }

    friend std::ostream& operator<<(std::ostream& out, const Exception& e) {
        out << "! Caught " << e.type() << ": " + e.what();
        return out;
    }
private:
    EXCEPTIONTYPE mType;
    std::string mMessage;
};


// Include guard
#endif
