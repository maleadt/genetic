/*
 * gen.cpp
 * Gene data structure.
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

/*
 * Configuration
 */

// Include header
#include "gen.h"

// Constants
const bool GENE_VERBOSE = false;



/*
 * Construction
 */

Gene::Gene(string inputGene)
{
	// Call the subroutine
	set(inputGene);
}

Gene::Gene()
{
	haveGene = false;
}


/*
 * Basic IO
 */

void Gene::set(string inputGene)
{
	// Save the gene
	dataGene = inputGene;
	haveGene = true;
	if (GENE_VERBOSE) cout << "* Gene\t\tconfigured with DNA " << inputGene << endl;

	// Parse the gene
	parse();
}

string Gene::get()
{
	return dataGene;
}





/*
 * Condition codon modification
 */

void Gene::setConditionCodon(unsigned int inputIndex, string inputCodon)
{
	dataConditionCodons[inputIndex] = inputCodon;
}

string Gene::getConditionCodon(unsigned int inputIndex)
{
	return dataConditionCodons[inputIndex];
}

unsigned int Gene::getAmountConditionCodons()
{
	return amountConditionCodons;
}

void Gene::addConditionCodon(string inputCodon)
{
	// Increase condition counter
	amountConditionCodons++;

	// Create new temporary gene container
	string *tempConditionCodons;
	tempConditionCodons = new string[amountConditionCodons];

	// Save previous genes
	for (unsigned int i = 0; i < amountConditionCodons-1; i++)
	{
		tempConditionCodons[i] = dataConditionCodons[i];
	}

	// Add new gene
	tempConditionCodons[amountConditionCodons-1] = inputCodon;

	// Replace old container with new one
	delete [] dataConditionCodons;
	dataConditionCodons = tempConditionCodons;
	rebuild();
}

void Gene::removeConditionCodon(unsigned int inputIndex)
{
	// Shift all codons
	for (unsigned int i = inputIndex; i < amountConditionCodons-1; i++)
	{
		dataConditionCodons[i] = dataConditionCodons[i+1];
	}

	// Virtually remove one gene
	amountConditionCodons--;
	rebuild();
}

void Gene::insertConditionCodon(unsigned int inputIndex, string inputCodon)
{
	// Increase the genes container
	addConditionCodon(inputCodon);	// Does not have to be inputCodon

	// Shift all genes from insertion place
	for (unsigned int i = amountConditionCodons-1; i > inputIndex; i--)
	{
		dataConditionCodons[i] = dataConditionCodons[i-1];
	}

	// Set new gene
	dataConditionCodons[inputIndex] = inputCodon;
	rebuild();
}





/*
 * Action codon modification
 */

void Gene::setActionCodon(unsigned int inputIndex, string inputCodon)
{
	dataActionCodons[inputIndex] = inputCodon;
}

string Gene::getActionCodon(unsigned int inputIndex)
{
	//cout << "Why don't I?" << endl;
	return dataActionCodons[inputIndex];
	//cout << "I do!" << endl;
}

unsigned int Gene::getAmountActionCodons()
{
	return amountActionCodons;
}

void Gene::addActionCodon(string inputCodon)
{
	// Increase action counter
	amountActionCodons++;

	// Create new temporary gene container
	string *tempActionCodons;
	tempActionCodons = new string[amountActionCodons];

	// Save previous genes
	for (unsigned int i = 0; i < amountActionCodons-1; i++)
	{
		tempActionCodons[i] = dataActionCodons[i];
	}

	// Add new gene
	tempActionCodons[amountActionCodons-1] = inputCodon;

	// Replace old container with new one
	delete [] dataActionCodons;
	dataActionCodons = tempActionCodons;
	rebuild();
}

void Gene::removeActionCodon(unsigned int inputIndex)
{
	// Shift all codons
	for (unsigned int i = inputIndex; i < amountActionCodons-1; i++)
	{
		dataActionCodons[i] = dataActionCodons[i+1];
	}

	// Virtually remove one gene
	amountActionCodons--;
	rebuild();
}

void Gene::insertActionCodon(unsigned int inputIndex, string inputCodon)
{
	// Increase the genes container
	addActionCodon(inputCodon);	// Does not have to be inputCodon

	// Shift all genes from insertion place
	for (unsigned int i = amountActionCodons-1; i > inputIndex; i--)
	{
		dataActionCodons[i] = dataActionCodons[i-1];
	}

	// Set new gene
	dataActionCodons[inputIndex] = inputCodon;
	rebuild();
}


/*
 * Operator codon modification
 */

string Gene::getOperatorCodon()
{
	return dataOperatorCodon;
}


/*
 * Gene parsing
 */

void Gene::rebuild()
{
	if (GENE_VERBOSE) cout << "* Gene\t\trebuilding gene" << endl;
	dataGene = dataOperatorCodon;

	for (unsigned int i = 0; i < amountConditionCodons; i++)
	{
		dataGene += dataConditionCodons[i];
	}

	for (unsigned int i = 0; i < amountActionCodons; i++)
	{
		dataGene += dataActionCodons[i];
	}
}

void Gene::parse()
{
	if (GENE_VERBOSE) cout << "* Gene\t\tparsing gene" << endl;

	// Save the operator
	dataOperatorCodon = dataGene.substr(0, 1);
	if (GENE_VERBOSE) cout << "* Gene\t\tfound operator " << dataOperatorCodon << endl;

	// Reset the counters
	amountConditionCodons = 0;
	amountActionCodons = 0;

	// Seperate the conditions from the actions
	string tempConditionCodons;
	string tempActionCodons;
	for (unsigned int i = 1; i < dataGene.length(); i+=2)
	{
		if (isCondition(dataGene.substr(i, 1)))
		{
			amountConditionCodons++;
		}
		else if (isAction(dataGene.substr(i, 1)))
		{
			amountActionCodons++;
		}
		else
		{
			valid = false;
			cout << "! Gene parsing failed." << endl;
			cout << "Gene contents: " << dataGene << endl;
			cout << "I was analysing position " << i << endl;
			return;
		}
	}
	if (GENE_VERBOSE) cout << "* Gene\t\tfound " << amountConditionCodons << " conditions, and " << amountActionCodons << " actions" << endl;

	// Initialise conditions and save them
	dataConditionCodons = new string[amountConditionCodons];
	for (unsigned int i = 0; i < amountConditionCodons; i++)
	{
		if (GENE_VERBOSE) cout << "* Gene\t\tsaving condition " << dataGene.substr(1+2*i, 2) << " at index " << i << endl;
		dataConditionCodons[i] = dataGene.substr(1+2*i, 2);
	}

	// Initialise actions and save them
	dataActionCodons = new string[amountActionCodons];
	for (unsigned int i = 0; i < amountActionCodons; i++)
	{
		if (GENE_VERBOSE) cout << "* Gene\t\tsaving action " << dataGene.substr(1+2*amountConditionCodons+2*i, 2) << " at index " << i << endl;
		dataActionCodons[i] = dataGene.substr(1+2*amountConditionCodons+2*i, 2);
	}

	valid = true;
}
