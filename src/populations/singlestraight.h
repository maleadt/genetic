/*
 * singlestraight.h
 * Evolve - Population model for single client, evolving straightly
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
#ifndef __SINGLESTRAIGHT
#define __SINGLESTRAIGHT

// Headers
#include "../population.h"



//////////////////////
// CLASS DEFINITION //
//////////////////////

class PopSingleStraight: public Population {
public:
    // Construction and destruction
    PopSingleStraight(Environment* inputEnvironment, const DNA& inputDNA)
    : Population(inputEnvironment, inputDNA) { }
    ~PopSingleStraight() { }

    // Required functions
    void evolve();

};

void PopSingleStraight::evolve() {
    // Calculate current fitness
    double dataFitness = dataEnvironment->fitness(dataDNA);

    // Loop
    while (dataEnvironment->condition())
    {
        // Create a client, and mutate the DNA
        Client tempClient(dataDNA, dataEnvironment->alphabet());
        tempClient.mutate();

        // Compare the new DNA
        const DNA& tempDNA = tempClient.get();
        double tempFitness = dataEnvironment->fitness(tempDNA);
        if (tempFitness > dataFitness)
        {
            dataFitness = tempFitness;
            dataDNA = DNA(tempClient.get());
            dataEnvironment->update(dataDNA);
        }
    }

}


// Include guard
#endif
