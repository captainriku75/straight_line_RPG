#include "sssf_VS\stdafx.h"
#include "BattleManager.h"

#include "Actor.h"

BattleManager::BattleManager(void)
{
	numActorsRegistered = 0;
	whoseTurn = 1;

	firstActorExtraDefense = 0;
	secondActorExtraDefense = 0;
}

BattleManager::~BattleManager(void)
{

}

void BattleManager::registerActor(Actor *theActor)
{
	if(numActorsRegistered == 0)
	{
		first = theActor;
	}
	else
	{
		second = theActor;
	}
	numActorsRegistered++;
}

void BattleManager::deregisterActors()
{
	if(numActorsRegistered > 0)
	{
		delete first;
		delete second;
	}

	numActorsRegistered = 0;
}

//Set it to be actor 1's turn again.
void BattleManager::resetTurns()
{
	whoseTurn = 1;
}

void BattleManager::implementAction(Action theAction)
{
	Actor *movingActor;
	Actor *target;
	if(whoseTurn == 1)
	{
		movingActor = first;
		target = second;

		if(firstActorExtraDefense > 0)
		{
			//Defending wears off.
			first->setDefense(first->getCurrentDefense() - firstActorExtraDefense);
			firstActorExtraDefense = 0;
		}
	}
	else
	{
		movingActor = second;
		target = first;

		if(secondActorExtraDefense > 0)
		{
			//Defending wears off.
			second->setDefense(second->getCurrentDefense() - secondActorExtraDefense);
			secondActorExtraDefense = 0;
		}
	}

	//Only permit action if there's enough MP to do the action - otherwise do nothing.
	if(movingActor->getCurrentMP() >= theAction.magicCost)
	{
		movingActor->setMP(movingActor->getCurrentMP() - theAction.magicCost);

		if(theAction.theType == DAMAGE)
		{
			int damageAmount = theAction.data - target->getCurrentDefense();

			if(damageAmount > 0)
			{
				target->setHealth(target->getCurrentHealth() - damageAmount);
			}
		}
		else if(theAction.theType == DEFEND)
		{
			int oldDefense = movingActor->getCurrentDefense();
			movingActor->setDefense(oldDefense + theAction.data);

			if(whoseTurn == 1)
			{
				firstActorExtraDefense = theAction.data;
			}
			else
			{
				secondActorExtraDefense = theAction.data;
			}
		}
		else if(theAction.theType == HEAL)
		{
			//Cap healing at max HP.
			movingActor->setHealth(movingActor->getCurrentHealth() + theAction.data);

			if(movingActor->getCurrentHealth() > movingActor->getMaxHealth())
			{
				movingActor->setHealth(movingActor->getMaxHealth());
			}
		}
		else
		{
			//other things, like charging mana, taunting, etc.
		}
	}
	else
	{
		//some sort of "out of magic" error
	}


	//Finished implementing action: change whose turn it is.
	if(whoseTurn == 1)
	{
		whoseTurn = 2;
	}
	else
	{
		whoseTurn = 1;
	}
}