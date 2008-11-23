/*
 * command.cpp
 * Command class.
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
#include "command.h"


//
// Construction and destruction
//

// Constructor
Command::Command()
{
	log(3, "command: constructing");
}

// Copy constructor
Command::Command(const Command& oldCommand)
{
	log(3, "command: constructing by given copy");

	// Copy identifier
	identifier = oldCommand.identifier;

	// Copy all parameters
	std::vector<char>::const_iterator it = oldCommand.parameters.begin();
	while (it != oldCommand.parameters.end())
	{
		parameters.push_back( (*it) );
		++it;
	}

	// Copy flags
	is_condition = oldCommand.is_condition;
	is_action = oldCommand.is_action;
}

void Command::mutate(unsigned int mutLevel)
{
	log(2, "command: checking mutation level");

	if (mutLevel == 0)
	{
		mutateSelf();
	}
}

void Command::mutateSelf()
{
	log(2, "command: mutating self");

	// We need at least 1 parameter
	if (parameters.size() < 1)
		return;

	// Random stuff we'll need
	int randMutation = random_range(1, 5);
	int randomParameter = random_range(0, parameters.size()-1);

	// Iterators to that random stuff
	std::vector<char>::iterator iteratorParameter = parameters.begin();
	for (int i = 0; i < randomParameter; i++)
		++iteratorParameter;

	// Mutate
	switch (randMutation)
	{
		// Deletion
		case 1:
			log(2, "command: deletion");
			parameters.erase(iteratorParameter);
			break;

		// Duplication
		case 2:
			log(2, "command: duplication");
			parameters.push_back( parameters[randomParameter] );
			break;

		// Inversion
		case 3:
			log(2, "command: inversion");
			if (parameters.size() > 1)
			{
				int randomParameter2 = randomParameter;
				while (randomParameter2 == randomParameter)
					randomParameter2 = random_range(0, parameters.size()-1);

				char temp = parameters[randomParameter];
				parameters[randomParameter] = parameters[randomParameter2];
				parameters[randomParameter2] = temp;
			}
			break;

		// Amplification
		case 4:
			log(2, "command: amplification");
			for (int i = 0; i < random_range(1, parameters.size()); i++)
				iteratorParameter = parameters.insert(iteratorParameter, (*iteratorParameter));
				// Vector insertion invalidates all iterators
			break;

		// Point-mutation
		case 5:
			log(2, "command: point-mutation");
			int randType = random_range(0, 1);
			if (randType == 0)
			{
				int randEl = random_range(0, dnaConditions.size()-1);
				std::map<std::string, char>::iterator it = dnaConditions.begin();
				for (int i = 0; i < randEl; i++)
					++it;
				identifier = it->second;
			}
			else if (randType == 1)
			{
				int randParam = random_range(0, parameters.size()-1);
				int randEl = random_range(0, dnaConditions.size()-1);
				std::map<std::string, char>::iterator it = dnaParameters.begin();
				for (int i = 0; i < randEl; i++)
					++it;
				parameters[randParam] = it->second;
			}
			break;
	}
}


//
// Input & output
//

// Input textual data
void Command::inputTextual(std::string inputCommand)
{
	log(3, "command: inputting textual data");

	// Detect instruction type
	unsigned int tempType = 0;	// Condition = 1, Action = 2
	std::map<std::string, char>::iterator it = dnaConditions.begin();
	while (it != dnaConditions.end())
	{
		if (it->second == inputCommand[0])
		{
			tempType = 1;
		}
		++it;
	}
	it = dnaActions.begin();
	while (it != dnaActions.end())
	{
		if (it->second == inputCommand[0])
		{
			tempType = 2;
		}
		++it;
	}

	if (tempType == 0)
		return;
	is_condition = tempType == 1;
	is_action = !is_condition;

	// Split and save parameters
	identifier = inputCommand[0];
	for (unsigned int i = 1; i < inputCommand.size(); i++)
		parameters.push_back(inputCommand[i]);
}

// Output textual data
std::string Command::outputTextual()
{
	log(3, "command: outputting textual data");

	std::string outputTextual;

	// Add identifier
	outputTextual += identifier;

	// Add parameters
	std::vector<char>::iterator it = parameters.begin();
	while (it != parameters.end())
	{
		outputTextual += (*it);
		++it;
	}

	return outputTextual;
}

// Output in code syntax
std::string Command::outputCode()
{
	log(3, "command: outputting code");

	std::string outputCode;

	// Action intendations
	if (is_action)
		outputCode += "\t";

	// Identifier piece (look for key and print it)
	std::map<std::string, char>::iterator it = dnaConditions.begin();
	while (it != dnaConditions.end())
	{
		if (it->second == identifier)
		{
			outputCode += it->first;
		}
		++it;
	}
	it = dnaActions.begin();
	while (it != dnaActions.end())
	{
		if (it->second == identifier)
		{
			outputCode += it->first;
		}
		++it;
	}
	outputCode += " ";

	// Parenthesis for conditions
	if (is_condition)
		outputCode += "( ";

	// Process all parameters
	for (unsigned int i = 0; i < parameters.size(); i++)
	{
		it = dnaParameters.begin();
		while (it != dnaParameters.end())
		{
			if (it->second == parameters[i])
			{
				outputCode += it->first;
			}
			++it;
		}
		outputCode += " ";
	}

	// Parenthesis for conditions
	if (is_condition)
		outputCode += ")";

	return outputCode;
}

//
// Code execution
//

// Check if valid
bool Command::is_valid()
{
	log(3, "command: checking validity");

	// General conditions
	if (	identifier == dnaConditions["if"] ||
			identifier == dnaConditions["if not"] ||
			identifier == dnaConditions["while"] ||
			identifier == dnaConditions["while not"] )
	{
		// We need 2 parameters
		if (parameters.size() != 2)
		{
			return false;
		}

		// First needs to be some things
		if (parameters[0] < dnaParameters["is free"] || parameters[0] > dnaParameters["is blocked"])
		{
			return false;
		}

		// Second needs to be a direction
		if (parameters[1] < dnaParameters["up"] || parameters[1] > dnaParameters["left"])
		{
			return false;
		}

	}

	// Move actions
	else if (	identifier == dnaActions["move"])
	{
			// We need 1 parameter
			if (parameters.size() != 1)
			{
				return false;
			}

			// First needs to be a direction
		if (parameters[0] < dnaParameters["up"] || parameters[0] > dnaParameters["left"])
		{
			return false;
		}
	}

	// Unrecognised identifier
	else
	{
		return false;
	}

	return true;
}
