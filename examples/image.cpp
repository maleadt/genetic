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
#include <cmath>
#include <queue>
#include <string>
#include <iostream>
#include <wx/filename.h>
#include <wx/cmdline.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../src/parser.h"

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
		// Required functons
		int fitness(std::list<std::list<int> >& inputList);
		int alphabet();

		// Image functions
		void setImage(wxImage& inputImage);
		bool valid_limits(std::list<std::list<int> >& inputList);
		void draw(wxMemoryDC& inputDC, std::list<std::list<int> >& inputList);
		int compare(wxMemoryDC* inputDC1, wxMemoryDC* inputD2, int width, int height);

	private:
		wxMemoryDC* dcInput;
		wxImage dataImage;
		int width, height;
};


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
	// DEBUG
	Parser tempParser(inputList);
	std::cout << std::endl << std::endl;
	tempParser.debug_queue();

	// Create a DC for the generated image
	wxBitmap tempBitmap(width, height, WXWIDGETS_DEPTH);
	wxMemoryDC tempDC;
	tempDC.SelectObject(tempBitmap);

	// Check the DNA's limit's
	if (!valid_limits(inputList))
		return -1;

	// Draw the DNA onto the DC
	draw(tempDC, inputList);

	// Convert DC to image
	wxImage tempImage = tempBitmap.ConvertToImage();

	// Compare them
	//int vfitness = compare(dcInput, &dc, width, height);
	int vfitness = 1.0/memcmp(dataImage.GetData(), tempImage.GetData(), width*height*4);
	std::cout << "Fitness: " << vfitness << std::endl;;
	return vfitness;

}


//
// Image functions
//

// Set input image
void EnvImage::setImage(wxImage& inputImage)
{
	dataImage = inputImage;
	height = dataImage.GetHeight();
	width = dataImage.GetWidth();
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
		if ((size-3)/2 > LIMIT_POLYGON_POINTS || size < 9)
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
	//DEBUG
	int x, y;
	inputDC.GetSize(&x, &y);
	std::cout << "Given DC is sized " << x << " x " << y << ", saved values are " << width << " x " << height << std::endl;

	// Loop all genes
	std::list<std::list<int> >::iterator it = inputList.begin();
	while (it != inputList.end())
	{
		std::list<int>::iterator it2 = it->begin();

		// Get colour code
		int r = *(it2++), g = *(it2++), b = *(it2++);
		wxColour colour(r, g, b);
		std::cout << "Colour: " << r << " - " << g << " - " << b << std::endl;

		// Load coördinates
		wxPoint* points = new wxPoint[(it->size()-3) / 2];
		std::cout << "Got " << (it->size()-3) / 2 << " points. " << std::endl;
		int i = 0;
		while (it2 != it->end())
		{
			// Points vary between 1 and 254, so let 1 be the lower bounw and 254 the upper one
			int x = *(it2++)-1, y = *(it2++)-1;
			points[i].x = width*x/253.0;
			points[i].y = height*y/253.0;
			std::cout << "Point at " << points[i].x << " x " << points[i].y << std::endl;
			i++;
		}

		// Draw
		inputDC.SetPen(wxPen(colour, 1));
		inputDC.DrawPolygon((it->size()-3) / 2, points);
		++it;
	}
}

// Compare two DC's
int EnvImage::compare(wxMemoryDC* inputDC1, wxMemoryDC* inputDC2, int width, int height)
{
	// Compare sizes
	int x, y;
	inputDC1->GetSize(&x, &y);
	if (width != x || height != y)
	{
		std::cout << "WARN: given width/height doesn't match first input DC";
	}
	inputDC2->GetSize(&x, &y);
	if (width != x || height != y)
	{
		std::cout << "WARN: given width/height doesn't match second input DC";
	}

	// Loop all pixels and calculate resemblance
	double resemblance = 0;
	for (x = 0; x < width; x++)
	{
		std::cout << x << std::endl;
		for (y = 0; y < height; y++)
		{
			// Get colours
			wxColour colourDC1, colourDC2;
			inputDC1->GetPixel(x, y, &colourDC1);
			inputDC2->GetPixel(x, y, &colourDC2);

			// Get RGV value of pixel 1
			int r1 = colourDC1.Red(), g1 = colourDC1.Green(), b1 = colourDC2.Blue();
			int r2 = colourDC2.Red(), g2 = colourDC2.Green(), b2 = colourDC2.Blue();

			// Calculate distance
			double distance = (r2-r1)*(r2-r1) + (g2-g1)*(g2-g1) + (b2-b1)*(b2-b1);
			resemblance += 1 / sqrt(distance);
		}
	}

	// Return the resemblance
	return (int)resemblance;
}



