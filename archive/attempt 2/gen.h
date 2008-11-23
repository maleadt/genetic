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

#ifndef __GENE /* include guard */
#define __GENE


/*
 * Configuration
 */

// Needed headers
#include <iostream>
#include "generic.h"


/*
 * Class defenition
 */

class Gene
{
	// Public member data, can be viewed from anywhere
	public:
		// Construction
		Gene();
		Gene(string);
		//~Gene();

		// Basic IO
		void set(string);
		string get();

		// Condition codon modification
		void setConditionCodon(unsigned int, string);
		string getConditionCodon(unsigned int);
		unsigned int getAmountConditionCodons();
		void addConditionCodon(string);
		void removeConditionCodon(unsigned int);
		void insertConditionCodon(unsigned int, string);

		// Action codon modification
		void setActionCodon(unsigned int, string);
		string getActionCodon(unsigned int);
		unsigned int getAmountActionCodons();
		void addActionCodon(string);
		void removeActionCodon(unsigned int);
		void insertActionCodon(unsigned int, string);

		// Operator codon modification
		string getOperatorCodon();

		// Main routine
		void parse();
		bool valid;

	// Private member data, only for internal usage
	private:
		void rebuild();
		string dataGene;
		string dataOperatorCodon;
		string *dataConditionCodons;
		string *dataActionCodons;
		unsigned int amountConditionCodons;
		unsigned int amountActionCodons;
		bool haveGene;
};



#endif /* __GENE */
