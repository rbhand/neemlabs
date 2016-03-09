//
//  ChooseSmallUnitsLayer.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "ChooseSmallUnitsLayer.h"
#include "SmallUnitMenuNode.h"
#include "SoundManager.h"
#include "Session.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;


bool ChooseSmallUnitsLayer::init()
{
	if (!NavigationMenuLayer::init(NavigationMenuLayerLayout_Internal))
	{
		return false;
	}
	
	unitsButtonsBaseY_ = getContentSize().height * 0.5 - ct(30);
	
	unitsMenu_ = CCMenu::create();
	unitsMenu_->setPosition(CCPoint(0, 0));
	unitsCaptions_ = CCLayer::create();
	
	CCLabelTTF* lb_title = CCLabelTTF::create(" CHOOSE YOUR TROOP UNITS ", "BadaBoom BB", ct(22.0));
	lb_title->setAnchorPoint(CCPoint(0.5, 1));
	lb_title->setColor(ccc3(52, 87, 113));
	//lb_title->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(85)));
	lb_title->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(65)));
	
	CCLabelTTF* lb_title2 = CCLabelTTF::create(" FOR EACH COLOR ", "BadaBoom BB", ct(22.0));
	lb_title2->setAnchorPoint(CCPoint(0.5, 1));
	lb_title2->setColor(ccc3(52, 87, 113));
	lb_title2->setPosition(CCPoint(getContentSize().width * 0.5, lb_title->getPositionY() - lb_title->getContentSize().height * 1.1));
	
	playerCaption_ = CCLabelTTF::create(" PLAYER ", "BadaBoom BB", ct(22.0));
	playerCaption_->setAnchorPoint(CCPoint(0.5, 1));
	playerCaption_->setColor(ccc3(255, 255, 255));
	//playerCaption_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(135)));
	playerCaption_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(115)));
		
	btnNext()->setPosition(CCPoint(btnNext()->getPosition().x, btnNext()->getContentSize().height * 0.5));
	btnBack()->setPosition(CCPoint(btnBack()->getPosition().x, btnBack()->getContentSize().height * 0.5));
	
	addChild(lb_title);
	addChild(lb_title2);
	addChild(playerCaption_);
	addChild(unitsMenu_);
	addChild(unitsCaptions_);
	
	return true;
}

void ChooseSmallUnitsLayer::cancelButtonPressed()
{
	delegate_->chooseSmallUnitsLayerBack(this);
}

void ChooseSmallUnitsLayer::playButtonPressed()
{
	delegate_->chooseSmallUnitsLayerNext(this);
}

void ChooseSmallUnitsLayer::backButtonPressed()
{
	delegate_->chooseSmallUnitsLayerBack(this);
}

void ChooseSmallUnitsLayer::nextButtonPressed()
{
	delegate_->chooseSmallUnitsLayerNext(this);
}

void ChooseSmallUnitsLayer::setPlayerCaptionEnabled(bool enabled)
{
	playerCaption_->setVisible(enabled);
}

void ChooseSmallUnitsLayer::setPlayerCaption(const char* caption)
{
	playerCaption_->setString(caption);
}

