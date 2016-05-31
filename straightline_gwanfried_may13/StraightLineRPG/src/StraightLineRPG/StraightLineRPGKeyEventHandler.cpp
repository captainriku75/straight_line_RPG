/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGKeyEventHandler.cpp

	See StraightLineRPGKeyEventHandler.h for a class description.
*/

#include "StraightLineRPG_VS\stdafx.h"
#include "StraightLineRPG\StraightLineRPG.h"
#include "StraightLineRPG\StraightLineRPGKeyEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

#include "sssf\gsm\ai\bots\MenuCursor.h"

StraightLineRPGKeyEventHandler::StraightLineRPGKeyEventHandler()
{
	FMOD::System_Create(&system);

	system->init(32, FMOD_INIT_NORMAL, 0);
	system->createSound("Blip_Select10.wav", FMOD_DEFAULT, 0, &sound);
	system->createSound("Hit_Hurt2.wav", FMOD_DEFAULT, 0, &HitSound);
	system->createSound("Hit_Hurt8.wav", FMOD_DEFAULT, 0, &HitSound2);
	system->createSound("Heal.wav", FMOD_DEFAULT, 0, &HealSound);
	system->createSound("Defend.wav", FMOD_DEFAULT, 0, &DefendSound);
	system->createStream("over 1.mp3", FMOD_LOOP_NORMAL, 0, &msound);
	system->createStream("battle 1.mp3", FMOD_LOOP_NORMAL, 0, &bsound);
	system->createStream("title.mp3", FMOD_LOOP_NORMAL, 0, &nsound);
	//system->createStream("Victory Fanfare.mp3", FMOD_LOOP_NORMAL, 0, &vsound);
	system->createStream("death.mp3", FMOD_LOOP_NORMAL, 0, &dsound);
	system->createStream("over 2.mp3", FMOD_LOOP_NORMAL, 0, &Main2sound);
	system->createStream("battle 2.mp3", FMOD_LOOP_NORMAL, 0, &Battle2sound);
	system->createStream("boss battle.mp3", FMOD_LOOP_NORMAL, 0, &bossSound);
	system->createStream("credits.mp3", FMOD_LOOP_NORMAL, 0, &creditsSound);

	//create stream mp3
}
/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void StraightLineRPGKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	b2Body *bbody = player->getb2Body();
	Viewport *viewport = game->getGUI()->getViewport();
	system->update(); 
	b2World* b2world=gsm->getSuperWorld();
	//b2world->Step(1/60.f,8,3);


	if(gsm->inMainMenuScreen())
	{
		bool playing=true;
		nchannel->isPlaying(&playing);
		if(!playing)
		{
			system->playSound(nsound, 0, false, &nchannel);
		}
		mchannel->isPlaying(&playing);
		if(playing)
		{
			mchannel->stop();
		}
		dchannel->isPlaying(&playing);
		if(playing)
		{
			dchannel->stop();
		}
		Main2channel->isPlaying(&playing);
		if(playing)
		{
			Main2channel->stop();
		}
	}
	if(game->getGSM()->isAtVictoryScreen())
	{
		bool playing=true;
		bossChannel->isPlaying(&playing);
		if(playing)
		{
			bossChannel->stop();
		}
		creditsChannel->isPlaying(&playing);
		if(!playing)
		{
			system->playSound(creditsSound, 0, false, &creditsChannel);
		}
		
	}

	// IF THE GAME IS IN PROGRESS
	if (gsm->inOverworld())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = bbody->GetLinearVelocity().x;
		float vY = bbody->GetLinearVelocity().y;
			bool playing=true;
			nchannel->isPlaying(&playing);
			if(playing)
			{
				nchannel->stop();
			}
			dchannel->isPlaying(&playing);
			if(playing)
			{
				dchannel->stop();
			}

		if(game->getGSM()->getCurrentLevel()==0)
		{
			bool playing=true;

			//Play main theme, stop playing menu, battle and/or death musics
			mchannel->isPlaying(&playing);
			if(!playing)
			{
				system->playSound(msound, 0, false, &mchannel);
			}
		}
		else
		{
			bool playing=true;
			//Play main theme, stop playing menu, battle and/or death musics
			mchannel->isPlaying(&playing);
			if(playing)
			{
				mchannel->stop();
			}
			Main2channel->isPlaying(&playing);
			if(!playing)
			{
				system->playSound(Main2sound, 0, false, &Main2channel);
			}
		}

		if (input->isKeyDown(LEFT_KEY))
		{
			vX = -PLAYER_SPEED;
			player->setCurrentState(WALKING_LEFT);
		}
		else if (input->isKeyDown(RIGHT_KEY))
		{
			vX = PLAYER_SPEED;
			player->setCurrentState(WALKING_RIGHT);
		}
		else
		{
			vX = 0.0f;
			player->setCurrentState(IDLE);
		}
		

		// NOW SET THE ACTUAL PLAYER VELOCITY
		b2Vec2 b2vec;
		b2vec.Set(vX,vY);
		bbody->SetLinearVelocity(b2vec);

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;

		//if cutscene or other camera-tracking needs to move viewport,
		//do it here

		Viewport *viewport = game->getGUI()->getViewport();
		if (viewportMoved)
			viewport->moveViewport((int)floor(viewportVx+0.5f), (int)floor(viewportVy+0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		
	}
	else if (gsm->inBattle())
	{
		if(input->isKeyDownForFirstTime(K_KEY))
		{
			BattleManager *bm = game->getBM();

			//player's action

			Actor *monster = bm->getSecond();
			monster->setHealth(1);
		}

			bool playing=true;
			mchannel->isPlaying(&playing);
			if(playing)
			{
				mchannel->stop();
			}
			Main2channel->isPlaying(&playing);
			if(playing)
			{
				Main2channel->stop();
			}

		if(game->getGSM()->getCurrentLevel()==0)
		{
			bool playing=true;
			bchannel->isPlaying(&playing);
			if(!playing)
			{
				system->playSound(bsound, 0, false, &bchannel);
			}
		}
		else if(game->getGSM()->getCurrentLevel()==1 && game->getGSM()->getNumMonstersKilled()<5)
		{
			bool playing=true;
			Battle2channel->isPlaying(&playing);
			if(!playing)
			{
				system->playSound(Battle2sound, 0, false, &Battle2channel);
			}
		}
		else
		{
			bool playing=true;
			bossChannel->isPlaying(&playing);
			if(!playing)
			{
				system->playSound(bossSound, 0, false, &bossChannel);
			}
		}
		
		//in battle keyboard controls

		MenuCursor *menuCursor = gsm->getSpriteManager()->getMenuCursor();
		int choice = menuCursor->getMenuChoice();
		if(input->isKeyDownForFirstTime(VK_RETURN))
		{
			//Submit choice to battle system.

			BattleManager *bm = game->getBM();

			//player's action

			Actor *player = bm->getFirst();
			Actor *monster = bm->getSecond();

			Action chosenAction = player->actionAt(choice);
			bm->implementAction(chosenAction);

			if(chosenAction.theType == DAMAGE)
			{
				system->playSound(HitSound, 0, false, &HitSoundChannel);
			}
			else if (chosenAction.theType == HEAL)
			{
				system->playSound(HealSound, 0, false, &HealSoundChannel);
			}
			else if (chosenAction.theType == DEFEND)
			{
				system->playSound(DefendSound, 0, false, &DefendSoundChannel);
			}

			game->getGSM()->addGameRenderItemsToRenderList(game);
			game->getGUI()->addRenderItemsToRenderList(game);
			game->getGraphics()->renderGame(game);
			Sleep(1500);

			if(monster->isDead())
			{
				game->getGraphics()->toggleBattleTextShouldBeRendered();

				bm->resetTurns();
				bm->deregisterActors();

				//Get rid of the monster sprite.
				gsm->getSpriteManager()->unloadSprites();
				game->getGUI()->addRenderItemsToRenderList(game);
				game->getGraphics()->renderGame(game);

				//turn off music
				bool playing=true;
				bchannel->isPlaying(&playing);
				if(playing)
				{
					bchannel->stop();
				}
				Battle2channel->isPlaying(&playing);
				if(playing)
				{
					Battle2channel->stop();
				}
				bossChannel->isPlaying(&playing);
				if(playing)
				{
					bossChannel->stop();
				}
				//Stop battle music, sleep the game while victory music plays.

				// vchannel->isPlaying(&playing);
				// if(!playing)
				// {
					// system->playSound(vsound, 0, false, &vchannel);
				// }

				// Sleep(5000);

				// vchannel->isPlaying(&playing);
				// if(playing)
				// {
					// vchannel->stop();
				// }
				if(game->getGSM()->getCurrentLevel()==0)
				{
					mchannel->isPlaying(&playing);
					if(!playing)
					{
						system->playSound(msound, 0, false, &mchannel);
					}
				}
				else if(game->getGSM()->getCurrentLevel()==1 && game->getGSM()->getNumMonstersKilled()<5)
				{
					Main2channel->isPlaying(&playing);
					if(!playing)
					{
						system->playSound(Main2sound, 0, false, &Main2channel);
					}
				}
				int killCount = gsm->getNumMonstersKilled();
				gsm->setNumMonstersKilled(killCount + 1);
				if(game->getGSM()->getNumMonstersKilled()<6)
				{
					gsm->overworldTransitionFlag = true;
					
				}
				else //killed final boss, go to victory screen
				{
					bossChannel->isPlaying(&playing);
					if(playing)
					{
						bossChannel->stop();
					}
					gsm->victoryTransitionFlag=true;
				}
				gsm->destroyBodiesFlag = true;
				game->getGraphics()->clearWorldTextures();
			}
			else
			{
				//monster's action
			
				Action monsterAction = bm->getSecond()->pickAction();
				bm->implementAction(monsterAction);

				if(monsterAction.theType == DAMAGE)
				{
					system->playSound(HitSound2, 0, false, &HitSound2Channel);
				}
				else if (monsterAction.theType == HEAL)
				{
					system->playSound(HealSound, 0, false, &HealSoundChannel);
				}
				else if (monsterAction.theType == DEFEND)
				{
					system->playSound(DefendSound, 0, false, &DefendSoundChannel);
				}

				gsm->addGameRenderItemsToRenderList(game);
				game->getGUI()->addRenderItemsToRenderList(game);
				game->getGraphics()->renderGame(game);
				Sleep(1500);

				if(player->isDead())
				{
					bm->resetTurns();
					bm->deregisterActors();

					//turn off music
					bool playing=true;
					bchannel->isPlaying(&playing);
					if(playing)
					{
						bchannel->stop();
					}
					Battle2channel->isPlaying(&playing);
					if(playing)
					{
						Battle2channel->stop();
					}
					bossChannel->isPlaying(&playing);
					if(playing)
					{
						bossChannel->stop();
					}
					dchannel->isPlaying(&playing);
					if(!playing)
					{
						system->playSound(dsound, 0, false, &dchannel);
					}

					game->getGraphics()->toggleBattleTextShouldBeRendered();

					gsm->deathTransitionFlag = true;
					gsm->destroyBodiesFlag = true;

					game->getGraphics()->clearWorldTextures();
				}
			}
		}
		else
		{
			if(input->isKeyDownForFirstTime(DOWN_KEY))
			{
				system->playSound(sound, 0, false, &channel);

				if(choice < 2)
				{
					menuCursor->setMenuChoice(choice + 1);
				}
			}
			else if(input->isKeyDownForFirstTime(UP_KEY))
			{
				system->playSound(sound, 0, false, &channel);

				if(choice > 0)
				{
					menuCursor->setMenuChoice(choice - 1);
				}
			}
		}
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
	}
}