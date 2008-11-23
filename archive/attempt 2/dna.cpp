/*
 * dna.cpp
 * DNA data structure
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
#include "dna.h"

// Constants
const bool DNA_VERBOSE = false;
const bool MUT_VERBOSE = false;






/*
 * Construction
 */

DNA::DNA()
{
	haveString = false;
}

DNA::DNA(string inputString)
{
	// Call subroutine
	set(inputString);
}





/*
 * Basic IO
 */

void DNA::set(string inputString)
{
	// Save the input DNA
	dataString = inputString;
	haveString = true;
	amountGenes = 0;
	if (DNA_VERBOSE) cout << "* DNA\t\tconfigured with DNA " << dataString << endl;

	// Run the DNA parser
	parse();
}

string DNA::get()
{
	return dataString;
}

void DNA::output()
{
	cout << "* " << dataString << endl;
	for (unsigned int i = 0; i < amountGenes; i++)
	{
		cout << "\t" << dataGenes[i].get() << endl;
	}
}




/*
 * Gene modification
 */

int DNA::getAmountGenes()
{
	return amountGenes;
}

Gene DNA::getGene(unsigned int inputGeneIndex)
{
	return dataGenes[inputGeneIndex];
}

void DNA::setGene(unsigned int inputIndex, Gene inputGene)
{
	dataGenes[inputIndex] = inputGene;
	rebuild();
}

void DNA::addGene(Gene inputGene)
{
	// Increase gene counter
	amountGenes++;

	// Create new temporary gene container
	Gene *tempGenes;
	tempGenes = new Gene[amountGenes];

	// Save previous genes
	for (unsigned int i = 0; i < amountGenes-1; i++)
	{
		tempGenes[i] = dataGenes[i];
	}

	// Add new gene
	tempGenes[amountGenes-1] = inputGene;

	// Replace old container with new one
	delete [] dataGenes;
	dataGenes = tempGenes;
	rebuild();
}

void DNA::removeGene(unsigned int inputIndex)
{
	// Shift all genes
	for (unsigned int i = inputIndex; i < amountGenes-1; i++)
	{
		dataGenes[i] = dataGenes[i+1];
	}

	// Virtually remove one gene
	amountGenes--;
	rebuild();
}

void DNA::insertGene(unsigned int inputIndex, Gene inputGene)
{
	// Increase the genes container
	addGene(inputGene);	// Does not have to be inputGene

	// Shift all genes from insertion place
	for (unsigned int i = amountGenes-1; i > inputIndex; i--)
	{
		dataGenes[i] = dataGenes[i-1];
	}

	// Set new gene
	dataGenes[inputIndex] = inputGene;
	rebuild();
}





/*
 * DNA Parsing
 */

// Divide the DNA string in seperate genes
void DNA::parse()
{
	//
	// Check
	//

	if (DNA_VERBOSE) cout << "* DNA\t\tparsing DNA"<< endl;

	// Count the operators in the DNA string
	int tempCountOperators = 0;
	for (unsigned int i = 0; i < dataString.length(); i++)
	{
		if (isOperator(dataString.substr(i, 1)))
		{
			tempCountOperators++;
		}
	}
	if (DNA_VERBOSE) cout << "* DNA\t\tCounted " << tempCountOperators << " operators" << endl;

	// Count the stopcodons
	int tempCountStopcodons = 0;
	for (unsigned int i = 0; i < dataString.length(); i++)
	{
		if (dataString.substr(i, 1) == "X")
		{
			tempCountStopcodons++;
		}
	}
	if (DNA_VERBOSE) cout << "* DNA\t\tCounted " << tempCountStopcodons << " stop codons" << endl;

	// Compare them
	if (tempCountOperators != tempCountStopcodons)
	{
		cout << "! Not the same amount of stopcodons and operators" << endl;
		valid = false;
		return;
	}

	//
	// Initialise
	//

	amountGenes = tempCountOperators;
	// Error: Gene *dataGenes;	// This causes the object var to be overwritten :(
	dataGenes = new Gene[amountGenes];



	//
	// Split
	//

	int tempCurrentGene = 0;
	for (unsigned int i = 0; i < dataString.length(); i++)
	{
		// Check
		if (! isOperator(dataString.substr(i, 1)))
		{
			cout << "! This character should be an operator." << endl;
			valid = false;
			return;
		}

		// Find X
		unsigned int tempIndexX = dataString.find_first_of("X", i);
		if (tempIndexX >= dataString.length())
		{
			valid = false;
			cout << "! No stopcodon found." << endl;
			return;
		}

		// Save gene
		if (DNA_VERBOSE) cout << "* DNA\t\tSaving gene " << dataString.substr(i, tempIndexX - i) << " at index " << tempCurrentGene << endl;
		dataGenes[tempCurrentGene].set(dataString.substr(i, tempIndexX - i));
		if (!dataGenes[tempCurrentGene].valid)
		{
			valid = false;
			cout << "! Failed setting gene." << endl;
			return;
		}

		// Adjust index
		i = tempIndexX;
		tempCurrentGene++;
	}

	valid = true;
}

