//
//  OnlineGameScreen.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/5/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "OnlineGameScreen.h"
#include "Session.h"
#include "CCAlertBox.h"
#include "GoogleAnalytics.h"


using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;




//WAITING FOR OPPONENT LAYER
bool OnlineGameWaitingLayer::init()
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
	
	addChild(lb_subtitle);
	
	return true;
}

void OnlineGameWaitingLayer::onShow()
{
	this->showLoadingIndicator();
	
	this->runAction( CCSequence::createWithTwoActions(
		CCDelayTime::create(2.5),
		CCCallFunc::create(this, callfunc_selector(NavigationMenuLayer::showPlayButton)) ) );
}


void OnlineGameWaitingLayer::onHide()
{
	this->hideLoadingIndicator();
	
	this->stopAllActions();
	hidePlayButtonWithoutLock();
}

void OnlineGameWaitingLayer::cancelButtonPressed()
{
	delegate_->waitingLayerBackToMenu();
}

void OnlineGameWaitingLayer::playButtonPressed()
{
	
}




//RANDOM OPPONENT LAYER
bool OnlineGameRandomLayer::init()
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
	
	CCLabelTTF* lb_subtitle2 = CCLabelTTF::create(" SOME RANDOM PLAYER... ", "BadaBoom BB", ct(22.0));
	lb_subtitle2->setAnchorPoint(CCPoint(0.5, 1));
	lb_subtitle2->setColor(ccc3(52, 87, 113));
	lb_subtitle2->setPosition(CCPoint(getContentSize().width * 0.5, lb_subtitle->getPositionY() - lb_subtitle->getContentSize().height * 1.1));
		
	addChild(lb_subtitle);
	addChild(lb_subtitle2);
	
	return true;
}

void OnlineGameRandomLayer::onShow()
{
	this->showLoadingIndicator();
//	this->runAction( CCSequence::createWithTwoActions(
//		CCDelayTime::create(2.5),
//		CCCallFunc::create(this, callfunc_selector(NavigationMenuLayer::showPlayButton)) ) );
}

void OnlineGameRandomLayer::onHide()
{
	this->hideLoadingIndicator();
	
	this->stopAllActions();
	hidePlayButtonWithoutLock();
}

void OnlineGameRandomLayer::cancelButtonPressed()
{
	delegate_->randomLayerBackToMenu();
}

void OnlineGameRandomLayer::playButtonPressed()
{
	delegate()->randomLayerShowWaitingLayer();
}




//SELECT TYPE LAYER
bool OnlineGameSelectTypeLayer::init()
{
	if (!NavigationMenuLayer::init())
	{
		return false;
	}
	
	
	CCMenuItemSprite* btn_back = menuItemSpriteHighlighted("btn_back.png", this,
		menu_selector(OnlineGameSelectTypeLayer::back));
	btn_back->setPosition(CCPoint(getContentSize().width * 0.5, btn_back->getContentSize().height));
	menu()->addChild(btn_back);
	
	CCMenuItemSprite* btn_random = menuItemSpriteHighlighted("btn_random.png", this,
		menu_selector(OnlineGameSelectTypeLayer::random));
	btn_random->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.65));
	menu()->addChild(btn_random);
	
	CCMenuItemSprite* btn_find_by_name = menuItemSpriteHighlighted("btn_find_by_name.png", this,
		menu_selector(OnlineGameSelectTypeLayer::findByName));
	btn_find_by_name->setPosition(CCPoint(getContentSize().width * 0.5, btn_random->getPositionY() - ct(70)));
	menu()->addChild(btn_find_by_name);
	
	CCMenuItemSprite* btn_with_friend = menuItemSpriteHighlighted("btn_with_friend.png", this,
		menu_selector(OnlineGameSelectTypeLayer::withFriend));
	btn_with_friend->setPosition(CCPoint(getContentSize().width * 0.5, btn_find_by_name->getPositionY() - ct(70)));
	menu()->addChild(btn_with_friend);
	
	return true;
}

void OnlineGameSelectTypeLayer::random()
{
	delegate_->selectTypeLayerRandom();
}

void OnlineGameSelectTypeLayer::findByName()
{
	delegate_->selectTypeLayerFindByName();
}

void OnlineGameSelectTypeLayer::withFriend()
{
	delegate_->selectTypeLayerWithFriend();
}

void OnlineGameSelectTypeLayer::back()
{
	delegate_->selectTypeLayerBackToMenu();
}




