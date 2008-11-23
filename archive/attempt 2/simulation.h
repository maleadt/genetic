/*
 * simulation.h
 * Automatic simulation methods.
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

#ifndef __SIMULATION /* include guard */
#define __SIMULATION

/*
 * Configuration
 */

// Needed headers
#include <iostream>
#include "world.h"
#include "creature.h"
#include "dna.h"
#include "generic.h"


/*
 * Class defenition
 */

class Simulation
{
	// Public member data, can be viewed from anywhere
	public:
		// Construction
		Simulation(string);
		Simulation();

		// Configuration
		void setDNA(string);
		void setWorld(int, int, int);
		void setRandomPositionStart(bool);
		void setRandomPositionEnd(bool);
		void setSimulations(int);
		void setSimulationsPerDNA(int);
		void setStepsPerCreature(int);

		// Simulations
		string simulateStraight();

	// Private member data, only for internal usage
	private:;
		string dataDNAString;
		bool haveDNAString;
		bool dataRandPosStart, dataRandPosEnd;
		int dataWorldRows, dataWorldColumns, dataWorldBlocks;
		int dataSimulations, dataSimulationsPerDNA, dataStepsPerCreature;
};




#endif /* __SIMULATION */
