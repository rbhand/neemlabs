//
//  StrengthIndicatorsLayer.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/13/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "StrengthIndicatorsLayer.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;



bool StrengthIndicatorsLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	labelsLayer_ = CCLayer::create();
	iconsLayer_ = CCLayer::create();
	bgLayer_ = CCLayerColor::create(ccc4(0, 0, 0, 128));
	
	
	addChild(bgLayer_);
	addChild(iconsLayer_);
	addChild(labelsLayer_);
	
	return true;
}


void StrengthIndicatorsLayer::onEnter()
{
	CCLayer::onEnter();
	
	time_ = 0;
	
	strengthIcons_.clear();
	strengthLabels_.clear();
	
	updateLabels();
	updateIcons();
	
	CCLayer::schedule(schedule_selector(StrengthIndicatorsLayer::update), 0.025);
}


void StrengthIndicatorsLayer::onExit()
{
	CCLayer::onExit();
	
	CCLayer::unschedule(schedule_selector(StrengthIndicatorsLayer::update));
	
	labelsLayer_->removeAllChildrenWithCleanup(true);
	iconsLayer_->removeAllChildrenWithCleanup(true);
}

void StrengthIndicatorsLayer::update(float dt)
{
	CCLayer::update(dt);
	
	time_ += dt;
	
	updateLabels();
	updateIcons();
}


void StrengthIndicatorsLayer::updateIcons()
{
	//create icons
	for (std::map<const Unit*,UnitNode*>::const_iterator i=gameFieldNode_->unitNodes().begin(), e=gameFieldNode_->unitNodes().end();
		i!=e; ++i)
	{
		map<const UnitNode*, CCSprite*>::const_iterator it = strengthIcons_.find(i->second);
		
		if (!it->second || it == strengthIcons_.end())
		{
			if (iconTagForUnitNode(i->second) && strengthForUnitNode(i->second))
			{
				CCSprite* icon = strengthIconForUnitNode(i->second);
				strengthIcons_.insert(pair<const UnitNode*, CCSprite*>(i->second, icon));
				
				iconsLayer_->addChild(icon);
			}
		}
		else
		{
			it->second->setPosition(i->second->getPosition());
			
			int strength = strengthForUnitNode(i->second);
			if (!strength)
				it->second->setVisible(false);
		}
	}
	
	//delete icons
	for (std::map<const UnitNode*, CCSprite*>::const_iterator i=strengthIcons_.begin(), e=strengthIcons_.end();
		i!=e; ++i)
	{
		bool delete_flag = true;
		
		for (std::map<const Unit*, UnitNode*>::const_iterator i2=gameFieldNode_->unitNodes().begin(), e2=gameFieldNode_->unitNodes().end();
		i2!=e2; ++i2)
		{
			if (i->first == i2->second && i->second->getTag() == iconTagForUnitNode(i2->second))
			{
				delete_flag = false;
			}
			
			if (!delete_flag)
				break;
		}
		
		if (delete_flag)
		{
			i->second->removeFromParentAndCleanup(true);
			strengthIcons_.erase(i->first);
		}
	}
}


void StrengthIndicatorsLayer::updateLabels()
{
	//create labels
	for (std::map<const Unit*,UnitNode*>::const_iterator i=gameFieldNode_->unitNodes().begin(), e=gameFieldNode_->unitNodes().end();
		i!=e; ++i)
	{
		map<const UnitNode*, CCLabelBMFont*>::const_iterator it = strengthLabels_.find(i->second);
		
		if (!it->second || it == strengthLabels_.end())
		{
			CCLabelBMFont* label = strengthLabelForUnitNode(i->second);
			strengthLabels_.insert(pair<const UnitNode*, CCLabelBMFont*>(i->second, label));

			labelsLayer_->addChild(label);
		}
		else
		{
			it->second->setPosition(labelPositionForUnitNode(i->second));
			
			int strength = strengthForUnitNode(i->second);
			if (strength)
			{
				it->second->setString(CCString::createWithFormat("%d", strength)->getCString());
			}
			else
				it->second->setVisible(false);
		}
	}
	
	//delete labels
	for (std::map<const UnitNode*,CCLabelBMFont*>::const_iterator i=strengthLabels_.begin(), e=strengthLabels_.end();
		i!=e; ++i)
	{
		bool delete_flag = true;
		
		for (std::map<const Unit*,UnitNode*>::const_iterator i2=gameFieldNode_->unitNodes().begin(), e2=gameFieldNode_->unitNodes().end();
		i2!=e2; ++i2)
		{
			if (i->first == i2->second && i->second->getTag() == iconTagForUnitNode(i2->second))
			{
				delete_flag = false;
			}
			
			if (!delete_flag)
				break;
		}
		
		if (delete_flag)
		{
			i->second->removeFromParentAndCleanup(true);
			strengthLabels_.erase(i->first);
		}
	}
}


