/*
 * dna.cpp
 * Evolve - DNA datastructure.
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
#include "dna.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Default constructor
DNA::DNA() {
    dataSize = 0;
}

// Copy constructor
DNA::DNA(const DNA& inputDNA) {
    DNA(inputDNA.dataGenes, inputDNA.dataSize);
    ptr_set(0);
}

// Constructor with parameters
DNA::DNA(const unsigned char* inputGenes, int inputSize) {
    dataSize = inputSize;

    // Deep copy of genes
    dataGenes = (unsigned char*) malloc(inputSize * sizeof(unsigned char));
    std::memcpy(dataGenes, inputGenes, inputSize);
    ptr_set(0);
}

// Destructor
DNA::~DNA() {
    if (dataSize > 0)
        free(dataGenes);
}


//
// Informational routines
//

// Amount of genes
unsigned int DNA::genes() const {
    int genes = 0;
    if (dataSize == 0)
        return 0;

    for (unsigned int i = 0; i < dataSize; i++) {
        if (dataGenes[i] == 0) {
            genes++;
        }
    }
    return genes+1;
}
unsigned int DNA::length() const
{
    return dataSize;
}


//
// Modifiers
//

// Erase a gene
bool DNA::erase(unsigned int index) {
    // Check boundaries
    unsigned int amountgenes = genes();
    if (index < 0 || index >= amountgenes)
        return false;

    // Case 1: gene at start
    if (index == 0) {
        if (amountgenes > 1) {
            unsigned int i_next = gene_start(1);
            unsigned char* p_next = ptr_set(i_next);
            memmove(dataGenes, p_next, dataSize-i_next);
            dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize-i_next * sizeof(unsigned char));
            dataSize -= i_next;
        } else {
            free(dataGenes);
            dataGenes = 0;
            dataSize = 0;
        }
    }

    // Case 2: gene at midst
    else if (index < amountgenes-1) {
        unsigned int i_self = gene_start(index);
        unsigned char* p_self = ptr_set(i_self);
        unsigned int i_next = gene_start(index+1);
        unsigned char* p_next = ptr_set(i_next);

        memmove(p_self, p_next, dataSize-i_next);
        dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize-(i_next-i_self) * sizeof(unsigned char));
        dataSize -= i_next-i_self;
    }

    // Case 3: gene at end
    else {
        if (amountgenes > 1) {
            unsigned int i_prev = gene_end(index-1);
            dataGenes = (unsigned char*) std::realloc(dataGenes, i_prev * sizeof(unsigned char));
            dataSize = i_prev;
        } else {
            delete[] dataGenes;
            dataGenes = 0;
            dataSize = 0;
        }
    }
    return true;
}

// Insert a gene
bool DNA::insert(unsigned int index, unsigned char* gene, unsigned int size) {
    // Check boundaries
    unsigned int amountgenes = genes();
    if (index < 0 || index > amountgenes) {
        return false;
    }

    // Enlarge array
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize+size);

    // Move data
    int i_start = gene_start(index);
    unsigned char* p_start = ptr_set(i_start);
    unsigned char* p_backup = ptr_set(i_start + size);
    memmove(p_backup, p_start, dataSize-i_start);

    // Copy new data
    memcpy(p_start, gene, size);
    dataSize += size;
    
    return true;
}

// Add an element
void DNA::push_back(unsigned char* gene, unsigned int size) {
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize+size);
    for (unsigned int i = 0; i < size; i++) {
        dataGenes[dataSize+i] = gene[i];
    }
    dataSize += size;
}


//
// Operators
//

// Comparison
bool DNA::operator== (const DNA& dna) {
    if (dataSize != dna.dataSize)
        return false;

    for (unsigned int i = 0; i < dataSize; i++) {
        if (dataGenes[i] != dna.dataGenes[i])
            return false;
    }

    return true;
}
bool DNA::operator!= (const DNA& dna) {
    return !(*this == dna);
}



//
// Debugging routines
//

void DNA::debug() const
{
	// Debug message
	std::cout << "* DNA.debug" << std::endl;

	// Process chararray
	std::cout << "Contents of DNA object (" << genes() << " genes): " << std::endl;
        for (unsigned int i = 0; i < genes(); i++) {
            std::cout << "\tgene " << i+1 << ":";
            unsigned int start = gene_start(i);
            unsigned int end = gene_end(i);
            while (start < end) {
                std::cout << " 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)dataGenes[start++] << std::dec;
            }
            std::cout << std::endl;
        }
}

void DNA::debug_raw() const {
	// Debug message
	std::cout << "* DNA.debug_raw" << std::endl;

        std::cout << "\t";
        unsigned char* start = dataGenes;
        for (unsigned int i = 0; i < dataSize; i++) {
            std::cout << " 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)*start << std::dec;
            if (*start == 0) {
                std::cout << std::endl << "\t";
            }
            start++;
        }
        std::cout << std::endl;
}


//
// Auxiliary
//

// Move the pointer relatively
unsigned char* DNA::ptr_move(unsigned int inputLocation) {
    // Prefer cheap moves
    if (inputLocation < (unsigned) std::abs((signed)dataPointerLocation - (signed)inputLocation))
        return ptr_set(0);

    // Move forward
    for (; dataPointerLocation < inputLocation; dataPointerLocation++) {
        dataPointer++;
    }

    // Move backwards
    for (; dataPointerLocation > inputLocation; dataPointerLocation--) {
        dataPointer--;
    }

    return dataPointer;
}

// Set the pointer absolutely
unsigned char* DNA::ptr_set(unsigned int inputLocation) {
    dataPointer = dataGenes;
    for (unsigned int i = 0; i < inputLocation; i++) {
        dataPointer++;
    }

    dataPointerLocation = inputLocation;
    return dataPointer;
}

// Find the location of a separator
unsigned int DNA::separator(unsigned int index) const {
    unsigned int sep = 0;
    for (unsigned int i = 0; i < dataSize; i++) {
        if (dataGenes[i] == 0) {
            if (++sep == index) {
                return i;
            }
        }
    }
    return 0;
}

// Returns the index of the first data part of a gene (inclusive)
unsigned int DNA::gene_start(unsigned int index) const {
    if (index > 0)
        return separator(index) + 1;
    else
        return 0;
}

// Returns the index of the last data part of a gene (exclusive)
// NOTE: this value is not guaranteed to be accessible!
unsigned int DNA::gene_end(unsigned int index) const {
    if (index < genes()-1)
        return separator(index+1);
    else
        return dataSize;
}