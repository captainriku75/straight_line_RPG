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
#include "sssf\gsm\ai\bots\RandomJumpingBot.h"
#include "sssf\gsm\ai\bots\Statue.h"
#include "sssf\gsm\ai\bots\MenuCursor.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\world\TiledLayer.h"
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

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

#include "StraightLineRPG\Contact.h"


/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void StraightLineRPGDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *StraightLineRPGOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *StraightLineRPGGraphics = new DirectXGraphics(game);
	StraightLineRPGGraphics->init(screenWidth, screenHeight);
	StraightLineRPGGraphics->initGraphics(StraightLineRPGOS, useFullscreen);
	StraightLineRPGGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(StraightLineRPGGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *StraightLineRPGInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *StraightLineRPGTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)StraightLineRPGGraphics,
								(GameInput*)StraightLineRPGInput,
								(GameOS*)StraightLineRPGOS,
								(GameTimer*)StraightLineRPGTimer);



	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;

	//Add game levels and their names.

	game->getGSM()->addLevel(LEVEL_1_TITLE, W_LEVEL_1_PATH);
	game->getGSM()->addLevel(LEVEL_2_TITLE, W_LEVEL_2_PATH);
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void StraightLineRPGDataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void StraightLineRPGDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void StraightLineRPGDataLoader::loadWorld(Game *game, wstring levelInitFile)	
{
	GameStateManager *gsm = game->getGSM();
	b2World *b2world = gsm->getSuperWorld();
	SpriteManager *spriteManager = gsm->getSpriteManager();

	if(gsm->inBattle())
	{
		b2World *b2world = gsm->getSuperWorld();
		textFlag=true;

		//Add a menu cursor.
		AnimatedSpriteType *cursorSpriteType = spriteManager->getSpriteType(2);
		b2BodyDef cursorDef;
		cursorDef.position.x = 750.0f;
		cursorDef.position.y = 500.0f;
		b2PolygonShape cursorPolygonShape;
		b2Body* cursorBody = b2world->CreateBody(&cursorDef);
		cursorBody->CreateFixture(&cursorPolygonShape,1);
		
		MenuCursor *cursorSprite = game->getGSM()->getSpriteManager()->getMenuCursor();
		cursorSprite->setb2Body(cursorBody);
		cursorSprite->setSpriteType(cursorSpriteType);
		cursorSprite->setCurrentState(IDLE);
		cursorSprite->setAlpha(255);

		cursorSprite->setOnTileThisFrame(false);
		cursorSprite->setOnTileLastFrame(false);
		cursorSprite->affixTightAABBBoundingVolume();

		spriteManager->addBot(cursorSprite);

		//Add monster's sprite to screen.
		int killCount = game->getGSM()->getNumMonstersKilled();
		AnimatedSpriteType *monSpriteType = spriteManager->getSpriteType(3 + killCount);

		b2BodyDef monDef;
		monDef.position.x = MONSTER_SPRITE_X;
		monDef.position.y = MONSTER_SPRITE_Y;
		b2PolygonShape monPolygonShape;
		b2Body* body = b2world->CreateBody(&monDef);
		monPolygonShape.SetAsBox(64/64/2.0f,64/64/2.0f);
		body->CreateFixture(&monPolygonShape,1);

		Statue *monSprite = new Statue();
		monSprite->setb2Body(body);
		monSprite->setSpriteType(monSpriteType);
		monSprite->setCurrentState(IDLE);
		monSprite->setAlpha(255);

		monSprite->setOnTileThisFrame(false);
		monSprite->setOnTileLastFrame(false);
		monSprite->affixTightAABBBoundingVolume();

		spriteManager->addBot(monSprite);

		BattleManager *bm = game->getBM();
		bm->deregisterActors();

		if(killCount == 0)
		{
			Actor *player = new Actor(L"Adam", 20, 20, 0, 0, 5, 5);

			struct Action basicAttack;
			basicAttack.name = L"Slash";
			basicAttack.magicCost = 0;
			basicAttack.theType = DAMAGE;
			basicAttack.data = player->getBaseStr();

			struct Action healSpell;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 30;

			struct Action defend;
			defend.name = L"Shield";
			defend.magicCost = 0;
			defend.theType = DEFEND;
			defend.data = 5;

			player->pushAction(basicAttack);
			player->pushAction(healSpell);
			player->pushAction(defend);

			bm->registerActor(player);

			Actor *monster = new Actor(L"Slimey", 15, 15, 0, 0, 0, 10);

			struct Action firstAttack;
			firstAttack.name = L"Punch";
			firstAttack.magicCost = 0;
			firstAttack.theType = DAMAGE;
			firstAttack.data = 10;

			struct Action secondAttack;
			secondAttack.name= L"Tackle";
			secondAttack.magicCost = 0;
			secondAttack.theType = DAMAGE;
			secondAttack.data = 15;

			monster->pushAction(firstAttack);
			monster->pushAction(secondAttack);

			bm->registerActor(monster);
		}
		else if(killCount == 1)
		{
			Actor *player = new Actor(L"Adam", 30, 30, 10, 10, 10, 10);

			struct Action basicAttack;
			basicAttack.name = L"Slash";
			basicAttack.magicCost = 0;
			basicAttack.theType = DAMAGE;
			basicAttack.data = player->getBaseStr();

			struct Action healSpell;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 30;

			struct Action defend;
			defend.name = L"Shield";
			defend.magicCost = 0;
			defend.theType = DEFEND;
			defend.data = 5;

			player->pushAction(basicAttack);
			player->pushAction(healSpell);
			player->pushAction(defend);

			bm->registerActor(player);

			Actor *monster = new Actor(L"Dardunk", 50, 50, 0, 0, 0, 10);

			struct Action firstAttack;
			firstAttack.name = L"Stomp";
			firstAttack.magicCost = 0;
			firstAttack.theType = DAMAGE;
			firstAttack.data = 15;

			struct Action secondAttack;
			secondAttack.name = L"Claw";
			secondAttack.magicCost = 0;
			secondAttack.theType = DAMAGE;
			secondAttack.data = 20;

			monster->pushAction(firstAttack);
			monster->pushAction(secondAttack);

			bm->registerActor(monster);
		}
		else if(killCount == 2)
		{
			Actor *player = new Actor(L"Adam", 50, 50, 20, 20, 20, 20);

			struct Action basicAttack;
			basicAttack.name = L"Slash";
			basicAttack.magicCost = 0;
			basicAttack.theType = DAMAGE;
			basicAttack.data = player->getBaseStr();

			struct Action healSpell;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 30;

			struct Action defend;
			defend.name = L"Shield";
			defend.magicCost = 0;
			defend.theType = DEFEND;
			defend.data = 10;

			player->pushAction(basicAttack);
			player->pushAction(healSpell);
			player->pushAction(defend);

			bm->registerActor(player);

			Actor *monster = new Actor(L"Grag", 60, 100, 45, 45, 0, 10);

			struct Action firstAttack;
			firstAttack.name = L"Mean Hook";
			firstAttack.magicCost = 0;
			firstAttack.theType = DAMAGE;
			firstAttack.data = 30;

			struct Action secondAttack;
			secondAttack.name = L"Hammer Strike";
			secondAttack.magicCost = 0;
			secondAttack.theType = DAMAGE;
			secondAttack.data = 40;

			struct Action firstMagic;
			firstMagic.name = L"Meteor Rain";
			firstMagic.magicCost = 10;
			firstMagic.theType = DAMAGE;
			firstMagic.data = 55;

			struct Action healMagic;
			healMagic.name = L"Regeneration";
			healMagic.magicCost = 5;
			healMagic.theType = HEAL;
			healMagic.data = 10;

			monster->pushAction(firstAttack);
			monster->pushAction(secondAttack);
			monster->pushAction(firstMagic);
			monster->pushAction(healMagic);

			bm->registerActor(monster);
		}
		else if(killCount==3)
		{
			Actor *player = new Actor(L"Adam", 70, 70, 30, 30, 30, 20);

			struct Action basicAttack;
			basicAttack.name = L"Slash";
			basicAttack.magicCost = 0;
			basicAttack.theType = DAMAGE;
			basicAttack.data = player->getBaseStr();

			struct Action healSpell;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 30;

			struct Action defend;
			defend.name = L"Shield";
			defend.magicCost = 0;
			defend.theType = DEFEND;
			defend.data = 10;

			player->pushAction(basicAttack);
			player->pushAction(healSpell);
			player->pushAction(defend);

			bm->registerActor(player);

			Actor *monster = new Actor(L"Rock Lord", 8, 10, 0, 0, 19, 75);

			struct Action firstAttack;
			firstAttack.name = L"Loafing Around";
			firstAttack.magicCost = 0;
			firstAttack.theType = DAMAGE;
			firstAttack.data = 0;

			struct Action secondAttack;
			secondAttack.name = L"Deadly strike";
			secondAttack.magicCost = 0;
			secondAttack.theType = DAMAGE;
			secondAttack.data = 75;


			monster->pushAction(firstAttack);
			monster->pushAction(firstAttack);
			monster->pushAction(secondAttack);


			bm->registerActor(monster);

		}
		else if(killCount==4)
		{
			Actor *player = new Actor(L"Adam", 100, 100, 50, 50, 30, 30);

			struct Action basicAttack;
			basicAttack.name = L"Slash";
			basicAttack.magicCost = 0;
			basicAttack.theType = DAMAGE;
			basicAttack.data = player->getBaseStr();

			struct Action healSpell;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 50;

			struct Action defend;
			defend.name = L"Shield";
			defend.magicCost = 0;
			defend.theType = DEFEND;
			defend.data = 20;

			player->pushAction(basicAttack);
			player->pushAction(healSpell);
			player->pushAction(defend);

			bm->registerActor(player);

			Actor *monster = new Actor(L"warlock", 80, 80, 1000, 1000, 20, 20);

			struct Action firstAttack;
			firstAttack.name = L"fireball";
			firstAttack.magicCost = 20;
			firstAttack.theType = DAMAGE;
			firstAttack.data = 40;

			struct Action secondAttack;
			secondAttack.name = L"flamestrike";
			secondAttack.magicCost = 50;
			secondAttack.theType = DAMAGE;
			secondAttack.data = 80;

			struct Action heal;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 30;


			monster->pushAction(firstAttack);
			monster->pushAction(secondAttack);
			monster->pushAction(heal);


			bm->registerActor(monster);

		}
		else if(killCount==5)
		{
			Actor *player = new Actor(L"Adam", 200, 200, 125, 125, 50, 70);

			struct Action basicAttack;
			basicAttack.name = L"Slash";
			basicAttack.magicCost = 0;
			basicAttack.theType = DAMAGE;
			basicAttack.data = player->getBaseStr();

			struct Action healSpell;
			healSpell.name = L"Heal";
			healSpell.magicCost = 5;
			healSpell.theType = HEAL;
			healSpell.data = 50;

			struct Action defend;
			defend.name = L"Shield";
			defend.magicCost = 0;
			defend.theType = DEFEND;
			defend.data = 30;

			player->pushAction(basicAttack);
			player->pushAction(healSpell);
			player->pushAction(defend);

			bm->registerActor(player);

			Actor *monster = new Actor(L"dragon", 300, 500, 500, 500, 20, 20);

			struct Action basic;
			basic.name = L"basic attack";
			basic.magicCost = 0;
			basic.theType = DAMAGE;
			basic.data = 70;

			struct Action flame;
			flame.name = L"flame breath";
			flame.magicCost = 0;
			flame.theType = DAMAGE;
			flame.data = 110;

			struct Action armor;
			armor.name = L"Shield";
			armor.magicCost = 0;
			armor.theType = DEFEND;
			armor.data = 15;

			struct Action Prep;
			Prep.name = L"prep attack";
			Prep.magicCost = 0;
			Prep.theType = DAMAGE;
			Prep.data = 10;

			struct Action ultAttack;
			ultAttack.name = L"inferno blast";
			ultAttack.magicCost = 0;
			ultAttack.theType = DAMAGE;
			ultAttack.data = 150;




			monster->pushAction(basic);
			monster->pushAction(flame);
			monster->pushAction(armor);
			monster->pushAction(Prep);
			monster->pushAction(ultAttack);

			bm->registerActor(monster);

		}

		if(!textGeneratorInitialized)
		{
			textGeneratorInitialized = true;
			// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
			// TEXT ON THE SCREEN EACH FRAME
			StraightLineRPGTextGenerator *textGen = new StraightLineRPGTextGenerator();
			textGen->initText(game);
			GameText *text = game->getText();
			text->initDebugFile(W_DEBUG_FILE);
			text->setTextGenerator((TextGenerator*)textGen);
		}

		game->getGraphics()->toggleBattleTextShouldBeRendered();
	}
	else
	{
		// LOAD THE LEVEL'S BACKGROUND TILES
		TMXMapImporter tmxMapImporter;
		if(game->getGSM()->getCurrentLevel() < 1)
		{
			tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);
		}
		else
		{
			tmxMapImporter.loadWorld(game, W_LEVEL_2_DIR, W_LEVEL_2_NAME);
		}

		// LOAD THE LEVEL'S SPRITE IMAGES
		PoseurSpriteTypesImporter psti;
		psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

		// LET'S MAKE A PLAYER SPRITE
		// @TODO - IT WOULD BE BETTER TO LOAD THIS STUFF FROM A FILE

		AnimatedSprite *player = spriteManager->getPlayer();
		b2BodyDef bodyDef;
		bodyDef.type = b2BodyType::b2_dynamicBody;
		bodyDef.position.x = PLAYER_INIT_X;
		bodyDef.position.y = PLAYER_INIT_Y;
		b2Body* body = b2world->CreateBody(&bodyDef);
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(64/64/2.0f,64/64/2.0f);
		body->CreateFixture(&polygonShape,1);
		player->setb2Body(body);
		b2world->SetContactListener(new MyContactListener(game));

		// NOTE THAT PLAYER IS SPRITE ID 0
		AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
		player->setSpriteType(playerSpriteType);
		player->setAlpha(255);
		player->setCurrentState(IDLE);
		player->setOnTileThisFrame(false);
		player->setOnTileLastFrame(false);
		player->affixTightAABBBoundingVolume();


		// AND LET'S ADD A FEW STATUES

		AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(1);

		int killCount = game->getGSM()->getNumMonstersKilled();\
		if(game->getGSM()->getCurrentLevel() == 0)
		{
			if(killCount < 1)
			{
				makeStatueBot(game, botSpriteType, 500, 315);
			}
			if(killCount < 2)
			{
				makeStatueBot(game, botSpriteType, 700, 315);
			}
			if(killCount < 3)
			{
				makeStatueBot(game, botSpriteType, 900, 315);
			}
		}
		else //level two
		{
			if(killCount < 4)
			{
				makeStatueBot(game, botSpriteType, 400, 315);
			}
			if(killCount < 5)
			{
				makeStatueBot(game, botSpriteType, 750, 315);
			}
			if(killCount < 6)
			{
				makeStatueBot(game, botSpriteType, 850, 315);
			}
		}
	}
}


