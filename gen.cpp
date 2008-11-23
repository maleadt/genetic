/*
 * gen.cpp
 * Gen class.
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
#include "world.h"
#include "creature.h"
#include "dna.h"
#include "gen.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

Gen::Gen()
{
}

Gen::Gen(Gen const& oldGen)
{
	// Copy all commands
	std::list<Command*>::const_iterator it = oldGen.dataCommands.begin();
	while (it != oldGen.dataCommands.end())
	{
		// Copy all commands and save them
		dataCommands.push_back( new Command( (*(*it)) ) );

		++it;
	}

	// Copy other data
	parent = oldGen.parent;
	dataRepeat = oldGen.dataRepeat;
}

Gen::~Gen()
{
	// Remove all commands
	std::list<Command*>::iterator it_com = dataCommands.begin();
	while (it_com != dataCommands.end())
	{
		delete (*it_com);
		++it_com;
	}
	dataCommands.clear();
}

//
// Input & output
//

// Input textual data
void Gen::inputTextual(std::string inputText)
{
	// Detect genes, and create a new object for them
	unsigned int index_start = 0;
	while (index_start < inputText.length())
	{
		unsigned int index_end = index_start+1;
		while (index_end < inputText.length() && inputText[index_end] != dnaSeparators["command"])
			index_end++;

		Command* outputCommand = new Command;
		outputCommand->inputTextual(inputText.substr(index_start, index_end - index_start));
		dataCommands.push_back(outputCommand);

		index_start = index_end+1;
	}
}

// Output textual data
std::string Gen::outputTextual()
{
	std::string outputTextual;

	// Add all commands
	std::list<Command*>::iterator it = dataCommands.begin();
	while (it != dataCommands.end())
	{
		if (it != dataCommands.begin())
			outputTextual += dnaSeparators["command"];
		outputTextual += (*it)->outputTextual();
		++it;
	}

	return outputTextual;
}

// Output code
std::string Gen::outputCode()
{
	std::string outputCode;

	// Add all conditions
	std::list<Command*>::iterator it = dataCommands.begin();
	int amountconditions = 0;
	while (it != dataCommands.end())
	{
		if ((*it)->is_condition)
		{
			if (amountconditions > 0)
				outputCode += "and ";

			outputCode += (*it)->outputCode();
			outputCode += "\n";
			amountconditions++;
		}
		++it;
	}

	// Add all actions
	it = dataCommands.begin();
	while (it != dataCommands.end())
	{
		if ((*it)->is_action)
		{
			outputCode += (*it)->outputCode();
			outputCode += "\n";
			amountconditions++;
		}
		++it;
	}

	return outputCode;
}


//
// Code execution helpers
//

void Gen::DeltaFromDirection(char inputDir, int &dx, int &dy)
{
	if (inputDir == dnaParameters["up"])
	{
		dx = 0;
		dy = -1;
	}
	else if (inputDir == dnaParameters["right"])
	{
		dx = 1;
		dy = 0;
	}
	else if (inputDir == dnaParameters["down"])
	{
		dx = 0;
		dy = 1;
	}
	else if (inputDir == dnaParameters["left"])
	{
		dx = -1;
		dy = 0;
	}

	else
	{
		dx = 0;
		dy = 0;
	}
}

//
// Code execution
//

void Gen::execute()
{
	dataRepeat = true;

	while (parent->parent->credits <= parent->parent->pointerWorld->creatureMaxCommands && dataRepeat && evaluateAll())
	{
		actAll();
	}
}

bool Gen::evaluateAll()
{
	// Essential stuff (an overhead credit)
	parent->parent->credits++;

	std::list<Command*>::iterator it = dataCommands.begin();
	while (it != dataCommands.end())
	{
		if ((*it)->is_condition)
		{
			if (!evaluate(*it) || parent->parent->credits > parent->parent->pointerWorld->creatureMaxCommands)
				return false;
		}
		++it;
	}
	return true;
}

bool Gen::evaluate(Command* inputCommand)
{
	// Do we have a repeating check (while / while not)?
	dataRepeat = (	inputCommand->identifier == dnaConditions["while"]		||
				   inputCommand->identifier == dnaConditions["while not"]	);

	// Do we have an inverse conditional?
	bool tempInvert = (	inputCommand->identifier == dnaConditions["if not"]		||
						inputCommand->identifier == dnaConditions["while not"]	);

	// Essential stuff
	bool outputBool = false;
	parent->parent->credits++;

	// We need at least 2 parameter (for now)
	if (inputCommand->parameters.size() < 2)
		return false;

	// An "is free" check
	if (inputCommand->parameters[0] == dnaParameters["is free"])
	{
		// Get the requested location
		int x = parent->parent->locationX;
		int y = parent->parent->locationY;
		int dx, dy;
		DeltaFromDirection(inputCommand->parameters[1], dx, dy);
		outputBool = (parent->parent->pointerWorld->checkSpot(x+dx, y+dy) == WORLD_FREE);
	}

	// An "is blocked" check
	else if (inputCommand->parameters[0] == dnaParameters["is blocked"])
	{
		// Get the requested location
		int x = parent->parent->locationX;
		int y = parent->parent->locationY;
		int dx, dy;
		DeltaFromDirection(inputCommand->parameters[1], dx, dy);
		outputBool = !(parent->parent->pointerWorld->checkSpot(x+dx, y+dy) == WORLD_FREE);
	}

	// Now invert checks if needed
	if (tempInvert)
		outputBool = !outputBool;

	return outputBool;
}

void Gen::actAll()
{
	// Essential stuff (an overhead credit)
	parent->parent->credits++;

	std::list<Command*>::iterator it = dataCommands.begin();
	while (it != dataCommands.end() && parent->parent->credits <= parent->parent->pointerWorld->creatureMaxCommands && parent->parent->status == CREATURE_ALIVE)
	{
		if ((*it)->is_action)
		{
			act(*it);
		}
		++it;
	}
}

void Gen::act(Command* inputCommand)
{
	// Essential stuff
	parent->parent->credits++;

	// We need at least 1 parameter
	if (inputCommand->parameters.size() < 1)
		return;

	// The "move" action
	if (inputCommand->identifier == dnaActions["move"])
	{
		int dx, dy;
		DeltaFromDirection(inputCommand->parameters[0], dx, dy);
		parent->parent->move(dx, dy);
	}
}


//
// Code mutation
//

void Gen::mutate(unsigned int mutLevel)
{
	if (mutLevel == 0)
	{
		mutateSelf();
	}
	else
	{
		std::list<Command*>::iterator it = dataCommands.begin();
		for (int i = 0; i < random_range(0, dataCommands.size()-1); i++)
			++it;
		(*it)->mutate(mutLevel-1);
	}
}

void Gen::mutateSelf()
{
	// We need at least 1 command
	if (dataCommands.size() < 1)
		return;

	// Get some random stuff we'll need
	int randomMutation = random_range(1, 5);
	int randomCommand = random_range(0, dataCommands.size()-1);
	int randomCommand2 = randomCommand;
	if (dataCommands.size() > 1)
	{
		while (randomCommand == randomCommand2)
			randomCommand2 = random_range(0, dataCommands.size()-1);
	}
	std::list<Command*>::iterator iteratorCommand = dataCommands.begin();
	for (int i = 0; i < randomCommand; i++)
		++iteratorCommand;
	std::list<Command*>::iterator iteratorCommand2 = dataCommands.begin();
	for (int i = 0; i < randomCommand2; i++)
		++iteratorCommand2;

	// Mutate
	switch (randomMutation)
	{
		// Deletion
		case 1:
			delete (*iteratorCommand);
			dataCommands.erase(iteratorCommand);
			break;

		// Duplication
		case 2:
			dataCommands.push_back( new Command(*(*iteratorCommand)) );
			break;

		// Inversion
		case 3:
			if (dataCommands.size() > 1)
			{
				Command* temp = *iteratorCommand;
				*iteratorCommand = *iteratorCommand2;
				*iteratorCommand2 = temp;
			}
			break;

		// Amplification
		case 4:
			for (int i = 0; i < random_range(1, dataCommands.size()); i++)
				dataCommands.insert(iteratorCommand, new Command(*(*iteratorCommand)));;
			break;

		// Translocation
		case 5:
			if (dataCommands.size() > 1)
			{
				Command* tempCommand = *iteratorCommand;
				dataCommands.erase(iteratorCommand);
				dataCommands.insert(iteratorCommand2, tempCommand);
			}
	}
}

// Check if valid
bool Gen::is_valid()
{
	// Add all commands
	std::list<Command*>::iterator it = dataCommands.begin();
	while (it != dataCommands.end())
	{
		if (! ((*it)->is_valid()))
		{
			return false;
		}
		++it;
	}
	return true;
}
