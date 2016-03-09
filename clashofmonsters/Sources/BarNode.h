//
//  BarNode.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/1/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

namespace monsters
{
	enum BarNodeDirection
	{
		BarNodeDirection_Up = 0,
		BarNodeDirection_Down = 1
	};
	
	class BarFilledNode;
	class BarNode : public cocos2d::CCNode
	{
	public:
		bool initWithSprites(cocos2d::CCSprite* backgroundSprite,
			cocos2d::CCSprite* filledSprite, GLbyte opacity = 255);
			
		static BarNode* barNodeWithSprites(cocos2d::CCSprite* backgroundSprite,
			cocos2d::CCSprite* filledSprite, GLbyte opacity = 255);
		
		void setValue(float value);
		float value() const;
		
		void setDirection(BarNodeDirection direction);
		BarNodeDirection direction() const;
		
	private:
		BarFilledNode* filledNode_;
	};
}
