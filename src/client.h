/*
 * client.h
 * Evolve - Evolutionary client
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
#ifndef __CLIENT
#define __CLIENT

// Headers
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include "generic.h"
#include "dna.h"


//
// Constants
//

// Mutation: amount of mutations
const int MUTATE_AMOUNT_LOWER = 1;
const int MUTATE_AMOUNT_UPPER = 50;


//////////////////////
// CLASS DEFINITION //
//////////////////////

class Client
{
	public:
		// Construction and destruction
		Client(DNA inputDNA);

		// DNA alteration
		void mutate();
		void crossover(Client&);
		void clean();

		// DNA output
		DNA getDNA();

		// Alphabet
		int dataAlphabet;

	private:
		// DNA
		DNA dataDNA;

		// Mutation routines
		void mutate_dna();
		void mutate_gen();
		void mutate_codon();
};

// Mutate a list structurally
template <class T>
void mutate_list(T& inputObject)
{
	// Pick mutation
	 int mutation = 0;
	 int size = inputObject.size();
	 if (size > 1)
	 {
	 	mutation = random_range(1, 5);
	 } else {
	 	mutation = random_range(1, 2);
	 }

	 // Pick random gene(s)
	 int random1 = random_range(0, size-1);
	 int random2 = random_range(0, size-1);
	 while (size > 1 && random1 == random2)
	 	random2 = random_range(0, size-1);

	// Calculate iterators to those genes
	typename T::iterator it1 = inputObject.begin(), it2 = inputObject.begin();
	for (int i = 0; i < random1; i++)
		it1++;
	for (int i = 0; i < random2; i++)
		it2++;


	// Mutate
	switch (mutation)
	{
		// Deletion
		case 1:
		{
			inputObject.erase(it1);
			break;
		}

		// Amplification (at current spot)
		case 2:
		{
			int randAmp = random_range(1, 5);
			for (int i = 0; i < randAmp; i++)
				inputObject.insert(it1, *(it1));
			break;
		}

		// Duplication (at random spot)
		case 3:
		{
			inputObject.insert(it2, *(it1));
			break;
		}

		// Inversion
		case 4:
		{
			std::swap(*(it1), *(it2));
			break;
		}

		// Translocation
		case 5:
		{
		//	T temp = *it1;
		//	inputObject.erase(it1);
		//	inputObject.insert(it2, temp);
			break;
		}

		// Merge
	}
}

// Include guard
#endif
