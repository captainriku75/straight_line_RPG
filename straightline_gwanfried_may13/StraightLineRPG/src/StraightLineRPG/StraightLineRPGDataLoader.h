/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGDataLoader.h

	This class provides a custom importer for the StraightLineRPGGame to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "StraightLineRPG_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"
#include <Box2D\Box2D.h>

static const wstring LEVEL_1_TITLE = L"Forest";
static const wstring LEVEL_2_TITLE = L"Mountains";

class StraightLineRPGDataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;

	//store screen width and height for gui setup
	int screenWidth;
	int screenHeight;
	
	bool textGeneratorInitialized;
public:
	bool textFlag;
	StraightLineRPGDataLoader()	{textGeneratorInitialized = false; textFlag=false;}
	~StraightLineRPGDataLoader()	{}

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}

	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN StraightLineRPGDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);

	// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
	void hardCodedLoadGUIExample(Game *game);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics,	map<wstring,wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring,wstring> *properties);

	void makeStatueBot(Game *game, AnimatedSpriteType *StatueBotType, float initX, float initY);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initAboutScreen(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initGameControlsScreen(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initVictoryScreen(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initDeathScreen(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initBattleScreenGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
};