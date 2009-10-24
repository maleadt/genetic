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


/*
 Configuration
 */

// Include header
#include "world.h"

// Constants
const bool WORLD_VERBOSE = false;
const unsigned int MAX_CREATURES = 20;




/*
 * Construction
 */

World::World()
{
	// Set flags
	haveDimension = false;
	haveBlocks = false;
}

World::World(unsigned int inputRows, unsigned int inputColumns, unsigned int inputBlocks)
{
	// Call subroutines
	setDimension(inputRows, inputColumns);
	setBlocks(inputBlocks);
}

World::~World()
{
	if (haveDimension)
	{
		for (unsigned int i = 0; i < dataRows; i++)
		{
			delete [] dataWorld[i];
		}
		delete [] dataWorld;
	}

	for (unsigned int i = 1; i < amountCreatures; i++)
	{
		dataCreatures[i].~Creature();
	}
	delete [] dataCreatures;
}






/*
 * Configuration
 */

void World::setDimension(unsigned int inputRows, unsigned int inputColumns)
{
	// Set flags
	haveDimension = true;

	// Save data
	dataRows = inputRows;
	dataColumns = inputColumns;

	// Initialise world
	if (WORLD_VERBOSE) cout << "* World\t\tinitialising matrix"<< endl;
	dataWorld = new string*[dataRows];
	for (unsigned int tempRow = 0; tempRow < dataRows; tempRow++)
	{
		dataWorld[tempRow] = new string[dataColumns];
		for (unsigned int tempColumn = 0; tempColumn < dataColumns; tempColumn++)
		{
		dataWorld[tempRow][tempColumn] = "0";
		}
	}

	// Initialise creatures
	if (WORLD_VERBOSE) cout << "* World\t\tcreating creatures box"<< endl;
	amountCreatures = 0;
	dataCreatures = new Creature[MAX_CREATURES];

	// Calculate target point
	targetRow = dataRows-1;
	targetColumn = dataColumns-1;

	// Set target point
	if (WORLD_VERBOSE) cout << "* World\t\tsetting target spot"<< endl;
	dataWorld[dataRows-1][dataColumns-1] = "E";
}

void World::setBlocks(unsigned int inputBlocks)
{
	// Calculate amount of blocks
	int tempBlocks = (int)((dataRows * dataColumns) * inputBlocks / 100.0);

	// Set blocks
	if (WORLD_VERBOSE) cout << "* World\t\tadding " << (int)((dataRows * dataColumns) * inputBlocks / 100.0) << " blocks" << endl;
	for (int i = 0; i < tempBlocks; i++)
	{
		// Generate random spot
		int tempRow = random_int(0, dataRows-1);
		int tempColumn = random_int(0, dataColumns-1);

		// Only overwrite empty spots with blocks
		while (dataWorld[tempRow][tempColumn] != "0")
		{
			tempRow = random_int(0, dataRows-1);
			tempColumn = random_int(0, dataColumns-1);
		}

		// Write the spot
		dataWorld[tempRow][tempColumn] = "1";
	}
}

void World::addCreature(Creature& inputCreature)
{
	// Check if we still got place
	if (WORLD_VERBOSE) cout << "* World\t\tadding new creature"<< endl;
	if (amountCreatures >= MAX_CREATURES)
	{
		cout << "! Maximum amount of creatures reached" << endl;
		return;
	}

	// Generate random spot
	int tempRow = random_int(0, dataRows-1);
	int tempColumn = random_int(0, dataColumns-1);

	// Only place our creature on an empty spot
	if (WORLD_VERBOSE) cout << "* World\t\tlooking up a location for the creature"<< endl;
	while (dataWorld[tempRow][tempColumn] != "0")
	{
		tempRow = random_int(0, dataRows-1);
		tempColumn = random_int(0, dataColumns-1);
	}

	// TEMP: force the creature in the upper left corner
	tempRow = 0;
	tempColumn = 0;

	// Reset our creature's iteration counter
	inputCreature.iteration = 0;

	// Mark the spot as occupied
	std::stringstream tempConv;
	tempConv << amountCreatures;
	dataWorld[tempRow][tempColumn] = "C" + tempConv.str();

	// And save our creature
	if (WORLD_VERBOSE) cout << "* World\t\tspawning creature"<< endl;
	inputCreature.row = tempRow;
	inputCreature.column = tempColumn;
	inputCreature.ID = amountCreatures;
	inputCreature.alive = true;	// I am god!
	inputCreature.finished = false;	// Let the quest begin!
	dataCreatures[amountCreatures] = inputCreature;

	// Increase the counter
	amountCreatures++;
}



