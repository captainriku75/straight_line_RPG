/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGTextGenerator.h

	This class is a custom text generator for the EmptyGame
	application. Basically, by defining the initText and updateText
	methods, we can have it display whatever text we like.
*/

#pragma once
#include "StraightLineRPG_VS\stdafx.h"
#include "sssf\text\TextGenerator.h"

class StraightLineRPGTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring textHealth;
	wstring textMP;
	wstring textStr;
	wstring textDefense;

public:
	// NOTHING TO INITIALIZE OR DESTROY
	StraightLineRPGTextGenerator()	{}
	~StraightLineRPGTextGenerator()	{}

	// DEFINED IN StraightLineRPGTextGenerator.cpp
	void appendPlayerHealth(Game *game);
	void appendPlayerMP(Game *game);
	void appendPlayerStr(Game *game);
	void appendPlayerDef(Game *game);
	void initText(Game *game);
	void updateText(Game *game);
};