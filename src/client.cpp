/*
 * client.cpp
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

// Headers
#include "client.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Create client with given DNA
Client::Client(std::list<std::list<int> >& inputList)
{
	dataDNA = inputList;
}


//
// DNA alteration
//

// Mutate the DNA
void Client::mutate()
{
	// Amount of mutations
	int tempAmount = random_range(MUTATE_AMOUNT_LOWER, MUTATE_AMOUNT_UPPER);

	// Process all mutations
	for (int i = 0; i < tempAmount; i++)
	{
		// Pick level of mutation
		int level = random_range(1, 3);

		// Call correct mutation routine
		switch (level)
		{
			case 1:
				mutate_dna();
				break;
			case 2:
				mutate_gen();
				break;
			case 3:
				mutate_codon();
				break;
		}
	}
}

// Combine the DNA with another client
void Client::crossover(Client&)
{
}


//
// DNA output
//

std::list<std::list<int> > Client::getDNA()
{
	return dataDNA;
}


//
// Mutation routines
//

void Client::mutate_dna()
{
	mutate_list(dataDNA);
}

void Client::mutate_gen()
{
	// Pick random gene
	int random = random_range(0, dataDNA.size()-1);
	std::list<std::list<int> >::iterator it = dataDNA.begin();
	for (int i = 0; i < random; i++)
		it++;

	mutate_list(*it);
}

void Client::mutate_codon()
{
}

