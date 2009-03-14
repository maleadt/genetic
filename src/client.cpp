/*
 * client.cpp
 * Evolve - Evolutionary client
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
#include "client.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Default constructor
Client::Client()
{
}

// Create client with given DNA
Client::Client(const DNA& inputDNA)
{
	dataDNA = DNA(inputDNA);
}

// Create client with given DNA and alphabet
Client::Client(const DNA& inputDNA, int inputAlphabet)
{
	dataDNA = DNA(inputDNA);
	dataAlphabet = inputAlphabet;
}


//
// DNA alteration
//

// Mutate the DNA
void Client::mutate()
{
	// Amount of mutations
	int tempAmount = random_range(MUTATE_AMOUNT_LOWER, MUTATE_AMOUNT_UPPER);

	// Process all mutations
	for (int i = 0; i < tempAmount; i++)
	{
		// Pick level of mutation
		int level = random_range(1, 3);

		// Call correct mutation routine
		switch (level)
		{
			case 1:
				mutate_dna();
				break;
			case 2:
				mutate_gen();
				break;
			case 3:
				mutate_codon();
				break;
		}
	}
}

// Combine the DNA with another client
void Client::crossover(Client& inputClient)
{
    // Acquire DNA queue representation
    std::deque<int> inputQueue1 = inputClient.get().dequeue();
    std::deque<int> inputQueue2 = dataDNA.dequeue();

    // Create a new DNA queue
    std::deque<int> outputQueue;
    outputQueue.push_back(255);

    // Remove semantics from both queue's, or every DNA string would be invalid
    inputQueue1.pop_front(); inputQueue1.pop_back();
    inputQueue2.pop_front(); inputQueue2.pop_back();


    // Pick a crossover-method
    int method = random_range(1, 3);

    // Crossover!
    switch (method)
    {
        // Sequential merge
        case 1:
        {
            while (!inputQueue1.empty())
            {
                outputQueue.push_back(inputQueue1.front());
                inputQueue1.pop_front();
            }
            while (!inputQueue2.empty())
            {
                outputQueue.push_back(inputQueue2.front());
                inputQueue2.pop_front();
            }
            break;
        }

        // Altering merge
        case 2:
        {
            while (!(inputQueue1.empty() && inputQueue2.empty()))
            {
                if (!inputQueue1.empty())
                {
                    outputQueue.push_back(inputQueue1.front());
                    inputQueue1.pop_front();
                }
                if (!inputQueue2.empty())
                {
                    outputQueue.push_back(inputQueue2.front());
                    inputQueue2.pop_front();
                }
            }
            break;
        }

        // Random merge
        case 3:
        {
            while (!(inputQueue1.empty() && inputQueue2.empty()))
            {
                int choice = random_range(0, 1);
                if (choice == 0 && !inputQueue1.empty())
                {
                    outputQueue.push_back(inputQueue1.front());
                }
                else if (!inputQueue2.empty())
                {
                    outputQueue.push_back(inputQueue2.front());
                    inputQueue2.pop_front();
                }
                if (!inputQueue1.empty())
                    inputQueue1.pop_front();
                if (!inputQueue2.empty())
                    inputQueue2.pop_front();
            }
            break;
        }
    }

    // Finish the queue
    outputQueue.push_back(255);

    // Save the DNA
    dataDNA = DNA(outputQueue);

    // Mutate?
    if (random_range(1, 5) == 3)
        mutate();
}

// Clean the DNA
void Client::clean()
{
	// Pick random gene
	int random = random_range(0, dataDNA.genes()-1);
	DNA::iterator it = dataDNA.begin();
	while (it != dataDNA.end())
	{
		if (it->empty())
		{
			it = dataDNA.erase(it);
		}
		++it;
	}
}


//
// DNA IO
//

void Client::set(const DNA& inputDNA)
{
	dataDNA = DNA(inputDNA);
}

DNA Client::get() const
{
	return dataDNA;
}


//
// Mutation routines
//

void Client::mutate_dna()
{
	// Prevent mutation of empty lists
	if (dataDNA.genes() < 1)
		return;

	 // Pick random gene(s)
	 int size = dataDNA.genes();
	 int random1 = random_range(0, size-1);
	 int random2 = random_range(0, size-1);
	 while (size > 1 && random1 == random2)
	 	random2 = random_range(0, size-1);

	// Calculate iterators to those genes
	DNA::iterator it1 = dataDNA.begin(), it2 = dataDNA.begin();
	for (int i = 0; i < random1; i++)
		it1++;
	for (int i = 0; i < random2; i++)
		it2++;

	// Pick mutation
	int mutation = random_range(1, 6);

	switch (mutation)
	{
		// Creation
		case 1:
		{
			// Generate random gene
			int size = random_range((int)it1->size()/2, (int)it1->size()*4);
			if (size == 0)
				size = random_range(5, 10);
			std::list<int> tempList;
			for (int i = 0; i < size; i++)
				tempList.push_back(random_range(0, dataAlphabet));
			dataDNA.insert(it1, tempList);
			break;
		}

		default:
			mutate_list(dataDNA);
			break;
	}
}

void Client::mutate_gen()
{
	// Prevent mutation of empty lists
    int size = dataDNA.genes();
	if (size < 1)
		return;

	// Pick random gene
	int random = random_range(0, size-1);
	DNA::iterator it = dataDNA.begin();
	for (int i = 0; i < random; i++)
		it++;

	// Prevent mutation of empty gene
	if (it->size() < 1)
		return;

	// Pick mutation
	int mutation = random_range(1, 6);

	// Pick a mutation
	switch (mutation)
	{
		// Creation
		case 1:
		{
			// Generate and insert
			it->push_back(random_range(1, dataAlphabet));
			break;
		}

		default:
			mutate_list(*it);
			break;
	}
}

void Client::mutate_codon()
{
	// Prevent mutation of empty lists
    int size = dataDNA.genes();
	if (size < 1)
        return;

	// Pick random gene
	int random = random_range(0, size-1);
	DNA::iterator it = dataDNA.begin();
	for (int i = 0; i < random; i++)
		it++;

	// Avoid altering empty gene
	if (it->empty())
		return;

	// Pick random codon
	random = random_range(0, it->size()-1);
	std::list<int>::iterator it2 = it->begin();
	for (int i = 0; i < random; i++)
		it2++;

	// Point mutate
	*it2 = random_range(1, dataAlphabet);
}

