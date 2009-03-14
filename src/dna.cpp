/*
 * dna.cpp
 * Evolve - DNA datastructure.
 *
 * Copyright (c) 2009 Tim Besard <tim.besard@gmail.com>
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
#include "dna.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Empty constructor (should not be used, only to be able to use in class definitions)
DNA::DNA()
{
}

// Construct dna object with a given string
DNA::DNA(std::queue<int> inputQueue)
{
	set(inputQueue);
}

// Construct dna object with a given list
DNA::DNA(std::list<std::list<int> > inputList)
{
	set(inputList);
}


//
// Setters
//

void DNA::set(std::list<std::list<int> > inputList)
{
    data = inputList;
}

void DNA::set(std::queue<int> inputQueue)
{
	// Reset the list
	data.clear();

	// Duplicate the list and work with that copy
	std::queue<int> inputQueueDup(inputQueue);

	// Check semantics
	if (inputQueueDup.front() != 255)
		throw(std::string("DNA.toList: saved DNA queue doesn't start with 255"));
	inputQueueDup.pop();

	// Process all
	std::list<int> tempVector;
	while (!inputQueueDup.empty() && inputQueueDup.front() != 255)
	{
		if (inputQueueDup.front() == 0)
		{
			data.push_back(tempVector);
			tempVector.clear();
		}
		else
		{
			tempVector.push_back(inputQueueDup.front());
		}
		inputQueueDup.pop();
	}

	// Check semantics
	if (inputQueueDup.front() != 255)
		throw(std::string("DNA.toList: saved DNA queue doesn't end with 255"));

	// Save last vector
	data.push_back(tempVector);
}


//
// Modifyers
//

// Erase an element
DNA::iterator DNA::erase(iterator it)
{
    return data.erase(it);
}

// Insert a gene
DNA::iterator DNA::insert(iterator it, std::list<int>& item)
{
    return data.insert(it, item);
}


//
// Informational routines
//

// Amount of genes
int DNA::genes() const
{
    return data.size();
}
int DNA::size() const
{
    return data.size();
}


//
// Debugging routines
//

void DNA::debug() const
{
	// Debug message
	std::cout << "* DNA.debug" << std::endl;

	// Process list
	std::cout << "Contents of list with size " << std::dec << data.size() << ":" << std::endl;
	std::list<std::list<int> >::const_iterator it = data.begin();
	while (it != data.end())
	{
		std::cout << "\tlist<int>: ";
		std::list<int>::const_iterator it2 = it->begin();
		while (it2 != it->end())
			std::cout << std::hex << "0x" << *(it2++) << " ";
		std::cout << std::endl;
		it++;
	}
}

