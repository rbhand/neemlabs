//
//  NavigationMenuLayer.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/11/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "NavigationMenuLayer.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;



//BASIC LAYER OF ONLINE GAME SCREEN
bool NavigationMenuLayer::init(NavigationMenuLayerLayout layout)
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	menu_ = CCMenu::create();
	menu_->setPosition(CCPoint(0, 0));
	addChild(menu_, 10);
	
	if (layout == NavigationMenuLayerLayout_Internal)
	{
		btnBack_ = menuItemSpriteHighlighted("btn_back.png", this,
			menu_selector(NavigationMenuLayer::backButtonPressed));
		btnBack_->setPosition(CCPoint(getContentSize().width * 0.5 - btnBack_->getContentSize().width * 0.5 - ct(5), btnBack_->getContentSize().height));
		menu()->addChild(btnBack_);
		
		btnNext_ = menuItemSpriteHighlighted("btn_next.png", this,
			menu_selector(NavigationMenuLayer::nextButtonPressed));
		btnNext_->setPosition(CCPoint(getContentSize().width * 0.5 + btnNext_->getContentSize().width * 0.5 + ct(5), btnNext_->getContentSize().height));
		menu()->addChild(btnNext_);
		
		btnCancel_ = menuItemSpriteHighlighted("btn_back.png", this,
			menu_selector(NavigationMenuLayer::cancelButtonPressed));
		btnCancel_->setPosition(CCPoint(getContentSize().width * 0.5 - btnCancel_->getContentSize().width * 0.5 - ct(5), btnCancel_->getContentSize().height));
		menu()->addChild(btnCancel_);
		btnCancel_->setVisible(false);
		
		btnPlay_ = menuItemSpriteHighlighted("btn_play.png", this,
			menu_selector(NavigationMenuLayer::playButtonPressed));
		btnPlay_->setPosition(CCPoint(getContentSize().width * 0.5 + btnPlay_->getContentSize().width * 0.5 + ct(5), btnPlay_->getContentSize().height));
		menu()->addChild(btnPlay_);
		btnPlay_->setVisible(false);
	}
	//DEFAULT LAYOUT
	else
	{
		btnCancel_ = menuItemSpriteHighlighted("btn_cancel.png", this,
			menu_selector(NavigationMenuLayer::cancelButtonPressed));
		btnCancel_->setPosition(CCPoint(getContentSize().width * 0.5, btnCancel_->getContentSize().height));
		menu()->addChild(btnCancel_);
		btnCancel_->setVisible(false);
		
		btnPlay_ = menuItemSpriteHighlighted("btn_play.png", this,
			menu_selector(NavigationMenuLayer::playButtonPressed));
		btnPlay_->setPosition(CCPoint(getContentSize().width * 0.5 + btnPlay_->getContentSize().width * 0.5 + ct(5), btnPlay_->getContentSize().height));
		menu()->addChild(btnPlay_);
		btnPlay_->setVisible(false);
		btnPlay_->setOpacity(0);
	}
	
	loadingIndicator_ = CCSprite::createWithSpriteFrameName("ships_wheel.png");
	loadingIndicator_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	loadingIndicator_->setVisible(false);
	addChild(loadingIndicator_);
	
	return true;
}

void NavigationMenuLayer::onEnter()
{
	CCLayer::onEnter();
	if (loadingIndicator_->isVisible())
		loadingIndicator_->runAction(CCRepeatForever::create(CCRotateBy::create(3.2, 360)));
}

void NavigationMenuLayer::setMenuEnabled(bool enabled)
{
	menu_->setTouchEnabled(enabled);
}

void NavigationMenuLayer::disableMenu(float mseconds)
{
	setMenuEnabled(false);
	
	CCAction* enable_menu = CCSequence::createWithTwoActions(
		CCDelayTime::create(mseconds),
		CCCallFunc::create(this, callfunc_selector(NavigationMenuLayer::enableMenu))
	);
	
	this->runAction(enable_menu);
}

void NavigationMenuLayer::showPlayButtonGroup()
{
	if (!btnBack_)
		 return;
		 
	btnBack_->setVisible(false);
	btnNext_->setVisible(false);
	btnCancel_->setVisible(true);
	btnPlay_->setVisible(true);
}

void NavigationMenuLayer::showNextButtonGroup()
{
	if (!btnBack_)
		 return;
		 
	btnBack_->setVisible(true);
	btnNext_->setVisible(true);
	btnCancel_->setVisible(false);
	btnPlay_->setVisible(false);
}

void NavigationMenuLayer::showPlayButton(bool lock_menu)
{
	if (btnPlay_->getOpacity() != 0)
		return;
	
	if (lock_menu)
		setMenuEnabled(false);
	
	btnPlay_->setOpacity(0);
	btnPlay_->setVisible(true);
	
	CCAction* show_button = CCSequence::createWithTwoActions(
		CCDelayTime::create(0.15),
		CCFadeIn::create(0.15) );
		
	CCAction* move_button = CCMoveTo::create(0.15, CCPoint(getContentSize().width * 0.5 - btnCancel_->getContentSize().width * 0.5 - ct(5), btnCancel_->getPositionY() ) );
	
	CCAction* enable_menu = CCSequence::createWithTwoActions(
		CCDelayTime::create(0.32),
		CCCallFunc::create(this, callfunc_selector(NavigationMenuLayer::enableMenu ) ) );
		
		
	btnCancel_->runAction(move_button);
	btnPlay_->runAction(show_button);
	
	if (lock_menu)
		this->runAction(enable_menu);
}

