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

#ifndef __CREATURE /* include guard */
#define __CREATURE

/*
 * Configuration
 */

// Needed headers
#include <iostream>
#include "dna.h"
#include "gen.h"


/*
 * Class defenition
 */

class Creature
{
	// Public member data, can be viewed from anywhere
	public:
		// Construction
		Creature();
		Creature(DNA, int);
		//~Creature();

		// Basic IO
		DNA getDNA();
		void setDNA(DNA);
		void setIteration(unsigned int);


		// Creature details
		unsigned int row;
		unsigned int column;
		int ID;
		unsigned int iteration;
		bool alive;
		bool finished;
		unsigned int maxIteration;

	// Private member data, only for internal usage
	private:
		DNA dataDNA;
		bool haveDNA;
};



#endif /* __CREATURE */
