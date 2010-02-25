/*
 * image_write.cpp
 * Evolve - Image generating environment which outputs the evolved image
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
#include "image.h"
#include "../../populations/singlestraight.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <sstream>
#include <ctime>
#include <cairo/cairo.h>



//////////////////////
// CLASS DEFINITION //
//////////////////////

// Environment
class EnvImgWrite : public EnvImage {
public:
    // Construction & destruction
    EnvImgWrite();

    // Required functions
    void update(const DNA& inputDNA);
    bool condition();

    // Additional functions
    void output(cairo_surface_t* inputSurface);
    void runtime(int inputTime);

private:
    int dataTime;
    int counter;
    clock_t start;
};



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

EnvImgWrite::EnvImgWrite() {
    dataTime = -1;
    counter = 0;

#ifdef WITH_OPENMP
    start = omp_get_wtime();
#else
    start = (double) clock();
#endif
}


//
// Required functions
//

// Update call
void EnvImgWrite::update(const DNA& inputDNA) {
    // Create surface
    cairo_surface_t* tempSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, dataInputWidth, dataInputHeight);

    // Draw the DNA onto the DC
    draw(tempSurface, inputDNA);

    // Let the application output the bitmap
    output(tempSurface);

    // Print a message
#ifdef WITH_OPENMP
    double ms = 1000 * (omp_get_wtime() - start);
#else
    long seconds = (double(clock()) - start) / CLOCKS_PER_SEC;
#endif
    std::cout << "\t- " << seconds << " sec: " << 100 * fitness(inputDNA) << " points" << std::endl;

    // Finish
    cairo_surface_destroy(tempSurface);
}

// Condition call
bool EnvImgWrite::condition() {
    if (dataTime == -1)
        return true;
#ifdef WITH_OPENMP
    double ms = 1000 * (omp_get_wtime() - start);
#else
    double ms = 1000 * (double(clock()) - start) / CLOCKS_PER_SEC;
#endif
    return ms < dataTime * 1000;
}


//
// Additional functions
//

// Output call
void EnvImgWrite::output(cairo_surface_t* inputSurface) {
    // Generate an output tag
    std::stringstream convert;
    convert << dataInputFile.substr(0, dataInputFile.find_last_of(".")) << "-";
    int zeros = counter == 0 ? IMAGE_DIGITS : IMAGE_DIGITS - log10(counter);
    for (int i = 0; i < zeros; i++)
        convert << "0";
    convert << counter++ << ".png";

    // Save the file
    cairo_surface_write_to_png(inputSurface, convert.str().c_str());
}

// Set runtime
void EnvImgWrite::runtime(int inputTime) {
    dataTime = inputTime;
}



//////////
// MAIN //
//////////

int main(int argc, char** argv) {
    //
    // Load image
    //

    // Check input
    if (argc < 2) {
        std::cout << "ERROR: input filename missing or too many parameters" << std::endl;
        return 1;
    }

    // Save input file
    std::string inputFile = argv[1];

    // Message
    std::cout << "NOTE: configured" << std::endl;


    //
    // Create environment
    //

    // Create object
    EnvImgWrite dataEnvironment;

    // Max time given?
    if (argc == 3)
        dataEnvironment.runtime(atoi(argv[2]));

    // Load base image
    if (!dataEnvironment.load(inputFile)) {
        std::cout << "ERROR: could not load image" << std::endl;
        return 1;
    }

    // Message
    std::cout << "NOTE: environment created" << std::endl;


    //
    // Create population
    //

    // Initial DNA (triangle)
    unsigned char dnastring[] = {50, 50, 50, 128,     // Semi transparent grey brush (RGB = 50 50 50, with 50% opacity)
                                 1, 254,              // Point one: (1, 254)
                                 128, 1,              // Point two: (128, 1)
                                 254, 254,            // Point three: (254, 254)
                                 0,                   // And a seperator for the next gene
                                 255, 1, 1, 128,
                                 25, 225,
                                 225, 225,
                                 225, 25,
                                 25, 25};

    DNA tempDNA(dnastring, 23);
    dataEnvironment.explain(tempDNA);

    // Create object
    Population* dataPopulation = new PopSingleStraight(&dataEnvironment, tempDNA);

    // Message
    std::cout << "NOTE: population created" << std::endl;

    // Evolve
    try {
    dataPopulation->evolve();
    } catch (std::string e) {
        std::cout << "ERROR: " << e << std::endl;
    }

    // Fetch and print the resulting DNA
    const DNA& outputDNA = dataPopulation->get();
    dataEnvironment.explain(outputDNA);

    delete dataPopulation;
    return 0;
}
