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
    data_nmse = NULL;
    data_average = NULL;
}

// Destructor
EnvImage::~EnvImage()
{
    // Delete comparison data
    if (data_nmse != NULL)
        delete[] data_nmse;
    if (data_average != NULL)
        delete[] data_average;
}


//
// Required functons
//

// Fitness function
double EnvImage::fitness(const DNA* inputDNA) {
    // Check amount of polygons
    unsigned int genes = inputDNA->genes();
    if (genes < 1 || genes > LIMIT_POLYGONS)
            return 0;   // TODO: define 0 or -1 as invalid

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
int EnvImage::alphabet() const {
	return 254;
}


//
// Image functions
//

// Set input image
bool EnvImage::load(std::string inputFile)
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

// Render the DNA code onto a draw container
void EnvImage::draw(cairo_surface_t* inputSurface, const DNA* inputDNA) const {
    // Create a Cairo context
    cairo_t *cr = cairo_create(inputSurface);

    // Draw white background
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, dataInputWidth, dataInputHeight);
    cairo_fill(cr);

    // Loop all genes
    unsigned int genes = inputDNA->genes();
    for (unsigned int gene = 0; gene < genes; gene++) {
        // Get gene
        unsigned int size; unsigned char* data;
        inputDNA->extract_gene(gene, data, size);
        unsigned char* gene_ptr = data;
        unsigned int gene_loc = 0;

        // Draw if we have a colour code and at least three points
        // Also, discard genes with too many points
        if (size >= 10 && (size-4)/2 < LIMIT_POLYGON_POINTS) {
            // Get colour code
            int r = *(gene_ptr++), g = *(gene_ptr++), b = *(gene_ptr++), a = *(gene_ptr++);
            gene_loc += 4;
            cairo_set_source_rgba(cr, (r - 1) / 253.0, (g - 1) / 253.0, (b - 1) / 253.0, (a - 1) / 253.0);

            // Save all points in a container
            std::vector<vertex> points;
            int x, y;
            while (gene_loc+1 < size) {
                // Points vary between 1 and 254, so let 1 be the lower bound and 254 the upper one
                x = *(gene_ptr++) - 1;
                y = *(gene_ptr++) - 1;
                int size = points.size();
                points.resize(size+1);
                points[size].x = dataInputWidth * x / 253.0;
                points[size].y = dataInputHeight * y / 253.0;
                gene_loc += 2;
            }

            // Sort the points to avoid complex polygons
            // http://www.computational-geometry.org/mailing-lists/compgeom-announce/2003-March/000731.html
            const int points_count = points.size();
            int start = 0;	// upper left point is starting point
            for (int i = 0; i < points_count; i++) {
                if (points[i].y > points[start].y)
                    start = i;
                else if (points[i].y == points[start].y && points[i].x < points[start].x)
                    start = i;
            }
            cairo_move_to(cr, points[start].x, points[start].y);
            points[start].drawn = true;
            for (int i = 0; i < points_count; i++) {    // calculate angle for each point against startpoint
                if (i == start)
                    continue;
                points[i].angle =  atan2(points[i].y - points[start].y, points[i].x - points[start].x);
            }
            for (int i = 0; i < points_count; i++) {    // draw points based on increasing angle
                double angle;
                int next = -1;
                for (int j = 0; j < points_count; j++) {
                    if (points[j].drawn) {
                        continue;
                    }
                    if (next == -1) {
                        next = j;
                        angle = points[j].angle;
                    } else {
                        if (points[j].angle < angle) {
                            angle = points[j].angle;
                            next = j;
                        }
                    }
                }
                if (next == -1) {
                    continue;
                } else {
                    cairo_line_to(cr, points[next].x, points[next].y);
                    points[next].drawn = true;
                }
            }

            // Draw
            cairo_close_path(cr);
            cairo_fill(cr);
        }

        // Clean
        free(data);
    }
    cairo_destroy(cr);
}