/////////////////
// APPLICATION //
/////////////////

//
// Class definition
//

class Image: public wxApp
{
	public:
		// Elements

	private:
		// Initialisation
		virtual bool OnInit();

		// Command-line parser
		virtual void OnInitCmdLine(wxCmdLineParser& parser);
		virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

		// Input filename
		wxFileName file;
};

// Configure the command-line parameters
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
	// Standard unnamed parameter
	{ wxCMD_LINE_PARAM, 0, 0, wxT("FILE"),
	  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

	{ wxCMD_LINE_NONE }
};

// Implement it
IMPLEMENT_APP(Image)


//
// Initialisation
//

bool Image::OnInit()
{
	//
	// Setup
	//

	// Call default behaviour (mandatory, it calls the command-line parser)
	if (!wxApp::OnInit())
		return false;

	// Message
	std::cout << "NOTE: initialisation comlpete" << std::endl;

	//
	// Load image
	//

	// Check file
	if (!file.FileExists())
	{
		std::cout << "ERROR: provided filename does not exist" << std::endl;
		return false;
	}

	// Load correct image handler
	wxImage inputImage;
	wxInitAllImageHandlers();

	// Load the file and create a bitmap
	if (!inputImage.LoadFile(file.GetFullPath()))
	{
		std::cout << "ERROR: could not load the file" << std::endl;
		return false;
	}
	wxBitmap inputBitmap(inputImage);
	inputBitmap.SetDepth(WXWIDGETS_DEPTH);

	// Convert the bitmap to a DC
	wxMemoryDC inputDC;
	inputDC.SelectObject(inputBitmap);

	// Message
	std::cout << "NOTE: file loaded" << std::endl;


	//
	// Create environment
	//

	// Create object
	EnvImage tempEnvironment;

	// Load image into environment
	tempEnvironment.setImage(inputImage);

	// Message
	std::cout << "NOTE: environment created" << std::endl;


	//
	// Create population
	//

	// Initial DNA (black window)
	std::queue<int> tempDNA;
	tempDNA.push(255);	// Start of DNA
	tempDNA.push(254);	// Black pen (RGB 254, 254, 254)
	tempDNA.push(254);
	tempDNA.push(254);
	tempDNA.push(1);	// Point one: (1, 1)
	tempDNA.push(1);
	tempDNA.push(254);	// Point two: (254, 1)
	tempDNA.push(1);
	tempDNA.push(254);	// Point three: (254, 254)
	tempDNA.push(254);
	tempDNA.push(1);	// Point four: (1, 254)
	tempDNA.push(254);
	tempDNA.push(255);	// End of DNA

	// Create object
	Population tempPopulation(&tempEnvironment, tempDNA);

	// Message
	std::cout << "NOTE: population created" << std::endl;

	// DEBUG
	try
	{
		wxBitmap bitmap(inputBitmap.GetWidth(), inputBitmap.GetHeight(), WXWIDGETS_DEPTH);
		wxMemoryDC dc;
		dc.SelectObject(bitmap);
		std::list<std::list<int> > tempDNAList = tempPopulation.getDNAList();
		tempEnvironment.draw(dc, tempDNAList);
		wxString outputFile = file.GetName() + _T("-evolved.png");
		bitmap.SaveFile(outputFile, wxBITMAP_TYPE_PNG);
		std::cout << "saved" << std::endl;
	}
	catch (std::string error)
	{
		std::cout << "ERROR: " << error << std::endl;
	}
	catch (...)
	{
		std::cout << "ERROR: unknown error" << std::endl;
	}


	//
	// Simulate
	//

	// Single straight evolution
	tempPopulation.evolve_single_straight(1);

	return false;
}


//
// Command-line parser
//

void Image::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
}

bool Image::OnCmdLineParsed(wxCmdLineParser& parser)
{
	// Get unnamed parameter (only one accepted)
	if (parser.GetParamCount() > 0)
	{
		file = wxFileName(parser.GetParam(0));
		return true;
	} else {
		std::cout << "ERROR: no input filename provided" << std::endl;
		return false;
	}
}


