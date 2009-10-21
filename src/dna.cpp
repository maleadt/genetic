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
}

// Copy constructor
DNA::DNA(const DNA& inputDNA) {
    DNA(inputDNA.dataGenes, inputDNA.dataSize);
}

// Constructor with parameters
DNA::DNA(const unsigned char* inputGenes, int inputSize) {
    dataSize = inputSize;

    // Deep copy of genes
    dataGenes = new unsigned char[inputSize];
    std::memcpy(dataGenes, inputGenes, inputSize);
}


//
// Informational routines
//

// Amount of genes
unsigned int DNA::genes() const {
    int genes = 0;
    bool data = false;

    for (unsigned int i = 0; i < dataSize; i++) {
        if (dataGenes[i] == 0) {
            data = false;
        } else if (!data) {
            data = true;
            genes++;
        }
    }
    return genes;
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
    unsigned int amountgenes = genes();
    if (index < 0 || index >= amountgenes) {
        return false;
    }

    // Determine part which should be deleted
    unsigned int startdel = gene_location(index);
    unsigned char* startdelloc = ptr_move(startdel);;
    unsigned enddel = dataSize;
    if (index < amountgenes) {
        enddel = gene_location(index+1);
    }
    unsigned char* enddelloc = ptr_move(enddel);

    // Alter DNA
    memcpy(startdelloc, enddelloc, dataSize-enddel);
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize-(enddel-startdel));
    return true;
}

// Insert a gene
bool DNA::insert(unsigned int index, unsigned char* gene, unsigned int size) {
    unsigned int amountgenes = genes();
    if (index < 0 || index >= amountgenes-1) {
        return false;
    }

    // Enlarge data
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize+size);

    // Preserve old data
    int startinsert = gene_location(index);
    unsigned char* startinsertloc = ptr_move(startinsert);
    int endinsert = gene_location(index+1);
    unsigned char* endinsertloc = ptr_move(endinsert);
    memmove(startinsertloc, endinsertloc, dataSize-startinsert);

    // Insert new data
    memcpy(startinsertloc, gene, size);

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
        int genes = 0;
        bool data = false;
        for (unsigned int i = 0; i < dataSize; i++) {
            if (dataGenes[i] == 0) {
                if (data)
                    std::cout << std::endl;
                data = false;
            } else {
                if (!data) {
                    data = true;
                    std::cout << "\tgene " << ++genes << ": ";
                }
                std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << (int)dataGenes[i] << " ";
            }
        }
        if (data)
           std::cout << std::endl;
    }


//
// Auxiliary
//

// Move the pointer relatively
unsigned char* DNA::ptr_move(unsigned int inputLocation) {
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

    return dataPointer;
}

// Find the location of a gene
unsigned int DNA::gene_location(unsigned int index) {
    unsigned int genes = 0;
    bool data = false;

    for (unsigned int i = 0; i < dataSize; i++) {
        if (dataGenes[i] == 0) {
            data = false;
        } else if (!data) {
            data = true;
            if (genes++ == index) {
                return i;
            }
        }
    }
    return 0;
}