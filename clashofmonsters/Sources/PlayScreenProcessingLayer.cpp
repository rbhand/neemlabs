//
//  PlayScreenProcessingLayer.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 5/17/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "PlayScreenProcessingLayer.h"
#include "Utils.h"

using namespace cocos2d;
using namespace cocos2d_utils;
using namespace monsters;

#define FADE_TIME 0.4

bool PlayScreenProcessingLayer::init()
{
	if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100)))
	{
		return false;
	}
	
	bg_ = CCSprite::createWithSpriteFrameName("bg_next_turn.png");
	bg_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	
	
	label_ = CCLabelTTF::create("processing...", "BadaBoom BB", ct(32.0));
	label_->setColor(ccWHITE);
	label_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	
	//setCascadeOpacityEnabled(true);
	
	addChild(bg_);
	addChild(label_);
	
	setTouchEnabled(true);
	
	return true;
}

bool PlayScreenProcessingLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

void PlayScreenProcessingLayer::setTitle(const char* name)
{
	label_->setString(createWithFormat(" %s ", name).c_str());
}
