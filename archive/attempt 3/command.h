/*
 * command.h
 * Command class.
 *
 * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
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
#ifndef __COMMAND
#define __COMMAND

// Headers
#include <vector>
#include <map>
#include "constants.h"
#include "generic.h"


//////////////////////
// CLASS DEFINITION //
//////////////////////

// Instruction struct
class Command
{
	public:
		// Construction and destruction
		Command();
		Command(const Command&);


		// Code mutation
		void mutate(unsigned int);
		void mutateSelf();

		// Input & output
		void inputTextual(std::string);
		std::string outputTextual();
		std::string outputCode();

		// Flags
		bool is_condition;
		bool is_action;

		// Code execution
		bool is_valid();

		// Data
		char identifier;
		std::vector<char> parameters;
};

// Include guard
#endif
