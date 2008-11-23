/*
 * creature.cpp
 * Creature data structure.
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




/*
 * Configuration
 */

// Include header
#include "creature.h"

// Constants
const bool CREATURE_VERBOSE = false;




/*
 * Construction
 */

Creature::Creature(DNA inputDNA, int inputIteration)
{
	// Call the subroutine
	setDNA(inputDNA);
	maxIteration = inputIteration;
}

Creature::Creature()
{
	haveDNA = false;
}



/*
 * Basic IO
 */

void Creature::setDNA(DNA inputDNA)
{
	// Save DNA
	dataDNA = inputDNA;
	haveDNA = true;
	if (CREATURE_VERBOSE) cout << "* Creature\tconfigured" << endl;
}

void Creature::setIteration(unsigned int inputIteration)
{
	maxIteration = inputIteration;
}

DNA Creature::getDNA()
{
	return dataDNA;
}
