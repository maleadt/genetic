/*
 * dna.h
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

// TODO: write a complete DNA datatype instead of parsing between a queue and list?

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Include guard
#ifndef __DNA
#define __DNA

// Headers
#include <iostream>
#include <vector>
#include <queue>
#include <list>


//
// Constants
//


//////////////////////
// CLASS DEFINITION //
//////////////////////

class DNA
{
	public:
		// Construction and destruction
		DNA();
		DNA(std::queue<int> inputQueue);
		DNA(std::list<std::list<int> > inputList);

		// Output routines
		std::queue<int> getQueue();
		std::list<std::list<int> > getList();

		// Debugging routines
		void debug_queue();
		void debug_list();

	private:
		// Data (TODO: only maintain ONE internal representation)
		std::queue<int> dataQueue;
		std::list<std::list<int> > dataList;

		// Conversion routines
		void toQueue();
		void toList();

		// Cache control
		bool cacheQueue;
		bool cacheList;
};



// Include guard
#endif