/*
 * Public simulation
 */

void World::runAll()
{
	//
	// Main simulation
	//

	int amountAlive = amountCreatures;
	iteration = 0;

	while (amountAlive>0)
	{
		// Start a new round!
		iteration++;
		if (WORLD_VERBOSE) cout << "* World\t\tsimulating the world (iteration " << iteration << ")" << endl;
		amountAlive = 0;

		// Let each creature do an action
		for (unsigned int i = 0; i < amountCreatures; i++)
		{
			if (dataCreatures[i].alive && !dataCreatures[i].finished)
			{
				// Do a run
				if (WORLD_VERBOSE) cout << "* World\t\tmake creature " << i << " run it's DNA" << endl;
				runCreature(dataCreatures[i]);
				//if (WORLD_VERBOSE) output();

				// Check if we exceeded the maximum of iterations
				if (dataCreatures[i].iteration >= dataCreatures[i].maxIteration)
				{
					if (WORLD_VERBOSE) cout << "* World\t\tkilling creature as it exceeded it's maximum iterations" << endl;
					dataCreatures[i].alive = false;
				}

				// Check if still alive, if not erase the spot
				if (! dataCreatures[i].alive)
				{
					if (WORLD_VERBOSE) cout << "* World\t\tcreature died, erasing spot" << endl;
					dataWorld[dataCreatures[i].row][dataCreatures[i].column] = "0";
					continue;
				}

				// Check if we reached the end
				if (dataCreatures[i].finished)
				{
					if (WORLD_VERBOSE) cout << "* World\t\tcreature reached endpoint!" << endl;
					dataCreatures[i].finished = true;
					continue;
				}

				// If we got here, our creature still is alive
				amountAlive++;
			}
		}
	}

	//
	// Why did we end?
	//

	if (amountAlive <= 0)
	{
		if (WORLD_VERBOSE) cout << "* World\t\tall creatures died/finished" << endl;
	}
	else
	{
		if (WORLD_VERBOSE) cout << "* World\t\treached maximal iteration count" << endl;
	}
}

bool World::isFinished(int inputID)
{
	return dataCreatures[inputID].finished;
}









/*
 * Private simulation
 */

bool World::conditionCheck(Creature& inputCreature, Gene& inputGene)
{
	// Process each single condition
	for (unsigned int i = 0; i < inputGene.getAmountConditionCodons(); i++)
	{
		// Cowardly refuse if we aint got credits left
		if (inputCreature.iteration >= inputCreature.maxIteration)
			return false;

		// Every condition costs an iteration
		inputCreature.iteration++;

		// Split the current codon in the test and it's parameter
		string tempCondition = inputGene.getConditionCodon(i);
		string tempTest = tempCondition.substr(0, 1);
		string tempParam = tempCondition.substr(1, 1);

		// We have a test on the places
		if (tempTest == "K"  ||  tempTest == "L")
		{
			// Look up the directions
			int *delta;
			//LEAK: delta = new int[2];
			delta = directionDelta(tempParam);
			if (tempParam == "W") delta = directionEff(inputCreature.row, inputCreature.column, targetRow, targetColumn);

			// Calculate new location
			int tempRow = inputCreature.row + delta[0];
			int tempColumn = inputCreature.column + delta[1];
			delete [] delta;

			// Check location
			int tempType = placeType(tempRow, tempColumn);

			// Check if blocked
			if (tempTest == "K" && tempType != 0)
				return false;

			// Check if free
			if (tempTest == "L" && tempType == 0)
				return false;
		}
	}
	return true;
}

