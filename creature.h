/*
 * creature.h
 * Creature class.
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
Deze klasse herbergt een creature, dewelke de volgende leden heeft:
* DNA, de code die uitgevoert wordt
* Locatie, coördinaten van de positie in de wereld

Creature heeft ook een "move" functie, waarbij de locatie geupdate wordt, maar een checkup naar de parent eerst controleert
of de zet wel geldig is (zoniet -> creature dood)
ID is niet nodig om te kunnen refereren naar parent (parent->is_valid(dx, dy), aangezien move functie in creature zelf zit

Biedt ook mutatie functies

Simuleren van creature roept en execute functie aan in dna.h
*/

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Include guard
#ifndef __CREATURE
#define __CREATURE


// Headers
#include "constants.h"
#include <list>
#include <string>
#include "dna.h"
#include "generic.h"
#include <iostream>
#include <stack>


//////////////////////
// CLASS DEFINITION //
//////////////////////

// Parent class (forward declaration)
class World;

// Creature class
class Creature
{
	public:
		// Construction and destruction
		Creature();
		~Creature();
		World* pointerWorld;

		// World handling
		void spawnSpecific(unsigned int, unsigned int);
		void spawnRandom();

		// Location specifics
		bool locationIsSet;
		unsigned int locationX;
		unsigned int locationY;
		unsigned int locationPrevX;
		unsigned int locationPrevY;
		void move (int dx, int dy);

		// Simulation handling
		int credits;
		int creditsTotal;
		bool do_simulate();
		void simulate();
		void reset();
		void mutate();

		void BackupDNA();
		void RestoreDNA();
		void RemoveBackupDNA();

		// Code execution
		bool is_valid();

		// Input & output
		void inputTextual(std::string&);
		std::string outputTextual();
		std::string outputCode();

		// Status variable
		int status;

	private:
		// DNA specifics
		DNA* dnaCode;
		std::stack<DNA*> dnaBackup;
		bool dnaIsSet;
};


// Include guard
#endif
