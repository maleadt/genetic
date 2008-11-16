               :/*
               : * dna.cpp
               : * DNA class.
               : *
               : * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
               : * All rights reserved.
               : *
               : * This program is free software: you can redistribute it and/or modify
               : * it under the terms of the GNU General Public License as published by
               : * the Free Software Foundation, either version 3 of the License, or
               : * at your option) any later version.
               : *
               : * This program is distributed in the hope that it will be useful,
               : * but WITHOUT ANY WARRANTY; without even the implied warranty of
               : * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
               : * GNU General Public License for more details.
               : *
               : * You should have received a copy of the GNU General Public License
               : * along with this program.  If not, see <http://www.gnu.org/licenses/>.
               : *
               : */
               :
               :///////////////////
               :// CONFIGURATION //
               :///////////////////
               :
               ://
               :// Essential stuff
               ://
               :
               :
               :// Headers
               :#include "dna.h"
               :#include "creature.h"
               :
               :
               :////////////////////
               :// CLASS ROUTINES //
               :////////////////////
               :
               ://
               :// Construction and destruction
               ://
               :
               :DNA::DNA()
               :{
               :}
               :
               :// Copy constructor
               :DNA::DNA(const DNA &oldDNA)
               :{
               :	// Copy all genes
               :	std::list<Gen*>::const_iterator it = oldDNA.dataGenes.begin();
               :	while (it != oldDNA.dataGenes.end())
               :	{
               :		// Copy all commands and save them
               :		dataGenes.push_back( new Gen( (*(*it)) ) );
               :
               :		++it;
               :	}
               :
               :	// Copy other data
               :	parent = oldDNA.parent;
               :}
               :
               :DNA::~DNA()
               :{
               :	// Remove all genes
               :	std::list<Gen*>::iterator it_gene = dataGenes.begin();
               :	while (it_gene != dataGenes.end())
               :	{
               :		(*it_gene)->~Gen();
               :		delete (*it_gene);
               :		++it_gene;
               :	}
               :	dataGenes.clear();
               :}
               :
               :
               ://
               :// Input & output
               ://
               :
               :// Input textual data
               :void DNA::inputTextual(std::string &inputText) /* DNA::inputTextual(std::string&) total:      4 6.9e-04 */
               :{
               :	// Detect genes, and create a new object for them
               :	unsigned int index_start = 0;
               :	while (index_start < inputText.length())
               :	{
               :		unsigned int index_end = index_start+1;
     4 6.9e-04 :		while (index_end < inputText.length() && inputText[index_end] != dnaSeparators["gen"])
               :			index_end++;
               :		Gen* outputGen = new Gen;
               :		outputGen->parent = this;
               :		outputGen->inputTextual(inputText.substr(index_start, index_end - index_start));
               :		dataGenes.push_back(outputGen);
               :		index_start = index_end+1;
               :	}
               :}
               :
               :// Output textual data
     1 1.7e-04 :std::string DNA::outputTextual() /* DNA::outputTextual() total:      3 5.2e-04 */
               :{
               :	std::string outputTextual;
               :
               :	// Process all genes
               :	std::list<Gen*>::iterator it = dataGenes.begin();
               :	while (it != dataGenes.end())
               :	{
               :		if (it != dataGenes.begin())
               :			outputTextual += dnaSeparators["gen"];
     2 3.4e-04 :		outputTextual += (*it)->outputTextual();
               :		++it;
               :	}
               :
               :	return outputTextual;
               :}
               :
               :// Output code
               :std::string DNA::outputCode()
               :{
               :	std::string outputCode;
               :
               :	// Process all genes
               :	std::list<Gen*>::iterator it = dataGenes.begin();
               :	while (it != dataGenes.end())
               :	{
               :		outputCode += (*it)->outputCode();
               :		outputCode += "\n";
               :		++it;
               :	}
               :
               :	return outputCode;
               :}
               :
               :
               ://
               :// Code execution
               ://
               :
    65  0.0112 :void DNA::execute() /* DNA::execute() total:   1908  0.3288 */
               :{
   118  0.0203 :	std::list<Gen*>::iterator it = dataGenes.begin();
   573  0.0987 :	while (it != dataGenes.end())
               :	{
   362  0.0624 :		(*it)->execute();
   506  0.0872 :		++it;
               :	}
   284  0.0489 :}
               :
               :bool DNA::is_valid() /* DNA::is_valid() total:      1 1.7e-04 */
               :{
               :	// Process all genes
               :	std::list<Gen*>::iterator it = dataGenes.begin();
               :	while (it != dataGenes.end())
               :	{
               :		if (!((*it)->is_valid()))
               :			return false;
               :		++it;
               :	}
               :
     1 1.7e-04 :	return true;
               :}
               :
               :
               :
               :
               ://
               :// Code mutation
               ://
               :
               :void DNA::mutate(unsigned int mutLevel) /* DNA::mutate(unsigned int) total:      1 1.7e-04 */
               :{
               :	if (mutLevel == 0)
               :	{
     1 1.7e-04 :		mutateSelf();
               :	}
               :	else
               :	{
               :		unsigned int randChild = random_range(0, dataGenes.size()-1);
               :		std::list<Gen*>::iterator it = dataGenes.begin();
               :		for (unsigned int i = 0; i < randChild; i++)
               :			++it;
               :		(*it)->mutate(mutLevel-1);
               :	}
               :}
               :
     1 1.7e-04 :void DNA::mutateSelf() /* DNA::mutateSelf() total:      2 3.4e-04 */
               :{
               :	// We need at least 1 gene
               :	if (dataGenes.size() < 1)
               :		return;
               :
               :	// Get some random stuff we'll need
               :	int randomMutation = random_range(1, 5);
               :	int randomGen = random_range(0, dataGenes.size()-1);
               :	int randomGen2 = randomGen;
     1 1.7e-04 :	if (dataGenes.size() > 1)
               :	{
               :		while (randomGen == randomGen2)
               :			randomGen2 = random_range(0, dataGenes.size()-1);
               :	}
               :	std::list<Gen*>::iterator iteratorGen = dataGenes.begin();
               :	for (int i = 0; i < randomGen; i++)
               :		++iteratorGen;
               :	std::list<Gen*>::iterator iteratorGen2 = dataGenes.begin();
               :	for (int i = 0; i < randomGen2; i++)
               :		++iteratorGen2;
               :
               :	// Mutate
               :	switch (randomMutation)
               :	{
               :		case 1:
               :			// Deletion
               :			(*iteratorGen)->~Gen();
               :			delete((*iteratorGen));
               :			dataGenes.erase(iteratorGen);
               :			break;
               :
               :		case 2:
               :			// Duplication
               :			dataGenes.push_back(new Gen((*(*iteratorGen))));
               :			break;
               :
               :		case 3:
               :			// Inversion
               :			if (dataGenes.size() > 1)
               :			{
               :				Gen* temp = *iteratorGen;
               :				*iteratorGen = *iteratorGen2;
               :				*iteratorGen2 = temp;
               :			}
               :			break;
               :
               :		case 4:
               :			// Amplification
               :			for (int i = 0; i < random_range(1, dataGenes.size()); i++)
               :				dataGenes.insert(iteratorGen, new Gen(*(*iteratorGen)));;
               :			break;
               :
               :		// Translocation
               :		case 5:
               :			if (dataGenes.size() > 1)
               :			{
               :				Gen* tempGen = *iteratorGen;
               :				dataGenes.erase(iteratorGen);
               :				dataGenes.insert(iteratorGen2, tempGen);
               :			}
               :	}
               :}
/* 
 * Total samples for file : "/home/tim/Documenten/Programmeren/C(++)/Genetic Evolution/V3: C++, STL/dna.cpp"
 * 
 *   1919  0.3307
 */


/* 
 * Command line: opannotate --source --output-dir=. ../bin/Debug/evolve 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: P4 / Xeon with 2 hyper-threads, speed 3391.7 MHz (estimated)
 * Counted GLOBAL_POWER_EVENTS events (time during which processor is not stopped) with a unit mask of 0x01 (mandatory) count 100000
 */
