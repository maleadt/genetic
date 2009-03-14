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
Population::Population(Environment* inputEnvironment, DNA inputDNA)
{
    // Save all data
    dataDNA = inputDNA;
    dataEnvironment = inputEnvironment;
}


//
// Output routines
//

DNA Population::get() const
{
    return dataDNA;
}


//
// Evolutionary methods
//

// Evolve a single client straightly
void Population::evolve_single_straight(int iterations)
{
    // Calculate current fitness
    double dataFitness = dataEnvironment->fitness(dataDNA);
    std::cout << "* Started single straight evolution, initial fitness is " << dataFitness << std::endl;

    // Loop
    while (iterations > 0)
    {
        // Create a client, and mutate the DNA
        Client tempClient(dataDNA);
        tempClient.dataAlphabet = dataEnvironment->alphabet();
        tempClient.mutate();

        // Compare the new DNA
        DNA tempDNA = tempClient.get();
        double tempFitness = dataEnvironment->fitness(tempDNA);
        if (tempFitness > dataFitness)
        {
            dataFitness = tempFitness;
            dataDNA = tempClient.get();
            dataEnvironment->update(dataDNA);
        }

        iterations--;
    }
}

// Evolve a box of clients straightly
void Population::evolve_box_straight(int iterations)
{
    evolve_box(iterations, 1);
}
void Population::evolve_box_straight_process(std::vector<CachedClient>& inputBox)
{
    // Look up valid threshold
    int limit = POPULATION_BOX_THRESHOLD;
    while (inputBox[limit].fitness == -1)
        limit--;

    // Fill the rest of the box with copies of best x mutations
    int j = 0;
    for (int i = limit; i < POPULATION_BOX_SIZE; i++)
    {
        inputBox[i].client = Client(inputBox[j++].client);
        if (j >= limit)
            j = 0;
    }

    // Mutate the box
    for (int i = limit; i < POPULATION_BOX_SIZE; i++)
        inputBox[i].client.mutate();

    // Fill the newely created fitness fields in the box
    for (int i = limit; i < POPULATION_BOX_SIZE; i++)
        inputBox[i].fitness = dataEnvironment->fitness(inputBox[i].client.get());
}

// Evolve a box of clients together
void Population::evolve_box_together(int iterations)
{
    evolve_box(iterations, 2);
}
void Population::evolve_box_together_process(std::vector<CachedClient>& inputBox)
{
    // Look up valid threshold
    int limit = POPULATION_BOX_THRESHOLD;
    while (inputBox[limit].fitness == -1)
        limit--;

    // Fill the rest of the box with copies of best x mutations
    int j = 0;
    for (int i = limit; i < POPULATION_BOX_SIZE; i++)
    {
        inputBox[i].client = Client(inputBox[j++].client);
        if (j >= limit)
            j = 0;
    }

    // Convolute clients
    for (int i = limit; i < POPULATION_BOX_SIZE; i++)
        inputBox[i].client.crossover( inputBox[random_range(0, limit)].client );

    // Fill the newely created fitness fields in the box
    for (int i = limit; i < POPULATION_BOX_SIZE; i++)
        inputBox[i].fitness = dataEnvironment->fitness(inputBox[i].client.get());
}

// Evolve a box
void Population::evolve_box(int iterations, int process)
{
    // Allocate the vector and fill it with the given DNA
    std::vector<CachedClient> tempBox(POPULATION_BOX_SIZE);
    std::cout << "* Started boxed straight evolution" << std::endl;

    // Mutate all but one
    tempBox[0].client = Client(dataDNA, dataEnvironment->alphabet());
    for (int i = 1; i < POPULATION_BOX_SIZE; i++)
    {
        tempBox[i].client = Client(dataDNA, dataEnvironment->alphabet());
        tempBox[i].client.mutate();
    }

    // Fill the initial fitness fields in the box
    for (int i = 0; i < POPULATION_BOX_SIZE; i++)
    {
        DNA tempDNA = DNA(tempBox[i].client.get());
        tempBox[i].fitness = dataEnvironment->fitness(tempDNA);
    }

    // Fitness watcher
    double fitness_critical = 0;

    // Loop
    while (iterations > 0)
    {
        // Sort the box by fitness value
        std::sort(tempBox.begin(), tempBox.end());

        // Check if we have any legal mutation
        if (tempBox[0].fitness == -1)
        {
            std::cout << "ERROR: evolution failed, every mutation is invalid" << std::endl;
            return;
        }

        // Call for an update
        if (tempBox[0].fitness > fitness_critical)
        {
            fitness_critical = tempBox[0].fitness;
            dataEnvironment->update(tempBox[0].client.get());
        }

        // Call a specific function to process the box
        switch (process)
        {
            case 1:
                evolve_box_straight_process(tempBox);
                break;
            case 2:
                evolve_box_together_process(tempBox);
                break;
        }

        // Save the best DNA
        dataDNA = tempBox[0].client.get();

        iterations--;
    }
}

