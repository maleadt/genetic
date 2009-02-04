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
	- 4 bits describing the colour (R, G, B, and alpha)
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
#include <wx/sizer.h>
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

// Image settings
const int IMAGE_DEPTH = 24;	// Set to 32 (if supported) to use transparency


//
// Class definitions
//

// Forward declarations
class EnvImage;
class Image;

// Application
class Image : public wxApp
{
	public:
		// Elements
		wxFrame* frame;
		wxPanel* drawPane;

	private:
		// Initialisation
		virtual bool OnInit();

		// Command-line parser
		virtual void OnInitCmdLine(wxCmdLineParser& parser);
		virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

		// Input filename
		wxFileName dataFile;

		// Evolutionary data
		EnvImage* dataEnvironment;
		Population* dataPopulation;
		wxImage* dataImage;
};

// Environment
class EnvImage : public Environment
{
	public:
		// Required functons
		double fitness(std::list<std::list<int> >& inputList);
		int alphabet();
		void update(std::list<std::list<int> >& inputList);

		// Image functions
		void setImage(wxImage* inputImage);
		bool valid_limits(std::list<std::list<int> >& inputList);
		void draw(wxDC* inputDC, std::list<std::list<int> >& inputList);
		double compare(wxImage* inputImage1, wxImage* inputImage2);
		void setGUI(Image*);

	private:
		Image* dataGUI;
		wxImage* dataImage;

		int counter;
};




/////////////////
// ENVIRONMENT //
/////////////////

//
// Required functons
//

// Fitness function
double EnvImage::fitness(std::list<std::list<int> >& inputList)
{
	// Create a DC for the generated image
	wxMemoryDC tempDC;
	wxBitmap tempBitmap(dataImage->GetWidth(), dataImage->GetHeight());
	tempDC.SelectObject(tempBitmap);

	// Check the DNA's limit's
	if (!valid_limits(inputList))
		return -1;

	// Draw the DNA onto the DC
	draw(&tempDC, inputList);

	// Convert DC to image
	wxImage tempImage = tempBitmap.ConvertToImage();

	// Compare them
	double resemblance = compare(dataImage, &tempImage);

	// Finish
	tempDC.SelectObject(wxNullBitmap);
	return resemblance;
}

// Alphabet (maximal amount of instructions)
int EnvImage::alphabet()
{
	return 254;
}

// Update call
void EnvImage::update(std::list<std::list<int> >& inputList)
{
	// Draw white background
	wxClientDC* tempDC = new wxClientDC(dataGUI->drawPane);
	wxColour tempWhite(255, 255, 255, 255);
	tempDC->SetBrush(tempWhite);
	tempDC->Clear();

	// Draw the DNA onto the DC
	draw(tempDC, inputList);

	// Clean
	delete tempDC;
}


//
// Image functions
//

// Set input image
void EnvImage::setImage(wxImage* inputImage)
{
	dataImage = inputImage;
	counter = 0;
}

