/*
 * benchmark_dna.cpp
 * Evolve - DNA datastructure benchmark application.
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
#include "../src/dna.h"
#include "benchmark.h"
#include "../src/generic.h"

//
// Constants
//



//////////
// MAIN //
//////////


int main() {
    //
    // Configure
    //

    Benchmark benchmark;


    //
    // Constructions
    //

    benchmark.init("object constructions");
    unsigned char dna1[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03};
    benchmark.start();
    while (benchmark.next()) {
        DNA tempDNA(dna1, 19);
    }
    benchmark.stop();
    
    benchmark.print();


    //
    // Gene count
    //

    benchmark.init("gene count");
    DNA tempDNA(dna1, 19);
    benchmark.start();
    while (benchmark.next()) {
        tempDNA.genes();
    }
    benchmark.stop();

    benchmark.print();


    //
    // Erases
    //

    benchmark.init("erases");
    int i = 0;
    int max = tempDNA.genes();
    benchmark.start();
    while (benchmark.next()) {
        DNA tempDNA(dna1, 19);
        tempDNA.erase_gene(i++);
        i %= max;
    }
    benchmark.stop();

    benchmark.print();


    //
    // Inserts
    //

    benchmark.init("insertions");
    unsigned char dna2[] = {0x02, 0x03, 0x02, 0x01, 0x02, 0x03};
    i = 0;
    max = tempDNA.genes();
    benchmark.start();
    while (benchmark.next()) {
        DNA tempDNA(dna1, 19);
        tempDNA.insert_gene(i++, dna2, 6);
        i %= max;
    }
    benchmark.stop();

    benchmark.print();

}