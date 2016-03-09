//
//  LargeUnitMenuItem.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/6/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "LargeUnitMenuItem.h"
#include <stdlib.h>

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;



bool LargeUnitMenuItem::init(const char* spriteFrameName, UnitType* type, cocos2d::CCObject* target, SEL_MenuHandler selector)
{
	iconHeight_ = ct(58);
	
	CCSprite* normal_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	CCSprite* selected_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	CCSprite* disabled_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	selected_sprite->setColor(ccc3(120, 120, 120));
	
	/*float scale = iconHeight_ / normal_sprite->getContentSize().height;
	normal_sprite->setScale(scale);
	selected_sprite->setScale(scale);
	disabled_sprite->setScale(scale);*/
	
	CCMenuItemSprite::initWithNormalSprite(normal_sprite, selected_sprite, disabled_sprite, target, selector);
	
	setContentSize(CCSizeMake(ct(310), normal_sprite->getContentSize().height * normal_sprite->getScale()));
	
	unitName_ = CCLabelBMFont::create(type->name().c_str(), cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str());
	unitName_->setAnchorPoint(CCPoint(0, 1));
	unitName_->setColor(titleColor_);
	unitName_->setPosition(CCPoint(normal_sprite->getContentSize().width * normal_sprite->getScale() + ct(5), getContentSize().height));
	addChild(unitName_);
	
	unitDescription_ = CCLabelBMFont::create(type->description().c_str(), cocos2d_utils::textureName("Fonts/MyriadPro-12.fnt").c_str(),
		getContentSize().width - ct(5) - normal_sprite->getContentSize().width, kCCTextAlignmentLeft);
	unitDescription_->setAnchorPoint(CCPoint(0, 1));
	unitDescription_->setColor(descriptionColor_);
	unitDescription_->setPosition(CCPoint(normal_sprite->getContentSize().width * normal_sprite->getScale() + ct(5), getContentSize().height - unitName_->getContentSize().height));
	//unitDescription_->setDimensions(CCSize(getContentSize().width - ct(5) - normal_sprite->getContentSize().width, 0/*getContentSize().height - unitName_->getContentSize().height*/));
	addChild(unitDescription_);
	
	std::string cost_formatted;
	char buf[5];
	int cost = type->cost();
	while (cost >= 1000)
	{
		sprintf(buf, ",%03d", cost % 1000);
		cost /= 1000;
		cost_formatted = buf + cost_formatted;
	}
	
	sprintf(buf, "%d", cost);
	cost_formatted = buf + cost_formatted;
	
	
	unitUnlockDescription_ = CCLabelBMFont::create( ("Unlock for " + cost_formatted + " XP").c_str(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str(),
		getContentSize().width - ct(5) - normal_sprite->getContentSize().width, kCCTextAlignmentLeft);
	unitUnlockDescription_->setAnchorPoint(CCPoint(0, 1));
	unitUnlockDescription_->setColor(descriptionColor_);
	unitUnlockDescription_->setPosition(CCPoint(normal_sprite->getContentSize().width * normal_sprite->getScale() + ct(5), getContentSize().height - unitName_->getContentSize().height));
	addChild(unitUnlockDescription_);
	
	
	float offset_x = getContentSize().width;
	float offset_y = getContentSize().height - ct(7);
	float label_width = ct(16);
	
	outTurnsToCharge_ = CCLabelBMFont::create( CCString::createWithFormat("%d", type->roundsToCharge())->getCString(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str());
	outTurnsToCharge_->setAnchorPoint(CCPoint(0, 0.5));
	outTurnsToCharge_->setPosition(CCPoint(offset_x-label_width, offset_y - ct(2)));
	outTurnsToCharge_->setColor(descriptionColor_);
	outTurnsToCharge_->setScale(0.7);
	addChild(outTurnsToCharge_);

	offset_x -= label_width + ct(2);
	
	bgTurnsToCharge_ = CCSprite::createWithSpriteFrameName("icon_clock.png");
	bgTurnsToCharge_->setAnchorPoint(CCPoint(1, 0.5));
	bgTurnsToCharge_->setPosition(CCPoint(offset_x, offset_y));
	addChild(bgTurnsToCharge_);
	
	offset_x -= bgTurnsToCharge_->getContentSize().width;
	
	outDefensive_ = CCLabelBMFont::create( CCString::createWithFormat("%d", type->defensive())->getCString(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str());
	outDefensive_->setAnchorPoint(CCPoint(0, 0.5));
	outDefensive_->setPosition(CCPoint(offset_x - label_width, offset_y - ct(2)));
	outDefensive_->setColor(descriptionColor_);
	outDefensive_->setScale(0.7);
	addChild(outDefensive_);
	
	offset_x -= label_width + ct(2);
	
	bgDefensive_ = CCSprite::createWithSpriteFrameName("icon_shield.png");
	bgDefensive_->setAnchorPoint(CCPoint(1, 0.5));
	bgDefensive_->setPosition(CCPoint(offset_x, offset_y));
	addChild(bgDefensive_);
	
	offset_x -= bgDefensive_->getContentSize().width;
	
	outOffensive_ = CCLabelBMFont::create( CCString::createWithFormat("%d", type->offensive())->getCString(),
		cocos2d_utils::textureName("Fonts/MyriadPro-Semibold-16.fnt").c_str());
	outOffensive_->setAnchorPoint(CCPoint(0, 0.5));
	outOffensive_->setPosition(CCPoint(offset_x - label_width, offset_y - ct(2)));
	outOffensive_->setColor(descriptionColor_);
	outOffensive_->setScale(0.7);
	addChild(outOffensive_);
	
	offset_x -= label_width + ct(2);
	
	bgOffensive_ = CCSprite::createWithSpriteFrameName("icon_sword.png");
	bgOffensive_->setAnchorPoint(CCPoint(1, 0.5));
	bgOffensive_->setPosition(CCPoint(offset_x, offset_y));
	addChild(bgOffensive_);
	
	bgLockedIcon_ = CCSprite::createWithSpriteFrameName("icon_locked.png");
	bgLockedIcon_->setAnchorPoint(CCPoint(0.5,0.5));
	bgLockedIcon_->setPosition(CCPoint(normal_sprite->getContentSize().width * 0.5, normal_sprite->getContentSize().height * 0.5));
	addChild(bgLockedIcon_);
	
	//setLocked(true);
	
	return true;
}


void LargeUnitMenuItem::setLocked(bool locked)
{
	isLocked_ = locked;
	
	unitDescription_->setVisible(!isLocked_);
	bgOffensive_->setVisible(!isLocked_);
	outDefensive_->setVisible(!isLocked_);
	bgDefensive_->setVisible(!isLocked_);
	outOffensive_->setVisible(!isLocked_);
	bgTurnsToCharge_->setVisible(!isLocked_);
	outTurnsToCharge_->setVisible(!isLocked_);
	
	unitUnlockDescription_->setVisible(isLocked_);
	bgLockedIcon_->setVisible(isLocked_);
	
	CCMenuItemSprite::setEnabled(!isLocked_);
	
	if (isLocked_)
		setOpacity(192);
}


void LargeUnitMenuItem::selected()
{
	CCMenuItemSprite::selected();
	
	unitName_->setColor(titleColorSelected_);
	unitDescription_->setColor(descriptionColorSelected_);
}


void LargeUnitMenuItem::unselected()
{
	CCMenuItemSprite::unselected();
	
	unitName_->setColor(titleColor_);
	unitDescription_->setColor(descriptionColor_);
}


void LargeUnitMenuItem::setOpacity(GLubyte opacity)
{
	if (isLocked_) opacity = 192;
	
	CCMenuItemSprite::setOpacity(opacity);
	
	CCObject* child;
	CCArray* button_children = this->getChildren();
	
	CCARRAY_FOREACH(button_children, child)
	{
		if (CCRGBAProtocol* node = dynamic_cast<CCRGBAProtocol*>(child))
		{
			node->setOpacity(opacity);
		}
	}
}
