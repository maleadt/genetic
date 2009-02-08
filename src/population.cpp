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
Population::Population(Environment* inputEnvironment, Parser inputDNA)
{
    // Save all data
    dataDNA = inputDNA;
    dataEnvironment = inputEnvironment;
}


//
// Output routines
//

std::queue<int> Population::getDNAQueue()
{
    return dataDNA.getQueue();
}

std::list<std::list<int> > Population::getDNAList()
{
    return dataDNA.getList();
}


//
// Evolutionary methods
//

// Evolve a single client straight
void Population::evolve_single_straight(int successes)
{
    // Calculate current fitness
    double dataFitness = dataEnvironment->fitness(dataDNA);
    std::cout << "* Started evolution, initial fitness is " << dataFitness << std::endl;

    // Loop
    while (successes > 0)
    {
        // Create a client, and mutate the DNA
        Client tempClient(dataDNA.getList());
        tempClient.dataAlphabet = dataEnvironment->alphabet();
        tempClient.mutate();

        // Compare the new DNA
        double tempFitness = dataEnvironment->fitness(tempClient.getDNA());
        if (tempFitness > dataFitness)
        {
            std::cout << "* Fitness increased to " << tempFitness << std::endl;
            dataFitness = tempFitness;
            dataDNA = tempClient.getDNA();
            successes--;
            dataEnvironment->update(dataDNA.getList());
        }
    }
}