void DNA::rebuild()
{
	if (DNA_VERBOSE) cout << "* DNA\t\trebuilding DNA"<< endl;
	string tempString;
	for (unsigned int i = 0; i < amountGenes; i++)
	{
		tempString += getGene(i).get() + "X";
	}
	dataString = tempString;
}





/*
 * Mutation
 */

void DNA::mutate()
{
	// How many times we want to mutate
	int randMutations = random_range(2, 8) / 2;

	for (int i = 0; i < randMutations; i++)
	{
		// Get random factor
		int tempRandom = random_range(1, 4);

		if (tempRandom == 1)
		{
			mutatePoint();
		}
		else if (tempRandom == 2)
		{
			mutateChromosome();
		}
		else if (tempRandom == 3)
		{
			mutateGen();
		}
		else if (tempRandom == 4)
		{
			mutateDirection();
		}
	}

	mutateCleanGenes();
	mutateCleanDNA();
}

void DNA::mutateCleanDNA()
{
	if (DNA_VERBOSE) cout << "* DNA\t\tcleaning DNA"<< endl;
	//TODO: compare last with first gene, how to do that cleanly?
	// Delete identical genes
	for (unsigned int i = 1; i < amountGenes; i++)
	{
		if (getGene(i).get() == getGene(i-1).get())
		{
			removeGene(i);
			i--;
		}
	}

	// Replace CYYYYXAYYYX genes with CYYYYX equivalent (and the same for B-D combo's)
	for (unsigned int i = 1; i < amountGenes; i++)
	{
		if (getGene(i).get().substr(1) == getGene(i-1).get().substr(1))
		{
			string operator1 = getGene(i-1).get().substr(0, 1);
			string operator2 = getGene(i).get().substr(0, 1);

			if (operator1=="C" && operator2=="A")
			{
				removeGene(i);
				i--;
			}
			else if (operator1=="A" && operator2=="C")
			{
				removeGene(i-1);
				i--;
			}
			else if (operator1=="B" && operator2=="D")
			{
				removeGene(i-1);
				i--;
			}
			else if (operator1=="D" && operator2=="B")
			{
				removeGene(i-1);
				i--;
			}
		}
	}
}

void DNA::mutateCleanGenes()
{
	if (DNA_VERBOSE) cout << "* DNA\t\tcleaning genes"<< endl;

	// Loop every gene
	for (unsigned int i = 0; i < amountGenes; i++)
	{
		// Fetch gene
		Gene tempGene = getGene(i);

		// Clean conditions
		for (unsigned int j = 0; j < tempGene.getAmountConditionCodons()-1; j++)
		{
			if (tempGene.getConditionCodon(j) == tempGene.getConditionCodon(j+1))
			{
				tempGene.removeConditionCodon(j);
				j--;
			}
		}

		// Clean actions
		for (unsigned int j = 0; j < tempGene.getAmountActionCodons()-1; j++)
		{
			if (tempGene.getActionCodon(j) == tempGene.getActionCodon(j+1))
			{
				tempGene.removeActionCodon(j);
				j--;
			}
		}
		// Save gene
		setGene(i, tempGene);
	}
}

void DNA::mutateDirection()
{
	if (DNA_VERBOSE) cout << "* DNA\t\tdirection mutating"<< endl;

	// Pick a random element
	int randElementIndex = random_range(0, dataString.length()-1);
	while (!isDirection(dataString.substr(randElementIndex, 1)))
	{
		randElementIndex = random_range(0, dataString.length()-1);
	}
	string randElement = dataString.substr(randElementIndex, 1);

	// Generate a mutated counterpart
	string mutated;
	mutated = random_char(dataDNADirections);

	// Replace the current DNA string
	for (unsigned int i = 0; i < dataString.length()-1; i++)
	{
		if (dataString.substr(i, 1) == randElement)
		{
			dataString.replace(i, 1, mutated);
		}
	}

	// Re-parse the DNA string (causes codons to get updated)
	parse();
	if (MUT_VERBOSE) cout << "* Mutate\tpoint mutated " << randElement << " to " << mutated << endl;
}

