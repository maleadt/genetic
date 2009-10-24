/*
 * generic.cpp
 * Generic routines.
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

#ifndef __GENERIC /* include guard */
#define __GENERIC




/*
 Configuration
 */

// Needed headers
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;


/*
 * Random generators
 */

// Will contain lowest and highest
int random_int(int lowest_number, int highest_number);
string random_char(string inputString);



/*
 * DNA data structure
 */

//  Main operators
//    A = if
//    B = if not
//    C = while
//    D = while not (aka unless)
//    E =
//    F =
const string dataDNAOperators = "AC";

//  Conditions
//    K = if free
//    L = if blocked
//    M =
//    N =
const string dataDNAConditions = "KL";

//  Actions
//    G = move
//    H =
//    I =
//    J =
const string dataDNAActions = "G";

//  Directions
//    O = right
//    P = left
//    Q = up
//    R = down
//    S = right up
//    T = right down
//    U = left up
//    V = left down
//    W = most efficient
const string dataDNADirections = "OPQRSTUVW";

bool isOperator(string inputChar);
bool isCondition(string inputChar);
bool isAction(string inputChar);
bool isDirection(string inputChar);



/*
 * Direction parameter parsing
 */

int *directionDelta (string inputDirection);
int *directionEff (int creatureRow, int creatureColumn, int targetRow, int targetColumn);

#endif /* __GENERIC */
