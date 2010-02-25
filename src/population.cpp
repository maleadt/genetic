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

// Headers
#include "population.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Constructor with given DNA and environment
Population::Population(Environment* iEnvironment, const DNA& iDNA)
: dataDNA(iDNA), dataEnvironment(iEnvironment) {
}


//
// Output routines
//

const DNA& Population::get() const {
    return dataDNA;
}


//
// Population helper functions
//

// Initialize a population
// TODO: amount == fill functionality
void Population::init(std::vector<CachedClient>& iPopulation, const DNA& iDNA, int iAmount) {
    int fitness = dataEnvironment->fitness(iDNA);
    for (int i = 0; i < iAmount; i++) {
        iPopulation[i].client = new Client(iDNA, dataEnvironment->alphabet());
        iPopulation[i].fitness = fitness;
    }

    for (int i = iAmount; i < POPULATION_BOX_SIZE; i++) {
        iPopulation[i].fitness = 0;
        iPopulation[i].client = 0;
    }
}

// Clean the DNA of a population
void Population::clean(std::vector<CachedClient>& iPopulation, int iStart)
{
    for (int i = 0; i < iStart; i++)
        iPopulation[i].client->clean();
}

// Fill a population with the starting DNA
void Population::fill(std::vector<CachedClient>& iPopulation, int iStart)
{
    // Copy the first clients
    int j = 0;
    for (unsigned int i = iStart; i < iPopulation.size(); i++)
    {
        if (iPopulation[i].client != 0)
            delete iPopulation[i].client;
        iPopulation[i].client = new Client(*iPopulation[j].client);
        iPopulation[i].fitness = iPopulation[j].fitness;
        if (++j == iStart)
            j = 0;
    }
}

// Mutate clients
void Population::mutate(std::vector<CachedClient>& iPopulation, int iStart)
{
    // Mutate clients
    for (unsigned int i = iStart; i < iPopulation.size(); i++)
        iPopulation[i].client->mutate();

    // Calculate new fitness
    for (unsigned int i = iStart; i < iPopulation.size(); i++)
        iPopulation[i].fitness = dataEnvironment->fitness(iPopulation[i].client->get());

    // Sort the population
    std::sort(iPopulation.begin(), iPopulation.end());
}

// Recombine clients
void Population::recombine(std::vector<CachedClient>& iPopulation, int iStart)
{
    // Recombine clients
    int j = 0;
    for (unsigned int i = iStart; i < iPopulation.size(); i++)
    {
        iPopulation[i].client->recombine(*iPopulation[j].client);
        if (j == iStart)
            j = 0;
    }

    // Calculate new fitness
    for (unsigned int i = iStart; i < iPopulation.size(); i++)
        iPopulation[i].fitness = dataEnvironment->fitness(iPopulation[i].client->get());

    // Sort the population
    std::sort(iPopulation.begin(), iPopulation.end());
}
