/*	gamestate.h
 *
 *	Created By: Nick Kane
 */

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
	bool IsPlaying;
};