void World::actionDo(Creature& inputCreature, Gene& inputGene)
{
	// Process each single condition
	for (unsigned int i = 0; i < inputGene.getAmountActionCodons(); i++)
	{
		// Cowardly refuse if we aint got credits left
		if (inputCreature.iteration >= inputCreature.maxIteration)
			return;

		// Every action costs an iteration
		inputCreature.iteration++;

		// Split the current codon in the test and it's parameter
		string getAction = inputGene.getActionCodon(i);
		string tempTest = getAction.substr(0, 1);
		string tempParam = getAction.substr(1, 1);

		// Move to a new place
		if (tempTest == "G")
		{
			// Look up the directions
			int *delta;
			delta = directionDelta(tempParam);
			if (tempParam == "W") delta = directionEff(inputCreature.row, inputCreature.column, targetRow, targetColumn);

			// Calculate new location
			int tempRow = inputCreature.row + delta[0];
			int tempColumn = inputCreature.column + delta[1];
			delete [] delta;

			// Check location
			int tempType = placeType(tempRow, tempColumn);

			// Check if blocked
			if (tempType != 0)
			{
				if (WORLD_VERBOSE) cout << "! Moved to illegal spot, killing!" << endl;
				inputCreature.alive = false;
				return;
			}

			// Did we reach the end?
			if (dataWorld[tempRow][tempColumn] == "E")
			{
				inputCreature.finished = true;
				dataWorld[inputCreature.row][inputCreature.column] = "0";
				return;
			}

			// Not yet, let's move
			string temp = dataWorld[inputCreature.row][inputCreature.column];

			dataWorld[inputCreature.row][inputCreature.column] = "0";
			dataWorld[tempRow][tempColumn] = temp;

			inputCreature.row = tempRow;
			inputCreature.column = tempColumn;
		}
	}
}

int World::placeType(unsigned int inputRow, unsigned int inputColumn)
{
	// Out of boundary
	if ((inputRow >= dataRows  ||  inputColumn >= dataColumns) || (inputRow < 0  ||  inputColumn < 0))
	{
		return -1;
	}

	// Free
	if (dataWorld[inputRow][inputColumn] == "0"  ||  dataWorld[inputRow][inputColumn] == "E")
	{
		return 0;
	}

	// Blocked
	else return 1;
}

void World::runCreature(Creature& inputCreature)
{
	// Fetch the DNA of our creature
	DNA inputDNA = inputCreature.getDNA();

	// Ask every gene to execute itself on the creature and it's world
	for (int i = 0; i < inputDNA.getAmountGenes(); i++)
	{
		// Get gene
		Gene inputGene = inputDNA.getGene(i);
		if (WORLD_VERBOSE) cout << "* World\t\tnow executing gene " << i << ": " << inputGene.get() << endl;

		// Get operator
		string tempOperator = inputGene.getOperatorCodon();

		// A IF structure
		if (tempOperator == "A")
		{
			if (conditionCheck(inputCreature, inputGene))
			{
				actionDo(inputCreature, inputGene);
			}
		}

		// A IF NOT structure
		else if (tempOperator == "B")
		{
			if (! conditionCheck(inputCreature, inputGene))
			{
				actionDo(inputCreature, inputGene);
			}
		}

		// A WHILE structure
		else if (tempOperator == "C")
		{
			while (conditionCheck(inputCreature, inputGene))
			{
				actionDo(inputCreature, inputGene);
				if (inputCreature.finished || !inputCreature.alive) break;
			}
		}

		// A WHILE NOT structure
		else if (tempOperator == "D")
		{
			while (! conditionCheck(inputCreature, inputGene))
			{
				actionDo(inputCreature, inputGene);
				if (inputCreature.finished || !inputCreature.alive) break;
			}
		}

		// Check if we got credits left
		if (inputCreature.iteration >= inputCreature.maxIteration)
			return;
	}
}





/*
 * Debug
 */

void World::output()
{
	for (unsigned int row = 0; row < dataRows; row++)
	{
		for (unsigned int column = 0; column < dataColumns; column++)
		{
			cout << "\t" << dataWorld[row][column];
		}
		cout << endl;
	}
}
