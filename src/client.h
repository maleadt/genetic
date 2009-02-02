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
#include <vector>
#include "generic.h"


//
// Constants
//

// Mutation: amount of mutations
const int MUTATE_AMOUNT_LOWER = 1;
const int MUTATE_AMOUNT_UPPER = 5;


//////////////////////
// CLASS DEFINITION //
//////////////////////

class Client
{
	public:
		// Construction and destruction
		Client(std::list<std::vector<int> >&);

		// DNA alteration
		void mutate();
		void crossover(Client&);

		// DNA output
		std::list<std::vector<int> > getDNA();

		// Alphabet
		int dataAlphabet;

	private:
		// DNA
		std::list<std::vector<int> > dataDNA;

		// Mutation routines
		void mutate_dna();
		void mutate_gen();
		void mutate_codon();
};

// Include guard
#endif
