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
		bool condition();

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

// Condition (mutate 10 times
bool EnvDebug::condition()
{
    return counter<10;
}



//////////
// MAIN //
//////////

int main()
{
    // Generate a DNA string
    unsigned char dna1[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03};
    DNA tempDNA(dna1, 19);
    tempDNA.debug();

    // Create an environment
    EnvDebug tempEnvironment;

    // Create a population with initial DNA
    Population tempPopulation(&tempEnvironment, tempDNA);

    // Simulate
    try
    {
            tempPopulation.evolve_single_straight();
            tempPopulation.get()->debug();
    }
    catch (std::string error)
    {
            std::cout << "Caught error: " << error << std::endl;
    }

    return 0;
}


