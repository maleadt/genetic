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
    // Both caches dirty
    cacheQueue = false;
    cacheList = false;
}

// Construct dna object with a given string
DNA::DNA(std::queue<int> inputQueue)
{
	// Save string
	dataQueue = inputQueue;
	cacheQueue = true;

	// Invalidate cache
	cacheList = false;
}

// Construct dna object with a given list
DNA::DNA(std::list<std::list<int> > inputList)
{
	// Save list
	dataList = inputList;
	cacheList = true;

	// Invalidate cache
	cacheQueue = false;
}


//
// Output routines
//

// Get the string representation
std::queue<int> DNA::getQueue()
{
    // Check cache
    if (!cacheQueue)
    {
        toQueue();
        cacheQueue = true;
    }

    // Return
    return dataQueue;
}

// Get the list representation
std::list<std::list<int> > DNA::getList()
{
    // Check cache
    if (!cacheList)
    {
        toList();
        cacheList = true;
    }

    // Return
    return dataList;
}

//
// Debugging routines
//

void DNA::debug_queue()
{
    // Check cache
    if (!cacheQueue)
    {
        toQueue();
        cacheQueue = true;
    }

	// Debug message
	std::cout << "* DNA.debug_queue" << std::endl;

	// Loop queue's contents
	std::cout << "Contents of queue with size " << std::dec << dataQueue.size() << ":" << std::endl;
	std::queue<int> dataQueueDup(dataQueue);
	std::cout << "\t";
	while (!dataQueueDup.empty())
	{
		std::cout << std::hex << "0x" << dataQueueDup.front() << " ";
		dataQueueDup.pop();
	}
	std::cout << std::endl;
}

void DNA::debug_list()
{
    // Check cache
    if (!cacheList)
    {
        toList();
        cacheList = true;
    }

	// Debug message
	std::cout << "* DNA.debug_list" << std::endl;

	// Process list
	std::cout << "Contents of list with size " << std::dec << dataList.size() << ":" << std::endl;
	std::list<std::list<int> >::iterator it = dataList.begin();
	while (it != dataList.end())
	{
		std::cout << "\tlist<int>: ";
		std::list<int>::iterator it2 = it->begin();
		while (it2 != it->end())
			std::cout << std::hex << "0x" << *(it2++) << " ";
		std::cout << std::endl;
		it++;
	}
}


//
// Conversion routines
//

// Convert list to string
void DNA::toQueue()
{
	// Reset the queue
	while (!dataQueue.empty())
		dataQueue.pop();

	// Starting semantics
	dataQueue.push(255);

	// Process all genes
	std::list<std::list<int> >::iterator it = dataList.begin();
	while (it != dataList.end())
	{
		std::list<int>::iterator it2 = it->begin();
		while (it2 != it->end())
			dataQueue.push(*(it2++));
		it++;

		// Only add 0 if not at end (can't fix this later on as queue has no pop_back)
		if (it != dataList.end())
			dataQueue.push(0);
	}

	// Ending semantics
	dataQueue.push(255);
}

// Convert list to string
void DNA::toList()
{
	// Reset the list
	dataList.clear();

	// Duplicate the list and work with that copy
	std::queue<int> dataQueueDup(dataQueue);

	// Check semantics
	if (dataQueueDup.front() != 255)
		throw(std::string("DNA.toList: saved DNA queue doesn't start with 255"));
	dataQueueDup.pop();

	// Process all
	std::list<int> tempVector;
	while (!dataQueueDup.empty() && dataQueueDup.front() != 255)
	{
		if (dataQueueDup.front() == 0)
		{
			dataList.push_back(tempVector);
			tempVector.clear();
		}
		else
		{
			tempVector.push_back(dataQueueDup.front());
		}
		dataQueueDup.pop();
	}

	// Check semantics
	if (dataQueueDup.front() != 255)
		throw(std::string("DNA.toList: saved DNA queue doesn't end with 255"));

	// Save last vector
	dataList.push_back(tempVector);
}
