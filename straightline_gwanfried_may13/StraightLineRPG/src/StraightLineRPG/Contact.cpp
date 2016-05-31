#include "StraightLineRPG_VS\stdafx.h"
#include <Box2D\Box2D.h>
#include "contact.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\sprite\SpriteManager.h"



void MyContactListener:: BeginContact(b2Contact *con)
{
	g->getGSM()->battleTransitionFlag=true;
	g->getGSM()->destroyBodiesFlag=true;
}
