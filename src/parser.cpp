/*
 * parser.cpp
 * Evolve - DNA conversion routines.
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
#include "parser.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Input routines
//

// Construct parser object with a given string
void Parser::setQueue(std::queue<int>& inputQueue)
{
	// Save string
	dataQueue = inputQueue;

	// Convert
	toList();
}

// Construct parser object with a given list
void Parser::setList(std::list<std::vector<int> >& inputList)
{
	// Save list
	dataList = inputList;

	// Convert
	toQueue();
}


//
// Output routines
//

// Get the string representation
std::queue<int> Parser::getQueue()
{
	return dataQueue;
}

// Get the list representation
std::list<std::vector<int> > Parser::getList()
{
	return dataList;
}

//
// Debugging routines
//

void Parser::debug_queue()
{
	// Debug message
	std::cout << "* Parser.debug_queue" << std::endl;

	// Loop queue's contents
	std::cout << "Contents of queue with size " << dataQueue.size() << ":" << std::endl;
	std::queue<int> dataQueueDup(dataQueue);
	while (!dataQueueDup.empty())
	{
		std::cout << std::hex << dataQueueDup.front() << " ";
		dataQueueDup.pop();
	}
	std::cout << std::endl;
}

void Parser::debug_list()
{
	// Debug message
	std::cout << "* Parser.debug_list" << std::endl;

	// Process list
	std::cout << "Contents of list with size " << dataList.size() << ":" << std::endl;
	std::list<std::vector<int> >::iterator it = dataList.begin();
	while (it != dataList.end())
	{
		std::cout << "\tvector<int>: ";
		for (int i = 0; i < it->size(); i++)
			std::cout << std::hex << it->at(i) << " ";
		std::cout << std::endl;
		it++;
	}
}


//
// Conversion routines
//

// Convert list to string
void Parser::toQueue()
{
	// Reset the queue
	while (!dataQueue.empty())
		dataQueue.pop();

	// Starting semantics
	dataQueue.push(0x255);

	// Process all genes
	std::list<std::vector<int> >::iterator it = dataList.begin();
	while (it != dataList.end())
	{
		for (int i = 0; i < it->size(); i++)
			dataQueue.push(it->at(i));
		dataQueue.push(0x0);
		it++;
	}

	// Ending semantics
	dataQueue.pop();
	dataQueue.push(0x255);
}

// Convert list to string
void Parser::toList()
{
	// Reset the list
	dataList.clear();

	// Duplicate the list and work with that copy
	std::queue<int> dataQueueDup(dataQueue);

	// Check semantics
	if (dataQueueDup.front() != 0x255)
		throw(std::string("Parser.toList: saved DNA queue doesn't start with 0x255"));
	dataQueueDup.pop();

	// Process all
	std::vector<int> tempVector;
	while (!dataQueueDup.empty() && dataQueueDup.front() != 0x255)
	{
		if (dataQueueDup.front() == 0x0)
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
	if (dataQueueDup.front() != 0x255)
		throw(std::string("Parser.toList: saved DNA queue doesn't end with 0x255"));

	// Save last vector
	dataList.push_back(tempVector);
}
