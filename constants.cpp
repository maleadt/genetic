/*
 * constants.h
 * All sort of constants.
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
#include "constants.h"

//
// World
//

// Block ID's
int WORLD_INVALID = -1;
int WORLD_FREE = 0;
int WORLD_OBSTACLE = 1;
int WORLD_CREATURE = 2;

// Creature statusses
int CREATURE_ALIVE = 1;
int CREATURE_FINISHED = 2;
int CREATURE_DEAD = 3;


//
// Unique level 1 (breaks EVERYTHING)
//
// TODO: leesbaar? new if free right do move right if blocked right free down move down end
// Structural code
std::pair<std::string, char> initSeparators[] =
{
	std::pair<std::string, char> ("gen", 'X'),
	std::pair<std::string, char> ("command", 'Y')
};
std::map<std::string, char> dnaSeparators(initSeparators, initSeparators + sizeof array(initSeparators));

//
// Unique level 2 (command identifiers)
//

// Condition identifiers
std::pair<std::string, char> initConditions[] =
{
	std::pair<std::string, char> ("if", '1'),
	std::pair<std::string, char> ("if not", '2'),
	std::pair<std::string, char> ("while", '3'),
	std::pair<std::string, char> ("while not", '4')
};
std::map<std::string, char> dnaConditions(initConditions, initConditions + sizeof array(initConditions));

// Action identifiers
std::pair<std::string, char> initActions[] =
{
	std::pair<std::string, char> ("move", 'A')
};
std::map<std::string, char> dnaActions(initActions, initActions + sizeof array(initActions));

//
// Not unique (command parameters)
//

// Condition parameters
std::pair<std::string, char> initParameters[] =
{
	std::pair<std::string, char> ("up", '1'),
	std::pair<std::string, char> ("right", '2'),
	std::pair<std::string, char> ("down", '3'),
	std::pair<std::string, char> ("left", '4'),
	std::pair<std::string, char> ("is free", 'A'),
	std::pair<std::string, char> ("is blocked", 'B')
};
std::map<std::string, char> dnaParameters(initParameters, initParameters + sizeof array(initParameters));
