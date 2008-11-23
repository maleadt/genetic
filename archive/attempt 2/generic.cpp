/*
 * generic.cpp
 * Generic routines.
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
#include "generic.h"

// Constants
const bool GENERIC_VERBOSE = false;
bool RAND_SET = false;


/*
 * Random generators
 */

// Will contain lowest and highest
int random_range(int lowest_number, int highest_number)
{
	if (GENERIC_VERBOSE) cout << "* Generic\tcalculating random integer between " << lowest_number << " and " << highest_number << endl;
	// Set seed
	if (!RAND_SET)
	{
		srand((unsigned)time(0));
		RAND_SET = true;
	}

	// Swap the numbers if needed
	if(lowest_number > highest_number){
	swap(lowest_number, highest_number);
	}

	// Calculate the range
	int range = highest_number - lowest_number + 1;

    // Return a number
	return lowest_number + rand()%range;
}

string random_char(string inputString)
{
	int length = inputString.length();
	int randomIndex = random_range(0, length-1);
	return inputString.substr(randomIndex, 1);
}

bool isOperator(string inputChar)
{
	unsigned int tempIndex = dataDNAOperators.find_first_of(inputChar);

	if ((tempIndex >= 0) && (tempIndex < dataDNAOperators.length()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isCondition(string inputChar)
{
	unsigned int tempIndex = dataDNAConditions.find_first_of(inputChar);

	if ((tempIndex >= 0) && (tempIndex < dataDNAConditions.length()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isAction(string inputChar)
{
	unsigned int tempIndex = dataDNAActions.find_first_of(inputChar);

	if ((tempIndex >= 0) && (tempIndex < dataDNAActions.length()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isDirection(string inputChar)
{
	unsigned int tempIndex = dataDNADirections.find_first_of(inputChar);

	if ((tempIndex >= 0) && (tempIndex < dataDNADirections.length()))
	{
		return true;
	}
	else
	{
		return false;
	}
}




/*
 * Direction parameter parsing
 */

int *directionDelta (string inputDirection)
{
	// Initialize
	int tempX = 0;
	int tempY = 0;

	if (inputDirection == "O"  ||  inputDirection == "S"  ||  inputDirection == "T")
		tempX++;
	if (inputDirection == "P"  ||  inputDirection == "U"  ||  inputDirection == "V")
		tempX--;
	if (inputDirection == "Q"  ||  inputDirection == "S"  ||  inputDirection == "U")
		tempY--;
	if (inputDirection == "R"  ||  inputDirection == "T"  ||  inputDirection == "V")
		tempY++;

	int *delta;
	delta = new int[2];
	delta[1] = tempX;
	delta[0] = tempY;

	return delta;
}

int *directionEff (int creatureRow, int creatureColumn, int targetRow, int targetColumn)
{
	// Initialize
	double deltaRow = 0;
	double deltaColumn = 0;

	deltaRow = targetRow - creatureRow;
	deltaColumn = targetColumn - creatureColumn;

	double max;
	if (deltaRow > deltaColumn)
	{
		max = deltaRow;
	}
	else {
		max = deltaColumn;
	}

	deltaRow = (deltaRow+0.25)/max;
	deltaColumn = (deltaColumn+0.25)/max;

	int *delta;
	delta = new int[2];
	delta[0] = (int)deltaRow;
	delta[1] = (int)deltaColumn;

	return delta;
}
