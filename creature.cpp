/*
 * creature.cpp
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

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//


// Headers
#include "creature.h"
#include "world.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

Creature::Creature()
{
	dnaCode = new DNA;
	dnaCode->parent = this;
	reset();
}

Creature::~Creature()
{
	// Clean and delete the DNA
	dnaCode->~DNA();
	delete dnaCode;
}

//
// World handling
//

// Spawn a creature in a random spot
void Creature::spawnRandom()
{
	// Find a random coördinate
	int tempX, tempY;
	bool available = false;
	while (!available)
	{
		tempX = random_range(0, pointerWorld->sizeColumns - 1);
		tempY = random_range(0, pointerWorld->sizeRows - 1);

		available = pointerWorld->checkSpot(tempX, tempY) == WORLD_FREE;
	}

	// And spawn on that place
	spawnSpecific(tempX, tempY);
}

// Spawn the creature in a specific spot
//   input: x coördinate (unsigned int), y coördinate (unsigned int)
void Creature::spawnSpecific(unsigned int inputX, unsigned int inputY)
{
	// Set our coördinates
	locationX = inputX;
	locationY = inputY;
	locationIsSet = true;
}

//
// Location specifics
//

void Creature::move(int dx, int dy)
{
	if (pointerWorld->checkSpot(locationX+dx, locationY+dy) != WORLD_FREE)
	{
		is_alive = false;
		return;
	}

	locationX += dx;
	locationY += dy;
}

//
// Input & output
//

// Input textual data
void Creature::inputTextual(std::string &inputText)
{
	// Clear current DNA
	dnaCode->~DNA();

	// Replace DNA
	dnaCode->inputTextual(inputText);
}

// Output textual data
std::string Creature::outputTextual()
{
	return dnaCode->outputTextual();
}

// Output code
std::string Creature::outputCode()
{
	return dnaCode->outputCode();
}


//
// Simulation handling
//

// Check if creature is finished
bool Creature::is_finished()
{
	if (pointerWorld->checkFinish(locationX, locationY))
	{
		return true;
	}

	return false;
}

// Simulate while creature hasn't found endpoint yet
bool Creature::do_simulate()
{
	// We cannot execute more commands than a certain limit
	if (credits > pointerWorld->creatureMaxCommands)
		return false;

	// Refuse to simulate if we have done something nasty
	if (!is_alive)
		return false;

	// Have we finished yet?
	if (is_finished())
		return false;

	// Check if we are not at exactly the same spot as before
	//  (but do allow the creature to re-exec at every world simulation though)
	if (credits > 1)
	{
		if (locationX == locationPrevX && locationY == locationPrevY)
			return false;

		locationPrevX = locationX;
		locationPrevY = locationY;
	}

	return true;
}

void Creature::simulate()
{
	// Launch the simulation
	while (do_simulate())
		dnaCode->execute();

	// Detect loops
	if (credits > pointerWorld->creatureMaxCommands)
	{
		// Creature spent all his credits, due to long dna or infinite loop
	}

	// Save credits
	creditsTotal += credits;
	credits = 0;
}


void Creature::mutate()
{
	// How many times do we want to mutate
	int amount = random_range(1, 5);

	// Backup DNA
	std::string backup = dnaCode->outputTextual();

	bool mutated_good = false;
	while (!mutated_good)
	{
		for (int i = 0; i <= amount; i++)
		{
		// Level to mutate
			unsigned int mutLevel = random_range(0, 2);

			// Mutate
			dnaCode->mutate(mutLevel);
		}

		// Check validity
		if (!dnaCode->is_valid())
		{
			// Replace with backup
			dnaCode->~DNA();
			dnaCode->inputTextual(backup);
		} else {
			mutated_good = true;
		}
	}
}

void Creature::BackupDNA()
{
	if (dnaBackupIsSet)
	{
		dnaBackup->~DNA();
		delete dnaBackup;
	}
	dnaBackup = new DNA(*dnaCode);
	dnaBackupIsSet = true;
}

void Creature::RestoreDNA()
{
	dnaCode->~DNA();
	delete dnaCode;
	dnaCode = new DNA(*dnaBackup);
}

void Creature::reset()
{
	credits = 0;
	creditsTotal = 0;
	locationIsSet = false;
	is_alive = true;
	creditsTotal = 0;
	locationPrevX = 0;
	locationPrevY = 0;
}

bool Creature::is_valid()
{
	return dnaCode->is_valid();
}
