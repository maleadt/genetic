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

/*
DNA format specification
	- 3 bits describing the colour (R, G and B)
	- All following pairs of 2 bytes describe a point, in which
	  a coordinate = byte/254 * image_width or image_height

Thus, a minimum of 9 bytes is required, and should be odd!

REMARK: ignoring invalidity and skipping the byte (might get corrected
on later mutation) could be better.
*/

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Headers
#include "../src/population.h"
#include "../src/environment.h"
#include <queue>
#include <string>
#include <iostream>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//
// Constants
//

// Polygon limits
const int LIMIT_POLYGONS = 50;
const int LIMIT_POLYGON_POINTS = 5;

// wxWidgets settings
const int WXWIDGETS_DEPTH = 32;

/////////////////
// ENVIRONMENT //
/////////////////

//
// Class definition
//

class EnvImage : public Environment
{
	public:
		// Construction and destruction
		EnvImage();

		// Required functons
		int fitness(std::list<std::list<int> >& inputList);
		int alphabet();

		// Image functions
		void setDC(wxMemoryDC* inputDC, int inputWidth, int inputHeight);
		bool valid_limits(std::list<std::list<int> >& inputList);
		void draw(wxMemoryDC& inputDC, std::list<std::list<int> >& inputList);
		int compare(wxMemoryDC* inputDC1, wxMemoryDC* inputD2, int width, int height);

	private:
		wxMemoryDC* dcInput;
		int width, height;
};


//
// Construction and destruction
//

// Constructor
EnvImage::EnvImage()
{
}


//
// Required functons
//

// Alphabet (maximal amount of instructions)
int EnvImage::alphabet()
{
	return 254;
}

// Fitness function
int EnvImage::fitness(std::list<std::list<int> >& inputList)
{
	// Create a DC for the generated image
	wxBitmap bitmap(width, height, WXWIDGETS_DEPTH);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);

	// Check the DNA's limit's
	if (!valid_limits(inputList))
	{
		return -1;
	}

	// Draw the DNA onto the DC
	draw(dc, inputList);

	// Compare them
	return compare(dcInput, &dc, width, height);
}


//
// Image functions
//

// Set input bitmap
void EnvImage::setDC(wxMemoryDC* inputDC, int inputWidth, int inputHeight)
{
	dcInput = inputDC;
	width = inputWidth;
	height = inputHeight;
}

// Validity function
bool EnvImage::valid_limits(std::list<std::list<int> >& inputList)
{
	// Check amount of polygons
	if (inputList.size() > LIMIT_POLYGONS)
		return false;

	// Check points per polygon
	std::list<std::list<int> >::iterator it = inputList.begin();
	while (it != inputList.end())
	{
		int size = (it++)->size();

		// 9 bytes at minimum
		if (size > LIMIT_POLYGON_POINTS || size < 9)
			return false;

		// Should be odd
		if (size%2 != 1)
			return false;
	}

	// All checked, seems valid
	return true;
}

// Render the DNA code onto a draw container
void EnvImage::draw(wxMemoryDC& inputDC, std::list<std::list<int> >& inputList)
{
	// Loop all genes
	std::list<std::list<int> >::iterator it = inputList.begin();
	while (it != inputList.end())
	{
		std::list<int>::iterator it2 = it->begin();

		// Get colour code
		int r = *(it2++), g = *(it2++), b = *(it2++);
		wxColour colour(r, g, b);

		// Load coördinates
		wxPoint* points = new wxPoint[(it->size()-3) / 2];
		int i = 0;
		while (it2 != it->end())
		{
			int x = *(it2++), y = *(it2++);
			points[i].x = width*x/254.0;
			points[i].y = height*y/254.0;
			i++;
		}

		// Draw
		inputDC.SetPen(wxPen(colour, 1));
		inputDC.DrawPolygon((it->size()-3) / 2, points);
		++it;
	}
}

// Compare two DC's
int EnvImage::compare(wxMemoryDC* inputDC1, wxMemoryDC* inputD2, int width, int height)
{

	return 0;
}


//////////
// MAIN //
//////////

int main(int argc, char* argv[])
{
	//
	// Load image
	//

	// Read file from command-line
	std::string inputFile(argv[1]);
	if (inputFile.size() < 1)
	{
		std::cout << "ERROR: please provide an input file to read" << std::endl;
		exit(1);
	}

	// Load the file
	wxString inputFileWx(inputFile.c_str(), wxConvUTF8);
	wxBitmap inputBitmap;
	if (!inputBitmap.LoadFile(inputFileWx))
	{
		std::cout << "ERROR: could not load the file" << std::endl;
	}
	inputBitmap.SetDepth(WXWIDGETS_DEPTH);

	// Convert the bitmap to a DC
	wxMemoryDC inputDC;
	inputDC.SelectObject(inputBitmap);


	//
	// Create environment
	//

	// Create object
	EnvImage tempEnvironment;

	// Load image into environment
	tempEnvironment.setDC(&inputDC, inputBitmap.GetWidth(), inputBitmap.GetHeight());


	//
	// Create population
	//

	// Initial DNA
	std::queue<int> tempDNA;

	// Create object
	Population tempPopulation(&tempEnvironment, tempDNA);

	return 0;
}


