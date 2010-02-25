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
#include <vector>


//
// Constants
//

// Box
const int POPULATION_BOX_SIZE = 50;
const int POPULATION_BOX_THRESHOLD = 10;



//////////////////////
// CLASS DEFINITION //
//////////////////////

// Population
struct CachedClient;
class Population
{
    public:
        // Construction and destruction
        Population(Environment* inputEnvironment, const DNA& inputDNA);
        ~Population();

        // Output routines
        const DNA* get() const;

        // Evolutionary methods
        virtual void evolve() = 0;

    protected:
        // Population helper methods
        void init(std::vector<CachedClient>& population, const DNA* dna, int amount);
        void clean(std::vector<CachedClient>& population, int start);
        void fill(std::vector<CachedClient>& population, int start);
        void mutate(std::vector<CachedClient>& population, int start);
        void recombine(std::vector<CachedClient>& population, int start);

        // Current DNA
        const DNA* dataDNA;
        Environment* dataEnvironment;
};

// A struct containing a client, as well as a field for its fitness
struct CachedClient
{
    Client* client;
    double fitness;
    friend bool operator<(const CachedClient& left, const CachedClient& right)
    {
        return (left.fitness > right.fitness);
    }
};
struct CachedClient_more {
    bool operator ()(CachedClient const& a, CachedClient const& b) const {
        if (a.fitness < b.fitness) return false;
        if (a.fitness > b.fitness) return true;

        return true;
    }
};



// Include guard
#endif