void DNA::mutatePoint()
{
	if (DNA_VERBOSE) cout << "* DNA\t\tpoint mutating"<< endl;
	// Pick a random element
	int randElementIndex = random_range(0, dataString.length()-1);
	while (dataString.substr(randElementIndex, 1) == "X")
	{
		randElementIndex = random_range(0, dataString.length()-1);
	}
	string randElement = dataString.substr(randElementIndex, 1);

	// Generate a mutated counterpart
	string mutated;
	if (isCondition(randElement))
	{
		mutated = random_char(dataDNAConditions);
	}
	else if (isAction(randElement))
	{
		mutated = random_char(dataDNAActions);
	}
	else if (isDirection(randElement))
	{
		mutated = random_char(dataDNADirections);
	}
	else if (isOperator(randElement))
	{
		mutated = random_char(dataDNAOperators);
	}

	// Replace the current DNA string
	dataString = dataString.substr(0, randElementIndex) + mutated + dataString.substr(randElementIndex+1, dataString.length() - randElementIndex -1);

	// Re-parse the DNA string (causes codons to get updated)
	parse();
	if (MUT_VERBOSE) cout << "* Mutate\tpoint mutated " << randElement << " to " << mutated << endl;
}

void DNA::mutateChromosome()
{
	if (DNA_VERBOSE) cout << "* DNA\t\tchromosome mutating"<< endl;

	// Pick random mutation
	int randMutation = random_range(1, 5);
	if (amountGenes == 1)
		randMutation = random_range(1, 2);	// With 1 gene, we can only do duplication or fusion

	// Duplication: duplicate a random gene
	if (randMutation == 1)
	{
		// Pick a random gene
		int randGene = random_range(0, amountGenes-1);

		// Fetch it
		Gene tempGene = getGene(randGene);

		// Random location for insertion
		int randLoc = random_range(0, amountGenes-1);

		// Duplicate it
		insertGene(randLoc, tempGene);

		if (MUT_VERBOSE) cout << "* Mutate\tduplication of gene " << randGene << " after place " << randLoc << endl;
	}

	// Fusion: split a gene in two single genes
	else if (true || randMutation == 2)
	{
		// Pick a random gene
		int randGene = random_range(0, amountGenes-1);

		// Fetch it
		Gene tempGene = getGene(randGene);
		string tempOperator = tempGene.getOperatorCodon();

		// Split condition
		string tempConditions;
		for (unsigned int i = 0; i < tempGene.getAmountConditionCodons(); i++)
		{
			tempConditions += tempGene.getConditionCodon(i);
		}
		string tempConditions1 = tempConditions;
		string tempConditions2 = tempConditions;
		if (tempGene.getAmountConditionCodons() > 1)
		{
			if (tempGene.getAmountConditionCodons()%2 != 0)
			{
				tempConditions1 = tempConditions.substr(0, tempGene.getAmountConditionCodons()-1);
				tempConditions2 = tempConditions.substr(tempGene.getAmountConditionCodons()-1, tempGene.getAmountConditionCodons()+1);
			}
			else
			{
				tempConditions1 = tempConditions.substr(0, tempGene.getAmountConditionCodons());
				tempConditions2 = tempConditions.substr(tempGene.getAmountConditionCodons(), tempGene.getAmountConditionCodons());
			}
		}

		// Split actions
		string tempActions;
		for (unsigned int i = 0; i < tempGene.getAmountActionCodons(); i++)
		{
			tempActions += tempGene.getActionCodon(i);
		}
		string tempActions1 = tempActions;
		string tempActions2 = tempActions;
		if (tempGene.getAmountActionCodons() > 1)
		{
			if (tempGene.getAmountActionCodons()%2 != 0)
			{
				tempActions1 = tempActions.substr(0, tempGene.getAmountActionCodons()-1);
				tempActions2 = tempActions.substr(tempGene.getAmountActionCodons()-1, tempGene.getAmountActionCodons()+1);
			}
			else
			{
				tempActions1 = tempActions.substr(0, tempGene.getAmountActionCodons());
				tempActions2 = tempActions.substr(tempGene.getAmountActionCodons(), tempGene.getAmountActionCodons());
			}
		}

		// Overwrite one gene
		tempGene.set(tempOperator + tempConditions1 + tempActions1);
		setGene(randGene, tempGene);

		// And add a new one
		Gene tempGene2(tempOperator + tempConditions2 + tempActions2);
		addGene(tempGene2);

		if (MUT_VERBOSE) cout << "* Mutate\tfusion of gene " << randGene << endl;
	}

	// Inversion: swap two genes
	else if (randMutation == 3)
	{
		// Find two victims
		int randGene1 = random_range(0, amountGenes-1);
		int randGene2 = random_range(0, amountGenes-1);
		while (randGene1 == randGene2)
		{
			randGene2 = random_range(0, amountGenes-1);;
		}

		// Fetch them both
		Gene tempGene1 = getGene(randGene1);
		Gene tempGene2 = getGene(randGene2);

		// Swap!
		setGene(randGene1, tempGene2);
		setGene(randGene2, tempGene1);

		if (MUT_VERBOSE) cout << "* Mutate\tswapped " << randGene1 << " and " << randGene2 << endl;
	}

	// Deletion: delete one gene
	else if (randMutation == 4)
	{
		// Find a victim
		int randGene = random_range(0, amountGenes-1);
		removeGene(randGene);
	}

	// Fission: make one from two genes
	else if (randMutation == 5)
	{
		// Find two victims
		int randGene1 = random_range(0, amountGenes-1);
		int randGene2 = random_range(0, amountGenes-1);
		while (randGene1 == randGene2)
		{
			randGene2 = random_range(0, amountGenes-1);;
		}

		// Fetch them both
		Gene tempGene1 = getGene(randGene1);
		Gene tempGene2 = getGene(randGene2);

		// New containers
		string tempOperator;
		string tempConditions;
		string tempActions;

		// Process gene 1
		for (unsigned int i = 0; i < tempGene1.getAmountConditionCodons(); i++)
		{
			tempConditions += tempGene1.getConditionCodon(i);
		}
		for (unsigned int i = 0; i < tempGene1.getAmountActionCodons(); i++)
		{
			tempActions += tempGene1.getActionCodon(i);
		}

		// Process gene 2
		for (unsigned int i = 0; i < tempGene2.getAmountConditionCodons(); i++)
		{
			tempConditions += tempGene2.getConditionCodon(i);
		}
		for (unsigned int i = 0; i < tempGene2.getAmountActionCodons(); i++)
		{
			tempActions += tempGene2.getActionCodon(i);
		}

		// Pick a operator
		int randOperator = random_range(1, 2);
		if (randOperator == 1)
		{
			tempOperator = tempGene1.getOperatorCodon();
		}
		else
		{
			tempOperator = tempGene2.getOperatorCodon();
		}

		// Create a new gene
		Gene tempGene(tempOperator + tempConditions + tempActions);

		// Save it
		setGene(randGene1, tempGene);
		removeGene(randGene2);
		if (MUT_VERBOSE) cout << "* Mutate\tfission of genes " << randGene1 << " and " << randGene2 << endl;
	}
}