// Validity function
bool EnvImage::valid_limits(std::list<std::list<int> >& inputList)
{
	// Check amount of polygons
	if (inputList.size() < 1 || inputList.size() > LIMIT_POLYGONS)
		return false;

	// Check points per polygon
	std::list<std::list<int> >::iterator it = inputList.begin();
	while (it != inputList.end())
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
void EnvImage::draw(wxDC* inputDC, std::list<std::list<int> >& inputList)
{

	// Loop all genes
	std::list<std::list<int> >::iterator it = inputList.begin();
	while (it != inputList.end())
	{
		std::list<int>::iterator it2 = it->begin();

		// Get colour code
		int r = *(it2++), g = *(it2++), b = *(it2++), a = *(it2++);
		wxColour colour(r, g, b, a);

		// Load coördinates
		wxPoint* points = new wxPoint[(it->size()-4) / 2];
		int i = 0;
		while (it2 != it->end())
		{
			// Points vary between 1 and 254, so let 1 be the lower bound and 254 the upper one
			int x = *(it2++)-1, y = *(it2++)-1;
			points[i].x = dataImage->GetWidth()*x/253.0;
			points[i].y = dataImage->GetHeight()*y/253.0;
			i++;
		}

		// Draw
		inputDC->SetPen(wxPen(colour, 1));
		inputDC->SetBrush(wxBrush(colour));
		inputDC->DrawPolygon((it->size()-4) / 2, points);
		++it;

		// Clean up
		delete[] points;
	}
}

// Compare two images
double EnvImage::compare(wxImage* inputImage1, wxImage* inputImage2)
{
	// Get and verify size
	int width = inputImage1->GetWidth(), height = inputImage1->GetHeight();
	if (width != inputImage2->GetWidth() || height != inputImage2->GetHeight())
	{
		std::cout << "WARNING: cannot calculate resemblance between two different-sized images" << std::endl;
		return 0;
	}

	// Get the raw data
	unsigned char* tempData1 = inputImage1->GetData();
	unsigned char* tempData2 = inputImage2->GetData();

	// Compare them
	long double difference = 0;
	for (int i = 0; i < width*height; i++)
	{
		// RGB (GetData always returns RGB, without Alpha)
		int dr = *(tempData1++) - *(tempData2++);
		int dg = *(tempData1++) - *(tempData2++);
		int db = *(tempData1++) - *(tempData2++);

		// Calculate difference
		difference += sqrt(abs(dr) + abs(dg) + abs(db) + 1);	// +1, in case of perfect resemblance the
																// difference will be width*height
	}

	// Get resemblance
	double resemblance = width*height / difference;
	return resemblance;
}

// Link the environment with the gui
void EnvImage::setGUI(Image* inputGUI)
{
	dataGUI = inputGUI;
}



/////////////////
// APPLICATION //
/////////////////

//
// Application events
//

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
		dataFile = wxFileName(parser.GetParam(0));
		return true;
	} else {
		std::cout << "ERROR: no input filename provided" << std::endl;
		return false;
	}
}


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
	if (!dataFile.FileExists())
	{
		std::cout << "ERROR: provided filename does not exist" << std::endl;
		return false;
	}

	// Load correct image handler (TODO)
	dataImage = new wxImage;
	wxInitAllImageHandlers();

	// Load the file
	if (!dataImage->LoadFile(dataFile.GetFullPath()))
	{
		std::cout << "ERROR: could not load the file" << std::endl;
		return false;
	}

	// Message
	std::cout << "NOTE: file loaded" << std::endl;


	//
	// Create environment
	//

	// Create object
	dataEnvironment = new EnvImage;

	// Load image into environment
	dataEnvironment->setImage(dataImage);

	// Message
	std::cout << "NOTE: environment created" << std::endl;


	//
	// GUI
	//

	// Initialise elements
	frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Genetic Evolution - ") + dataFile.GetFullName(), wxPoint(50,50), wxSize(dataImage->GetWidth(),dataImage->GetHeight()));
	drawPane = new wxPanel(frame);

	// Configure elements
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(drawPane, 1, wxEXPAND);
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
    frame->Show();

    // Give the environment access to the GUI
    wxClientDC* outputDC = new wxClientDC(drawPane);
    dataEnvironment->setGUI(this);

	// Message
	std::cout << "NOTE: GUI configured" << std::endl;


	//
	// Create population
	//

	// Initial DNA (black window)
	std::queue<int> tempDNA;
	tempDNA.push(255);	// Start of DNA
	tempDNA.push(254);	// Semi transparent black brush (RGBa = 254 254 254 128)
	tempDNA.push(254);
	tempDNA.push(254);
	tempDNA.push(128);
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
	dataPopulation = new Population(dataEnvironment, tempDNA);

	// Message
	std::cout << "NOTE: population created" << std::endl;

	dataPopulation->evolve_single_straight(10000);

	return false;
}

