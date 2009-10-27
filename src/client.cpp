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

// Copy constructor
Client::Client(const Client& inputClient) {
    dataDNA = new DNA(*inputClient.get());
    dataAlphabet = inputClient.dataAlphabet;
}

// Create client with given DNA and alphabet
Client::Client(const DNA& inputDNA, int inputAlphabet) {
	dataDNA = new DNA(inputDNA);
	dataAlphabet = inputAlphabet;
}

// Destructor
Client::~Client() {
    if (dataDNA != 0)
        delete dataDNA;
}


//
// DNA IO
//

const DNA* Client::get() const {
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
		// Pick a mutation
		int mutation = random_int(1, 6);

		// Process a mutation
		switch (mutation)
		{
			case 1:
				mutate_point();
				break;
			case 2:
				mutate_delete();
				break;
			case 3:
				mutate_duplicate();
				break;
			case 4:
				mutate_amplify();
				break;
			case 5:
				mutate_inverse();
				break;
		}
	}
}

// Combine the DNA slightly with another client
void Client::recombine(Client& inputClient) {

	// Amount of recombinations
	int tempAmount = random_int(RECOMBINE_AMOUNT_LOWER, RECOMBINE_AMOUNT_UPPER);

	// Process all recombinations
	for (int i = 0; i < tempAmount; i++)
	{
		// Pick a recombinations
		int recombination = random_int(1, 4);

		// Process a recombinations
		switch (recombination)
		{
			case 1:
				recombine_insert(inputClient);
				break;
			case 2:
				recombine_crossover_single(inputClient);
				break;
			case 3:
				recombine_crossover_double(inputClient);
				break;
		}
	}
}

// Clean the DNA
void Client::clean()
{
	// Remove empty genes
}


//
// Mutation methods
//

// Change one or more bytes randomly
void Client::mutate_point() {
    // Pick a random byte to start
    unsigned int start = random_int(0, dataDNA->length());

    // Determine the window size
    unsigned int window_size = random_int(1, 5);
    if (start+window_size > dataDNA->length())
        window_size = dataDNA->length() - start - 1;

    // Generate replacment gen
    unsigned char* replace = (unsigned char*) malloc(window_size * sizeof(unsigned char));
    for (unsigned int i = 0; i < window_size; i++)
        replace[i] = (unsigned char) random_int(0, dataAlphabet);

    // Commit the replacment
    dataDNA->replace(start, replace, window_size);
    free(replace);
}

// Delete one or more bytes
void Client::mutate_delete() {
    // Pick a random byte to start
    unsigned int start = random_int(0, dataDNA->length());

    // Determine the window size
    unsigned int window_size = random_int(1, 5);
    if (start+window_size > dataDNA->length())
        window_size = dataDNA->length() - start - 1;
    int end = start + window_size;

    // Commit the deletion
    dataDNA->erase(start, end);
}

// Copy a selection of bytes one time
void Client::mutate_duplicate() {
    // Pick a random byte to start
    unsigned int start = random_int(0, dataDNA->length());

    // Determine the window size
    unsigned int window_size = random_int(1, 5);
    if (start+window_size >= dataDNA->length())
        window_size = dataDNA->length() - start - 1;
    unsigned int end = start + window_size;

    // Extract the window
    unsigned char* window;
    dataDNA->extract(start, end, window);
    DNA tempDNA(window, window_size);

    // Commit the duplication
    end++;
    if (end == dataDNA->length())
        dataDNA->push_back(window, window_size);
    else
        dataDNA->insert(end, window, window_size);
    free(window);
}

// Amplify a selection of bytes
void Client::mutate_amplify() {
    // Pick a random byte to start
    unsigned int start = random_int(0, dataDNA->length());

    // Determine the window size
    unsigned int window_size = random_int(1, 5);
    if (start+window_size >= dataDNA->length())
        window_size = dataDNA->length() - start - 1;
    unsigned int end = start + window_size;

    // Extract the window
    unsigned char* window;
    dataDNA->extract(start, end, window);

    // Amplify the window
    int amplify = random_int(2, 10);
    window = (unsigned char*) realloc(window, window_size * amplify * sizeof(unsigned char));
    for (int i = 1; i < amplify; i++)
        memcpy(&window[i * window_size], &window[0], window_size);

    // Commit the duplication
    end++;
    if (end == dataDNA->length())
        dataDNA->push_back(window, window_size*amplify);
    else
        dataDNA->insert(end, window, window_size*amplify);
    free(window);
}

// Inverse a selection of bytes
void Client::mutate_inverse() {
    // Pick a random byte to start
    unsigned int start = random_int(0, dataDNA->length());

    // Determine the window size
    unsigned int window_size = random_int(1, 5);
    if (start+window_size >= dataDNA->length())
        window_size = dataDNA->length() - start - 1;
    unsigned int end = start + window_size;

    // Extract the window
    unsigned char* window;
    dataDNA->extract(start, end, window);

    // Inverse the window order
    unsigned char* window_copy = (unsigned char*) malloc(window_size * sizeof(unsigned char));
    for (unsigned int i = 0; i < window_size; i++)
        window_copy[i] = window[window_size - i - 1];
    memcpy(window, window_copy, window_size);
    free(window_copy);

    // Commit the duplication
    dataDNA->replace(start, window, window_size);
    free(window);
}


//
// Recombination methods
//

// Insert foreign bytes in the DNA
void Client::recombine_insert(Client& inputClient) {
    // Extract foreign DNA //

    // Pick a random byte to start
    unsigned int start = random_int(0, inputClient.get()->length());

    // Determine the window size
    unsigned int window_size = random_int(1, 5);
    if (start+window_size >= inputClient.get()->length())
        window_size = inputClient.get()->length() - start - 1;
    unsigned int end = start + window_size;

    // Extract the window
    unsigned char* window;
    inputClient.get()->extract(start, end, window);


    // Insert the DNA //

    // Pick a random byte to start
    int i_start = random_int(0, dataDNA->length());

    // Insert the DNA
    dataDNA->insert(i_start, window, window_size);


    // Clean //

    free(window);
}

// Perform a single crossover
void Client::recombine_crossover_single(Client& inputClient) {
    // Determine crossover point
    int max = std::min(dataDNA->length(), inputClient.get()->length());
    int crossover_point = random_int(0, max);

    // Wipe own DNA
    dataDNA->erase(crossover_point, dataDNA->length());

    // Extract foreign DNA
    int window_size = inputClient.get()->length() - crossover_point;
    unsigned char* window;
    inputClient.get()->extract(crossover_point, crossover_point+window_size, window);

    // Append foreign DNA
    dataDNA->push_back(window, window_size);

    // Clean
    free(window);
}

// Perform a double crossover
void Client::recombine_crossover_double(Client& inputClient) {
    // Determine starting crossover region
    unsigned int max = std::min(dataDNA->length(), inputClient.get()->length());
    unsigned int crossover_start = random_int(0, max);
    unsigned int crossover_end = random_int(crossover_start, max);
    if (crossover_end <= crossover_start)
        return;

    // Wipe own DNA
    dataDNA->erase(crossover_start, crossover_end);

    // Extract foreign DNA
    unsigned int window_size = crossover_end - crossover_start;
    unsigned char* window;
    inputClient.get()->extract(crossover_start, crossover_end, window);

    // Append foreign DNA
    crossover_end++;
    if (crossover_end == dataDNA->length())
        dataDNA->push_back(window, window_size);
    else
        dataDNA->insert(crossover_start, window, window_size);

    // Clean
    free(window);
}
