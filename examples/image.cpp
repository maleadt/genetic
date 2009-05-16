/*
 * image.cpp
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
 */

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Headers
#include "image.h"



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction & destruction
//

// Constructor
EnvImage::EnvImage()
{
    // Reset comparison data
    data_manhattan = NULL;
    data_average = NULL;
}

// Destructor
EnvImage::~EnvImage()
{
    // Delete comparison data
    if (data_manhattan != NULL)
        delete[] data_manhattan;
    if (data_average != NULL)
        delete[] data_average;
}


//
// Required functons
//

// Fitness function
double EnvImage::fitness(const DNA& inputDNA) const
{
	// Check the DNA's limit's
	if (!valid_limits(inputDNA))
		return -1;

	// Create a DC for the generated image
        cairo_surface_t* tempSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, dataInputWidth, dataInputHeight);

	// Draw the DNA onto the DC
	draw(tempSurface, inputDNA);

	// Compare them
	double resemblance = compare(tempSurface);

	// Finish
	cairo_surface_destroy(tempSurface);
	return resemblance;
}

// Alphabet (maximal amount of instructions)
int EnvImage::alphabet() const
{
	return 254;
}


//
// Image functions
//

// Set input image
bool EnvImage::loadImage(std::string inputFile)
{
    // Update variables
	dataInputFile = inputFile;

	// Create Cairo surface from file
	cairo_surface_t* tempSurface = cairo_image_surface_create_from_png(dataInputFile.c_str());
	if (tempSurface == NULL)
	{
	    std::cout << "ERROR: could not load PNG image" << std::endl;
	    return false;
	}
	if (cairo_image_surface_get_format(tempSurface) != CAIRO_FORMAT_RGB24)
	{
	    std::cout << "ERROR: resulting surface is not RGB24 type" << std::endl;
	    return false;
	}

	// Save size
	dataInputWidth = cairo_image_surface_get_width(tempSurface);
	dataInputHeight = cairo_image_surface_get_height(tempSurface);

        // Setup the comparison
        setup(tempSurface);

        // Finish
        cairo_surface_destroy(tempSurface);

	// Return
	return true;
}

// Validity function
bool EnvImage::valid_limits(const DNA& inputDNA) const
{
	// Check amount of polygons
	if (inputDNA.genes() < 1 || inputDNA.genes() > LIMIT_POLYGONS)
		return false;

	// Check points per polygon
	DNA::const_iterator it = inputDNA.begin();
	while (it != inputDNA.end())
	{
		int size = (it++)->size();

		// 10 bytes at minimum
		if ((size-4)/2 > LIMIT_POLYGON_POINTS || size < 10)
			return false;

		// Should be even
		if (size%2 != 0)
			return false;
	}

	// All checked, seems valid
	return true;
}

// Render the DNA code onto a draw container
void EnvImage::draw(cairo_surface_t* inputSurface, const DNA& inputDNA) const
{
    // Create a Cairo context
    cairo_t *cr = cairo_create(inputSurface);

    // Draw white background
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, dataInputWidth, dataInputHeight);
    cairo_fill(cr);

	// Loop all genes
	DNA::const_iterator it = inputDNA.begin();
	while (it != inputDNA.end())
	{
		std::list<int>::const_iterator it2 = it->begin();

		// Get colour code
		int r = *(it2++), g = *(it2++), b = *(it2++), a = *(it2++);
        cairo_set_source_rgba(cr, (r-1)/253.0, (g-1)/253.0, (b-1)/253.0, (a-1)/253.0);

        // Move to start point
        int x = *(it2++)-1, y = *(it2++)-1;
        cairo_move_to(cr, dataInputWidth*x/253.0, dataInputHeight*y/253.0);

		// Load coördinates
		while (it2 != it->end())
		{
			// Points vary between 1 and 254, so let 1 be the lower bound and 254 the upper one
			x = *(it2++)-1;
			y = *(it2++)-1;
			cairo_line_to(cr, dataInputWidth*x/253.0, dataInputHeight*y/253.0);
		}

		// Draw
		cairo_close_path(cr);
		cairo_fill(cr);
		++it;
	}
	cairo_destroy(cr);
}


//
// Comparison setup
//

// Setup the comparison data
void EnvImage::setup(cairo_surface_t* inputSurface) {
    switch(COMPARISON_METHOD)
    {
        case 0:
            setup_manhattan(inputSurface);
            break;
        case 1:
            setup_average(inputSurface);
            break;
    }
}

// Setup the comparison data -- Manhattan method
void EnvImage::setup_manhattan(cairo_surface_t* inputSurface) {
        std::cout << "NOTE: using Manhattan comparison method" << std::endl;

        // Get data
        unsigned char* tempData = cairo_image_surface_get_data(inputSurface);

        // Copy data (TODO: memcpy)
        data_manhattan = new unsigned char[dataInputHeight*dataInputWidth*4+1];
        for (int i = 0; i < dataInputHeight*dataInputWidth*4; i++)
            data_manhattan[i] = *(tempData++);
}

// Setup the comparison data -- averaging method
void EnvImage::setup_average(cairo_surface_t* inputSurface) {
        std::cout << "NOTE: using block-averaging comparison method" << std::endl;

        // Get data
        unsigned char* tempData = cairo_image_surface_get_data(inputSurface);

	// Calculate 32x32 colour matrixes for image A
	data_average = new int[3*AVERAGE_DIV_X*AVERAGE_DIV_Y];
	help_average_divide(tempData, data_average, dataInputWidth, dataInputHeight);
}


