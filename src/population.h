/*
 * Population.h
 * Evolve - Population handling
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
#ifndef __POPULATION
#define __POPULATION

// Headers
#include "client.h"
#include "environment.h"
#include "dna.h"
#include <list>
#include <queue>
#include <vector>
#include <algorithm>


//
// Constants
//

// Box
const int POPULATION_BOX_SIZE = 50;
const int POPULATION_BOX_THRESHOLD = 5;


//////////////////////
// CLASS DEFINITION //
//////////////////////

// Population
struct CachedClient;
class Population
{
    public:
        // Construction and destruction
        Population(Environment* inputEnvironment, DNA inputDNA);

        // Output routines
        DNA get() const;

        // Evolutionary methods
        void evolve_single_straight(int iterations);
        void evolve_box(int iterations, int process);
        void evolve_box_straight(int iterations);
        void evolve_box_straight_process(std::vector<CachedClient>& inputBox);
        void evolve_box_together(int iterations);
        void evolve_box_together_process(std::vector<CachedClient>& inputBox);

    private:
        // Current DNA
        DNA dataDNA;
        Environment* dataEnvironment;
};

// A struct containing a client, as well as a field for its fitness
struct CachedClient
{
    Client client;
    double fitness;
    friend bool operator<(const CachedClient& left, const CachedClient& right)
    {
        return (left.fitness > right.fitness);
    }
};


// Include guard
#endif