CCPoint StrengthIndicatorsLayer::labelPositionForUnitNode(UnitNode* unit_node)
{
	return CCPoint(unit_node->getPosition().x, unit_node->getPosition().y +
		((iconTagForUnitNode(unit_node) == 3 && unit_node->unit()->unitType()->isLarge() && strengthForUnitNode(unit_node) > 10 ) ? ct(2) : ct(1)) );
}


CCLabelBMFont* StrengthIndicatorsLayer::strengthLabelForUnitNode(UnitNode* unit_node)
{
	int strength = strengthForUnitNode(unit_node);
	
	CCLabelBMFont* label = CCLabelBMFont::create(createWithFormat("%d", 0).c_str(),
		//cocos2d_utils::textureName(  (iconTagForUnitNode(unit_node) == 3 && !unit_node->unit()->unitType()->isLarge()) ? "Fonts/strength_points_big.fnt" : "Fonts/strength_points_big.fnt").c_str());
		cocos2d_utils::textureName("Fonts/strength_points.fnt").c_str());
		
	label->setPosition(labelPositionForUnitNode(unit_node));
	label->setString(CCString::createWithFormat("%d", strength)->getCString());
	label->setTag(iconTagForUnitNode(unit_node));
	
	if (!strength)
		label->setVisible(false);
	
	return label;
}


int StrengthIndicatorsLayer::iconTagForUnitNode(UnitNode* unit_node)
{
	if (unit_node->unit()->isWall())
	{
		return 1;
	}
	else if (unit_node->unit()->attackFormation() && !unit_node->unit()->isAbsorbed() && unit_node->unit()->indexInAttackFormation() == 0 )
	{
		return 2;
	}
	else if (!unit_node->unit()->attackFormation() && !unit_node->unit()->isAbsorbed())
	{
		return 3;
	}
	else
		return 0;
}


CCSprite* StrengthIndicatorsLayer::strengthIconForUnitNode(UnitNode* unit_node)
{
	CCSprite* sprite;
	
	if (unit_node->unit()->isWall())
	{
		sprite = CCSprite::createWithSpriteFrameName("icon_strength_wall.png");
	}
	else if (unit_node->unit()->attackFormation() && !unit_node->unit()->isAbsorbed() && unit_node->unit()->indexInAttackFormation() == 0 )
	{
		sprite = CCSprite::createWithSpriteFrameName("icon_strength_circle.png");
	}
	else if (!unit_node->unit()->attackFormation() && !unit_node->unit()->isAbsorbed())
	{
		sprite = CCSprite::createWithSpriteFrameName("icon_strength_shield.png");
		
		if (!unit_node->unit()->unitType()->isLarge())
		{
			sprite->setScale(0.7);
		}
		else if (strengthForUnitNode(unit_node) < 10)
		{
			sprite->setScale(0.8);
		}
	}
	else
	{
		sprite = CCSprite::create();
	}
	
	sprite->setTag(iconTagForUnitNode(unit_node));
	sprite->setPosition(CCPoint(unit_node->getPosition().x, unit_node->getPosition().y));
	
	return sprite;
}

int StrengthIndicatorsLayer::strengthForUnitNode(UnitNode* unit_node)
{
	if (unit_node->unit()->isWall())
	{
		return unit_node->unit()->wallStrength();
	}
	else if (unit_node->unit()->attackFormation() && !unit_node->unit()->isAbsorbed() && unit_node->unit()->indexInAttackFormation() == 0 )
	{
		return unit_node->unit()->attackFormation()->strength();
	}
	else if (!unit_node->unit()->attackFormation())
	{
		return unit_node->unit()->unitType()->defensive();
	}
	
	return 0;
}


void StrengthIndicatorsLayer::fade(float time, bool fade_in)
{
	CCObject* child;
	CCArray* children = labelsLayer_->getChildren();
	
	CCARRAY_FOREACH(children, child)
	{
		if (CCLabelBMFont* node = dynamic_cast<CCLabelBMFont*>(child))
		{
			if (fade_in)
				node->runAction(CCFadeIn::create(time));
			else
				node->runAction(CCFadeOut::create(time));
		}
	}
	
	children = iconsLayer_->getChildren();
	
	CCARRAY_FOREACH(children, child)
	{
		if (CCSprite* node = dynamic_cast<CCSprite*>(child))
		{
			if (fade_in)
				node->runAction(CCFadeIn::create(time));
			else
				node->runAction(CCFadeOut::create(time));
		}
	}
}


void StrengthIndicatorsLayer::fadeIn()
{
	bgLayer_->runAction(CCFadeTo::create(0.25, 128));
	
	fade(0.25, true);
}


void StrengthIndicatorsLayer::fadeOut()
{
	bgLayer_->runAction(CCFadeTo::create(0.25, 0));

	fade(0.25, false);
}


