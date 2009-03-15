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
    std::cout << "* Started single straight evolution, initial fitness is " << dataFitness << std::endl;

    // Loop
    while (dataEnvironment->condition())
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
    }
}

// Evolve a box of clients straightly
void Population::evolve_box_straight()
{
    evolve_box(1);
}
void Population::evolve_box_straight_process(
    std::vector<CachedClient>::iterator good_start, std::vector<CachedClient>::iterator good_end,
    std::vector<CachedClient>::iterator process_start, std::vector<CachedClient>::iterator process_end)
{
    // Fill the rest of the box with copies of best x mutations
    std::vector<CachedClient>::iterator good = good_start, process = process_start;
    while (process != process_end)
    {
        *(process++) = *(good++);
        if (good == good_end)
            good = good_start;
    }

    // Mutate clients
    process = process_start;
    while (process != process_end)
        (process++)->client.mutate();
}

// Evolve a box of clients together
void Population::evolve_box_together()
{
    evolve_box(2);
}
void Population::evolve_box_together_process(
    std::vector<CachedClient>::iterator good_start, std::vector<CachedClient>::iterator good_end,
    std::vector<CachedClient>::iterator process_start, std::vector<CachedClient>::iterator process_end)
{
    // Fill the rest of the box with copies of best x mutations
    std::vector<CachedClient>::iterator good = good_start, process = process_start;
    while (process != process_end)
    {
        *(process++) = *(good++);
        if (good == good_end)
            good = good_start;
    }

    // Shuffle that part
    std::random_shuffle(process_start, process_end);

    // Convolute clients
    process = process_start;
    good = good_start;
    while (process != process_end)
    {
        (process++)->client.crossover((good++)->client);
        if (good == good_end)
            good = good_start;
    }
}

// Evolve a box in a mixed manner
void Population::evolve_box_mix()
{
    evolve_box(3);
}
void Population::evolve_box_mix_process(
    std::vector<CachedClient>::iterator good_start, std::vector<CachedClient>::iterator good_end,
    std::vector<CachedClient>::iterator process_start, std::vector<CachedClient>::iterator process_end)
{
    // Get the distance between the two iterators
    int size = 0;
    std::vector<CachedClient>::iterator process = process_start;
    while ((process++) != process_end)
        size++;
    std::vector<CachedClient>::iterator process_mid = process_start;
    for (int i = 0; i < size/2; i++)
        process_mid++;

    // Call both process routines independantly
    evolve_box_straight_process(good_start, good_end, process_start, process_mid++);
    evolve_box_together_process(good_start, good_end, process_mid, process_end);
}

// Evolve a box
void Population::evolve_box(int process)
{
    // Allocate the vector and fill it with the given DNA
    std::vector<CachedClient> tempBox(POPULATION_BOX_SIZE);
    std::cout << "* Started boxed evolution" << std::endl;

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
    while (dataEnvironment->condition())
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

        // Look up valid threshold
        int limit = POPULATION_BOX_THRESHOLD;
        while (tempBox[limit].fitness == -1)
            limit--;

        // Get some iterators
        std::vector<CachedClient>::iterator good_start = tempBox.begin(), good_end = good_start;
        std::advance(good_end , limit);
        std::vector<CachedClient>::iterator process_start = tempBox.begin(), process_end = tempBox.end();
        std::advance(process_start, limit+1);

        // Call a specific function to process the box
        switch (process)
        {
            case 1:
                evolve_box_straight_process(good_start, good_end, process_start, process_end);
                break;
            case 2:
                evolve_box_together_process(good_start, good_end, process_start, process_end);
                break;
            case 3:
                evolve_box_mix_process(good_start, good_end, process_start, process_end);
                break;
        }

        // Calculate fitness of newely modified clients
        for (int i = limit; i < POPULATION_BOX_SIZE; i++)
        {
            tempBox[i].fitness = dataEnvironment->fitness( tempBox[i].client.get() );
        }

        // Save the best DNA
        dataDNA = tempBox[0].client.get();
    }
}

