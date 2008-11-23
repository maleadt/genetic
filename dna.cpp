/*
 * dna.cpp
 * DNA class.
 *
 * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
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
#include "dna.h"
#include "creature.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

DNA::DNA()
{
}

// Copy constructor
DNA::DNA(const DNA &oldDNA)
{
	// Copy all genes
	std::list<Gen*>::const_iterator it = oldDNA.dataGenes.begin();
	while (it != oldDNA.dataGenes.end())
	{
		// Copy all genes and save them
		dataGenes.push_back( new Gen( (*(*it)) ) );

		// Update our children's parent-pointer
		dataGenes.back()->parent = this;

		++it;
	}

	// Copy other data
	parent = oldDNA.parent;
}

DNA::~DNA()
{
	// Remove all genes
	std::list<Gen*>::iterator it_gene = dataGenes.begin();
	while (it_gene != dataGenes.end())
	{
		(*it_gene)->~Gen();
		delete (*it_gene);
		++it_gene;
	}
	dataGenes.clear();
}


//
// Input & output
//

// Input textual data
void DNA::inputTextual(std::string &inputText)
{
	// Detect genes, and create a new object for them
	unsigned int index_start = 0;
	while (index_start < inputText.length())
	{
		unsigned int index_end = index_start+1;
		while (index_end < inputText.length() && inputText[index_end] != dnaSeparators["gen"])
			index_end++;
		Gen* outputGen = new Gen;
		outputGen->parent = this;
		outputGen->inputTextual(inputText.substr(index_start, index_end - index_start));
		dataGenes.push_back(outputGen);
		index_start = index_end+1;
	}
}

// Output textual data
std::string DNA::outputTextual()
{
	std::string outputTextual;

	// Process all genes
	std::list<Gen*>::iterator it = dataGenes.begin();
	while (it != dataGenes.end())
	{
		if (it != dataGenes.begin())
			outputTextual += dnaSeparators["gen"];
		outputTextual += (*it)->outputTextual();
		++it;
	}

	return outputTextual;
}

// Output code
std::string DNA::outputCode()
{
	std::string outputCode;

	// Process all genes
	std::list<Gen*>::iterator it = dataGenes.begin();
	while (it != dataGenes.end())
	{
		outputCode += (*it)->outputCode();
		outputCode += "\n";
		++it;
	}

	return outputCode;
}


//
// Code execution
//

void DNA::execute()
{
	std::list<Gen*>::iterator it = dataGenes.begin();
	while (it != dataGenes.end())
	{
		(*it)->execute();
		++it;
	}
}

bool DNA::is_valid()
{
	// Process all genes
	std::list<Gen*>::iterator it = dataGenes.begin();
	while (it != dataGenes.end())
	{
		if (!((*it)->is_valid()))
		{
			return false;
		}
		++it;
	}
	return true;
}




//
// Code mutation
//

void DNA::mutate(unsigned int mutLevel)
{
	if (mutLevel == 0)
	{
		mutateSelf();
	}
	else
	{
		unsigned int randChild = random_range(0, dataGenes.size()-1);
		std::list<Gen*>::iterator it = dataGenes.begin();

		for (unsigned int i = 0; i < randChild; i++)
		{
			++it;
		}

		(*it)->mutate(mutLevel-1);
	}
}

void DNA::mutateSelf()
{
	// We need at least 1 gene
	if (dataGenes.size() < 1)
		return;

	// Get some random stuff we'll need
	int randomMutation = random_range(1, 5);
	int randomGen = random_range(0, dataGenes.size()-1);
	int randomGen2 = randomGen;
	if (dataGenes.size() > 1)
	{
		while (randomGen == randomGen2)
			randomGen2 = random_range(0, dataGenes.size()-1);
	}
	std::list<Gen*>::iterator iteratorGen = dataGenes.begin();
	for (int i = 0; i < randomGen; i++)
		++iteratorGen;
	std::list<Gen*>::iterator iteratorGen2 = dataGenes.begin();
	for (int i = 0; i < randomGen2; i++)
		++iteratorGen2;

	// Mutate
	switch (randomMutation)
	{
		// Deletion
		case 1:
			(*iteratorGen)->~Gen();
			delete((*iteratorGen));
			dataGenes.erase(iteratorGen);
			break;

		// Duplication
		case 2:
			dataGenes.push_back(new Gen((*(*iteratorGen))));
			break;


		// Inversion
		case 3:
			if (dataGenes.size() > 1)
			{
				Gen* temp = *iteratorGen;
				*iteratorGen = *iteratorGen2;
				*iteratorGen2 = temp;
			}
			break;

		// Amplification
		case 4:
			for (int i = 0; i < random_range(1, dataGenes.size()); i++)
				dataGenes.insert(iteratorGen, new Gen(*(*iteratorGen)));;
			break;

		// Translocation
		case 5:
			if (dataGenes.size() > 1)
			{
				Gen* tempGen = *iteratorGen;
				dataGenes.erase(iteratorGen);
				dataGenes.insert(iteratorGen2, tempGen);
			}
	}
}
