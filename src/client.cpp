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
// DNA IO
//

void Client::set(const DNA& inputDNA)
{
	dataDNA = DNA(inputDNA);
}

DNA& Client::get() const
{
	return dataDNA;
}


//
// DNA alteration
//

// Mutate the DNA
void Client::mutate()
{
	// Amount of mutations
	int tempAmount = random_int(MUTATE_AMOUNT_LOWER, MUTATE_AMOUNT_UPPER);

	// Process all mutations
	for (int i = 0; i < tempAmount; i++)
	{
		// Pick level of mutation
		int level = random_int(1, 4);

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

// Combine the DNA slightly with another client
void Client::recombine(Client& inputClient)
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
    int method = random_int(1, 4);

    // Recombine!
    switch (method)
    {
        // Crossover
        case 1:
        {
            unsigned int size1 = inputQueue1.size();
            unsigned int size2 = inputQueue2.size();
            if (size1 <= 3 && size2 <= 3)
            {
                // Calculate part to exchange out of DNA 1
                unsigned int p1 = random_int(0, size1);
                unsigned int p2 = random_int(1, size1+1);
                if (p1 > p2)
                {
                    int temp = p1;
                    p1 = p2;
                    p2 = temp;
                }
                while (p2 == p1)
                    p2 = random_int(p1+1, size1+1);

                // Calculate part to exchange out of DNA 2
                unsigned int p3 = random_int(0, size2);
                unsigned int p4 = random_int(1, size2+1);
                if (p3 > p4)
                {
                    int temp = p3;
                    p3 = p4;
                    p4 = temp;
                }
                while (p4 == p3)
                    p4 = random_int(p3+1, size2+1);

                // First part
                for (unsigned int i = 0; i < p1; i++)
                {
                    outputQueue.push_back(inputQueue1.front());
                    inputQueue1.pop_front();
                }
                for (unsigned int i = 0; i < p3; i++)
                    inputQueue2.pop_front();

                // Middle part
                for (unsigned int i = p3; i < p4; i++)
                {
                    outputQueue.push_back(inputQueue2.front());
                    inputQueue2.pop_front();
                }
                for (unsigned int i = p1; i < p2; i++)
                    inputQueue1.pop_front();

                // End part
                for (unsigned int i = p2; i < size1; i++)
                {
                    outputQueue.push_back(inputQueue1.front());
                    inputQueue1.pop_front();
                }
            }
            else
            {
                // Do nothing
                outputQueue = inputQueue1;
                outputQueue.push_front(255);
            }
            break;
        }

        // Insertion
        case 2:
        {
            unsigned int size1 = inputQueue1.size();
            unsigned int size2 = inputQueue2.size();
            if (size1 <= 2 && size2 <= 3)
            {
                // Calculate insertion spot of DNA 1
                unsigned int p1 = random_int(1, size1);

                // Calculate part to get out of DNA 2
                unsigned int p3 = random_int(0, size2);
                unsigned int p4 = random_int(1, size2+1);
                if (p3 > p4)
                {
                    int temp = p3;
                    p3 = p4;
                    p4 = temp;
                }
                while (p4 == p3)
                    p4 = random_int(p3+1, size2+1);

                // First part
                for (unsigned int i = 0; i < p1; i++)
                {
                    outputQueue.push_back(inputQueue1.front());
                    inputQueue1.pop_front();
                }
                for (unsigned int i = 0; i < p3; i++)
                    inputQueue2.pop_front();

                // Middle part
                for (unsigned int i = p3; i < p4; i++)
                {
                    outputQueue.push_back(inputQueue2.front());
                    inputQueue2.pop_front();
                }

                // End part
                for (unsigned int i = p1; i < size1; i++)
                {
                    outputQueue.push_back(inputQueue1.front());
                    inputQueue1.pop_front();
                }
            }
            else
            {
                // Do nothing
                outputQueue = inputQueue1;
                outputQueue.push_front(255);
            }
            break;
        }

        // Gene conversion
        case 3:
        {
            unsigned int size1 = inputQueue1.size();
            unsigned int size2 = inputQueue2.size();
            if (size1 <= 3 && size2 <= 3)
            {
                // Calculate split points
                unsigned int p1 = random_int(1, size1);
                unsigned int p2 = random_int(1, size2);

                // First part
                for (unsigned int i = 0; i < p1; i++)
                {
                    outputQueue.push_back(inputQueue1.front());
                    inputQueue1.pop_front();
                }

                // Second part
                for (unsigned int i = 0; i < p2; i++)
                    inputQueue2.pop_front();
                for (unsigned int i = p2; i < size2; i++)
                {
                    outputQueue.push_back(inputQueue2.front());
                    inputQueue2.pop_front();
                }
            }
            else
            {
                // Do nothing
                outputQueue = inputQueue1;
                outputQueue.push_front(255);
            }
            break;
        }
    }

    // Finish the queue
    outputQueue.push_back(255);

    // Save the DNA
    dataDNA = DNA(outputQueue);

    // Mutate
    mutate();
}

// Clean the DNA
void Client::clean()
{
	// Pick random gene
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
// Mutation routines
//

void Client::mutate_dna()
{
	// Prevent mutation of empty lists
	if (dataDNA.genes() < 1)
		return;

	 // Pick random gene(s)
	 int size = dataDNA.genes();
	 int random1 = random_int(0, size);
	 int random2 = random_int(0, size);
	 while (size > 1 && random1 == random2)
	 	random2 = random_int(0, size);

	// Calculate iterators to those genes
	DNA::iterator it1 = dataDNA.begin(), it2 = dataDNA.begin();
	for (int i = 0; i < random1; i++)
		it1++;
	for (int i = 0; i < random2; i++)
		it2++;

	// Pick mutation
	int mutation = random_int(1, 3);

	switch (mutation)
	{
		// Creation
		case 1:
		{
			// Generate random gene
			int size = random_int((int)it1->size()/2, (int)it1->size()*4+1);
			if (size == 0)
				size = random_int(5, 11);
			std::list<int> tempList;
			for (int i = 0; i < size; i++)
				tempList.push_back(random_int(0, dataAlphabet+1));
			dataDNA.insert(it1, tempList);
			break;
		}

		case 2:
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
	int random = random_int(0, size);
	DNA::iterator it = dataDNA.begin();
	for (int i = 0; i < random; i++)
		it++;

	// Prevent mutation of empty gene
	if (it->size() < 1)
		return;

	// Pick mutation
	int mutation = random_int(1, 3);

	// Pick a mutation
	switch (mutation)
	{
		// Creation
		case 1:
		{
			// Generate and insert
			it->push_back(random_int(1, dataAlphabet+1));
			break;
		}

		case 2:
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
	int random = random_int(0, size);
	DNA::iterator it = dataDNA.begin();
	for (int i = 0; i < random; i++)
		it++;

	// Avoid altering empty gene
	if (it->empty())
		return;

	// Pick random codon
	random = random_int(0, it->size());
	std::list<int>::iterator it2 = it->begin();
	for (int i = 0; i < random; i++)
		it2++;

	// Point mutate
	*it2 = random_int(1, dataAlphabet+1);
}

