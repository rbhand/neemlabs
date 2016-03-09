//
//  NextRoundLayer.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "NextRoundLayer.h"
#include "Utils.h"

using namespace cocos2d;
using namespace cocos2d_utils;
using namespace monsters;

#define FADE_TIME 0.4

bool NextRoundLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	bg_ = CCSprite::createWithSpriteFrameName("bg_next_turn.png");
	bg_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	
	
	label_ = CCLabelTTF::create("next round", "BadaBoom BB", ct(32.0));
	label_->setColor(ccWHITE);
	label_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	
	
	setCascadeOpacityEnabled(true);
	
	addChild(bg_);
	addChild(label_);
	
	return true;
}


void NextRoundLayer::setPlayerName(const char* name)
{
	label_->setString(createWithFormat(" %s turn ", name).c_str());
}


//void NextRoundLayer::setOpacity(GLubyte opacity)
//{
//	bg_->setOpacity(opacity);
//	label_->setOpacity(opacity);
//}
//
//void NextRoundLayer::setColor(const cocos2d::ccColor3B& color)
//{
//	bg_->setColor(color);
//	label_->setColor(color);
//}

void NextRoundLayer::show()
{
	CCLayer::setVisible(true);
}


void NextRoundLayer::hide()
{
	CCLayer::setVisible(true);
}