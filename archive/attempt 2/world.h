 /*
 * world.cpp
 * World data structure.
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


#ifndef __WORLD /* include guard */
#define __WORLD

/*
 Configuration
 */

// Needed headers
#include <iostream>
#include <sstream>
#include "generic.h"
#include "creature.h"


/*
 Class defenition
 */

class World
{
	// Public member data, can be viewed from anywhere
	public:
		// Construction
		World(unsigned int, unsigned int, unsigned int);
		World();
		~World();

		// Configuration
		void setDimension(unsigned int, unsigned int);
		void setBlocks(unsigned int);
		void addCreature(Creature&);

		// Simulation
		void runAll();
		bool isFinished(int);

		// Endpoint
		int targetRow;
		int targetColumn;

		// Output routines
		void output();

		// World statistics
		int iteration;

	// Private member data, only for internal usage
	private:
		// Data fields
		unsigned int dataRows, dataColumns;
		string **dataWorld;

		// Creatures on the field
		Creature *dataCreatures;
		unsigned int amountCreatures;

		// Bools
		bool haveDimension;
		bool haveBlocks;

		// Simulation
		bool conditionCheck(Creature&, Gene&);
		void actionDo(Creature&, Gene&);
		int placeType(unsigned int, unsigned int);
		void runCreature(Creature&);
};

#endif /* __WORLD */
