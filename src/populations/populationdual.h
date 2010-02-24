/*
 * populationdual.h
 * Evolve - Population model for group of clients, evolving together
 *
 * Copyright (c) 2010 Tim Besard <tim.besard@gmail.com>
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
#ifndef __POPULATIONDUAL
#define __POPULATIONDUAL

// Headers
#include "../population.h"



//////////////////////
// CLASS DEFINITION //
//////////////////////

class PopPopulationDual: public Population {
public:
    // Constructor
    PopPopulationDual(Environment* inputEnvironment, const DNA& inputDNA)
    : Population(inputEnvironment, inputDNA) { }

    // Required functions
    void evolve();

};

void PopPopulationDual::evolve() {
    // Allocate first population
    std::vector<CachedClient> population1(POPULATION_BOX_SIZE);
    init(population1, dataDNA, 1);
    fill(population1, 1);
    mutate(population1, 1);

    // Allocate second population
    std::vector<CachedClient> population2(POPULATION_BOX_SIZE);
    init(population2, dataDNA, 1);
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
            dataDNA = new DNA(*(*population)[0].client->get());
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

    // Clean
    for (int i = 0; i < POPULATION_BOX_SIZE; i++) {
        if (population1[i].client != 0)
            delete population1[i].client;
        if (population2[i].client != 0)
            delete population2[i].client;
    }
}


// Include guard
#endif