void NavigationMenuLayer::hidePlayButton(bool lock_menu)
{
	if (btnPlay_->getOpacity() != 255)
		return;
	
	if (lock_menu)
		setMenuEnabled(false);
	
	CCAction* hide_button = CCFadeOut::create(0.15);
	
	CCAction* move_button =CCSequence::createWithTwoActions(
		CCDelayTime::create(0.15),
		CCMoveTo::create(0.15, CCPoint(getContentSize().width * 0.5, btnCancel_->getPositionY() )) );
	
	CCAction* enable_menu = CCSequence::createWithTwoActions(
		CCDelayTime::create(0.32),
		CCCallFunc::create(this, callfunc_selector(NavigationMenuLayer::enableMenu ) ) );
	
	btnPlay_->runAction(hide_button);
	btnCancel_->runAction(move_button);
	
	if (lock_menu)
		this->runAction(enable_menu);
}

void NavigationMenuLayer::showPlayButtonWithoutLock()
{
	showPlayButton(false);
}

void NavigationMenuLayer::hidePlayButtonWithoutLock()
{
	hidePlayButton(false);
}

void NavigationMenuLayer::showLoadingIndicator()
{
	loadingIndicator_->setVisible(true);
	loadingIndicator_->runAction(CCRepeatForever::create(CCRotateBy::create(3.2, 360)));
}

void NavigationMenuLayer::hideLoadingIndicator()
{
	loadingIndicator_->setVisible(false);
	loadingIndicator_->stopAllActions();
}

void NavigationMenuLayer::setActionNextEnabled(bool enabled)
{
	if (btnNext_)
	{
		if (btnNext_->isVisible())
		{
			btnNext_->setEnabled(enabled);
			btnNext_->setOpacity((enabled) ? 255 : 100);
		}
	}
	
	if (btnPlay_)
	{
		if (btnPlay_->isVisible())
		{
			btnPlay_->setEnabled(enabled);
			btnPlay_->setOpacity((enabled) ? 255 : 100);
		}
	}
}



bool NavigationMenuScreen::init()
{
	if (!CCScene::init())
	{
		return false;
	}
	
	currentLayer_ = 0;
	return true;
}

void NavigationMenuScreen::showLayer(NavigationMenuLayer* layer, bool animated, bool reversed, float time, float offset)
{
	if (layer == currentLayer_)
		return;

	layer->onShow();
	if (currentLayer_)
		currentLayer_->onHide();
	
	if (!animated)
	{
		if (currentLayer_)
		{
			currentLayer_->setPosition(CCPoint(-getContentSize().width - offset, currentLayer_->getPositionY()));
			currentLayer_->onHideDone();
		}
		
		layer->setPosition(CCPoint(getPositionX(), getPositionY()));
		
		currentLayer_ = layer;
		currentLayer_->setMenuEnabled(true);
		currentLayer_->onShowDone();
	}
	else
	{
		if (currentLayer_)
			currentLayer_->setMenuEnabled(false);
		layer->setMenuEnabled(false);
		
		CCAction* hide_layer;
		CCAction* show_layer;
		
		if (reversed)
		{
			layer->setPosition(CCPoint(getPositionX() - getContentSize().width, getPositionY()));
			hide_layer = CCMoveTo::create(time, CCPoint(getPositionX() + getContentSize().width + offset, getPositionY()));
			show_layer = CCMoveTo::create(time, CCPoint(getPositionX(), getPositionY()));
		}
		else
		{
			layer->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
			hide_layer = CCMoveTo::create(time, CCPoint(getPositionX() - getContentSize().width - offset, getPositionY()));
			show_layer = CCMoveTo::create(time, CCPoint(getPositionX(), getPositionY()));
		}
		
		if (currentLayer_)
			currentLayer_->runAction(hide_layer);
		
		layer->runAction(show_layer);
		lastLayer_ = currentLayer_;
		currentLayer_ = layer;
		
		currentLayer_->runAction(
			CCSequence::createWithTwoActions(
				CCDelayTime::create(time),
				CCCallFunc::create(this, callfunc_selector(NavigationMenuScreen::enableCurrentLayer))
			)
		);
		
		if (lastLayer_)
		{
			lastLayer_->runAction(
				CCSequence::createWithTwoActions(
					CCDelayTime::create(time),
					CCCallFunc::create(this, callfunc_selector(NavigationMenuScreen::hideDoneLastLayer))
				)
			);
		}
	}
}

void NavigationMenuScreen::enableCurrentLayer()
{
	currentLayer_->onShowDone();
	currentLayer_->setMenuEnabled(true);
}

void NavigationMenuScreen::hideDoneLastLayer()
{
	lastLayer_->onHideDone();
}







