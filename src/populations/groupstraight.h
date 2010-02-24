/*
 * groupstraight.h
 * Evolve - Population model for group of clients, evolving straightly
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
#ifndef __GROUPSTRAIGHT
#define __GROUPSTRAIGHT

// Headers
#include "../population.h"



//////////////////////
// CLASS DEFINITION //
//////////////////////

class PopGroupStraight: public Population {
public:
    // Constructor
    PopGroupStraight(Environment* inputEnvironment, const DNA& inputDNA)
    : Population(inputEnvironment, inputDNA) { }

    // Required functions
    void evolve();

};

void PopGroupStraight::evolve() {
    // Allocate new population
    std::vector<CachedClient> population(POPULATION_BOX_SIZE);
    init(population, dataDNA, 1);
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
            dataDNA = new DNA(*population[0].client->get());
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

    // Clean
    for (int i = 0; i < POPULATION_BOX_SIZE; i++) {
        if (population[i].client != 0)
            delete population[i].client;
    }
}


// Include guard
#endif
