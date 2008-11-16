#include "constants.h"

//
// World
//

// Block ID's
int WORLD_INVALID = -1;
int WORLD_FREE = 0;
int WORLD_OBSTACLE = 1;
int WORLD_CREATURE = 2;


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
