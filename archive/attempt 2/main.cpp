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

#include <iostream>
#include <cmath>
#include "creature.h"
#include "dna.h"
#include "world.h"
#include "generic.h"
using namespace std;

// World constants
const int WORLD_ROWS = 10;
const int WORLD_COLUMNS = 10;
const int WORLD_BLOCKS = 35;

// Creature constants
const int CREATURE_RUNS = 100;	// How many times the creature can run in a world
const int CREATURE_ITERATIONS = 50;	// How many steps a creature may set

string evolveBox(string inputDNA, int inputRuns)
{
	// Initial values
	DNA baseDNABox[100];
	double baseDNABoxRates[100] = {0.0};
	DNA currentBest;

	// Fill the box
	cout << "* Filling box" << endl;
	DNA tempDNA(inputDNA);
	baseDNABox[0] = tempDNA;
	for (int i = 1; i < 100; i++)
	{
		// Mutate
		tempDNA.set(inputDNA);
		tempDNA.mutate();

		// Save DNA
		baseDNABox[i] = tempDNA;
	}

	// Run the simulation a given times
	for (int tempIteration = 0; tempIteration < inputRuns; tempIteration++)
	{
		cout << "* Box simulation " << tempIteration+1 << endl;

		// Check every DNA code
		for (int i = 0; i < 100; i++)
		{
			// Spawn a creature and some statistical variables
			Creature tempCreature(baseDNABox[i], CREATURE_ITERATIONS);
			int dataTotal = 0;
			int dataSuccess = 0;

			// Simulate every creature CREATURE_RUNS times
			for (int j = 0; j < CREATURE_RUNS; j++)
			{
				World tempWorld(WORLD_ROWS, WORLD_COLUMNS, WORLD_BLOCKS);
				tempWorld.addCreature(tempCreature);
				tempWorld.runAll();

				dataTotal++;
				if (tempWorld.isFinished(tempCreature.ID))
					dataSuccess++;
			}

			// Calculate and rates
			double dataRates = 100*dataSuccess/dataTotal;
			baseDNABoxRates[i] = dataRates; // /(baseDNABox[i].getAmountGenes())
		}

		// Sort box
		cout << "* Sorting box" << endl;
		bool swapped = true;
		while (swapped)
		{
			swapped = false;
			for (int i = 0; i < 100-1; i++)
			{
				if (baseDNABoxRates[i] < baseDNABoxRates[i+1])
				{
					swapped = true;
					swap(baseDNABoxRates[i], baseDNABoxRates[i+1]);
					swap(baseDNABox[i], baseDNABox[i+1]);
				}
			}
		}

		// Save 10 best
		cout << "* Selecting mutations for next simulation (best selection with biggest diversity)" << endl;
		currentBest = baseDNABox[0];
		DNA savedDNA[10];
		unsigned int maxLength = 0;	// Output var
		int savedIndexes[10] = {0};
		for (int i = 0; i < 10; i++)
		{
			// Preserve diversity
			bool needDiversity = true;
			int offSet = 0;
			while (needDiversity  &&  i>0  &&  i+offSet<100-1-10)
			{
				needDiversity = false;
				for (int j = 0; j<i; j++)
				{
					if (savedDNA[j].get() == baseDNABox[i+offSet].get())
					{
						needDiversity = true;
					}
				}

				if (needDiversity)
					offSet++;
			}

			// Save
			savedDNA[i] = baseDNABox[i+offSet];
			if (savedDNA[i].get().length() > maxLength) maxLength = savedDNA[i].get().length();
			savedIndexes[i] = i+offSet;
		}

		// Output them
		for (unsigned int i = 0; i < 10; i++)
		{
			cout << "\t-" << baseDNABox[savedIndexes[i]].get();
			for (unsigned int j = 0; j < maxLength+10-baseDNABox[savedIndexes[i]].get().length(); j++)
			{
				cout << " ";
			}
			cout << baseDNABoxRates[savedIndexes[i]] << "% success" << endl;
		}

		// Mutate 10 best
		cout << "* Mutating box" << endl;
		for (int i = 0; i < 10; i++)
		{
			// Keep the parent
			baseDNABox[i*10] = DNA(savedDNA[i].get());

			// Mutate the parent 9 times
			for (int j = 1+i*10; j < i*10 + 10; j++)
			{
				baseDNABox[j].mutate();
			}
		}
	}

	// Return the most succesfull DNA code
	return currentBest.get();
}