void DNA::mutateGen()
{
	if (DNA_VERBOSE) cout << "* DNA\t\tgen mutating"<< endl;

	// Pick random mutation
	int randMutation = random_range(1, 2);

	// Pick random gene
	int randGene = random_range(0, amountGenes-1);

	// Inversion: switch two codons
	if (randMutation == 1)
	{
		// It makes only sense switching actions, as all conditions
		//  go through the verifier before any action is done

		// Fetch the gene
		Gene tempGene = getGene(randGene);

		if (tempGene.getAmountActionCodons() > 1)
		{
			// Pick two different codons
			int randCodon1 = random_range(0, tempGene.getAmountActionCodons()-1);
			int randCodon2 = random_range(0, tempGene.getAmountActionCodons()-1);
			while (randCodon1 == randCodon2)
			{
				randCodon2 = random_range(0, tempGene.getAmountActionCodons()-1);
			}

			// Switch them
			string tempCodon = tempGene.getActionCodon(randCodon1);
			tempGene.setActionCodon(randCodon1, tempGene.getActionCodon(randCodon2));
			tempGene.setActionCodon(randCodon2, tempCodon);
			if (MUT_VERBOSE) cout << "* Mutate\tswitch of action codons " << randCodon1 << " and " << randCodon2 << " in gene " << randGene << endl;

			// Save the gene
			setGene(randGene, tempGene);
		}
	}

	// Duplication: duplicate a codon
	if (randMutation == 2)
	{
		// Pick condition, action, or both
		int randMethod = random_range(1, 3);

		// Fetch the gene
		Gene tempGene = getGene(randGene);

		// Condition codon
		if ((randMethod == 1) || (randMethod == 2))
		{
			// Pick a codon
			int randCodon = random_range(0, tempGene.getAmountConditionCodons()-1);

			// Pick a random location
			int randLocation = random_range(0, tempGene.getAmountConditionCodons()-1);

			// Insert the duplicated codon
			tempGene.insertConditionCodon(randLocation, tempGene.getConditionCodon(randCodon));
			if (MUT_VERBOSE) cout << "* Mutate\tduplication of condition codon " << randCodon << " after " << randLocation << " in gene " << randGene << endl;
		}

		// Action codon
		if ((randMethod == 3) || (randMethod == 2))
		{
			// Pick a codon
			int randCodon = random_range(0, tempGene.getAmountActionCodons()-1);

			// Pick a random location
			int randLocation = random_range(0, tempGene.getAmountActionCodons()-1);

			// Insert the duplicated codon
			tempGene.insertActionCodon(randLocation, tempGene.getActionCodon(randCodon));
			if (MUT_VERBOSE) cout << "* Mutate\tduplication of action codon " << randCodon << " after " << randLocation << " in gene " << randGene << endl;

		}

		// Save the gene
		setGene(randGene, tempGene);
	}
}
