/*
 * benchmark.h
 * Evolve - Benchmark framework.
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

// Include guard
#ifndef __BENCHMARK
#define __BENCHMARK

// Headers
#include <iostream>
#include <ctime>
#include <sstream>


//
// Constants
//

const int TEST_RUNS = 1000000;


//////////////////////
// CLASS DEFINITION //
//////////////////////

class Benchmark {
public:
    // Construction and destruction
    Benchmark();
    ~Benchmark();

    // Benchmark functionality
    void init(std::string name);
    void start();
    bool next();
    void stop();
    void print();

private:
    // Auxiliary
    double diff_ms(clock_t clock1, clock_t clock2);
    double round(double input, int decimals);
    std::string smart_time(double ms);
    std::string smart_ops(long operations, double millisec);

    // Member data
    std::string dataTestName;
    long dataOperations;
    clock_t dataTestStart, dataTestStop;

};



// Include guard
#endif