string evolveStraight(string inputDNA, int inputRuns)
{
	// Simulate the situation inputRuns times
	int resultRate = 0;
	string resultDNA = inputDNA;

	// Output
	unsigned int spaces = 50;

	// Spawn a creature
	DNA tempDNA(inputDNA);

	for (int tempIteration = 0; tempIteration < inputRuns; tempIteration++)
	{
		//
		// Process
		//

		// Spawn a new creature
		Creature tempCreature(tempDNA, CREATURE_ITERATIONS);

		// Partial output
		cout << tempIteration+1 << ": " << tempDNA.get();

		// Run a simulation
		int currentTries = 0;
		int currentFinishes = 0;
		for (int i = 0; i < CREATURE_RUNS; i++)
		{
			// Create the world
			World tempWorld(WORLD_ROWS, WORLD_COLUMNS, WORLD_BLOCKS);

			// Place the creature in the world
			tempWorld.addCreature(tempCreature);

			// Run the world
			tempWorld.runAll();

			// Stats
			currentTries++;
			if (tempWorld.isFinished(tempCreature.ID)) currentFinishes++;
		}


		//
		// Output
		//

		// Some alignment
		if (tempCreature.getDNA().get().length() > spaces-10)
		{
			spaces *= 2;
		}
		int tempExtraSpaces = spaces-tempCreature.getDNA().get().length()-int(log10(tempIteration+1));
		for (int i = 0; i < tempExtraSpaces; i++)
		{
			cout << " ";
		}

		// Stats
		int tempRate = (int)(currentFinishes*100/currentTries);
		cout << tempRate << "% finished";


		//
		// Mutation
		//

		// Efficiënt mutation?

		if (tempRate > resultRate)
		{
			resultRate = tempRate;
			resultDNA = tempDNA.get();
			cout << " *";
		}
		else
		{
			tempDNA.set(resultDNA);
		}

		cout << endl;

		// Mutate
		tempDNA.mutate();

		// Mutate twice sometimes
		if (random_int(1,10) < 3)
		{
			tempDNA.mutate();
		}
	}

	// Return best DNA
	return resultDNA;

}

int main()
{
	// Welcome
	cout << "- Evolutionary test-case application" << endl;
	cout << "   Tim Besard (tim.besard@gmail.com), 05/2008" << endl;

	// Explain DNA
	cout << "- DNA structure: " << endl;
	cout << "  A DNA string is build with multiple genes." << endl;
	cout << "  Every gene consists out of one single operator, one or more conditions, one or more actions, and a stopcodon." << endl;
	cout << "\tMain operators (stand-alone)" << endl;
	cout << "\t\tA = if" << endl;
	cout << "\t\tB = if not" << endl;
	cout << "\t\tC = while" << endl;
	cout << "\t\tD = while not" << endl;
	cout << "\tConditions (combined with a direction)" << endl;
	cout << "\t\tK = if free" << endl;
	cout << "\t\tL = if blocked" << endl;
	cout << "\tActions (combined with a direction)" << endl;
	cout << "\t\tG = move" << endl;
	cout << "\tDirections" << endl;
	cout << "\t\tO = right" << endl;
	cout << "\t\tP = left" << endl;
	cout << "\t\tQ = up" << endl;
	cout << "\t\tR = down" << endl;
	cout << "\t\tS = right up" << endl;
	cout << "\t\tT = right down" << endl;
	cout << "\t\tU = left up" << endl;
	cout << "\t\tV = left down" << endl;
	cout << "\t\tW = most efficient" << endl;
	cout << endl;
	cout << "   Example: CKOGOXAKRGRX" << endl;
	cout << "            - while there is a free spot right to me, move to it" << endl;
	cout << "            - if there is a free spot below me, move to it" << endl;
	cout << "   If all genes have been executed, the cycle repeats unless the creature has reached the maximum amount of iterations it's allowed to execute (" << CREATURE_RUNS << ")." << endl;
	cout << endl << "   Remark: few mutations can be applied on single genes, please take two or more genes." << endl;

	// Get DNA
	cout << "? Please insert a DNA string: ";
	string inputDNA;
	cin >> inputDNA;

	// Select a method
	cout << "? Which simulation to run:" << endl;
	cout << "\t1)Straight evolution" << endl;
	cout << "\t  Mutate the most effective DNA string (only keep one in memory)." << endl;
	cout << "\t2)Box evolution" << endl;
	cout << "\t  Mutate the 10 most effective DNA strings 10 times, and select the 10 most effective." << endl;
	cout << "\t  Warning: 100 times \"slower\" per run, but possibly more effective." << endl;
	cout << "  Choice: ";
	int inputChoice;
	cin >> inputChoice;

	// Get the amount of runs
	cout << "? Amount of runs: ";
	int inputRuns;
	cin >> inputRuns;

	// Straight evolution
	if (inputChoice == 1)
	{
		string mutatedDNA = evolveStraight(inputDNA, inputRuns);
		cout << "- Original DNA string:\t" << inputDNA << endl;
		cout << "  Mutated DNA string:\t" << mutatedDNA << endl;
	}

	// Box evolution
	else if (inputChoice == 2)
	{
		string mutatedDNA = evolveBox(inputDNA, inputRuns);
		cout << "- Original DNA string:\t" << inputDNA << endl;
		cout << "  Mutated DNA string:\t" << mutatedDNA << endl;
	}

	return 0;
}
