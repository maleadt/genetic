               :/*
               : * creature.cpp
               : * Creature class.
               : *
               : * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
               : * All rights reserved.
               : *
               : * This program is free software: you can redistribute it and/or modify
               : * it under the terms of the GNU General Public License as published by
               : * the Free Software Foundation, either version 3 of the License, or
               : * at your option) any later version.
               : *
               : * This program is distributed in the hope that it will be useful,
               : * but WITHOUT ANY WARRANTY; without even the implied warranty of
               : * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
               : * GNU General Public License for more details.
               : *
               : * You should have received a copy of the GNU General Public License
               : * along with this program.  If not, see <http://www.gnu.org/licenses/>.
               : *
               : */
               :
               :///////////////////
               :// CONFIGURATION //
               :///////////////////
               :
               ://
               :// Essential stuff
               ://
               :
               :
               :// Headers
               :#include "creature.h"
               :#include "world.h"
               :
               :
               :////////////////////
               :// CLASS ROUTINES //
               :////////////////////
               :
               ://
               :// Construction and destruction
               ://
               :
               :Creature::Creature()
               :{
               :	dnaCode = new DNA;
               :	dnaCode->parent = this;
               :	reset();
               :}
               :
               :Creature::~Creature()
               :{
               :	// Clean and delete the DNA
               :	dnaCode->~DNA();
               :	delete dnaCode;
               :}
               :
               ://
               :// World handling
               ://
               :
               :// Spawn a creature in a random spot
    14  0.0024 :void Creature::spawnRandom() /* Creature::spawnRandom() total:     33  0.0057 */
               :{
               :	// Find a random coördinate
               :	int tempX, tempY;
               :	bool available = false;
     6  0.0010 :	while (!available)
               :	{
     3 5.2e-04 :		tempX = random_range(0, pointerWorld->sizeColumns - 1);
     1 1.7e-04 :		tempY = random_range(0, pointerWorld->sizeRows - 1);
               :
     6  0.0010 :		available = pointerWorld->checkSpot(tempX, tempY) == WORLD_FREE;
               :	}
               :
               :	// And spawn on that place
     1 1.7e-04 :	spawnSpecific(tempX, tempY);
     2 3.4e-04 :}
               :
               :// Spawn the creature in a specific spot
               ://   input: x coördinate (unsigned int), y coördinate (unsigned int)
     4 6.9e-04 :void Creature::spawnSpecific(unsigned int inputX, unsigned int inputY) /* Creature::spawnSpecific(unsigned int, unsigned int) total:      6  0.0010 */
               :{
               :	// Set our coördinates
     1 1.7e-04 :	locationX = inputX;
     1 1.7e-04 :	locationY = inputY;
               :	locationIsSet = true;
               :}
               :
               ://
               :// Location specifics
               ://
               :
   262  0.0451 :void Creature::move(int dx, int dy) /* Creature::move(int, int) total:    833  0.1435 */
               :{
   373  0.0643 :	if (pointerWorld->checkSpot(locationX+dx, locationY+dy) != WORLD_FREE)
               :	{
     5 8.6e-04 :		is_alive = false;
               :		return;
               :	}
               :
    50  0.0086 :	locationX += dx;
    63  0.0109 :	locationY += dy;
    80  0.0138 :}
               :
               ://
               :// Input & output
               ://
               :
               :// Input textual data
               :void Creature::inputTextual(std::string &inputText)
               :{
               :	// Clear current DNA
               :	dnaCode->~DNA();
               :
               :	// Replace DNA
               :	dnaCode->inputTextual(inputText);
               :}
               :
               :// Output textual data
               :std::string Creature::outputTextual()
               :{
               :	return dnaCode->outputTextual();
               :}
               :
               :// Output code
               :std::string Creature::outputCode()
               :{
               :	return dnaCode->outputCode();
               :}
               :
               :
               ://
               :// Simulation handling
               ://
               :
               :// Check if creature is finished
   135  0.0233 :bool Creature::is_finished() /* Creature::is_finished() total:    657  0.1132 */
               :{
   339  0.0584 :	if (pointerWorld->checkFinish(locationX, locationY))
               :	{
     8  0.0014 :		return true;
               :	}
               :
    92  0.0159 :	return false;
    83  0.0143 :}
               :
               :// Simulate while creature hasn't found endpoint yet
   197  0.0339 :bool Creature::do_simulate() /* Creature::do_simulate() total:   1077  0.1856 */
               :{
               :	// We cannot execute more commands than a certain limit
   128  0.0221 :	if (credits > pointerWorld->creatureMaxCommands)
     2 3.4e-04 :		return false;
               :
               :	// Refuse to simulate if we have done something nasty
    92  0.0159 :	if (!is_alive)
     6  0.0010 :		return false;
               :
               :	// Have we finished yet?
   126  0.0217 :	if (is_finished())
     7  0.0012 :		return false;
               :
               :	// Check if we are not at exactly the same spot as before
               :	//  (but do allow the creature to re-exec at every world simulation though)
    76  0.0131 :	if (credits > 1)
               :	{
   173  0.0298 :		if (locationX == locationPrevX && locationY == locationPrevY)
    34  0.0059 :			return false;
               :
     3 5.2e-04 :		locationPrevX = locationX;
               :		locationPrevY = locationY;
               :	}
               :
   153  0.0264 :	return true;
    80  0.0138 :}
               :
    84  0.0145 :void Creature::simulate() /* Creature::simulate() total:   2103  0.3624 */
               :{
               :	// Launch the simulation
  1630  0.2809 :	while (do_simulate())
   244  0.0420 :		dnaCode->execute();
               :
               :	// Detect loops
               :	if (credits > pointerWorld->creatureMaxCommands)
               :	{
               :		// Creature spent all his credits, due to long dna or infinite loop
               :	}
               :
               :	// Save credits
    72  0.0124 :	creditsTotal += credits;
    19  0.0033 :	credits = 0;
    54  0.0093 :}
               :
               :
               :void Creature::mutate() /* Creature::mutate() total:      1 1.7e-04 */
               :{
               :	// How many times do we want to mutate
               :	int amount = random_range(1, 5);
               :
               :	// Backup DNA
               :	std::string backup = dnaCode->outputTextual();
               :
               :	bool mutated_good = false;
               :	while (!mutated_good)
               :	{
               :		for (int i = 0; i <= amount; i++)
               :		{
               :		// Level to mutate
               :			unsigned int mutLevel = random_range(0, 2);
               :
               :			// Mutate
               :			dnaCode->mutate(mutLevel);
               :		}
               :
               :		// Check validity
     1 1.7e-04 :		if (!dnaCode->is_valid())
               :		{
               :			// Replace with backup
               :			dnaCode->~DNA();
               :			dnaCode->inputTextual(backup);
               :		} else {
               :			mutated_good = true;
               :		}
               :	}
               :}
               :
               :void Creature::BackupDNA()
               :{
               :	if (dnaBackupIsSet)
               :	{
               :		dnaBackup->~DNA();
               :		delete dnaBackup;
               :	}
               :	dnaBackup = new DNA(*dnaCode);
               :	dnaBackupIsSet = true;
               :}
               :
               :void Creature::RestoreDNA()
               :{
               :	dnaCode->~DNA();
               :	delete dnaCode;
               :	dnaCode = new DNA(*dnaBackup);
               :}
               :
    11  0.0019 :void Creature::reset() /* Creature::reset() total:     13  0.0022 */
               :{
               :	credits = 0;
               :	creditsTotal = 0;
               :	locationIsSet = false;
               :	is_alive = true;
     1 1.7e-04 :	creditsTotal = 0;
               :	locationPrevX = 0;
     1 1.7e-04 :	locationPrevY = 0;
               :}
               :
               :bool Creature::is_valid()
               :{
               :	return dnaCode->is_valid();
               :}
/* 
 * Total samples for file : "/home/tim/Documenten/Programmeren/C(++)/Genetic Evolution/V3: C++, STL/creature.cpp"
 * 
 *   4723  0.8138
 */


/* 
 * Command line: opannotate --source --output-dir=. ../bin/Debug/evolve 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: P4 / Xeon with 2 hyper-threads, speed 3391.7 MHz (estimated)
 * Counted GLOBAL_POWER_EVENTS events (time during which processor is not stopped) with a unit mask of 0x01 (mandatory) count 100000
 */
