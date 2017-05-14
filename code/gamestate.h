/*	gamestate.h
 *
 *	Created By: Nick Kane
 */

#include "gamememory.h"


struct GameState
{
	unsigned int StartMS;
	// TODO(nick): CycleEndMS for debugging
	unsigned int CycleEndMS;
	unsigned int CurrentMS;
	unsigned int DeltaMS;
	// TODO(nick): do better research for how to create a better
	// engine api / memory allocation
	GameMemory *Memory;
};

