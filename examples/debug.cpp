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
	return 254;	// Pick maximum as of debugging purposes
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
	tempDNA.push(255);
	tempDNA.push(123);
	tempDNA.push(64);
	tempDNA.push(0);
	tempDNA.push(0);
	tempDNA.push(48);
	tempDNA.push(91);
	tempDNA.push(31);
	tempDNA.push(0);
	tempDNA.push(49);
	tempDNA.push(255);

	// Create an environment
	EnvDebug tempEnvironment;

	// Create a population with initial DNA
	Population tempPopulation(&tempEnvironment, tempDNA);

	return 0;
}
