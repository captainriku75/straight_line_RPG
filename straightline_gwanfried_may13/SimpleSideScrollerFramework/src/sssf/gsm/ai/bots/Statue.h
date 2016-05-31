#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"
#include <Box2D\Box2D.h>

class Statue : public Bot
{
private:


public:
	Statue() {initBot();}
	~Statue();
	Bot*	clone();
	void initBot();
	void	think(Game *game);


};