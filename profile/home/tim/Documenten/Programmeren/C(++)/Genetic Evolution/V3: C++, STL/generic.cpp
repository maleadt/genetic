               :/*
               : * generic.cpp
               : * Some generic all-around functions.
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
               :// Global variables
               :bool GENERIC_SRAND = false;
               :
               :
               :// Headers
               :#include "generic.h"
               :
               :
               ://////////////
               :// ROUTINES //
               ://////////////
               :
               :// Swap two items through references
               :template <class X> void swap(X &a, X &b)
               :{
               :	X temp = a;
               :	a = b;
               :	b = temp;
               :}
               :
               :// Generate a number from lower up to (and with) upper
    55  0.0095 :int random_range(int lowest_number, int highest_number) /* random_range(int, int) total:   1617  0.2786 */
               :{
               :	// Set seed
    39  0.0067 :	if (!GENERIC_SRAND)
               :	{
               :		//srand((unsigned)time(0));
               :		srand(10);
               :		GENERIC_SRAND = true;
               :	}
               :
               :	// Swap the numbers if needed
    15  0.0026 :	if (lowest_number > highest_number)
               :	{
               :		swap(lowest_number, highest_number);
               :	}
               :
               :	// Calculate the range
    42  0.0072 :	int range = highest_number - lowest_number + 1;
               :
               :	// Return a number (using lower-order bits)
               :	//return lowest_number + rand()%range;
               :
               :	// Return a number (using high-order bits)
               :	// Numerical Recipes in C: The Art of Scientific Computing
               :	//   (William H. Press, Brian P. Flannery, Saul A. Teukolsky,
               :	//   William T. Vetterling; New York: Cambridge University Press,
               :	//   1992 (2nd ed., p. 277))
  1365  0.2352 :	return lowest_number + range*(rand()/(RAND_MAX+1.0));
   101  0.0174 :}
               :
               :
/* 
 * Total samples for file : "/home/tim/Documenten/Programmeren/C(++)/Genetic Evolution/V3: C++, STL/generic.cpp"
 * 
 *   1617  0.2786
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
