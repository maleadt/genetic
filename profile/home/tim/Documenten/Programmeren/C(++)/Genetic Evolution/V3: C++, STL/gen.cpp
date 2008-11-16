               :/*
               : * gen.cpp
               : * Gen class.
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
               :#include "world.h"
               :#include "creature.h"
               :#include "dna.h"
               :#include "gen.h"
               :
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
               :Gen::Gen()
               :{
               :}
               :
               :Gen::Gen(Gen const& oldGen)
               :{
               :	// Copy all commands
               :	std::list<Command*>::const_iterator it = oldGen.dataCommands.begin();
               :	while (it != oldGen.dataCommands.end())
               :	{
               :		// Copy all commands and save them
               :		dataCommands.push_back( new Command( (*(*it)) ) );
               :
               :		++it;
               :	}
               :
               :	// Copy other data
               :	parent = oldGen.parent;
               :	dataRepeat = oldGen.dataRepeat;
               :}
               :
               :Gen::~Gen()
               :{
               :	// Remove all commands
               :	std::list<Command*>::iterator it_com = dataCommands.begin();
               :	while (it_com != dataCommands.end())
               :	{
               :		delete (*it_com);
               :		++it_com;
               :	}
               :	dataCommands.clear();
               :}
               :
               ://
               :// Input & output
               ://
               :
               :// Input textual data
     1 1.7e-04 :void Gen::inputTextual(std::string inputText) /* Gen::inputTextual(std::string) total:      3 5.2e-04 */
               :{
               :	// Detect genes, and create a new object for them
               :	unsigned int index_start = 0;
               :	while (index_start < inputText.length())
               :	{
               :		unsigned int index_end = index_start+1;
               :		while (index_end < inputText.length() && inputText[index_end] != dnaSeparators["command"])
               :			index_end++;
               :
               :		Command* outputCommand = new Command;
     2 3.4e-04 :		outputCommand->inputTextual(inputText.substr(index_start, index_end - index_start));
               :		dataCommands.push_back(outputCommand);
               :
               :		index_start = index_end+1;
               :	}
               :}
               :
               :// Output textual data
               :std::string Gen::outputTextual() /* Gen::outputTextual() total:      2 3.4e-04 */
               :{
               :	std::string outputTextual;
               :
               :	// Add all commands
               :	std::list<Command*>::iterator it = dataCommands.begin();
               :	while (it != dataCommands.end())
               :	{
     1 1.7e-04 :		if (it != dataCommands.begin())
     1 1.7e-04 :			outputTextual += dnaSeparators["command"];
               :		outputTextual += (*it)->outputTextual();
               :		++it;
               :	}
               :
               :	return outputTextual;
               :}
               :
               :// Output code
               :std::string Gen::outputCode() /* Gen::outputCode() total:      1 1.7e-04 */
               :{
               :	std::string outputCode;
               :
               :	// Add all conditions
               :	std::list<Command*>::iterator it = dataCommands.begin();
               :	int amountconditions = 0;
               :	while (it != dataCommands.end())
               :	{
               :		if ((*it)->is_condition)
               :		{
               :			if (amountconditions > 0)
               :				outputCode += "and ";
               :
               :			outputCode += (*it)->outputCode();
               :			outputCode += "\n";
               :			amountconditions++;
               :		}
               :		++it;
               :	}
               :
               :	// Add all actions
               :	it = dataCommands.begin();
               :	while (it != dataCommands.end())
               :	{
               :		if ((*it)->is_action)
               :		{
     1 1.7e-04 :			outputCode += (*it)->outputCode();
               :			outputCode += "\n";
               :			amountconditions++;
               :		}
               :		++it;
               :	}
               :
               :	return outputCode;
               :}
               :
               :
               ://
               :// Code execution helpers
               ://
               :
   727  0.1253 :void Gen::DeltaFromDirection(char inputDir, int &dx, int &dy) /* Gen::DeltaFromDirection(char, int&, int&) total:  12449  2.1450 */
               :{
  3512  0.6051 :	if (inputDir == dnaParameters["up"])
               :	{
   203  0.0350 :		dx = 0;
    43  0.0074 :		dy = -1;
               :	}
  3519  0.6063 :	else if (inputDir == dnaParameters["right"])
               :	{
   574  0.0989 :		dx = 1;
   220  0.0379 :		dy = 0;
               :	}
  2094  0.3608 :	else if (inputDir == dnaParameters["down"])
               :	{
   159  0.0274 :		dx = 0;
    98  0.0169 :		dy = 1;
               :	}
   757  0.1304 :	else if (inputDir == dnaParameters["left"])
               :	{
    26  0.0045 :		dx = -1;
    41  0.0071 :		dy = 0;
               :	}
               :
               :	else
               :	{
               :		dx = 0;
               :		dy = 0;
               :	}
   476  0.0820 :}
               :
               ://
               :// Code execution
               ://
               :
   118  0.0203 :void Gen::execute() /* Gen::execute() total:   3584  0.6175 */
               :{
    63  0.0109 :	dataRepeat = true;
   125  0.0215 :	bool do_loop = true;
   425  0.0732 :	while (do_loop)
               :	{
   692  0.1192 :		int a = parent->parent->credits;
   358  0.0617 :		int b = parent->parent->pointerWorld->creatureMaxCommands;
   386  0.0665 :		if (a <= b)
               :		{
   180  0.0310 :			if (dataRepeat)
               :			{
   517  0.0891 :				if (evaluateAll())
               :				{
   271  0.0467 :					actAll();
               :				} else {
   186  0.0320 :					do_loop = false;
               :				}
               :			} else {
   130  0.0224 :				do_loop = false;
               :			}
               :		} else {
     3 5.2e-04 :			do_loop = false;
               :		}
               :	}/*
               :
               :	while (parent->parent->credits <= parent->parent->pointerWorld->creatureMaxCommands && dataRepeat && evaluateAll())
               :	{
               :		actAll();
               :	}*/
   130  0.0224 :}
               :
   138  0.0238 :bool Gen::evaluateAll() /* Gen::evaluateAll() total:   8147  1.4038 */
               :{
               :	// Essential stuff (an overhead credit)
   279  0.0481 :	parent->parent->credits++;
               :
   462  0.0796 :	std::list<Command*>::iterator it = dataCommands.begin();
  1943  0.3348 :	while (it != dataCommands.end())
               :	{
  1501  0.2586 :		if ((*it)->is_condition)
               :		{
  2391  0.4120 :			if (!evaluate(*it) || parent->parent->credits > parent->parent->pointerWorld->creatureMaxCommands)
    91  0.0157 :				return false;
               :		}
   460  0.0793 :		++it;
               :	}
   546  0.0941 :	return true;
   336  0.0579 :}
               :
   373  0.0643 :bool Gen::evaluate(Command* inputCommand) /* Gen::evaluate(Command*) total:  26088  4.4951 */
               :{
               :	// Do we have a repeating check (while / while not)?
               :	dataRepeat = (	inputCommand->identifier == dnaConditions["while"]		||
  6411  1.1047 :				   inputCommand->identifier == dnaConditions["while not"]	);
               :
               :	// Do we have an inverse conditional?
               :	bool tempInvert = (	inputCommand->identifier == dnaConditions["if not"]		||
  6373  1.0981 :						inputCommand->identifier == dnaConditions["while not"]	);
               :
               :	// Essential stuff
   231  0.0398 :	bool outputBool = false;
   795  0.1370 :	parent->parent->credits++;
               :
               :	// We need at least 2 parameter (for now)
  1036  0.1785 :	if (inputCommand->parameters.size() < 2)
               :		return false;
               :
               :	// An "is free" check
  3439  0.5926 :	if (inputCommand->parameters[0] == dnaParameters["is free"])
               :	{
               :		// Get the requested location
   834  0.1437 :		int x = parent->parent->locationX;
   186  0.0320 :		int y = parent->parent->locationY;
               :		int dx, dy;
   768  0.1323 :		DeltaFromDirection(inputCommand->parameters[1], dx, dy);
  1360  0.2343 :		outputBool = (parent->parent->pointerWorld->checkSpot(x+dx, y+dy) == WORLD_FREE);
               :	}
               :
               :	// An "is blocked" check
  1628  0.2805 :	else if (inputCommand->parameters[0] == dnaParameters["is blocked"])
               :	{
               :		// Get the requested location
   220  0.0379 :		int x = parent->parent->locationX;
   112  0.0193 :		int y = parent->parent->locationY;
               :		int dx, dy;
   368  0.0634 :		DeltaFromDirection(inputCommand->parameters[1], dx, dy);
   931  0.1604 :		outputBool = !(parent->parent->pointerWorld->checkSpot(x+dx, y+dy) == WORLD_FREE);
               :	}
               :
               :	// Now invert checks if needed
   153  0.0264 :	if (tempInvert)
     1 1.7e-04 :		outputBool = !outputBool;
               :
   420  0.0724 :	return outputBool;
   449  0.0774 :}
               :
    98  0.0169 :void Gen::actAll() /* Gen::actAll() total:   4647  0.8007 */
               :{
               :	// Essential stuff (an overhead credit)
   314  0.0541 :	parent->parent->credits++;
               :
   352  0.0607 :	std::list<Command*>::iterator it = dataCommands.begin();
  2357  0.4061 :	while (it != dataCommands.end() && parent->parent->credits <= parent->parent->pointerWorld->creatureMaxCommands)
               :	{
   577  0.0994 :		if ((*it)->is_action)
               :		{
   539  0.0929 :			act(*it);
               :		}
   218  0.0376 :		++it;
               :	}
   192  0.0331 :}
               :
   101  0.0174 :void Gen::act(Command* inputCommand) /* Gen::act(Command*) total:   2654  0.4573 */
               :{
               :	// Essential stuff
   106  0.0183 :	parent->parent->credits++;
               :
               :	// We need at least 1 parameter
   313  0.0539 :	if (inputCommand->parameters.size() < 1)
               :		return;
               :
               :	// The "move" action
  1346  0.2319 :	if (inputCommand->identifier == dnaActions["move"])
               :	{
               :		int dx, dy;
   337  0.0581 :		DeltaFromDirection(inputCommand->parameters[0], dx, dy);
   324  0.0558 :		parent->parent->move(dx, dy);
               :	}
   127  0.0219 :}
               :
               :
               ://
               :// Code mutation
               ://
               :
               :void Gen::mutate(unsigned int mutLevel)
               :{
               :	if (mutLevel == 0)
               :	{
               :		mutateSelf();
               :	}
               :	else
               :	{
               :		std::list<Command*>::iterator it = dataCommands.begin();
               :		for (int i = 0; i < random_range(0, dataCommands.size()-1); i++)
               :			++it;
               :		(*it)->mutate(mutLevel-1);
               :	}
               :}
               :
               :void Gen::mutateSelf() /* Gen::mutateSelf() total:      3 5.2e-04 */
               :{
               :	// We need at least 1 command
               :	if (dataCommands.size() < 1)
               :		return;
               :
               :	// Get some random stuff we'll need
               :	int randomMutation = random_range(1, 5);
               :	int randomCommand = random_range(0, dataCommands.size()-1);
               :	int randomCommand2 = randomCommand;
               :	if (dataCommands.size() > 1)
               :	{
               :		while (randomCommand == randomCommand2)
               :			randomCommand2 = random_range(0, dataCommands.size()-1);
               :	}
     1 1.7e-04 :	std::list<Command*>::iterator iteratorCommand = dataCommands.begin();
               :	for (int i = 0; i < randomCommand; i++)
               :		++iteratorCommand;
     1 1.7e-04 :	std::list<Command*>::iterator iteratorCommand2 = dataCommands.begin();
               :	for (int i = 0; i < randomCommand2; i++)
               :		++iteratorCommand2;
               :
               :	// Mutate
               :	switch (randomMutation)
               :	{
               :		// Deletion
               :		case 1:
               :			delete (*iteratorCommand);
               :			dataCommands.erase(iteratorCommand);
               :			break;
               :
               :		// Duplication
               :		case 2:
               :			dataCommands.push_back( new Command(*(*iteratorCommand)) );
               :			break;
               :
               :		// Inversion
               :		case 3:
               :			if (dataCommands.size() > 1)
               :			{
               :				Command* temp = *iteratorCommand;
               :				*iteratorCommand = *iteratorCommand2;
               :				*iteratorCommand2 = temp;
               :			}
               :			break;
               :
               :		// Amplification
               :		case 4:
               :			for (int i = 0; i < random_range(1, dataCommands.size()); i++)
     1 1.7e-04 :				dataCommands.insert(iteratorCommand, new Command(*(*iteratorCommand)));;
               :			break;
               :
               :		// Translocation
               :		case 5:
               :			if (dataCommands.size() > 1)
               :			{
               :				Command* tempCommand = *iteratorCommand;
               :				dataCommands.erase(iteratorCommand);
               :				dataCommands.insert(iteratorCommand2, tempCommand);
               :			}
               :	}
               :}
               :
               :// Check if valid
               :bool Gen::is_valid() /* Gen::is_valid() total:      1 1.7e-04 */
               :{
               :	// Add all commands
               :	std::list<Command*>::iterator it = dataCommands.begin();
               :	while (it != dataCommands.end())
               :	{
     1 1.7e-04 :		if (! ((*it)->is_valid()))
               :			return false;
               :		++it;
               :	}
               :
               :	return true;
     1 1.7e-04 :} /* __static_initialization_and_destruction_0(int, int) total:      1 1.7e-04 */
/* 
 * Total samples for file : "/home/tim/Documenten/Programmeren/C(++)/Genetic Evolution/V3: C++, STL/gen.cpp"
 * 
 *  57580  9.9214
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
