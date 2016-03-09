//
//  LargeUnitMenuItem.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/6/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//


#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "NavigationMenuLayer.h"
#include "GameUnits.h"
#include "Unit.h"
#include "Player.h"

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	class ILargeUnitMenuItemDelegate
	{
	};
		
	class LargeUnitMenuItem : public cocos2d::CCMenuItemSprite
	{
	public:
		bool init(const char* spriteFrameName, UnitType* type, cocos2d::CCObject *target, SEL_MenuHandler selector);
		
		void setDelegate(ILargeUnitMenuItemDelegate* delegate) { delegate_ = delegate; }
		ILargeUnitMenuItemDelegate* delegate() const { return delegate_; }
		
		//CREATE_FUNC(LargeUnitMenuItem);
		void setOpacity(GLubyte opacity);
		void setLocked(bool locked);
	
	private:
		void selected();
		void unselected();
		
	private:
		ILargeUnitMenuItemDelegate* delegate_;
		
		bool isLocked_;
		
		CCSprite* unitIcon_;
		CCLabelTTF* unitName_;
		CCLabelTTF* unitDescription_;
		CCLabelTTF* unitUnlockDescription_;
		CCLabelTTF* outOffensive_;
		CCLabelTTF* outDefensive_;
		CCLabelTTF* outTurnsToCharge_;
		CCSprite* bgOffensive_;
		CCSprite* bgDefensive_;
		CCSprite* bgTurnsToCharge_;
		CCSprite* bgLockedIcon_;
		
		float iconHeight_;
		
		ccColor3B titleColor_ = ccc3(0, 0, 0);
		ccColor3B titleColorSelected_ = ccc3(60, 60, 60);
		ccColor3B descriptionColor_ = ccc3(52, 87, 113);
		ccColor3B descriptionColorSelected_ = ccc3(120, 120, 120);
	};
}

