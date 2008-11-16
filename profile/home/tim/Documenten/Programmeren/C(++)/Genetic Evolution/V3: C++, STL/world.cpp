               :/*
               : * world.cpp
               : * World class.
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
               :// Constructor
               :World::World(const unsigned int rows, const unsigned  int columns)
               :{
               :	// Save size
               :	sizeColumns = columns;
               :	sizeRows = rows;
               :
               :	reset();
               :
               :	// Maximum commands per creature
               :	creatureMaxCommands = 100;
               :	worldMaxSimulations = 100;
               :}
               :
               :// Destructor
               :World::~World()
               :{
               :	// Remove and delete all objects
               :	clearObj();
               :
               :	// Remove all creatures (cleanup is handled by client)
               :	clearCreat();
               :
               :	// Remove and delete all finishes
               :	std::list<Obstacle*>::iterator it_finish = dataFinishes.begin();
               :	std::list<Obstacle*>::iterator ite_finish = dataFinishes.end();
               :	while (it_finish != ite_finish)
               :	{
               :		delete (*it_finish);
               :		++it_finish;
               :	}
               :	dataObstacles.clear();
               :}
               :
               :
               ://
               :// Obstacles
               ://
               :
               :// Add an obstacle
               ://   input: percentage of obstacles (unsigned int)
     9  0.0016 :void World::addObstacles(const unsigned int percentage) /* World::addObstacles(unsigned int) total:   6489  1.1181 */
               :{
               :	// How many blocks do we need?
     5 8.6e-04 :	unsigned int obstacles = (sizeColumns*sizeRows) * percentage / 100;
               :
               :	// Add those blocks!
    23  0.0040 :	while (obstacles > 0)
               :	{
    27  0.0047 :		Obstacle *p_obst = new Obstacle;
    20  0.0034 :		bool obstacle_valid = false;
               :
    48  0.0083 :		while (!obstacle_valid)
               :		{
               :			// Generate new block coördinates
    51  0.0088 :			p_obst->dataX = random_range(0, sizeColumns-1);
    60  0.0103 :			p_obst->dataY = random_range(0, sizeRows-1);
               :
               :			// Loop other obstacles to detect overlapping
    16  0.0028 :			obstacle_valid = true;
    84  0.0145 :			if (dataObstacles.size() > 0)
               :			{
    44  0.0076 :				std::list<Obstacle*>::iterator it = dataObstacles.begin();
    54  0.0093 :				std::list<Obstacle*>::iterator ite = dataObstacles.end();
  3516  0.6058 :				while (obstacle_valid && it != ite)
               :				{
  2385  0.4109 :					if ((*(*it)).dataX == p_obst->dataX && (*(*it)).dataY == p_obst->dataY)
     1 1.7e-04 :						obstacle_valid = false;
   104  0.0179 :					++it;
               :				}
               :			}
               :		}
               :
               :		// Save the generated coördinates to the obstacle container
    23  0.0040 :		dataObstacles.push_back(p_obst);
               :
    17  0.0029 :		obstacles--;
               :	}
     2 3.4e-04 :}
               :
               :
               ://
               :// Spots
               ://
               :
               :// Check spot availability
   404  0.0696 :int World::checkSpot(const unsigned int x, const unsigned int y) /* World::checkSpot(unsigned int, unsigned int) total:  78672 13.5556 */
               :{
               :	// Check if spot is between main borders
   858  0.1478 :	if (x<0 || y<0 || (unsigned)x>=sizeColumns || (unsigned)y>=sizeRows)
   385  0.0663 :		return WORLD_INVALID;
               :
               :	// Check for obstacles
   502  0.0865 :	std::list<Obstacle*>::iterator it_obst = dataObstacles.begin();
   479  0.0825 :	std::list<Obstacle*>::iterator ite_obst = dataObstacles.end();
 25123  4.3288 :	while (it_obst != ite_obst)
               :	{
 42875  7.3876 :		if ((*it_obst)->dataX == x && (*it_obst)->dataY == y)
    94  0.0162 :			return WORLD_OBSTACLE;
  1965  0.3386 :		++it_obst;
               :	}
               :
               :	// Check for creatures
  1781  0.3069 :	std::list<Creature*>::iterator it_creat = dataCreatures.begin();
   497  0.0856 :	std::list<Creature*>::iterator ite_creat = dataCreatures.end();
   725  0.1249 :	while (it_creat != ite_creat)
               :	{
  1499  0.2583 :		if ((*it_creat)->locationIsSet && (*it_creat)->locationX == x && (*it_creat)->locationY == y)
               :			return WORLD_CREATURE;
    42  0.0072 :		++it_creat;
               :	}
               :
  1040  0.1792 :	return WORLD_FREE;
   403  0.0694 :}
               :
               :// Check for finish
   127  0.0219 :bool World::checkFinish(const unsigned int x, const unsigned int y) /* World::checkFinish(unsigned int, unsigned int) total:   2261  0.3896 */
               :{
               :	// Check for finish
   196  0.0338 :	std::list<Obstacle*>::iterator it_finish = dataFinishes.begin();
   383  0.0660 :	std::list<Obstacle*>::iterator ite_finish = dataFinishes.end();
   489  0.0843 :	while (it_finish != ite_finish)
               :	{
   549  0.0946 :		if ((*it_finish)->dataX == x && (*it_finish)->dataY == y)
    11  0.0019 :			return true;
    46  0.0079 :		++it_finish;
               :	}
   353  0.0608 :	return false;
   107  0.0184 :}
               :
               :// Add a finish
               :void World::addFinish(const unsigned int x, const unsigned int y)
               :{
               :	Obstacle* finish = new Obstacle;
               :	finish->dataX = x;
               :	finish->dataY = y,
               :	dataFinishes.push_back(finish);
               :}
               :
               :
               ://
               :// Creatures
               ://
               :
               :// Add a creature to the world
               ://   input: the creature to place (pointer to Creature)
               :// NOTE: creature placment is handled by the creature itself, this routine only links the creature to the world
               :void World::creatureAdd(Creature *inputCreature)
               :{
               :	// Save a pointer to the creature
               :	dataCreatures.push_back(inputCreature);
               :
               :	// Give our creature access to the world
               :	inputCreature->pointerWorld = this;
               :}
               :
               :// Add a creature to the world
               ://   input: the creature to place (Creature)
               :void World::creatureAdd(Creature &inputCreature)
               :{
               :	// Save a pointer to the creature
               :	Creature* pointerCreature = &inputCreature;
               :	dataCreatures.push_back(pointerCreature);
               :
               :	// Give our creature access to the world
               :	inputCreature.pointerWorld = this;
               :}
               :
               :
               ://
               :// Simulation handling
               ://
               :
               :// Simulate while not all creatures have finished
    75  0.0129 :bool World::do_simulate() /* World::do_simulate() total:   1972  0.3398 */
               :{
    88  0.0152 :	if (worldSimulations > worldMaxSimulations)
               :	{
     9  0.0016 :		return false;
               :	}
               :
    74  0.0128 :	int alive = dataCreatures.size();
   210  0.0362 :	std::list<Creature*>::iterator it = dataCreatures.begin();
   280  0.0482 :	std::list<Creature*>::iterator ite = dataCreatures.end();
   447  0.0770 :	while (it != ite)
               :	{
   442  0.0762 :		if ((*it)->is_finished() && !(*it)->is_alive)
               :		{
               :			alive--;
               :		}
    26  0.0045 :		++it;
               :	}
   213  0.0367 :	if (alive == 0)
               :	{
               :		return false;
               :	}
               :
    65  0.0112 :	return true;
    43  0.0074 :}
               :
     1 1.7e-04 :void World::simulate() /* World::simulate() total:   1761  0.3034 */
               :{
   130  0.0224 :	while (do_simulate())
               :	{
   139  0.0240 :		std::list<Creature*>::iterator it = dataCreatures.begin();
   310  0.0534 :		std::list<Creature*>::iterator ite = dataCreatures.end();
   461  0.0794 :		while (it != ite)
               :		{
   367  0.0632 :			(*it)->simulate();
   294  0.0507 :			++it;
               :		}
    53  0.0091 :		worldSimulations++;
               :	}
     6  0.0010 :}
               :
     1 1.7e-04 :void World::reset() /* World::reset() total:      3 5.2e-04 */
               :{
     1 1.7e-04 :	worldSimulations = 0;
     1 1.7e-04 :}
               :
     2 3.4e-04 :void World::clearObj() /* World::clearObj() total:    153  0.0264 */
               :{
               :	// Remove all objects
     1 1.7e-04 :	std::list<Obstacle*>::iterator it_obst = dataObstacles.begin();
     9  0.0016 :	std::list<Obstacle*>::iterator ite_obst = dataObstacles.end();
    58  0.0100 :	while (it_obst != ite_obst)
               :	{
    52  0.0090 :		delete (*it_obst);
    21  0.0036 :		++it_obst;
               :	}
     5 8.6e-04 :	dataObstacles.clear();
     5 8.6e-04 :}
               :
               :void World::clearCreat()
               :{
               :	// Remove all creatures
               :	dataCreatures.clear();
               :}
/* 
 * Total samples for file : "/home/tim/Documenten/Programmeren/C(++)/Genetic Evolution/V3: C++, STL/world.cpp"
 * 
 *  91311 15.7334
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
