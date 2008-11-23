// Include guard
#ifndef __CONSTANTS
#define __CONSTANTS

// Headers
#include <map>
#include <string>

//
// World
//

// Block ID's
extern int WORLD_INVALID;
extern int WORLD_FREE;
extern int WORLD_OBSTACLE;
extern int WORLD_CREATURE;

// Creature statusses
extern int CREATURE_ALIVE;
extern int CREATURE_FINISHED;
extern int CREATURE_DEAD;


//////////////
// DNA code //
//////////////

//
// Array template
//

template <typename T, int N> char (&array(T(&)[N]))[N];


//
// Unique level 1 (breaks EVERYTHING)
//
// TODO: leesbaar? new if free right do move right if blocked right free down move down end
// Structural code
extern std::pair<std::string, char> initSeparators[];
extern std::map<std::string, char> dnaSeparators;

//
// Unique level 2 (copmmand identifiers)
//

// Condition identifiers
extern std::pair<std::string, char> initConditions[];
extern std::map<std::string, char> dnaConditions;

// Action identifiers
extern std::pair<std::string, char> initActions[];
extern std::map<std::string, char> dnaActions;

//
// Not unique (command parameters)
//

// Directional parameters
extern std::pair<std::string, char> initDirections[];
extern std::map<std::string, char> dnaParameters;

// Condition parameters
extern std::pair<std::string, char> initParameters[];
extern std::map<std::string, char> dnaParameters;




#endif
