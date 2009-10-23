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
// Raw modifiers
//

// Erase data
//   erases data from i_start (inclusive) to i_end (exclusive)
void DNA::erase(unsigned int i_start, unsigned int i_end) {
    // Get pointers
    unsigned char* p_start = ptr_set(i_start);
    unsigned char* p_end = ptr_set(i_end);

    // Move genes
    memmove(p_start, p_end, dataSize-i_end);

    // Shrink data container
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize-(i_end-i_start) * sizeof(unsigned char));
    dataSize -= i_end-i_start;
}

// Insert data
//   inserts data before i_start
void DNA::insert(unsigned int i_start, unsigned char* gene, unsigned int size) {
    // Enlarge array
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize+size);

    // Get pointers
    unsigned char* p_start = ptr_set(i_start);
    unsigned char* p_backup = ptr_set(i_start + size);

    // Move genes
    memmove(p_backup, p_start, dataSize-i_start);

    // Copy new data
    memcpy(p_start, gene, size);
    dataSize += size;
}

// Replace data
//   replaces data from i_start (inclusive)
void DNA::replace(unsigned int i_start, unsigned char* gene, unsigned int size) {
    erase(i_start, i_start+size);
    if (i_start == dataSize)
        push_back(gene, size);
    else
        insert(i_start, gene, size);
}

// Append data
void DNA::push_back(unsigned char* gene, unsigned int size) {
    dataGenes = (unsigned char*) std::realloc(dataGenes, dataSize+size);
    for (unsigned int i = 0; i < size; i++) {
        dataGenes[dataSize+i] = gene[i];
    }
    dataSize += size;
}

// Extract data
//   extracts data from i_start (inclusive) to i_end (exclusive)
void DNA::extract(unsigned int i_start, unsigned int i_end, unsigned char*& gene) {
    gene = (unsigned char*) malloc((i_end-i_start) * sizeof(unsigned char));
    unsigned char* p_start = ptr_set(i_start);
    memcpy(gene, p_start, i_end-i_start);
}


//
// Gene modifiers
//

// Erase a gene
bool DNA::erase_gene(unsigned int index) {
    unsigned int amountgenes = genes();
    
    // Case 1: gene at start
    if (index == 0) {
        if (amountgenes > 1) {
            unsigned int i_next = gene_start(1);
            erase(0, i_next);
        } else {
            free(dataGenes);
            dataGenes = 0;
            dataSize = 0;
        }
    }

    // Case 2: gene at midst
    else if (index < amountgenes-1) {
        unsigned int i_self = gene_start(index);
        unsigned int i_next = gene_start(index+1);
        erase(i_self, i_next);
    }

    // Case 3: gene at end
    else {
        if (amountgenes > 1) {
            unsigned int i_prev = gene_end(index-1);
            erase(i_prev, dataSize);
        } else {
            free(dataGenes);
            dataSize = 0;
        }
    }
    return true;
}

// Insert a gene
bool DNA::insert_gene(unsigned int index, unsigned char* gene, unsigned int size) {
    unsigned int amountgenes = genes();

    // Case 1: gene at start
    if (index == 0) {
        if (amountgenes > 1) {
            unsigned char* gene_mod = (unsigned char*) malloc((size+1) * sizeof(unsigned char));
            memcpy(gene_mod, gene, size);
            gene_mod[size] = 0;
            insert(0, gene_mod, size+1);
            free(gene_mod);
        } else {
            dataGenes = (unsigned char*) malloc(size * sizeof(unsigned char));
            memcpy(dataGenes, gene, size);
            dataSize = size;
        }
    }

    // Case 2: gene at midst
    else if (index < amountgenes) {
        unsigned char* gene_mod = (unsigned char*) malloc((size+1) * sizeof(unsigned char));
        memcpy(gene_mod, gene, size);
        gene_mod[size] = 0;

        unsigned int i_self = gene_start(index);
        insert(i_self, gene_mod, size+1);
        free(gene_mod);
    }

    // Case 3: gene at end
    else {
        push_back_gene(gene, size);
    }
    return true;
}

// Replace a gene
bool DNA::replace_gene(unsigned int index, unsigned char* gene, unsigned int size) {
    erase_gene(index);
    insert_gene(index, gene, size);

    return true;
}

// Add a gene
bool DNA::push_back_gene(unsigned char* gene, unsigned int size) {
    unsigned int amountgenes = genes();

    if (amountgenes == 0)
        push_back(gene, size);
    else {
        unsigned char* gene_mod = (unsigned char*) malloc((size+1) * sizeof(unsigned char));
        memcpy(gene_mod+1, gene, size);
        gene_mod[0] = 0;
        push_back(gene_mod, size+1);
        free(gene_mod);
    }
    
    return true;
}

// Extract a gene
bool DNA::extract_gene(unsigned int index, unsigned char*& gene, unsigned int& size) {
    unsigned int i_start = gene_start(index);
    unsigned int i_end = gene_end(index);
    size = i_end-i_start;
    extract(i_start, i_end, gene);

    return true;
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