//ONLINE GAME SCREEN
bool OnlineGameScreen::init()
{
	if (!NavigationMenuScreen::init())
	{
		return false;
	}
	
	
	CCLayer* bg = CCLayer::create();
	
	CCSprite* bg_menu = CCSprite::create(cocos2d_utils::textureName("Textures/Menu-Background.jpg").c_str());
	bg_menu->setScale(getContentSize().height / bg_menu->getContentSize().height);
	bg_menu->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	bg_menu->setAnchorPoint(CCPoint(0.5, 0.5));
	bg->addChild(bg_menu);
	
	CCSprite* logo = CCSprite::createWithSpriteFrameName("lb_online_game.png");
	logo->setAnchorPoint(CCPoint(0.5, 1));
	logo->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(20)));
	bg->addChild(logo);
	
	
	//creating layers
	selectTypeLayer_ = OnlineGameSelectTypeLayer::create();
	selectTypeLayer_->setDelegate(this);
	selectTypeLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	selectTypeLayer_->setPosition(CCPoint(getPositionX(), getPositionY()));
	
	randomOpponentLayer_ = OnlineGameRandomLayer::create();
	randomOpponentLayer_->setDelegate(this);
	randomOpponentLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	randomOpponentLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	
	findByNameLayer_ = OnlineGameFindByNameLayer::create();
	findByNameLayer_->setDelegate(this);
	findByNameLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	findByNameLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	
	withFriendLayer_ = OnlineGameWithFriendLayer::create();
	withFriendLayer_->setDelegate(this);
	withFriendLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	withFriendLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	
	waitingForOpponentLayer_ = OnlineGameWaitingLayer::create();
	waitingForOpponentLayer_->setDelegate(this);
	waitingForOpponentLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	waitingForOpponentLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	
	addChild(bg);
	addChild(selectTypeLayer_);
	addChild(randomOpponentLayer_);
	addChild(findByNameLayer_);
	addChild(withFriendLayer_);
	addChild(waitingForOpponentLayer_);
	
	showLayer(selectTypeLayer_, false);
	
	//currentLayer_ = selectTypeLayer_;
	
	return true;
}
/*
void OnlineGameScreen::showLayer(NavigationMenuLayer* layer, bool animated, bool reversed)
{
	layer->onShow();
	currentLayer_->onHide();
	
	if (!animated)
	{
		currentLayer_->setPosition(CCPoint(-getContentSize().width, currentLayer_->getPositionY()));
		layer->setPosition(CCPoint(getPositionX(), getPositionY()));
		currentLayer_ = layer;
		currentLayer_->setMenuEnabled(true);
	}
	else
	{
		currentLayer_->setMenuEnabled(false);
		layer->setMenuEnabled(false);
		
		CCAction* hide_layer;
		CCAction* show_layer;
		
		if (reversed)
		{
			layer->setPosition(CCPoint(getPositionX() - getContentSize().width, getPositionY()));
			hide_layer = CCMoveTo::create(0.25, CCPoint(getPositionX() + getContentSize().width, getPositionY()));
			show_layer = CCMoveTo::create(0.25, CCPoint(getPositionX(), getPositionY()));
		}
		else
		{
			layer->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
			hide_layer = CCMoveTo::create(0.25, CCPoint(getPositionX() - getContentSize().width, getPositionY()));
			show_layer = CCMoveTo::create(0.25, CCPoint(getPositionX(), getPositionY()));
		}
		
		currentLayer_->runAction(hide_layer);
		layer->runAction(show_layer);
		currentLayer_ = layer;
		
		currentLayer_->runAction(
			CCSequence::create(
				CCDelayTime::create(0.25),
				CCCallFunc::create(this, callfunc_selector(OnlineGameScreen::enableCurrentLayer))
			)
		);
	}
}


void OnlineGameScreen::enableCurrentLayer()
{
	currentLayer_->setMenuEnabled(true);
}
*/

void OnlineGameScreen::selectTypeLayerBackToMenu()
{
	delegate_->onlineGameScreenBackToMenu();
}

void OnlineGameScreen::onEnter()
{
	NavigationMenuScreen::onEnter();
	
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Online Game");
	
	if (Session::sharedSession().activeRandomGameInvite())
	{
		showLayer(randomOpponentLayer_, false, false);
	}
	else
	{
		showLayer(selectTypeLayer_, false, false);
	}
}

void OnlineGameScreen::onStartRandomGame(OnlineGame* game, const Error& error)
{
	if (!error.isOk())
	{
		showLayer(selectTypeLayer_, true, true);
		CCAlertBox::show("Random Game Failed", error.description(), "Dismiss");
	}
	else
	{
		delegate_->onlineGameScreenGameProceedToGame(game);
	}
}

void OnlineGameScreen::selectTypeLayerRandom()
{
//	randomOpponentLayer_->showLoadingIndicator();
	showLayer(randomOpponentLayer_, true, false);
	
	Session::sharedSession().startRandomGame(Session::StartRandomGameCallback(this,
		(Session::StartRandomGameCallbackSelector)&OnlineGameScreen::onStartRandomGame));
}

void OnlineGameScreen::selectTypeLayerFindByName()
{
	showLayer(findByNameLayer_, true, false);
}

void OnlineGameScreen::selectTypeLayerWithFriend()
{
	showLayer(withFriendLayer_, true, false);
}

void OnlineGameScreen::randomLayerBackToMenu()
{
	Session::sharedSession().cancelRandomGame();
	showLayer(selectTypeLayer_, true, true);
}

void OnlineGameScreen::randomLayerShowWaitingLayer()
{
	showLayer(waitingForOpponentLayer_, true, false);
}

void OnlineGameScreen::findByNameLayerBackToMenu()
{
	showLayer(selectTypeLayer_, true, true);
}

void OnlineGameScreen::withFriendLayerBackToMenu()
{
	showLayer(selectTypeLayer_, true, true);
}

void OnlineGameScreen::showWaitingForOpponentLayer()
{
	//showLayer(waitingForOpponentLayer_, true, false);
}

void OnlineGameScreen::waitingLayerBackToMenu()
{
	showLayer(selectTypeLayer_, true, true);
}

void OnlineGameScreen::waitingLayerGameConfigurationScreen()
{
	delegate()->onlineGameScreenGameConfigurationScreen();
}

OnlineGameScreen::~OnlineGameScreen()
{
	
}

void OnlineGameScreen::reloadData()
{
	findByNameLayer_->reloadData();
	withFriendLayer_->reloadData();
}

