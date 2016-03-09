//
//  SmallUnitMenuNode.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/7/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "SmallUnitMenuNode.h"


using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;



bool SmallUnitMenuNode::init(const char* spriteFrameName, UnitType* type)
{
	setContentSize(CCSizeMake( isIPhone() ? 280 : ct(310), ct(35)));
	
	unitName_ = CCLabelBMFont::create(type->name().c_str(), cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str());
	unitName_->setAnchorPoint(CCPoint(0, 0.5));
	unitName_->setColor(descriptionColor_);
	unitName_->setPosition(CCPoint(ct(1), getContentSize().height * 0.5 - ct(2)));
	addChild(unitName_);
	
	float offset_x = getContentSize().width;
	float offset_y = getContentSize().height * 0.5;
	float label_width = ct(20);
	
	outTurnsToCharge_ = CCLabelBMFont::create( CCString::createWithFormat("%d", type->roundsToCharge())->getCString(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str(), ct(10), kCCTextAlignmentLeft);
	outTurnsToCharge_->setAnchorPoint(CCPoint(0, 0.5));
	outTurnsToCharge_->setPosition(CCPoint(offset_x - label_width, offset_y - ct(2)));
	outTurnsToCharge_->setColor(descriptionColor_);
	addChild(outTurnsToCharge_);
	
	offset_x -= label_width + ct(2);
	
	bgTurnsToCharge_ = CCSprite::createWithSpriteFrameName("icon_clock.png");
	bgTurnsToCharge_->setAnchorPoint(CCPoint(1, 0.5));
	bgTurnsToCharge_->setPosition(CCPoint(offset_x, offset_y));
	addChild(bgTurnsToCharge_);
	
	offset_x -= bgTurnsToCharge_->getContentSize().width;
	
	outDefensive_ = CCLabelBMFont::create( CCString::createWithFormat("%d", type->defensive())->getCString(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str(), label_width, kCCTextAlignmentLeft);
	outDefensive_->setAnchorPoint(CCPoint(0, 0.5));
	outDefensive_->setPosition(CCPoint(offset_x - label_width, offset_y - ct(2)));
	outDefensive_->setColor(descriptionColor_);
	addChild(outDefensive_);
	
	offset_x -= label_width + ct(2);
	
	bgDefensive_ = CCSprite::createWithSpriteFrameName("icon_shield.png");
	bgDefensive_->setAnchorPoint(CCPoint(1, 0.5));
	bgDefensive_->setPosition(CCPoint(offset_x, offset_y));
	addChild(bgDefensive_);
	
	offset_x -= bgDefensive_->getContentSize().width;
	
	outOffensive_ = CCLabelBMFont::create( CCString::createWithFormat("%d", type->offensive())->getCString(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str(), label_width, kCCTextAlignmentLeft);
	outOffensive_->setAnchorPoint(CCPoint(0, 0.5));
	outOffensive_->setPosition(CCPoint(offset_x - label_width, offset_y - ct(2)));
	outOffensive_->setColor(descriptionColor_);
	addChild(outOffensive_);
	
	offset_x -= label_width + ct(2);
	
	bgOffensive_ = CCSprite::createWithSpriteFrameName("icon_sword.png");
	bgOffensive_->setAnchorPoint(CCPoint(1, 0.5));
	bgOffensive_->setPosition(CCPoint(offset_x, offset_y));
	addChild(bgOffensive_);
	
	
	return true;
}