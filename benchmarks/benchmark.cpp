/*
 * benchmark.cpp
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

// Headers
#include "benchmark.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Default constructor
Benchmark::Benchmark() {
    std::cout << "* Initialising benchmark framework" << std::endl;
}

// Default destructor
Benchmark::~Benchmark() {
    std::cout << "* Exiting benchmark framework" << std::endl;
}


//
// Benchmark functionality
//

// Initialise a new test run
void Benchmark::init(std::string name) {
    std::cout << "- Testing '" << name << "'" << std::endl;
    dataTestName = name;
    dataOperations = -1;
}

// Start the clock
void Benchmark::start() {
    std::cout << "\t- Running" << std::endl;
    dataTestStart = clock();
}

// Count an operation
bool Benchmark::next() {
    dataOperations++;
    
    // TODO: statistical analysis
    if (dataOperations < TEST_RUNS) {
        return true;
    }
    return false;
}

// Stop the test
void Benchmark::stop() {
    dataTestStop = clock();
}

// Print results
void Benchmark::print() {
    double millisec = diff_ms(dataTestStop, dataTestStart);
    std::cout << "\t- Time elapsed: " << smart_time(millisec) << std::endl;

    std::cout << "\t- Operations: " << dataOperations << std::endl;

    std::cout << "\t- Operations per second: " << smart_ops(dataOperations, millisec) << std::endl;
}


//
// Auxiliary
//

// Difference between two clocks
double Benchmark::diff_ms(clock_t clock1, clock_t clock2) {
    double ticks = clock1 - clock2;
    return (ticks * 1000) / CLOCKS_PER_SEC;
}

// Round a double to a given amount of decimals
double Benchmark::round(double input, int decimals) {
    double factor = 1;
    for (int i = 0; i < decimals; i++)
        factor *= 10;
    return ((int)(input*factor))/factor;
}

// Generate a view on the elapsed time
std::string Benchmark::smart_time(double ms) {
    std::stringstream buffer;
    if (ms < 1000) {
        buffer << round(ms, 2) << " ms";
    } else {
        double seconds = ms / 1000;
        if (seconds < 60) {
            buffer << seconds << " sec";
        } else {
            int minutes = seconds / 60;
            seconds -= minutes*60;
            buffer << minutes << " minutes and " << round(seconds, 2) << " seconds";
        }
    }
    return buffer.str();
}

// Generate a view on the operations per time interval
std::string Benchmark::smart_ops(long operations, double ms) {
    double ops = operations / ms;
    std::stringstream buffer;

    if (ops > 1000) {
        buffer << round(ops/1000, 2) << " ops/ns";
    } else if (ops > 1) {
        buffer << round(ops, 2) << " ops/ms";
    } else if (ops > 1/1000) {
        buffer << round(ops*1000, 2) << " ops/sec";
    } else {
        buffer << round(ops*60000, 2) << " ops/min";
    }
    
    return buffer.str();
}