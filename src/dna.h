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
#include <iomanip>
#include <cstring>
#include <cstdlib>


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
		DNA(const DNA& inputData);
                DNA(const unsigned char* inputData, int inputSize);
                ~DNA();

		// Informational routines
		unsigned int genes() const;
		unsigned int length() const;

                // Modifiers
                bool erase(unsigned int index);
                bool insert(unsigned int index, unsigned char* gene, unsigned int size);
                void push_back(unsigned char* item, unsigned int size);

                // Operators
                bool operator== (const DNA& dna);
                bool operator!= (const DNA& dna);

		// Debugging routines
		void debug() const;
		void debug_raw() const;
                
                // Auxiliary
                unsigned char* ptr_move(unsigned int inputLocation);
                unsigned char* ptr_set(unsigned int inputLocation);
                unsigned int separator(unsigned int index) const;
                unsigned int gene_start(unsigned int index) const;
                unsigned int gene_end(unsigned int index) const;

	private:
                // Member data
		unsigned char* dataGenes;
                unsigned int dataSize;
                unsigned char* dataPointer;
                unsigned int dataPointerLocation;
};



// Include guard
#endif
