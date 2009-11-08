/*
 * image.h
 * Evolve - Image generating environment
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
 *
 * DNA interpretation
 * 	- 4 bits describing the colour (R, G, B, and alpha)
 * 	- All following pairs of 2 bytes describe a point, in which
 * 	  a coordinate = byte/254 * image_width or image_height
 *
 * Thus, a minimum of 9 bytes is required, and should be odd!
 *
 * REMARK: ignoring invalidity and skipping the byte (might get corrected
 * on later mutation) could be better.
 *
 */

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Include guard
#ifndef __IMAGE
#define __IMAGE

// Headers
#include "../../population.h"
#include "../../environment.h"
#include "../../dna.h"
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <cairo/cairo.h>
#ifdef WITH_OPENMP
#include <omp.h>
#endif


//
// Constants
//

// Image output settings
const int IMAGE_DIGITS = 5;

// Polygon limits
const unsigned int LIMIT_POLYGONS = 50;
const unsigned int LIMIT_POLYGON_POINTS = 5;

// Division settings
const int AVERAGE_DIV_X = 32;
const int AVERAGE_DIV_Y = 32;

// Comparison method
const int COMPARISON_METHOD = 1;


//
// Additional structures
//

struct vertex {
    int x;
    int y;
    bool drawn;
    double angle;
};



//////////////////////
// CLASS DEFINITION //
//////////////////////

// Environment
class EnvImage : public Environment
{
	public:
		// Construction & destruction
		EnvImage();
		~EnvImage();

		// Required functons
		double fitness(const DNA* inputDNA);
		int alphabet() const;

		// Image functions
		bool load(std::string inputFile);
		void draw(cairo_surface_t* inputSurface, const DNA* inputDNA) const;
                void explain(const DNA* inputDNA) const;

                // Comparison setup
                void setup(cairo_surface_t* inputSurface);
                void setup_nmse(cairo_surface_t* inputSurface);
                void setup_average(cairo_surface_t* inputSurface);

                // Image comparison
		double compare(cairo_surface_t* inputSurface) const;
                double compare_nmse(cairo_surface_t* inputSurface) const;
		double compare_average(cairo_surface_t* inputSurface) const;

        private:
                // Comparison helper functions
                void help_average_divide(unsigned char* rgb, int* avg, int width, int height) const;

                // Comparison data
                unsigned char* data_nmse;
                int* data_average;

	protected:
		std::string dataInputFile;
		int dataInputWidth, dataInputHeight;
};

// Include guard
#endif