void ChooseSmallUnitsLayer::setAvatar(Avatar* avatar)
{
	avatar_ = avatar;
	
	unitsMenu_->removeAllChildrenWithCleanup(true);
	unitsCaptions_->removeAllChildrenWithCleanup(true);
	
	redButtons_.clear();
	greenButtons_.clear();
	blueButtons_.clear();
	
	redUnitIndex_ = 0;
	greenUnitIndex_ = 0;
	blueUnitIndex_ = 0;
	
	for (std::vector<UnitType*>::const_iterator i=(*avatar_).normalUnits().begin(),
		e=(*avatar_).normalUnits().end(); i!=e; ++i)
	{
		if (!(*i)->isLarge())
		{
			SmallUnitMenuNode* menu_caption = new SmallUnitMenuNode();
			menu_caption->init(CCString::createWithFormat("menu_avatar_%d_big_%d.png",
					(*i)->avatar()->index(),
					(*i)->index())->getCString(), *i);
			menu_caption->setTag((*i)->index());
			menu_caption->setAnchorPoint(CCPoint(0.5, 0.5));
			
			CCMenuItemSprite* menu_item = menuItemSpriteHighlighted(
				CCString::createWithFormat("menu_avatar_%d_normal_%d_red.png",
					(*i)->avatar()->index(),
					(*i)->index())->getCString(),
				this, menu_selector(ChooseSmallUnitsLayer::selectRedUnit));
			menu_item->setTag((*i)->index());
			menu_item->setOpacity( ((*i)->index() == 0) ? 255 : 100 );
			menu_item->setAnchorPoint(CCPoint(0.5, 1));
			
			menu_item->setPosition(CCPoint(getContentSize().width * 0.2, unitsButtonsBaseY_ - menu_item->getContentSize().height * (*i)->index() * 1.1 + menu_item->getContentSize().height ));
			
			unitsMenu_->addChild(menu_item);
			redButtons_.push_back(menu_item);
			
			menu_item = menuItemSpriteHighlighted(
				CCString::createWithFormat("menu_avatar_%d_normal_%d_green.png",
					(*i)->avatar()->index(),
					(*i)->index())->getCString(),
				this, menu_selector(ChooseSmallUnitsLayer::selectGreenUnit));
			menu_item->setTag((*i)->index());
			menu_item->setOpacity( ((*i)->index() == 0) ? 255 : 100 );
			menu_item->setAnchorPoint(CCPoint(0.5, 1));
			
			menu_item->setPosition(CCPoint(getContentSize().width * 0.5, unitsButtonsBaseY_ - menu_item->getContentSize().height * (*i)->index() * 1.1 + menu_item->getContentSize().height ));
			
			unitsMenu_->addChild(menu_item);
			greenButtons_.push_back(menu_item);
						
			menu_item = menuItemSpriteHighlighted(
				CCString::createWithFormat("menu_avatar_%d_normal_%d_blue.png",
					(*i)->avatar()->index(),
					(*i)->index())->getCString(),
				this, menu_selector(ChooseSmallUnitsLayer::selectBlueUnit));
			menu_item->setTag((*i)->index());
			menu_item->setOpacity( ((*i)->index() == 0) ? 255 : 100 );
			menu_item->setAnchorPoint(CCPoint(0.5, 1));
			
			menu_item->setPosition(CCPoint(getContentSize().width * 0.8, unitsButtonsBaseY_ - menu_item->getContentSize().height * (*i)->index() * 1.1 + menu_item->getContentSize().height ));
			
			unitsMenu_->addChild(menu_item);
			blueButtons_.push_back(menu_item);
			
			
			if ((*i)->index() == 0)
				menu_caption->setPosition(CCPoint(getContentSize().width * 0.5, menu_item->getPosition().y + ct(16)));
			else
				menu_caption->setPosition(CCPoint(getContentSize().width * 0.5, menu_item->getPosition().y - menu_item->getContentSize().height - ct(16)));
			
			unitsCaptions_->addChild(menu_caption);
		}
	}
}


void ChooseSmallUnitsLayer::selectRandomUnits()
{
	redUnitIndex_ = rand() % avatar_->normalUnits().size();
	greenUnitIndex_ = rand() % avatar_->normalUnits().size();
	blueUnitIndex_ = rand() % avatar_->normalUnits().size();
	
	for (std::vector<CCMenuItemSprite*>::iterator i=(redButtons_).begin(),
		e=(redButtons_).end(); i!=e; ++i)
	{
		if ((*i)->getTag() == redUnitIndex_ )
			(*i)->setOpacity(255);
		else
			(*i)->setOpacity(100);
	}
	
	for (std::vector<CCMenuItemSprite*>::iterator i=(greenButtons_).begin(),
		e=(greenButtons_).end(); i!=e; ++i)
	{
		if ((*i)->getTag() == greenUnitIndex_ )
			(*i)->setOpacity(255);
		else
			(*i)->setOpacity(100);
	}
	
	for (std::vector<CCMenuItemSprite*>::iterator i=(blueButtons_).begin(),
		e=(blueButtons_).end(); i!=e; ++i)
	{
		if ((*i)->getTag() == blueUnitIndex_ )
			(*i)->setOpacity(255);
		else
			(*i)->setOpacity(100);
	}
}


void ChooseSmallUnitsLayer::selectRedUnit(CCObject* sender)
{
	for (std::vector<CCMenuItemSprite*>::iterator i=(redButtons_).begin(),
		e=(redButtons_).end(); i!=e; ++i)
	{
		if ((*i) == sender)
		{
			redUnitIndex_ = (*i)->getTag();
			(*i)->setOpacity(255);
		}
		else
			(*i)->setOpacity(100);
	}
}

void ChooseSmallUnitsLayer::selectGreenUnit(CCObject* sender)
{
	for (std::vector<CCMenuItemSprite*>::iterator i=(greenButtons_).begin(),
		e=(greenButtons_).end(); i!=e; ++i)
	{
		if ((*i) == sender)
		{
			greenUnitIndex_ = (*i)->getTag();
			(*i)->setOpacity(255);
		}
		else
			(*i)->setOpacity(100);
	}
}

void ChooseSmallUnitsLayer::selectBlueUnit(CCObject* sender)
{
	for (std::vector<CCMenuItemSprite*>::iterator i=(blueButtons_).begin(),
		e=(blueButtons_).end(); i!=e; ++i)
	{
		if ((*i) == sender)
		{
			blueUnitIndex_ = (*i)->getTag();
			(*i)->setOpacity(255);
		}
		else
			(*i)->setOpacity(100);
	}
}


/*void ChooseSmallUnitsLayer::onShow()
{
	
}

void ChooseSmallUnitsLayer::onHide()
{
	
}*/
