#include "sssf_VS\stdafx.h"

#include "sssf\game\Actor.h"

Actor::Actor(wstring aName, int aMaxHealth, int startingHealth, int aMaxMana, int startingMana, int startingDef, int startStr)
{
	name = aName;
	maxHealth = aMaxHealth;
	currentHealth = startingHealth;
	maxMP = aMaxMana;
	currentMP = startingMana;
	currentDefense = startingDef;
	baseStr= startStr;

	numActionsPerformed = 0;
}

Actor::~Actor(void)
{
	//nothing to do right now
}

//getters
wstring Actor::getName()
{
	return name;
}

int Actor::getMaxHealth()
{
	return maxHealth;
}

int Actor::getCurrentHealth()
{
	return currentHealth;
}

int Actor::getMaxMP()
{
	return maxMP;
}

int Actor::getCurrentMP()
{
	return currentMP;
}

int Actor::getCurrentDefense()
{
	return currentDefense;
}

int Actor::getBaseStr()
{
	return baseStr;
}

bool Actor::isDead()
{
	return (currentHealth <= 0);
}

//setters
void Actor::setHealth(int newHealth)
{
	currentHealth = newHealth;
}

void Actor::setDefense(int newDefense)
{
	currentDefense = newDefense;
}

void Actor::setMP(int newMP)
{
	currentMP = newMP;
}

//Add an action to the list.
void Actor::pushAction(Action theAction)
{
	behaviorList.push_back(theAction);
}

Action Actor::actionAt(int index)
{
	return behaviorList.at(index);
}

//Pick next action.
Action Actor::pickAction()
{
	//Get next action from the list.
	
	Action nextAction = behaviorList.at(numActionsPerformed);

	//Increment number of actions performed (so next time, the next action in list is picked).
	numActionsPerformed++;
	if(numActionsPerformed == behaviorList.size())
	{
		numActionsPerformed = 0;
	}

	return nextAction;
}