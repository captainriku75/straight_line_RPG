/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGTextGenerator.cpp

	See StraightLineRPGTextGenerator.h for a class description.
*/

#include "StraightLineRPG_VS\stdafx.h"
#include <string>
#include "StraightLineRPG\StraightLineRPG.h"
#include "StraightLineRPG\StraightLineRPGTextGenerator.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\text\GameText.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

static const int		W_TEXT_X = 100;
static const int		W_TEXT_Y = 500;

static const int		W_TEXT_P_MP_X = 100;
static const int		W_TEXT_P_MP_Y = 570;

static const int		W_TEXT_P_STR_X = 100;
static const int		W_TEXT_P_STR_Y = 640;

static const int		W_TEXT_P_DEF_X = 100;
static const int		W_TEXT_P_DEF_Y = 710;

void StraightLineRPGTextGenerator::appendPlayerHealth(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	int health=game->getBM()->getFirst()->getCurrentHealth();
	wstring healthString=to_wstring(health);

	int maxHealth = game->getBM()->getFirst()->getMaxHealth();
	wstring maxHealthString=to_wstring(maxHealth);

	wss << healthString;
	wss << L"/";
	wss << maxHealthString;

	textHealth.append(wss.str());
}

void StraightLineRPGTextGenerator::appendPlayerMP(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	int mana=game->getBM()->getFirst()->getCurrentMP();
	wstring mpString=to_wstring(mana);

	int maxMana = game->getBM()->getFirst()->getMaxMP();
	wstring maxMPString=to_wstring(maxMana);

	wss << mpString;
	wss << L"/";
	wss << maxMPString;

	textMP.append(wss.str());
}

void StraightLineRPGTextGenerator::appendPlayerStr(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	int str=game->getBM()->getFirst()->getBaseStr();
	wstring strString=to_wstring(str);
	textStr.append(strString);
}

void StraightLineRPGTextGenerator::appendPlayerDef(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	int def=game->getBM()->getFirst()->getCurrentDefense();
	wstring defString=to_wstring(def);
	textDefense.append(defString);
}

/*
	initText - Provides an example of how to render text to our screen every frame,
	allowing for the text to continually change. This method adds a text object
	to the text manager (GameText), which allows it to be updated each frame. This
	method should be called only once for each object.
*/
void StraightLineRPGTextGenerator::initText(Game *game)
{
	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	appendPlayerHealth(game);

	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&textHealth, W_TEXT_X, W_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());

	appendPlayerMP(game);
	text->addText(&textMP, W_TEXT_P_MP_X, W_TEXT_P_MP_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());

	appendPlayerStr(game);
	text->addText(&textStr, W_TEXT_P_STR_X, W_TEXT_P_STR_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());

	appendPlayerDef(game);
	text->addText(&textDefense, W_TEXT_P_DEF_X, W_TEXT_P_DEF_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
}

/*
	updateText - updates the text. Note that the text must already have been added
	to the text manager via the initText method. Also not that there should be a
	separate TextGenerator for each line of text we wish to render.
*/
void StraightLineRPGTextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	textHealth.clear();
	textMP.clear();
	textStr.clear();
	textDefense.clear();

	appendPlayerHealth(game);
	appendPlayerMP(game);
	appendPlayerStr(game);
	appendPlayerDef(game);
}