//
//  SmallUnitMenuNode.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/7/13.
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
	class ISmallUnitMenuNodeDelegate
	{
	};
		
	class SmallUnitMenuNode : public cocos2d::CCNode
	{
	public:
		bool init(const char* spriteFrameName, UnitType* type);
		
		void setDelegate(ISmallUnitMenuNodeDelegate* delegate) { delegate_ = delegate; }
		ISmallUnitMenuNodeDelegate* delegate() const { return delegate_; }
		
		//CREATE_FUNC(LargeUnitMenuItem);
		//void setOpacity(GLubyte opacity);
	
	private:
		//void selected();
		//void unselected();
		
	private:
		ISmallUnitMenuNodeDelegate* delegate_;
		
		CCLabelBMFont* unitName_;
		CCLabelBMFont* outOffensive_;
		CCLabelBMFont* outDefensive_;
		CCLabelBMFont* outTurnsToCharge_;
		CCSprite* bgOffensive_;
		CCSprite* bgDefensive_;
		CCSprite* bgTurnsToCharge_;
		
		ccColor3B titleColor_ = ccc3(0, 0, 0);
		ccColor3B titleColorSelected_ = ccc3(60, 60, 60);
		ccColor3B descriptionColor_ = ccc3(52, 87, 113);
		ccColor3B descriptionColorSelected_ = ccc3(120, 120, 120);
	};
}