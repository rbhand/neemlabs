//
//  PlayerCaption.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

namespace monsters
{
	class PlayerCaption : public cocos2d::CCNode
	{
	public:
		PlayerCaption(void);
		
		void setLayout(bool align_left, bool place_avatar_at_top);
		void setAvatar(const char* frame_name);
		void setName(const char* name);
	
	private:
		cocos2d::CCSprite* avatar_;
		cocos2d::CCLabelTTF* name_;
	};
}
