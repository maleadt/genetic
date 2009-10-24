/*
 * gen.cpp
 * Gene data structure.
 *
 * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
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

/*
 * Configuration
 */

// Include header
#include "simulation.h"

// Constants
const bool SIM_VERBOSE = true;

/*
 Construction
*/
//TODO: remove parameter dependant constructors, setXXX routines are cleaner

Simulation::Simulation(string inputDNAString)
{
    setDNA(inputDNAString);

    // Set initial values
    dataRandPosStart = false;
    dataRandPosEnd = false;
}

Simulation::Simulation()
{
    haveDNAString = false;

    // Set initial values
    dataRandPosStart = false;
    dataRandPosEnd = false;
}

/*
 Basic IO
*/

void Simulation::setDNA(string inputDNAString)
{
    haveDNAString = true;
    dataDNAString = inputDNAString;
    if (SIM_VERBOSE) cout << "* simulation\tconfigured with DNA " << inputDNAString << endl;
}

void Simulation::setWorld(int inputRows, int inputColumns, int inputBlocks)
{
    dataWorldRows = inputRows;
    dataWorldColumns = inputColumns;
    dataWorldBlocks = inputBlocks;
    if (SIM_VERBOSE) cout << "* simulation\tconfigured with world of " << inputRows << " x " << inputColumns << " (" << inputBlocks << " % blocks)" << endl;
}

void Simulation::setRandomPositionStart(bool inputBool)
{
    dataRandPosStart = inputBool;
}

void Simulation::setRandomPositionEnd(bool inputBool)
{
    dataRandPosEnd = inputBool;
}

void Simulation::setSimulations(int inputSimulations)
{
    dataSimulations = inputSimulations;
}

void Simulation::setSimulationsPerDNA(int inputSimulations)
{
    dataSimulationsPerDNA = inputSimulations;
}

void Simulation::setStepsPerCreature(int inputSteps)
{
    dataStepsPerCreature = inputSteps;
}

/*
 Simulation
*/

string Simulation::simulateStraight()
{
    //
    // Prepare
    //

    // Create initial DNA structure
    DNA tempDNA(dataDNAString);

    // Create a creature with the initial DNA
    Creature tempCreature(tempDNA, dataStepsPerCreature);

    // Statistical variables for the best DNA code (needed to compare later on)
    DNA statBestDNA;
    double statBestRate = -1;

    //
    // Simulate
    //

    // Loop a given times
    for (int simulation = 1; simulation <= dataSimulations; simulation++)
    {
        // Output
        if (SIM_VERBOSE) cout << "* simulation\tcurrent DNA: " << tempDNA.get() << endl;

        // Statistical variables for the current DNA code
        int statCurrentFinishes = 0;
        double statCurrentRate;

        // Loop a given times
        for (int subsimulation = 1; subsimulation <= dataSimulationsPerDNA; subsimulation++)
        {
            // Create a new world
            World tempWorld;
            tempWorld.setDimension(dataWorldRows, dataWorldColumns);
            tempWorld.setBlocks(dataWorldBlocks);

            // Generate a new world with appropriate end spot
            int tempEndRow, tempEndColumn;
            if (dataRandPosEnd == true)
            {
                tempEndRow = random_int(0, dataWorldRows-1);
                tempEndColumn = random_int(0, dataWorldColumns-1);
            }
            else
            {
                tempEndRow = dataWorldRows-1;
                tempEndColumn = dataWorldColumns-1;
            }
            tempWorld.setDestination(tempEndRow, tempEndColumn);

            // Add a creature at the requested spot
            int tempStartRow, tempStartColumn;
            if (dataRandPosStart == true)
            {
                while (tempStartRow == tempEndRow)
                {
                    tempStartRow = random_int(0, dataWorldRows-1);
                }
                while (tempStartColumn == tempEndColumn)
                {
                    tempStartColumn = random_int(0, dataWorldColumns-1);
                }
            }
            else
            {
                tempStartRow = 0;
                tempStartColumn = 0;
            }
            tempWorld.addCreature(tempCreature, tempStartRow, tempStartColumn);

            // Do the actual simulation
            tempWorld.runAll();

            // Did we finish?
            if (tempWorld.isFinished(tempCreature.ID))
            {
                statCurrentFinishes++;
            }
        }

        // Was this an efficient mutation
        statCurrentRate = 1.0*statCurrentFinishes / dataSimulationsPerDNA;  // TODO: can't this be done nicer, like forcing a non-integer division?
        if (SIM_VERBOSE) cout << "* simulation\tcurrent DNA rate: " << (int)(statCurrentRate*100) << endl;
        if (statCurrentRate > statBestRate)
        {
            statBestDNA = tempDNA;
            statBestRate = statCurrentRate;
            if (SIM_VERBOSE) cout << "* simulation\tthis was an efficient one, saving it!" << endl;
        }
        else
        {
            tempDNA = statBestDNA;
        }

        // Do a new mutation
        tempDNA.mutate();
    }

    return statBestDNA.get();
}
