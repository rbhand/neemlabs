//
//  Settings.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/4/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "Settings.h"
#include "CCUserDefault.h"

using namespace cocos2d;
using namespace monsters;

Settings::Settings()
{
	//this->setSoundEnabled(true);
	//this->setMusicEnabled(false);
}

void Settings::save()
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("is_music_enabled", isMusicEnabled_);
	CCUserDefault::sharedUserDefault()->setBoolForKey("is_sound_enabled", isSoundEnabled_);
}

void Settings::load()
{
	isMusicEnabled_ = CCUserDefault::sharedUserDefault()->getBoolForKey("is_music_enabled", true);
	isSoundEnabled_ = CCUserDefault::sharedUserDefault()->getBoolForKey("is_sound_enabled", true);
	
	#ifdef MN_DISABLE_SOUND
	isMusicEnabled_ = false;
	isSoundEnabled_ = false;
	#endif
}

void Settings::setSoundEnabled(bool is_sound_enabled)
{
	isSoundEnabled_ = is_sound_enabled;
	SoundManager::sharedSoundManager()->setIsSoundEnabled(is_sound_enabled);
	
	save();
}

void Settings::setMusicEnabled(bool is_music_enabled)
{
	isMusicEnabled_ = is_music_enabled;
	SoundManager::sharedSoundManager()->setIsMusicEnabled(is_music_enabled);
	
	save();
}
