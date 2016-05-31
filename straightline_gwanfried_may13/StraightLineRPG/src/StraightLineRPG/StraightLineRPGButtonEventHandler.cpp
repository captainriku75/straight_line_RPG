/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGButtonEventHandler.cpp

	See StraightLineRPGButtonEventHandler.h for a class description.
*/

#include "StraightLineRPG_VS\stdafx.h"
#include "StraightLineRPG\StraightLineRPG.h"
#include "StraightLineRPG\StraightLineRPGButtonEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"

void StraightLineRPGButtonEventHandler::handleButtonEvents(	Game *game, 
													wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();

		//Set kill count to 0.
		gsm->setNumMonstersKilled(0);

		gsm->goToMainMenu();
	}

	else if (command.compare(W_ABOUT_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToAboutScreen();
	}
	else if (command.compare(W_GAME_CONTROLS_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToGameControlsScreen();
	}

	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(W_START_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->startGame();

		game->getGSM()->getSpriteManager()->setPlayerPresent(true);
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		game->quitGame();
		game->getGSM()->destroyBodiesFlag=true;
		game->getGSM()->update(game);
	}
}