/*
 * environment.h
 * Evolve - Starting environment
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
#include "environment.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Required routines
//

// WARNING: obviousely at least one of these fitness functions
//          should be derived, as this would otherwise cause
//          a never ending loop!

// Convert the DNA and call the other fitness function
int Environment::fitness(std::queue<int>& inputQueue)
{
	objParser.setQueue(inputQueue);
	std::list<std::vector<int> > tempList = objParser.getList();
	return fitness(tempList);
}

// Convert the DNA and call the other fitness function
int Environment::fitness(std::list<std::vector<int> >& inputList)
{
	objParser.setList(inputList);
	std::queue<int> tempQueue = objParser.getQueue();
	return fitness(tempQueue);
}
