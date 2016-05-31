#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\Statue.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

#include "sssf\gsm\ai\bots\Statue.h"
#include "sssf\gsm\ai\bots\MenuCursor.h"

/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
MenuCursor::MenuCursor()
{
	// INIT THE BASIC STUFF
	initBot();
}

/*
	clone - this method makes another RandomFloatingBot object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* MenuCursor::clone()
{
	MenuCursor *botClone = new MenuCursor();
	return botClone;
}

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void MenuCursor::initBot()
{
	menuChoice = 0;
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void MenuCursor::think(Game *game)
{
	b2Vec2 *newPosition = new b2Vec2();

	newPosition->x = body->GetPosition().x;

	if(getMenuChoice() == 0)
	{
		newPosition->y = 480;
		body->SetTransform(*newPosition, 0.0f);
	}
	else if(getMenuChoice() == 1)
	{
		newPosition->y = 540;
		body->SetTransform(*newPosition, 0.0f);
	}
	else
	{
		newPosition->y = 620;
		body->SetTransform(*newPosition, 0.0f);
	}
}