void StraightLineRPGDataLoader::makeStatueBot(Game *game, AnimatedSpriteType *StatueBotType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	b2World *b2world = game->getGSM()->getSuperWorld();
	Statue *bot = new Statue();
	b2BodyDef bodyDef;
	bodyDef.position.x = initX;
	bodyDef.position.y = initY;
	b2Body* body = b2world->CreateBody(&bodyDef);
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(75/2/3.0f,75/2/3.0f);
	body->CreateFixture(&polygonShape,1);
	bot->setb2Body(body);
	bot->setSpriteType(StatueBotType);
	bot->setCurrentState(IDLE);
	bot->setAlpha(255);
	bot->affixTightAABBBoundingVolume();
	spriteManager->addBot(bot);
}

/*
	initStraightLineRPGGUI - This method builds a GUI for the StraightLineRPG Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void StraightLineRPGDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initVictoryScreen(gui, guiTextureManager);
	initDeathScreen(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
	initAboutScreen(gui, guiTextureManager);
	initGameControlsScreen(gui, guiTextureManager);
	initBattleScreenGUI(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void StraightLineRPGDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void StraightLineRPGDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void StraightLineRPGDataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							312,
							600,
							0,
							255,
							200,
							100,
							false,
							W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							312,
							450,
							0,
							255,
							200,
							100,
							false,
							W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							650,
							450,
							0,
							255,
							200,
							100,
							false,
							W_ABOUT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();

	normalTextureID = guiTextureManager->loadTexture(W_GAME_CONTROLS_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_GAME_CONTROLS_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							612,
							600,
							0,
							255,
							200,
							100,
							false,
							W_GAME_CONTROLS_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}


void StraightLineRPGDataLoader::initAboutScreen(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A ABOUT SCREEN
	ScreenGUI *AboutGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_ABOUT_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	AboutGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							600,
							0,
							0,
							255,
							200,
							100,
							false,
							W_GO_TO_MM_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	AboutGUI->addButton(buttonToAdd);
	gui->addScreenGUI(GS_ABOUT_SCREEN, AboutGUI);
}

void StraightLineRPGDataLoader::initGameControlsScreen(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A ABOUT SCREEN
	ScreenGUI *GameControlsGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_GAME_CONTROLS_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	GameControlsGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							600,
							0,
							0,
							255,
							200,
							100,
							false,
							W_GO_TO_MM_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	GameControlsGUI->addButton(buttonToAdd);
	gui->addScreenGUI(GS_GAME_CONTROLS_SCREEN, GameControlsGUI);
}


/*
	initVictoryScreen - initializes the game's victory screen.
*/
void StraightLineRPGDataLoader::initVictoryScreen(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A VICTORY SCREEN
	ScreenGUI *victoryGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_VICTORY_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	victoryGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							812,
							500,
							0,
							255,
							200,
							100,
							false,
							W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	victoryGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_VICTORY_SCREEN, victoryGUI);
}

