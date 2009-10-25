/*
 * generic.cpp
 * Evolve - generic functions.
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

// Global variables
bool GENERIC_SRAND = false;


// Headers
#include "generic.h"


//////////////
// ROUTINES //
//////////////

// Generate a number from lower up to (exclusive) upper
int random_int(int lowest_number, int highest_number)
{
	// Set seed
	if (!GENERIC_SRAND) {
                unsigned int seed = (unsigned)time(0);

                std::cout << "DEBUG: using seed " << (unsigned)time(0) << std::endl;                
		srand(seed);
		GENERIC_SRAND = true;
	}

	// Swap the numbers if needed
	if (lowest_number > highest_number)
	{
		std::swap(lowest_number, highest_number);
	}

	// Calculate the range
	int range = highest_number - lowest_number;

	// Return a number (using lower-order bits)
	//return lowest_number + rand()%range;

	// Return a number (using high-order bits)
	// Numerical Recipes in C: The Art of Scientific Computing
	//   (William H. Press, Brian P. Flannery, Saul A. Teukolsky,
	//   William T. Vetterling; New York: Cambridge University Press,
	//   1992 (2nd ed., p. 277))
	return int(lowest_number + range*(rand()/(RAND_MAX+1.0)));
}


