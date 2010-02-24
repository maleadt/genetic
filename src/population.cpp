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
Population::Population(Environment* inputEnvironment, const DNA& inputDNA) {
    dataDNA = new DNA(inputDNA);
    dataEnvironment = inputEnvironment;
}

// Destructor
Population::~Population() {
    delete dataDNA;
}


//
// Output routines
//

const DNA* Population::get() const
{
    return dataDNA;
}


//
// Population helper functions
//

// Initialize a population
// TODO: amount == fill functionality
void Population::init(std::vector<CachedClient>& population, const DNA* dna, int amount) {
    int fitness = dataEnvironment->fitness(dna);
    for (int i = 0; i < amount; i++) {
        population[i].client = new Client(*dna, dataEnvironment->alphabet());
        population[i].fitness = fitness;
    }

    for (int i = amount; i < POPULATION_BOX_SIZE; i++) {
        population[i].fitness = 0;
        population[i].client = 0;
    }
}

// Clean the DNA of a population
void Population::clean(std::vector<CachedClient>& population, int start)
{
    for (int i = 0; i < start; i++)
        population[i].client->clean();
}

// Fill a population with the starting DNA
void Population::fill(std::vector<CachedClient>& population, int start)
{
    // Copy the first clients
    int j = 0;
    for (unsigned int i = start; i < population.size(); i++)
    {
        if (population[i].client != 0)
            delete population[i].client;
        population[i].client = new Client(*population[j].client);
        population[i].fitness = population[j].fitness;
        if (++j == start)
            j = 0;
    }
}

// Mutate clients
void Population::mutate(std::vector<CachedClient>& population, int start)
{
    // Mutate clients
    for (unsigned int i = start; i < population.size(); i++)
        population[i].client->mutate();

    // Calculate new fitness
    for (unsigned int i = start; i < population.size(); i++)
        population[i].fitness = dataEnvironment->fitness(population[i].client->get());

    // Sort the population
    std::sort(population.begin(), population.end());
}

// Recombine clients
void Population::recombine(std::vector<CachedClient>& population, int start)
{
    // Recombine clients
    int j = 0;
    for (unsigned int i = start; i < population.size(); i++)
    {
        population[i].client->recombine(*population[j].client);
        if (j == start)
            j = 0;
    }

    // Calculate new fitness
    for (unsigned int i = start; i < population.size(); i++)
        population[i].fitness = dataEnvironment->fitness(population[i].client->get());

    // Sort the population
    std::sort(population.begin(), population.end());
}
