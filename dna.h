/*
 * dna.h
 * DNA class.
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
#ifndef __DNA
#define __DNA


// Headers
#include <list>
#include <string>
#include "gen.h"
#include "generic.h"
#include "constants.h"


//////////////////////
// CLASS DEFINITION //
//////////////////////

// Parent class (forward declaration)
class Creature;

// DNA class
class DNA
{
	public:
		// Construction and destruction
		DNA();
		DNA(const DNA&);
		~DNA();
		Creature* parent;

		// Input & output
		void inputTextual(std::string&);
		std::string outputTextual();
		std::string outputCode();

		// Code execution
		void execute();
		bool is_valid();

		// Code mutation
		void mutate(unsigned int mutLevel);
		void mutateSelf();

	private:
		std::list<Gen*> dataGenes;
};


// Include guard
#endif
