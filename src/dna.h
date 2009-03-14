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
		DNA(std::deque<int> inputQueue);
		DNA(std::list<std::list<int> > inputList);

		// Setters
		void set(std::deque<int> inputQueue);
		void set(std::list<std::list<int> > inputList);
        // Const iterators
        typedef std::list<std::list<int> >::const_iterator const_iterator;
        const_iterator begin() const
        {
                return data.begin();
        }
        const_iterator end() const
        {
                return data.end();
        }

        // Normal iterators
        typedef std::list<std::list<int> >::iterator iterator;
        iterator begin()
        {
                return data.begin();
        }
        iterator end()
        {
                return data.end();
        }

        // Modifyers
        iterator erase(iterator it);
        iterator insert(iterator it, std::list<int>& item);
        void push_back(std::list<int>& item);

		// Informational routines
		int genes() const;
		int size() const;
		std::deque<int> dequeue() const;

		// Debugging routines
		void debug() const;

	private:
		std::list<std::list<int> > data;
};



// Include guard
#endif
