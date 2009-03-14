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
		// Constructor
		EnvDebug();

		// Required functons
		double fitness(DNA& inputDNA);
		int alphabet();

	private:
		int counter;
};

// Constructor
EnvDebug::EnvDebug()
{
	counter = 0;
}

// Alphabet (maximal amount of instructions)
int EnvDebug::alphabet()
{
	return 254;	// Pick maximum as of debugging purposes
}

// Fitness function
double EnvDebug::fitness(DNA& inputDNA)
{
	return counter++;
}


//////////
// MAIN //
//////////

int main()
{
	// Generate a DNA string
	std::queue<int> tempQueue;
	tempQueue.push(255);
	tempQueue.push(123);
	tempQueue.push(64);
	tempQueue.push(0);
	tempQueue.push(0);
	tempQueue.push(48);
	tempQueue.push(91);
	tempQueue.push(31);
	tempQueue.push(0);
	tempQueue.push(49);
	tempQueue.push(255);
	DNA tempDNA(tempQueue);
    tempDNA.debug();

	// Create an environment
	EnvDebug tempEnvironment;

	// Create a population with initial DNA
	Population tempPopulation(&tempEnvironment, tempDNA);

	// Simulate
	try
	{
		tempPopulation.evolve_single_straight(1);
		tempPopulation.get().debug();
	}
	catch (std::string error)
	{
		std::cout << "Caugt error: " << error << std::endl;
	}

	return 0;
}


