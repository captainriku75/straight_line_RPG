#pragma once

#include "sssf_VS\stdafx.h"
#include "Actor.h"

class BattleManager
{
private:
	unsigned int whoseTurn;

	//For now, the system will only support 1-on-1 battles, for simplicity.
	unsigned int numActorsRegistered;
	Actor *first;
	Actor *second;

	unsigned int firstActorExtraDefense;
	unsigned int secondActorExtraDefense;

public:
	BattleManager(void);
	~BattleManager(void);

	Actor *getFirst() {return first;}
	Actor *getSecond() {return second;}

	void resetTurns();

	void registerActor(Actor *theActor);
	void deregisterActors();
	void implementAction(Action theAction);
};

