/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StraightLineRPGKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "StraightLineRPG_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"
#include <fmod.hpp>

class Game;


class StraightLineRPGKeyEventHandler : public KeyEventHandler
{
private:
	FMOD ::System  *system;
	FMOD ::Channel *channel;
	FMOD ::Channel *mchannel;
	FMOD ::Channel *bchannel;
	FMOD ::Channel *vchannel;
	FMOD ::Channel *nchannel;	
	FMOD ::Channel *dchannel;
	FMOD ::Channel *HitSoundChannel;
	FMOD ::Channel *HitSound2Channel;
	FMOD ::Channel *HealSoundChannel;
	FMOD ::Channel *DefendSoundChannel;
	FMOD ::Channel *Main2channel;
	FMOD ::Channel *Battle2channel;
	FMOD ::Channel *bossChannel;
	FMOD ::Channel *creditsChannel;
	FMOD ::Sound  *sound;
	FMOD ::Sound  *msound;
	FMOD ::Sound  *bsound;
	FMOD ::Sound  *vsound;
	FMOD ::Sound  *nsound;
	FMOD ::Sound  *dsound;
	FMOD ::Sound  *HitSound;
	FMOD ::Sound  *HitSound2;
	FMOD ::Sound  *HealSound;
	FMOD ::Sound  *DefendSound;
	FMOD ::Sound  *Main2sound;
	FMOD ::Sound  *Battle2sound;
	FMOD ::Sound  *bossSound;
	FMOD ::Sound  *creditsSound;
	bool pause;
	
	


public:
	StraightLineRPGKeyEventHandler();
	~StraightLineRPGKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
};