//
// Image comparison
//

// Compare two images
double EnvImage::compare(cairo_surface_t* inputSurface) const {
    switch(COMPARISON_METHOD)
    {
        case 0:
            return compare_manhattan(inputSurface);
            break;
        case 1:
            return compare_average(inputSurface);
            break;
        default:
            return 0;
    }
}

// Compare two images -- Manhattan method (TODO: return value between 0 and 1)
double EnvImage::compare_manhattan(cairo_surface_t* inputSurface) const
{
    // Get and verify size
    if ((cairo_image_surface_get_width(inputSurface) != dataInputWidth) || (cairo_image_surface_get_height(inputSurface) != dataInputHeight))
    {
            std::cout << "WARNING: cannot calculate resemblance between two different-sized images" << std::endl;
            return 0;
    }

    // Verify formats
    if (cairo_image_surface_get_format(inputSurface) != CAIRO_FORMAT_RGB24)
    {
        std::cout << "WARNING: can only process RGB24 data" << std::endl;
        return 0;
    }

    // Get the raw data of the given surface
    unsigned char* tempData1 = data_manhattan;
    unsigned char* tempData2 = cairo_image_surface_get_data(inputSurface);

    // Total difference
    long int difference = 0;

    #ifdef WITH_OPENMP
    // Split the image in some parts
    const int parts = omp_get_max_threads();
    long int* difference_part = new long int[parts];
    const int size = (dataInputWidth*dataInputHeight*4)/parts;
    for (int i = 0; i < parts; i++)
        difference_part[i] = 0;

    // Process each part
    #pragma omp parallel
    {
        int i, dr, dg, db;
        int part = omp_get_thread_num();
        for (i = size * part; i < size * (part+1); i+=4)
        {
                // RGBa
                db = tempData1[i] - tempData2[i];
                dg = tempData1[i+1] - tempData2[i+1];
                dr = tempData1[i+2] - tempData2[i+2];

                // Calculate difference
                difference_part[part] += dr*dr + dg*dg + db*db;
        }
    }

    // Sum the total difference
    for(int i = 0; i < parts; i++)
    {
        difference += difference_part[i];
    }
    delete[] difference_part;
    #else
    // Variables
    int i, db, dg, dr;

    for (i = 0; i < dataInputWidth*dataInputHeight*4; i+=4)
    {
        // RGBa
        db = tempData1[i] - tempData2[i];
        dg = tempData1[i+1] - tempData2[i+1];
        dr = tempData1[i+2] - tempData2[i+2];

        // Calculate difference (Manhatten distance)
        difference += dr*dr + dg*dg + db*db;
    }
    #endif

    // Get resemblance
    double resemblance = dataInputWidth*dataInputHeight / (COMPARISON_SAMPLE_RATE*double(difference));
    return resemblance;
}


// Compare two images -- averaging method
double EnvImage::compare_average(cairo_surface_t* inputSurface) const
{
    // Verify formats
    if (cairo_image_surface_get_format(inputSurface) != CAIRO_FORMAT_RGB24)
    {
        std::cout << "WARNING: can only process RGB24 data" << std::endl;
        return 0;
    }

    // Get the raw data of the given surface
    unsigned char* tempDataB = cairo_image_surface_get_data(inputSurface);

    // Calculate 32x32 colour matrixes for image B
    int* avgB = new int[3 * AVERAGE_DIV_X * AVERAGE_DIV_Y];
    help_average_divide(tempDataB, avgB, dataInputWidth, dataInputHeight);

    // Compare colour matrices
    int difference = 0;
    for (int i = 0; i < 3 * AVERAGE_DIV_X * AVERAGE_DIV_Y; i++) {
        difference += std::abs(data_average[i] - avgB[i]);
    }
    double similarity = 1.0 - ((double) difference / (255.0 * AVERAGE_DIV_X * AVERAGE_DIV_Y * 3.0));

    // Clean up
    delete[] avgB;

    // Return similarity
    return similarity;
}


//
// Comparison helper routines
//

// Subdivide an image into 32x32 blocks
void EnvImage::help_average_divide(unsigned char* rgb, int* avg, int width, int height) const {
	// Calculate step sizes
	int step_x = width/AVERAGE_DIV_X;
	int step_y = height/AVERAGE_DIV_Y;

	// Process all blocks
	int y_max = 0;
	for (int ys = 0; ys < AVERAGE_DIV_Y; ys++) {
		int x_max = 0;
		for (int xs = 0; xs < AVERAGE_DIV_X; xs++) {
			int r = 0, g = 0, b = 0;

			// Calculate colour in block
			for (int y = y_max; y < y_max+step_y; y++) {
				for (int x = x_max; x < x_max+step_x; x++) {
					r += *(rgb++);
					g += *(rgb++);
					b += *(rgb++);
					rgb++;		// Skip alpha
				}
				x_max += step_x;
			}
			y_max += step_y;

			r /= step_x*step_y;
			g /= step_x*step_y;
			b /= step_x*step_y;

			// Save colours
			*(avg++) = r;
			*(avg++) = g;
			*(avg++) = b;
		}
	}
}