void StraightLineRPGDataLoader::initDeathScreen(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A VICTORY SCREEN
	ScreenGUI *deathGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_DEATH_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	deathGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							500,
							0,
							255,
							200,
							100,
							false,
							W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	deathGUI->addButton(buttonToAdd);

	

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_DEATH_SCREEN, deathGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void StraightLineRPGDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							173,
							0,
							0,
							255,
							200,
							100,
							false,
							W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_OVERWORLD,	inGameGUI);
}

/*
	initInGameGUI - initializes the game's battle-screen GUI.
*/
void StraightLineRPGDataLoader::initBattleScreenGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *battleGUI = new ScreenGUI();

	unsigned int actionMenuWidth = 200;
	unsigned int actionMenuHeight = 300;
	unsigned int actionMenuBGID = guiTextureManager->loadTexture(W_ACTION_MENU_BG_PATH);
	OverlayImage *actionMenuBG = new OverlayImage();
	actionMenuBG->x = screenWidth - actionMenuWidth;
	actionMenuBG->y = screenHeight - actionMenuHeight;
	actionMenuBG->z = 0;
	actionMenuBG->alpha = 255; //255 means opaque
	actionMenuBG->width = actionMenuWidth;
	actionMenuBG->height = actionMenuHeight;
	actionMenuBG->imageID = actionMenuBGID;
	battleGUI->addOverlayImage(actionMenuBG);

	unsigned int actionMenuHPWidth = 0;
	unsigned int actionMenuHPHeight = 300;
	unsigned int actionMenuHPID = guiTextureManager->loadTexture(W_ACTION_MENU_HP_PATH);
	OverlayImage *actionMenuHP = new OverlayImage();
	actionMenuHP->x =0;
	actionMenuHP->y = screenHeight - actionMenuHPHeight;
	actionMenuHP->z = 0;
	actionMenuHP->alpha = 255; //255 means opaque
	actionMenuHP->width = actionMenuHPWidth;
	actionMenuHP->height = actionMenuHPHeight;
	actionMenuHP->imageID = actionMenuHPID;
	battleGUI->addOverlayImage(actionMenuHP);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_BATTLE_SCREEN,	battleGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void StraightLineRPGDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
}