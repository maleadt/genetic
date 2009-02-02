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
	// Pick mutation
	 int mutation = 0;
	 if (dataDNA.size() > 1)
	 {
	 	mutation = random_range(1, 5);
	 } else {
	 	mutation = random_range(1, 2);
	 }

	 // Pick random gene(s)
	 int random1 = random_range(0, dataDNA.size()-1);
	 int random2 = random_range(0, dataDNA.size()-1);
	 while (dataDNA.size() > 1 && random1 == random2)
	 	random2 = random_range(0, dataDNA.size()-1);

	// Calculate iterators to those genes
	std::list<std::list<int> >::iterator it1 = dataDNA.begin(), it2 = dataDNA.begin();
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
			dataDNA.erase(it1);
			break;
		}

		// Amplification (at current spot)
		case 2:
		{
			int randAmp = random_range(1, dataDNA.size());
			for (int i = 0; i < randAmp; i++)
				dataDNA.insert(it1, *(it1));
			break;
		}

		// Duplication (at random spot)
		case 3:
		{
			dataDNA.insert(it2, *(it1));
			break;
		}

		// Inversion
		case 4:
		{
			swap(*(it1), *(it2));
			break;
		}

		// Translocation
		case 5:
		{
			std::list<int> temp = *it1;
			dataDNA.erase(it1);
			dataDNA.insert(it2, temp);
			break;
		}
	}
}

void Client::mutate_gen()
{
}

void Client::mutate_codon()
{
}

