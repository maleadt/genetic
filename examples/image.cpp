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
    dataInputRGB24 = NULL;
}

// Destructor
EnvImage::~EnvImage()
{
    if (dataInputRGB24 != NULL)
        delete[] dataInputRGB24;
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

	// Get data
	unsigned char* tempRGB24 = cairo_image_surface_get_data(tempSurface);

	// Save size
	dataInputWidth = cairo_image_surface_get_width(tempSurface);
	dataInputHeight = cairo_image_surface_get_height(tempSurface);

	// Save data
	dataInputRGB24 = new unsigned char[dataInputHeight*dataInputWidth*4+1];
	for (int i = 0; i < dataInputHeight*dataInputWidth*4; i++)
        dataInputRGB24[i] = *(tempRGB24++);
    dataInputRGB24[dataInputHeight*dataInputWidth*4] = 0;

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

// Compare two images
double EnvImage::compare(cairo_surface_t* inputSurface) const
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
    unsigned char* tempData1 = dataInputRGB24;
    unsigned char* tempData2 =cairo_image_surface_get_data(inputSurface);

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
