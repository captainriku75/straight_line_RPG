#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"

static const wstring	W_ZERO_STATE = L"ZERO";
static const wstring	W_ONE_STATE = L"ONE";
static const wstring	W_TWO_STATE = L"TWO";
static const wstring	W_THREE_STATE = L"THREE";
static const wstring	W_FOUR_STATE = L"FOUR";
static const wstring	W_FIVE_STATE = L"FIVE";
static const wstring	W_SIX_STATE = L"SIX";
static const wstring	W_SEVEN_STATE = L"SEVEN";
static const wstring	W_EIGHT_STATE = L"EIGHT";
static const wstring	W_NINE_STATE = L"NINE";
static const wstring	W_TEN_STATE = L"TEN";

class HealthBar : public AnimatedSprite
{
private:
	static wstring stateNames[11];

	AnimatedSprite* mySprite;

public:
	HealthBar(AnimatedSprite* aSprite)	{
		mySprite = aSprite;
		stateNames[0] = W_ZERO_STATE;
		stateNames[1] = W_ONE_STATE;
		stateNames[2] = W_TWO_STATE;
		stateNames[3] = W_THREE_STATE;
		stateNames[4] = W_FOUR_STATE;
		stateNames[5] = W_FIVE_STATE;
		stateNames[6] = W_SIX_STATE;
		stateNames[7] = W_SEVEN_STATE;
		stateNames[8] = W_EIGHT_STATE;
		stateNames[9] = W_NINE_STATE;
		stateNames[10] = W_TEN_STATE;
	}

	~HealthBar()	{}

	AnimatedSprite* getAssignedSprite() {return mySprite;}
	void setAnimatedSprite(AnimatedSprite* theSprite) {mySprite = theSprite;}

	wstring getStateName();
};