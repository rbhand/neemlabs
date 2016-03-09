//
//  Settings.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/4/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "SoundManager.h"


namespace monsters
{
	class Settings
	{
		public:
			Settings();
			
			void save();
			void load();
			
			void setSoundEnabled(bool is_sound_enabled);
			bool isSoundEnabled() const { return isSoundEnabled_; };
			
			void setMusicEnabled(bool is_music_enabled);
			bool isMusicEnabled() const { return isMusicEnabled_; };
			
		private:
			bool isSoundEnabled_;
			bool isMusicEnabled_;
	};
}
