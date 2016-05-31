#include "StraightLineRPG_VS\stdafx.h"
#include <Box2D\Box2D.h>
#include "sssf\game\Game.h"

class MyContactListener : public b2ContactListener 
{
private:
	Game *g;
public: 

	MyContactListener(Game *game)	{g=game;}
	 void BeginContact(b2Contact* contact);
	 void EndContact(b2Contact* contact) {}
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) 
	{
		B2_NOT_USED(contact); 
		B2_NOT_USED(oldManifold);
	}
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}
}; 
