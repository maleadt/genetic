/*
 * main.cpp
 * Main simulation program.
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

// Headers
#include "world.h"
#include <iostream>
#include <cmath>
using namespace std;

// Constants
const int WORLD_ROWS = 25;
const int WORLD_COLUMNS = 25;
const int WORLD_OBSTACLES = 10;
const int WORLD_TESTS = 6;
const int CREATURE_TESTS = 10;
//string CREATURE_DNA = "3A2YA2X3A3YA3X1B2Y1B3Y1A1YA1X1B2Y1B3Y1A4YA4";
string CREATURE_DNA = "3A2X3A2X3A2X3A2X3A2X3A2X3A2X3A2";

void print_world(World &inputWorld)
{
	for (int y = 0; y < WORLD_ROWS; y++)
	{
		cout << "\t";
		for (int x = 0; x < WORLD_COLUMNS; x++)
		{
			int type = inputWorld.checkSpot(x, y);
			if (type == WORLD_FREE)
			{
				cout << ".";
			}
			else if (type == WORLD_OBSTACLE)
			{
				cout << "x";
			}
			else if (type == WORLD_CREATURE)
			{
				cout << "o";
			}
			else
			{
				cout << " ";
			}
			cout << " ";
		}
		cout << endl;
	}
}

int main()
{
	// Generate world
	cout << "* Create " << WORLD_COLUMNS << " x " << WORLD_ROWS << " world" << endl;
	World simWorld(WORLD_ROWS, WORLD_COLUMNS);
	simWorld.addFinish(WORLD_COLUMNS - 1, WORLD_ROWS - 1);

	// Create a creature
	cout << "* Creating creature" << endl;
	Creature simCreature;
	simCreature.inputTextual(CREATURE_DNA);
	simWorld.creatureAdd(simCreature);

	// Preserve previous DNA
	double prev_coeff = 0;
	simCreature.BackupDNA();

	// Simulate
	cout << "* Simulating" << endl;
	for (int j = 0; j < WORLD_TESTS; j++)
	{
		cout << "****************** SIMULATION ******************" << endl;
		cout << "-> Current code:" << endl;
		cout << simCreature.outputCode() << endl;
		int finishes = 0;
		int steps = 0;
		int commands = 0;
		for (int i = 0; i < CREATURE_TESTS; i++)
		{
			// Reset counters etc
			simWorld.reset();
			simCreature.reset();

			// Draw a new world
			simWorld.clearObj();
			simWorld.addObstacles(WORLD_OBSTACLES);

			// Spawn our creature
			simCreature.spawnRandom();
			int locx = simCreature.locationX;
			int locy = simCreature.locationY;

			// Simulate
			simWorld.simulate();
			if (simCreature.status == CREATURE_FINISHED)
			{
				commands += simCreature.creditsTotal;
				int locx2 = simCreature.locationX;
				int locy2 = simCreature.locationY;
				steps += (int) sqrt((locx2 - locx)*(locx2 - locx) + (locy2 - locy)*(locy2 - locy));
				finishes++;
			}
		}

		// Calculate efficiency coefficient
		if (steps > 0)
		{
			double slowness = commands / ((double)steps);
			double efficiency = (finishes*100.0/CREATURE_TESTS);
			double coefficient = efficiency * 100 / (slowness * slowness);
			if (coefficient > prev_coeff)
			{
				simCreature.RemoveBackupDNA();
				simCreature.BackupDNA();
			} else {
				simCreature.RestoreDNA();
			}
			cout << "-> Efficiency coefficient: " << coefficient << endl;
		}
		cout << endl << endl;


		// Mutate
		simCreature.mutate();
	}

	// Clean up
	cout << "* Cleaning up" << endl;
	simCreature.RemoveBackupDNA();


	return 0;
}
