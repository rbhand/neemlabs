//
//  WaitingForOpponentLayer.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/19/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "WaitingForOpponentLayer.h"

using namespace monsters;
using namespace cocos2d;

bool WaitingForOpponentLayer::init()
{
	if (!NavigationMenuLayer::init())
	{
		return false;
	}
	
	btnCancel()->setVisible(true);
	
	CCLabelTTF* lb_subtitle = CCLabelTTF::create(" WAITING FOR ", "BadaBoom BB", ct(22.0));
	lb_subtitle->setAnchorPoint(CCPoint(0.5, 1));
	lb_subtitle->setColor(ccc3(52, 87, 113));
	lb_subtitle->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.8));
	
	outOpponent_ = CCLabelTTF::create("", "BadaBoom BB", ct(22.0));
	outOpponent_->setAnchorPoint(CCPoint(0.5, 1));
	outOpponent_->setColor(ccc3(52, 87, 113));
	outOpponent_->setPosition(CCPoint(getContentSize().width * 0.5, lb_subtitle->getPositionY() - lb_subtitle->getContentSize().height * 1.1));
		
	addChild(lb_subtitle);
	addChild(outOpponent_);
	
	return true;
}

void WaitingForOpponentLayer::setOpponent(const User& opponent)
{
	outOpponent_->setString((" " + opponent.name() + " ").c_str());
}

void WaitingForOpponentLayer::onShow()
{
	this->showLoadingIndicator();
}

void WaitingForOpponentLayer::onHide()
{
	this->hideLoadingIndicator();
	
	this->stopAllActions();
	hidePlayButtonWithoutLock();
}

void WaitingForOpponentLayer::cancelButtonPressed()
{
	delegate_->waitingForOpponentCancel();
}
