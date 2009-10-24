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

// Constructor with given environment
Population::Population(Environment* inputEnvironment)
{
    // Save all data
    dataEnvironment = inputEnvironment;
}

// Constructor with given DNA and environment
Population::Population(Environment* inputEnvironment, DNA inputDNA)
{
    // Save all data
    set(inputDNA);
    dataEnvironment = inputEnvironment;
}


//
// Output routines
//

DNA Population::get() const
{
    return dataDNA;
}

void Population::set(DNA& inputDNA)
{
    dataDNA = inputDNA;
}


//
// Evolutionary methods
//

// Evolve a single client straightly
void Population::evolve_single_straight()
{
    // Calculate current fitness
    double dataFitness = dataEnvironment->fitness(dataDNA);

    // Loop
    while (dataEnvironment->condition())
    {
        // Create a client, and mutate the DNA
        Client tempClient(dataDNA);
        tempClient.dataAlphabet = dataEnvironment->alphabet();
        tempClient.mutate();

        // Compare the new DNA
        const DNA* tempDNA = tempClient.get();
        double tempFitness = dataEnvironment->fitness(tempDNA);
        if (tempFitness > dataFitness)
        {
            dataFitness = tempFitness;
            dataDNA = tempClient.get();
            dataEnvironment->update(dataDNA);
        }
    }
}

// Evolve a population
void Population::evolve_population()
{
    // Allocate new population
    std::vector<CachedClient> population(POPULATION_BOX_SIZE);
    population[0].fitness = dataEnvironment->fitness(dataDNA);
    population[0].client = dataDNA;
    population[0].client.dataAlphabet = dataEnvironment->alphabet();
    fill(population, 1);

    // Initial mutation
    mutate(population, 1);

    // Critical fitness
    double fitness_critical = 0;

    // Loop
    while (dataEnvironment->condition())
    {
        // Check if we got good mutations
        if (population[0].fitness == -1)
            throw std::string("No successfull mutations...");

        // Get good region
        int threshold = POPULATION_BOX_THRESHOLD -1;
        while (population[threshold].fitness == -1)
            threshold--;

        // Update?
        if (population[0].fitness > fitness_critical)
        {
            fitness_critical = population[0].fitness;
            dataDNA = population[0].client.get();
            dataEnvironment->update(dataDNA);    // TODO: pass fitness
        }

        // Refill the population
        clean(population, threshold+1);
        fill(population, threshold+1);

        // Shuffle the population
        std::vector<CachedClient>::iterator it = population.begin();
        std::advance(it, threshold+1);
        std::random_shuffle(it, population.end());

        // Mutate new ones
        recombine(population, threshold+1);
    }
}

// Evolve a population straightly
void Population::evolve_population_straight()
{
    // Allocate new population
    std::vector<CachedClient> population(POPULATION_BOX_SIZE);
    population[0].fitness = dataEnvironment->fitness(dataDNA);
    population[0].client = dataDNA;
    population[0].client.dataAlphabet = dataEnvironment->alphabet();
    fill(population, 1);

    // Initial mutation
    mutate(population, 1);

    // Critical fitness
    double fitness_critical = 0;

    // Loop
    while (dataEnvironment->condition())
    {
        // Check if we got good mutations
        if (population[0].fitness == -1)
            throw std::string("No successfull mutations...");

        // Get good region
        int threshold = POPULATION_BOX_THRESHOLD -1;
        while (population[threshold].fitness == -1)
            threshold--;

        // Update?
        if (population[0].fitness > fitness_critical)
        {
            fitness_critical = population[0].fitness;
            dataDNA = population[0].client.get();
            dataEnvironment->update(dataDNA);    // TODO: pass fitness
        }

        // Refill the population
        clean(population, threshold+1);
        fill(population, threshold+1);

        // Mutate new ones
        mutate(population, threshold+1);
    }
}

// Evolve two populations simulteanously
void Population::evolve_population_dual()
{
    // Allocate first population
    std::vector<CachedClient> population1(POPULATION_BOX_SIZE);
    population1[0].fitness = dataEnvironment->fitness(dataDNA);
    population1[0].client = dataDNA;
    population1[0].client.dataAlphabet = dataEnvironment->alphabet();
    fill(population1, 1);
    mutate(population1, 1);

    // Allocate second population
    std::vector<CachedClient> population2(POPULATION_BOX_SIZE);
    population2[0].fitness = dataEnvironment->fitness(dataDNA);
    population2[0].client = dataDNA;
    population2[0].client.dataAlphabet = dataEnvironment->alphabet();
    fill(population2, 1);
    mutate(population2, 1);

    // Critical fitness
    double fitness_critical = 0;

    // Population pointer
    std::vector<CachedClient>* population = &population1;
    std::vector<CachedClient>* population_other = &population2;

    // Loop
    while (dataEnvironment->condition())
    {
        // Alter populations
        std::vector<CachedClient>* temp = population;
        population = population_other;
        population_other = temp;

        // Check if we got good mutations
        if ((*population)[0].fitness == -1 && (*population)[0].fitness == -1)
            throw std::string("No successfull mutations...");

        // Get good region
        int threshold = POPULATION_BOX_THRESHOLD -1;
        while ((*population)[threshold].fitness == -1)
            threshold--;

        // Update?
        if ((*population)[0].fitness > fitness_critical)
        {
            fitness_critical = (*population)[0].fitness;
            dataDNA = (*population)[0].client.get();
            dataEnvironment->update(dataDNA);    // TODO: pass fitness
        }

        // Refill the population
        clean(*population, threshold+1);
        fill((*population), threshold+1);

        // Shuffle the population
        std::vector<CachedClient>::iterator it = population->begin();
        std::advance(it, threshold+1);
        std::random_shuffle(it, population->end());

        // Population cross-contamination!
        if (random_int(1, 27) == 13)
            std::swap( (*population)[0], (*population_other)[0] );

        // Mutate new ones
        recombine((*population), threshold+1);
    }
}

//
// Population helper functions
//

// Clean the DNA of a population
void Population::clean(std::vector<CachedClient>& population, int start)
{
    for (int i = 0; i < start; i++)
        population[i].client.clean();
}

// Fill a population with the starting DNA
void Population::fill(std::vector<CachedClient>& population, int start)
{
    // Copy the DNA of the first clients to the rest of the population
    int j = 0;
    for (unsigned int i = start; i < population.size(); i++)
    {
        population[i].client = population[j++].client;
        if (j == start)
            j = 0;
    }

    // Copy the fitness of the first clients
    j = 0;
    for (unsigned int i = start; i < population.size(); i++)
    {
        population[i].fitness = population[j++].fitness;
        if (j == start)
            j = 0;
    }
}

// Mutate clients
void Population::mutate(std::vector<CachedClient>& population, int start)
{
    // Mutate clients
    for (unsigned int i = start; i < population.size(); i++)
        population[i].client.mutate();

    // Calculate new fitness
    for (unsigned int i = start; i < population.size(); i++)
        population[i].fitness = dataEnvironment->fitness(population[i].client.get());

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
        population[i].client.recombine(population[j].client);
        if (j == start)
            j = 0;
    }

    // Calculate new fitness
    for (unsigned int i = start; i < population.size(); i++)
        population[i].fitness = dataEnvironment->fitness(population[i].client.get());

    // Sort the population
    std::sort(population.begin(), population.end());
}
