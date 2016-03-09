//
//  SoundManager.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#define kCMBackgroundMusicPath	"playscreen_bgmusic"

namespace monsters
{
	class SoundManager
	{
	public:
		SoundManager();
		~SoundManager();
		
		void setIsSoundEnabled(bool enabled);
		void setIsMusicEnabled(bool enabled);
		
		void startBackgroundMusicAtPath(const char* path);
		void stopBackgroundMusic();
		void startBackgroundMusic();
		cocos2d::CCString* currentMusic();
		
		unsigned int playEffect(const char* effect_name, bool loop = false);
		unsigned int playEffectOneAtTime(const char* effect_name, float time, bool loop = false);
		void stopEffect(unsigned int effectId);
		void stopAllEffects();
		
		static SoundManager* sharedSoundManager();
		
	private:
		bool isSoundEnabled_;
		bool isMusicEnabled_;
		
		//std::list<const char*> singleEffects_;
		cocos2d::CCString* currentBGMusicName_;
	};
}