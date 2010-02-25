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
#include "generic.h"
#include "dna.h"


//
// Constants
//

// Amount of mutations
const int MUTATE_AMOUNT_LOWER = 0;
const int MUTATE_AMOUNT_UPPER = 5;

// Amount of recombinations
const int RECOMBINE_AMOUNT_LOWER = 0;
const int RECOMBINE_AMOUNT_UPPER = 5;

// TODO: move constant usage from recombine() to caller



//////////////////////
// CLASS DEFINITION //
//////////////////////

class Client
{
	public:
		// Construction and destruction
		Client(const Client& inputClient);
		Client(const DNA& inputDNA, int inputAlphabet);
                ~Client();

		// DNA alteration
		void mutate();
		void recombine(Client& inputClient);
		void clean();

		// DNA output
		const DNA& get() const;

	private:
		// DNA properties
		DNA dataDNA;
		int dataAlphabet;

                // Mutation methods
                void mutate_point();
                void mutate_delete();
                void mutate_duplicate();
                void mutate_amplify();
                void mutate_inverse();

                // Recombination methods
                void recombine_insert(Client& inputClient);
                void recombine_crossover_single(Client& inputClient);
                void recombine_crossover_double(Client& inputClient);
};

// Include guard
#endif
