/*
 * gen.h
 * Gen class..
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
#ifndef __GEN
#define __GEN


// Headers
#include <list>
#include "command.h"
#include <string>


//////////////////////
// CLASS DEFINITION //
//////////////////////

// Parent class (forward declaration)
class DNA;

class Gen
{
	public:
		// Construction and destruction
		Gen();
		~Gen();
		Gen(const Gen&);
		DNA* parent;

		// Input & output
		void inputTextual(std::string);
		std::string outputTextual();
		std::string outputCode();

		// Code execution helpers
		void DeltaFromDirection(char, int&, int&);

		// Code execution
		void execute();
		bool evaluateAll();
		bool evaluate(Command* inputCommand);
		void actAll();
		void act(Command* inputCommand);
		bool is_valid();

		// Code mutation
		void mutate(unsigned int);
		void mutateSelf();

	private:
		std::list<Command*> dataCommands;
		bool dataRepeat;
};


// Include guard
#endif
