/*
 * dna.cpp
 * DNA data structure
 *
 * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
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

#ifndef __DNA /* include guard */
#define __DNA


/*
 * Configuration
 */

// Needed headers
#include <iostream>
#include "gen.h"


/*
 * Class defenition
 */

class DNA
{
	// Public member data, can be viewed from anywhere
	public:
		// Construction
		DNA();
		DNA(string);
		//~DNA();

		// Basic IO
		void set(string);
		string get();
		void output();

		// Gene modification
		void setGene(unsigned int, Gene);
		void addGene(Gene);
		void removeGene(unsigned int);
		void insertGene(unsigned int, Gene);
		Gene getGene(unsigned int);
		int getAmountGenes();

		// Mutation
		void mutate();
		void mutateCleanGenes();
		void mutateCleanDNA();
		void mutateGen();
		void mutateChromosome();
		void mutatePoint();
		void mutateDirection();

		// Flags
		bool valid;

	// Private member data, only for internal usage
	private:
		// Private data
		string dataString;
		Gene *dataGenes;
		unsigned int amountGenes;
		bool haveString;

		// Private routines
		void parse();
		void rebuild();
};

#endif /* __DNA */

