#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\HealthBar.h"

void HealthBar::updateSprite() {
	PhysicalProperties* pp = mySprite.getPhysicalProperties();
	int currentHealth = pp->getCurrentHealth();

	setCurrentState(stateNames[currentHealth]);
}