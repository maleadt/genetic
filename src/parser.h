/*
 * parser.h
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

// Include guard
#ifndef __PARSER
#define __PARSER

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

class Parser
{
	public:
		// Construction and destruction
		Parser(std::list<std::vector<int> >&);
		Parser(std::queue<int>&);
		~Parser();

		// Output routines
		std::queue<int> getQueue();
		std::list<std::vector<int> > getList();

		// Debugging routines
		void debug_queue();
		void debug_list();

	private:
		// Data
		std::queue<int> dataQueue;
		std::list<std::vector<int> > dataList;

		// Conversion routines
		void toQueue();
		void toList();
};



// Include guard
#endif
