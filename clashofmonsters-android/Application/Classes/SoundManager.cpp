//
//  SoundManager.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "SoundManager.h"

using namespace monsters;
using namespace CocosDenshion;
using namespace cocos2d;

static SoundManager* g_sharedSoundManager = NULL;


SoundManager::SoundManager()
{
	currentBGMusicName_ = CCString::createWithFormat("%s", "");
	currentBGMusicName_->retain();
}

SoundManager::~SoundManager()
{
	currentBGMusicName_->release();
}

void SoundManager::setIsSoundEnabled(bool enabled)
{
	isSoundEnabled_ = enabled;
	
	if (!isSoundEnabled_)
	{
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
	}
}

void SoundManager::setIsMusicEnabled(bool enabled)
{
	isMusicEnabled_ = enabled;
	
	if (!isMusicEnabled_)
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
	else if (isMusicEnabled_)
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.85);
}

void SoundManager::startBackgroundMusicAtPath(const char* path)
{
	currentBGMusicName_->release();
	currentBGMusicName_ = CCString::createWithFormat("%s", path);
	currentBGMusicName_->retain();
	
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("Sound/%s.mp3", path)->getCString(), true);
}

void SoundManager::startBackgroundMusic()
{
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

	startBackgroundMusicAtPath(kCMBackgroundMusicPath);
}

void SoundManager::stopBackgroundMusic()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

cocos2d::CCString* SoundManager::currentMusic()
{
	return currentBGMusicName_;
};

unsigned int SoundManager::playEffect(const char* effect_name, bool loop)
{
	//std::cout << std::endl << "Playing effect " << CCString::createWithFormat("Sound/%s.caf", effect_name)->getCString() << std::endl;
	
	if (isSoundEnabled_)
		return SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("Sound/%s.wav", effect_name)->getCString(), loop);
	
	return 0;
}

unsigned int SoundManager::playEffectOneAtTime(const char* effect_name, float time, bool loop)
{
	/*for (std::list<const char*>::const_iterator i=singleEffects_.begin(),
		e=singleEffects_.end(); i!=e; ++i)
	{
		if (find((*i)->units().begin(), (*i)->units().end(), unit) != (*i)->units().end())
			return 0;
	}*/
	
	return playEffect(effect_name, loop);
}

void SoundManager::stopEffect(unsigned int effectId)
{
	SimpleAudioEngine::sharedEngine()->stopEffect(effectId);
}

void SoundManager::stopAllEffects()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

SoundManager* SoundManager::sharedSoundManager()
{
	if (!g_sharedSoundManager)
	{
		g_sharedSoundManager = new SoundManager();
	}
	
	return g_sharedSoundManager;
}
