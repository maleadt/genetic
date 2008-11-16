               :/*
               : * main.cpp
               : * Main simulation program.
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
               :// Headers
               :#include "world.h"
               :#include <iostream>
               :#include <cmath>
               :using namespace std;
               :
               :// Constants
               :const int WORLD_ROWS = 25;
               :const int WORLD_COLUMNS = 25;
               :const int WORLD_OBSTACLES = 10;
               :const int WORLD_TESTS = 20;
               :const int CREATURE_TESTS = 500;
               :string CREATURE_DNA = "3A2YA2X3A3YA3X1B2Y1B3Y1A1YA1X1B2Y1B3Y1A4YA4";
               :
               :void print_world(World &inputWorld)
               :{
               :	for (int y = 0; y < WORLD_ROWS; y++)
               :	{
               :		cout << "\t";
               :		for (int x = 0; x < WORLD_COLUMNS; x++)
               :		{
               :			int type = inputWorld.checkSpot(x, y);
               :			if (type == WORLD_FREE)
               :			{
               :				cout << ".";
               :			}
               :			else if (type == WORLD_OBSTACLE)
               :			{
               :				cout << "x";
               :			}
               :			else if (type == WORLD_CREATURE)
               :			{
               :				cout << "o";
               :			}
               :			else
               :			{
               :				cout << " ";
               :			}
               :			cout << " ";
               :		}
               :		cout << endl;
               :	}
               :}
               :
               :int main() /* main total:     57  0.0098 */
               :{
               :	// Generate world
               :	cout << "* Create " << WORLD_COLUMNS << " x " << WORLD_ROWS << " world" << endl;
               :	World simWorld(WORLD_ROWS, WORLD_COLUMNS);
               :	simWorld.addFinish(WORLD_COLUMNS - 1, WORLD_ROWS - 1);
               :
               :	// Create a creature
               :	cout << "* Creating creature" << endl;
               :	Creature simCreature;
               :	simCreature.inputTextual(CREATURE_DNA);
               :	simWorld.creatureAdd(simCreature);
               :
               :	// Preserve previous DNA
               :	double prev_coeff = 0;
               :	string prev_dna = CREATURE_DNA;
               :
               :	// Simulate
               :	cout << "* Simulating" << endl;
               :	for (int j = 0; j < WORLD_TESTS; j++)
               :	{
               :		cout << "****************** SIMULATION ******************" << endl;
               :		cout << "-> Current code:" << endl;
               :		cout << simCreature.outputCode() << endl;
               :		int finishes = 0;
               :		int steps = 0;
               :		int commands = 0;
    13  0.0022 :		for (int i = 0; i < CREATURE_TESTS; i++)
               :		{
               :			// Reset counters etc
     7  0.0012 :			simWorld.reset();
     2 3.4e-04 :			simCreature.reset();
               :
               :			// Draw a new world
               :			simWorld.clearObj();
     1 1.7e-04 :			simWorld.addObstacles(WORLD_OBSTACLES);
               :
               :			// Spawn our creature
     8  0.0014 :			simCreature.spawnRandom();
     1 1.7e-04 :			int locx = simCreature.locationX;
     1 1.7e-04 :			int locy = simCreature.locationY;
               :
               :			// Simulate
               :			simWorld.simulate();
    21  0.0036 :			if (simCreature.is_finished())
               :			{
               :				commands += simCreature.creditsTotal;
               :				int locx2 = simCreature.locationX;
               :				int locy2 = simCreature.locationY;
     1 1.7e-04 :				steps += (int) sqrt((locx2 - locx)*(locx2 - locx) + (locy2 - locy)*(locy2 - locy));
     1 1.7e-04 :				finishes++;
               :			} else {
               :			}
               :		}
               :
               :		// Calculate efficiency coefficient
               :		if (steps > 0)
               :		{
               :			double slowness = commands / ((double)steps);
               :			double efficiency = (finishes*100.0/CREATURE_TESTS);
               :			double coefficient = efficiency * 100 / (slowness * slowness);
               :			if (coefficient > prev_coeff)
               :			{
               :				prev_dna = simCreature.outputTextual();
               :				prev_coeff = coefficient;
               :			} else {
               :				simCreature.inputTextual(prev_dna);
               :			}
               :			cout << "-> Efficiency coefficient: " << coefficient << endl;
               :		}
     1 1.7e-04 :		cout << endl << endl;
               :
               :
               :		// Mutate
               :		simCreature.mutate();
               :	}
               :
               :	// Clean up
               :	cout << "* Cleaning up" << endl;
               :
               :
               :	return 0;
               :}
/* 
 * Total samples for file : "/home/tim/Documenten/Programmeren/C(++)/Genetic Evolution/V3: C++, STL/main.cpp"
 * 
 *     57  0.0098
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
