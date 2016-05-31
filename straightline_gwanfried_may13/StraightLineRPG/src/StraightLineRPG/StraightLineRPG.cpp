/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGGame.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "StraightLineRPG_VS\stdafx.h"

// StraightLineRPG GAME INCLUDES
#include "StraightLineRPG\StraightLineRPGButtonEventHandler.h"
#include "StraightLineRPG\StraightLineRPGDataLoader.h"
#include "StraightLineRPG\StraightLineRPG.h"
#include "StraightLineRPG\StraightLineRPGKeyEventHandler.h"
#include "StraightLineRPG\StraightLineRPGTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/

struct Nums
{
	int num1;
	int num2;
};
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *StraightLineRPGGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	StraightLineRPGDataLoader *loader = new StraightLineRPGDataLoader();
	loader->initWinHandle(hInstance, nCmdShow);
	StraightLineRPGGame->setDataLoader(loader);
	loader->loadGame(StraightLineRPGGame, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	loader->loadGUI(StraightLineRPGGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	StraightLineRPGButtonEventHandler *StraightLineRPGButtonHandler = new StraightLineRPGButtonEventHandler();
	GameGUI *gui = StraightLineRPGGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)StraightLineRPGButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	StraightLineRPGKeyEventHandler *StraightLineRPGKeyHandler = new StraightLineRPGKeyEventHandler();
	StraightLineRPGGame->getInput()->registerKeyHandler((KeyEventHandler*)StraightLineRPGKeyHandler);


	// START THE GAME LOOP
	StraightLineRPGGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)StraightLineRPGGame->getOS();
	delete (WindowsInput*)StraightLineRPGGame->getInput();
	delete (WindowsTimer*)StraightLineRPGGame->getTimer();
	delete (DirectXGraphics*)StraightLineRPGGame->getGraphics();
	if(loader->textFlag)
	{
		delete (StraightLineRPGTextGenerator*)StraightLineRPGGame->getText()->getTextGenerator();
	}
	delete StraightLineRPGButtonHandler;
	delete StraightLineRPGKeyHandler;
	delete StraightLineRPGGame;

	// AND RETURN
	return 0;
}