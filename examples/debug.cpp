/*
 * debug.cpp
 * Evolve - Debugging environment
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
#include "../src/population.h"
#include "../src/environment.h"
#include "../src/parser.h"
#include <queue>

//
// Constants
//

/////////////////
// ENVIRONMENT //
/////////////////

// Class definition
class EnvDebug : public Environment
{
	public:
		// Required functons
		int fitness(std::list<std::vector<int> >&);
		int alphabet();
};

// Alphabet (maximal amount of instructions)
int EnvDebug::alphabet()
{
	return 0x254;	// Pick maximum as of debugging purposes
}

// Fitness function
int EnvDebug::fitness(std::list<std::vector<int> >& inputList)
{
	return 1;
}



//////////
// MAIN //
//////////

int main()
{
	// Generate a DNA string
	std::queue<int> tempDNA;
	tempDNA.push(0x255);
	tempDNA.push(0x123);
	tempDNA.push(0x64);
	tempDNA.push(0x0);
	tempDNA.push(0x0);
	tempDNA.push(0x48);
	tempDNA.push(0x91);
	tempDNA.push(0x31);
	tempDNA.push(0x0);
	tempDNA.push(0x49);
	tempDNA.push(0x255);

	// Create an environment
	EnvDebug tempEnvironment;

	// Create a population with initial DNA
	Population tempPopulation(&tempEnvironment, tempDNA);

	return 0;
}
