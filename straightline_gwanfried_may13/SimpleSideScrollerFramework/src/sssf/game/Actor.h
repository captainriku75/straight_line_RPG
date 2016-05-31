#pragma once

enum ActionType
{
	DAMAGE,
	DEFEND,
	HEAL,
	FOCUS,
	TAUNT
};

struct Action
{
	wstring name;
	int magicCost;
	ActionType theType;
	int data;
};

class Actor
{
private:
	wstring name;
	int maxHealth;
	int currentHealth;
	int maxMP;
	int currentMP;

	int currentDefense;
	int baseStr;

	int numActionsPerformed;
	vector<Action> behaviorList;

public:
	Actor(wstring aName, int aMaxHealth, int startingHealth, int aMaxMana, int startingMana, int startingDef, int startStr);
	~Actor(void);

	wstring getName();
	int getMaxHealth();
	int getCurrentHealth();
	int getMaxMP();
	int getCurrentMP();
	int getCurrentDefense();
	int getBaseStr();

	bool isDead();
	void setHealth(int newHealth);
	void setDefense(int newDefense);
	void setMP(int newMP);

	void pushAction(Action theAction);
	Action actionAt(int index);
	Action pickAction();
};