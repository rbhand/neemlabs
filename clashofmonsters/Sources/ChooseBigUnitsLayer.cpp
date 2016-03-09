//
//  ChooseBigUnitsLayer.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "ChooseBigUnitsLayer.h"
#include "SoundManager.h"
#include "LargeUnitMenuItem.h"
#include "Session.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;

bool ChooseBigUnitsLayer::init()
{
	if (!NavigationMenuLayer::init(NavigationMenuLayerLayout_Internal))
	{
		return false;
	}
	
	unitsButtonsBaseY_ = getContentSize().height * 0.5 - ct(30);
	
	unitsMenu_ = CCMenu::create();
	unitsMenu_->setPosition(CCPoint(0, 0));
	
	CCLabelTTF* lb_title = CCLabelTTF::create(" CHOOSE YOUR CREATURES ", "BadaBoom BB", ct(22.0));
	lb_title->setAnchorPoint(CCPoint(0.5, 1));
	lb_title->setColor(ccc3(52, 87, 113));
	//lb_title->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(85)));
	lb_title->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(65)));
	
	playerCaption_ = CCLabelTTF::create(" PLAYER ", "BadaBoom BB", ct(22.0));
	playerCaption_->setAnchorPoint(CCPoint(0.5, 1));
	playerCaption_->setColor(ccc3(255, 255, 255));
	//playerCaption_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(135)));
	playerCaption_->setPosition(CCPoint(getContentSize().width * 0.5, lb_title->getPositionY() - lb_title->getContentSize().height * 1.1));
	
	btnNext()->setPosition(CCPoint(btnNext()->getPosition().x, btnNext()->getContentSize().height * 0.5));
	btnBack()->setPosition(CCPoint(btnBack()->getPosition().x, btnBack()->getContentSize().height * 0.5));
	btnCancel()->setPosition(CCPoint(btnCancel()->getPosition().x, btnCancel()->getContentSize().height * 0.5));
	btnPlay()->setPosition(CCPoint(btnPlay()->getPosition().x, btnPlay()->getContentSize().height * 0.5));
	
	addChild(lb_title);
	addChild(playerCaption_);
	addChild(unitsMenu_);
	
	return true;
}

void ChooseBigUnitsLayer::cancelButtonPressed()
{
	delegate_->chooseBigUnitsLayerBack(this);
}

void ChooseBigUnitsLayer::playButtonPressed()
{
	delegate_->chooseBigUnitsLayerNext(this);
}

void ChooseBigUnitsLayer::backButtonPressed()
{
	delegate_->chooseBigUnitsLayerBack(this);
}

void ChooseBigUnitsLayer::nextButtonPressed()
{
	delegate_->chooseBigUnitsLayerNext(this);
}

void ChooseBigUnitsLayer::setPlayerCaptionEnabled(bool enabled)
{
	playerCaption_->setVisible(enabled);
}

void ChooseBigUnitsLayer::setPlayerCaption(const char* caption)
{
	playerCaption_->setString(caption);
}

void ChooseBigUnitsLayer::setAvatar(Avatar* avatar)
{
	avatar_ = avatar;
	
	unitsMenu_->removeAllChildrenWithCleanup(true);
	
	unitsButtons_.clear();
	
	firstUnitIndex_ = 0;
	secondUnitIndex_ = 1;
	
	setActionNextEnabled(false);
	
	int position_x = 0, position_y = ct(117);
	position_y += cocos2d_utils::isIPhone() ? 0 : 20;
	
	for (std::vector<UnitType*>::const_iterator i=(*avatar_).largeUnits().begin(),
		e=(*avatar_).largeUnits().end(); i!=e; ++i)
	{
		if ((*i)->isLarge())
		{
			LargeUnitMenuItem* menu_item = new LargeUnitMenuItem();
			menu_item->init(CCString::createWithFormat("menu_avatar_%d_big_%d.png",
					(*i)->avatar()->index(),
					(*i)->index())->getCString(), *i,this, menu_selector(ChooseBigUnitsLayer::selectUnit));
			menu_item->setTag((*i)->index());
			menu_item->setOpacity(100);
			menu_item->setAnchorPoint(CCPoint(0.5, 1));
			
			position_x = getContentSize().width * 0.5;
			
			menu_item->setPosition(CCPoint(position_x, getContentSize().height - position_y ));
			
			menu_item->setLocked(Session::sharedSession().experiencePoints() < (*i)->cost());
			
			unitsMenu_->addChild(menu_item);
			unitsButtons_.push_back(menu_item);
			
			position_y += menu_item->getContentSize().height + ct(4);
		}
	}
	
	this->updateUnitsButtons();
}

void ChooseBigUnitsLayer::selectRandomUnits()
{
	while (firstUnitIndex_ == -1 || secondUnitIndex_ == -1)
	{
		int temp_index = rand() % avatar_->largeUnits().size();
		
		UnitType* ut = avatar_->largeUnits().at(temp_index);
		if (ut->cost() <= Session::sharedSession().experiencePoints())
		{
			if (firstUnitIndex_ == -1)
			{
				firstUnitIndex_ = temp_index;
			}
			else if (firstUnitIndex_ != temp_index)
			{
				secondUnitIndex_ = temp_index;
			}
		}
	}
	
	for (std::vector<CCMenuItemSprite*>::iterator i=(unitsButtons_).begin(),
		e=(unitsButtons_).end(); i!=e; ++i)
	{
		if ((*i)->getTag() == firstUnitIndex_ || (*i)->getTag() == secondUnitIndex_)
			(*i)->setOpacity(255);
		else
			(*i)->setOpacity(100);
	}
	
	setActionNextEnabled(!(firstUnitIndex_ == -1 || secondUnitIndex_ == -1));
}

void ChooseBigUnitsLayer::selectUnit(CCObject* sender)
{
	for (std::vector<CCMenuItemSprite*>::iterator i=(unitsButtons_).begin(),
		e=(unitsButtons_).end(); i!=e; ++i)
	{
		if ((*i) == sender)
		{
			if (firstUnitIndex_ == (*i)->getTag())
			{
				firstUnitIndex_ = -1;
			}
			else if (secondUnitIndex_ == (*i)->getTag())
			{
				secondUnitIndex_ = -1;
			}
			else if (firstUnitIndex_ == -1)
			{
				firstUnitIndex_ = (*i)->getTag();
			}
			else if (secondUnitIndex_ == -1)
			{
				secondUnitIndex_ = (*i)->getTag();
			}
			else
			{
				firstUnitIndex_ = secondUnitIndex_;
				secondUnitIndex_ = (*i)->getTag();
			}
		}
	}
	
	this->updateUnitsButtons();
}

void ChooseBigUnitsLayer::updateUnitsButtons()
{
	for (std::vector<CCMenuItemSprite*>::iterator i=(unitsButtons_).begin(),
		e=(unitsButtons_).end(); i!=e; ++i)
	{
		GLbyte opacity = 0;
		
		if ((*i)->getTag() == firstUnitIndex_ || (*i)->getTag() == secondUnitIndex_)
		{
			opacity = 255;
		}
		else
		{
			opacity = 100;
		}
		
		(*i)->setOpacity(opacity);

		
		/*CCObject* child;
		CCArray* button_children = (*i)->getChildren();
		
		CCARRAY_FOREACH(button_children, child)
		{
			if (CCRGBAProtocol* node = dynamic_cast<CCRGBAProtocol*>(child))
			{
				node->setOpacity(opacity);
			}
		}*/
		
	}
	
	setActionNextEnabled(!(firstUnitIndex_ == -1 || secondUnitIndex_ == -1));
}


