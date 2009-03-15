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
class EnvImgWrite : public EnvImage
{
    public:
        // Construction & destruction
        EnvImgWrite();

        // Required functions
		void update(const DNA& inputDNA);
		bool condition();

		// Additional functions
		void output(cairo_surface_t* inputSurface);

    private:
		int counter;
		clock_t start;
};



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

EnvImgWrite::EnvImgWrite()
{
	counter = 0;
	start = clock();
}


//
// Required functions
//

// Update call
void EnvImgWrite::update(const DNA& inputDNA)
{
    // Create surface
    cairo_surface_t* tempSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, dataInputWidth, dataInputHeight);

	// Draw the DNA onto the DC
	draw(tempSurface, inputDNA);

    // Let the application output the bitmap
    output(tempSurface);

    // Print a message
    double ms = 1000*(clock()-start)/CLOCKS_PER_SEC;
    //std::cout << "\t- " << int(ms*100)/100 << " ms: " << int(10000000*fitness(inputDNA))/100.0 << " points" << std::endl;
    std::cout << int(ms*100)/100 << " " << int(10000000*fitness(inputDNA))/100.0 << std::endl;

    // Finish
    cairo_surface_destroy(tempSurface);
}

// Condition call
bool EnvImgWrite::condition()
{
    return true;
}


//
// Additional functions
//

// Output call
void EnvImgWrite::output(cairo_surface_t* inputSurface)
{
    // Generate an output tag
    std::stringstream convert;
    convert << dataInputFile.substr( 0, dataInputFile.find_last_of(".") ) << "-";
    int zeros = counter == 0 ? IMAGE_DIGITS : IMAGE_DIGITS-log10(counter);
    for (int i = 0; i < zeros; i++)
        convert << "0";
    convert << counter++ << ".png";

    // Save the file
    cairo_surface_write_to_png(inputSurface, convert.str().c_str());
}

//////////
// MAIN //
//////////

int main(int argc, char** argv)
{
	//
	// Load image
	//

	// Check input
	if (argc != 2)
	{
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

	// Load base image
	if (!dataEnvironment.loadImage(inputFile))
	{
	    std::cout << "ERROR: could not load image" << std::endl;
	    return 1;
	}

	// Message
	std::cout << "NOTE: environment created" << std::endl;


	//
	// Create population
	//

	// Initial DNA (triangle)
	std::deque<int> tempQueue;
	tempQueue.push_back(255);	// Start of DNA
	tempQueue.push_back(50);	// Semi transparent grey brush (RGB = 50 50 50, with 50% opacity)
	tempQueue.push_back(50);
	tempQueue.push_back(50);
	tempQueue.push_back(128);
	tempQueue.push_back(1);     // Point one: (1, 254)
	tempQueue.push_back(254);
	tempQueue.push_back(128);	// Point two: (128, 1)
	tempQueue.push_back(1);
	tempQueue.push_back(254);	// Point three: (254, 254)
	tempQueue.push_back(254);
	tempQueue.push_back(255);	// End of DNA
	DNA tempDNA(tempQueue);

	// Create object
	Population dataPopulation(&dataEnvironment, tempDNA);

	// Message
	std::cout << "NOTE: population created" << std::endl;

    // Evolve
	dataPopulation.evolve_single_straight();

	return 0;
}
