/*
 * world.h
 * World class.
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
Deze klassen implementeert een "wereld", die verschillende "creatures" in zijn object opgeslaan heeft.

een World->Simulate call,simuleert alle creatures to een fixed limiet, zolang die niet dood zijn of einddoel bereikt hebben
*/

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Include guard
#ifndef __WORLD
#define __WORLD


// Headers
#include "constants.h"
#include "generic.h"
#include "creature.h"
#include <list>
#include <iostream>


//////////////////////
// CLASS DEFINITION //
//////////////////////

// An obstacle
struct Obstacle
{
	unsigned int dataX;
	unsigned int dataY;
};

class World
{
	public:
		// Construction and destruction
		World(const unsigned int, const unsigned int);
		~World();

		// Obstacles
		void addObstacles(const unsigned int);
		void clearObj();
		void clearCreat();

		// Spots
		int checkSpot(const unsigned int, const unsigned int);
		void addFinish(const unsigned int, const unsigned int);
		bool checkFinish(const unsigned int, const unsigned int);

		// Creature handling
		void creatureAdd(Creature*);
		void creatureAdd(Creature&);
		int creatureMaxCommands;

		// World handling
		int worldSimulations;
		int worldMaxSimulations;
		void reset();

		// Simulation handling
		bool do_simulate();
		void simulate();

		// Size of the world
		unsigned int sizeColumns;
		unsigned int sizeRows;


	private:
		// Elements in the world
		std::list<Obstacle*> dataObstacles;
		std::list<Creature*> dataCreatures;
		std::list<Obstacle*> dataFinishes;
};


// Include guard
#endif
