/*
 * environment.h
 * Evolve - Starting environment
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
#include "environment.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Required routines
//

/*
WARNING:
  Only ONE fitness functions should be overloaded,
  the other one should not and return a value of
  zero.
*/

// Fitness functions
double Environment::fitness(const DNA* inputDNA) const
{
    std::cout << "WARNING: NON-OVERRIDDEN FITNESS FUNCTION CALLED" << std::endl;
	return 0;
}

// Return the alphabet value
int Environment::alphabet() const
{
    std::cout << "WARNING: NON-OVERRIDDEN ALPHABET FUNCTION CALLED" << std::endl;
	return 254;
}

// Update call
void Environment::update(const DNA& inputDNA)
{
    std::cout << "WARNING: NON-OVERRIDDEN UPDATE FUNCTION CALLED" << std::endl;
}

// Condition call
bool Environment::condition()
{
    std::cout << "WARNING: NON-OVERRIDDEN CONDITION FUNCTION CALLED" << std::endl;
    return true;
}