// Explain a given DNA set
void EnvImage::explain(const DNA* inputDNA) const {
    std::cout << "* DNA explanation" << std::endl;

    // Main characteristics
    std::cout << "\t- Main characteristics:" << std::endl;
    std::cout << "\t\tamount of polygons: " << inputDNA->genes() << std::endl;

    // Process all polygons
    std::cout << "\t- List of polygons" << std::endl;
    unsigned int genes = inputDNA->genes();
    for (unsigned int gene = 0; gene < genes; gene++) {
        // Get gene
        unsigned int size; unsigned char* data;
        inputDNA->extract_gene(gene, data, size);
        unsigned char* gene_ptr = data;
        unsigned int gene_loc = 0;

        // Get polygon name
        int points = (size-4)/2;
        std::cout << "\t\t";
        switch (points) {
            case 3:
                std::cout << "triangle";
                break;
            case 4:
                std::cout << "rectangle";
                break;
            case 5:
                std::cout << "pentagon";
                break;
            case 6:
                std::cout << "hexagon";
                break;
            case 7:
                std::cout << "heptagon";
                break;
            case 8:
                std::cout << "octagon";
                break;
            case 9:
                std::cout << "nonagon";
                break;
            default:
                std::cout << points << "-point polygon";
                break;
        }

        // Get colour code
        int r = *(gene_ptr++), g = *(gene_ptr++), b = *(gene_ptr++), a = *(gene_ptr++);
        gene_loc += 4;
        r = 255 * ( (r-1.0)/253.0);
        g = 255 * ( (g-1.0)/253.0);
        b = 255 * ( (b-1.0)/253.0);
        a = 255 * ( (a-1.0)/253.0);
        std::cout << " with colour (" << r << ", " << g << ", " << b << ") % " << a << std::endl;

        // TODO: do something with points?

        // Advance
        free(data);
    }
}



//
// Comparison setup
//

// Setup the comparison data
void EnvImage::setup(cairo_surface_t* inputSurface) {
    switch(COMPARISON_METHOD) {
        case 0:
            setup_nmse(inputSurface);
            break;
        case 1:
            setup_average(inputSurface);
            break;
    }
}

// Setup the comparison data -- Normalised Mean Square Error method
void EnvImage::setup_nmse(cairo_surface_t* inputSurface) {
        std::cout << "NOTE: using NMSE comparison method" << std::endl;

        // Get data
        unsigned char* tempData = cairo_image_surface_get_data(inputSurface);

        // Copy data (TODO: memcpy)
        data_nmse = new unsigned char[dataInputHeight*dataInputWidth*4+1];
        for (int i = 0; i < dataInputHeight*dataInputWidth*4; i++)
            data_nmse[i] = *(tempData++);
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
            return compare_nmse(inputSurface);
            break;
        case 1:
            return compare_average(inputSurface);
            break;
        default:
            return 0;
    }
}

// Compare two images -- Normalised Mean Square Error method
double EnvImage::compare_nmse(cairo_surface_t* inputSurface) const
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
    unsigned char* tempData1 = data_nmse;
    unsigned char* tempData2 = cairo_image_surface_get_data(inputSurface);

    // Total difference
    long int difference = 0;

    // Variables
    int i, db, dg, dr;
    for (i = 0; i < dataInputWidth*dataInputHeight*4; i+=4)
    {
        // RGBa
        db = tempData1[i] - tempData2[i];
        dg = tempData1[i+1] - tempData2[i+1];
        dr = tempData1[i+2] - tempData2[i+2];

        // Calculate difference (Normalised Mean Square Error)
        difference += sqrt(dr*dr + dg*dg + db*db);
    }

    // Calculate similarity
    double similarity = 1.0 - ((double) difference / (sqrt(3.0*255.0*255.0) * dataInputWidth * dataInputHeight));
    return similarity;
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
    long int difference = 0;
    for (int i = 0; i < 3 * AVERAGE_DIV_X * AVERAGE_DIV_Y; i++) {
        difference += std::abs(data_average[i] - avgB[i]);
    }

    // Clean up
    delete[] avgB;

    // Calculate similarity
    double similarity = 1.0 - ((double) difference / (255.0 * AVERAGE_DIV_X * AVERAGE_DIV_Y * 3.0));
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
    int r = 0, g = 0, b = 0;
	int y_max = 0;
	for (int ys = 0; ys < AVERAGE_DIV_Y; ys++) {
		int x_max = 0;
		for (int xs = 0; xs < AVERAGE_DIV_X; xs++) {

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
