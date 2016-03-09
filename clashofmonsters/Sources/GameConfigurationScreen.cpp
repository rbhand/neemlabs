//
//  GameConfigurationScreen.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/7/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "GameConfigurationScreen.h"
#include "SoundManager.h"
#include "Session.h"
#include "CCAlertBox.h"
#include "GoogleAnalytics.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;


GameConfigurationScreen::GameConfigurationScreen() :
	alertOnlineBack_("Online Game",
		"Do you want to resign from the game completely or temporary quit to main menu?",
		"Main Menu", "Resign", this)
{
	
}

bool GameConfigurationScreen::init()
{
	if (!NavigationMenuScreen::init())
	{
		return false;
	}
	
	
	bg_ = CCLayer::create();
	
	CCSprite* bg_menu = CCSprite::create(cocos2d_utils::textureName("Textures/Menu-Background.jpg").c_str());
	bg_menu->setScale(getContentSize().height / bg_menu->getContentSize().height);
	bg_menu->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	bg_menu->setAnchorPoint(CCPoint(0.5, 0.5));
	bg_->addChild(bg_menu);
	
	
	chooseAvatarLayer_ = ChooseAvatarLayer::create();
	chooseAvatarLayer_->setDelegate(this);
	chooseAvatarLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	chooseAvatarLayer_->showNextButtonGroup();
	//chooseAvatarLayer_->setPosition(CCPoint(getPositionX(), getPositionY()));
	
	chooseSmallUnitsLayer_ = ChooseSmallUnitsLayer::create();
	chooseSmallUnitsLayer_->setDelegate(this);
	chooseSmallUnitsLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	//chooseSmallUnitsLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseSmallUnitsLayer_->showNextButtonGroup();
	chooseSmallUnitsLayer_->setPlayerCaption(" PLAYER 1 ");
	
	chooseSmallUnitsLayer2_ = ChooseSmallUnitsLayer::create();
	chooseSmallUnitsLayer2_->setDelegate(this);
	chooseSmallUnitsLayer2_->setAnchorPoint(CCPoint(0.5, 0.5));
	//chooseSmallUnitsLayer2_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseSmallUnitsLayer2_->showNextButtonGroup();
	chooseSmallUnitsLayer2_->setPlayerCaption(" PLAYER 2 ");
	
	chooseBigUnitsLayer_ = ChooseBigUnitsLayer::create();
	chooseBigUnitsLayer_->setDelegate(this);
	chooseBigUnitsLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	//chooseBigUnitsLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseBigUnitsLayer_->showPlayButtonGroup();
	chooseBigUnitsLayer_->setPlayerCaption(" PLAYER 1 ");
	
	chooseBigUnitsLayer2_ = ChooseBigUnitsLayer::create();
	chooseBigUnitsLayer2_->setDelegate(this);
	chooseBigUnitsLayer2_->setAnchorPoint(CCPoint(0.5, 0.5));
	//chooseBigUnitsLayer2_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseBigUnitsLayer2_->showPlayButtonGroup();
	chooseBigUnitsLayer2_->setPlayerCaption(" PLAYER 2 ");
	
	waitingForOpponentLayer_ = WaitingForOpponentLayer::create();
	waitingForOpponentLayer_->setAnchorPoint(CCPoint(0.5, 0.5));
	waitingForOpponentLayer_->setDelegate(this);
	
	addChild(bg_);
	addChild(chooseAvatarLayer_);
	addChild(chooseSmallUnitsLayer_);
	addChild(chooseBigUnitsLayer_);
	addChild(chooseSmallUnitsLayer2_);
	addChild(chooseBigUnitsLayer2_);
	addChild(waitingForOpponentLayer_);
	
	//onlineGame_ = 0;
	
	chooseAvatarLayer_->setPosition(CCPoint(getPositionX(), getPositionY()));
	chooseSmallUnitsLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseSmallUnitsLayer2_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseBigUnitsLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	chooseBigUnitsLayer2_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	waitingForOpponentLayer_->setPosition(CCPoint(getPositionX() + getContentSize().width, getPositionY()));
	
	CCArray* layers = CCArray::create();
	layers->addObject(chooseAvatarLayer_);
	layers->addObject(chooseSmallUnitsLayer_);
	layers->addObject(chooseSmallUnitsLayer2_);
	layers->addObject(chooseBigUnitsLayer_);
	layers->addObject(chooseBigUnitsLayer2_);
	layers->addObject(waitingForOpponentLayer_);
	
	loadingIndicatorLayer_ = LoadingIndicatorLayer::create();
	loadingIndicatorLayer_->setCaption("Resigning...");
	loadingIndicatorLayer_->setLinkedNodes(layers);
	addChild(loadingIndicatorLayer_);
	
	showLayer(chooseAvatarLayer_, false);
	
	return true;
}


void GameConfigurationScreen::onEnter()
{
	NavigationMenuScreen::onEnter();
	
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Game Configuration");
	
	if (!CCUserDefault::sharedUserDefault()->getIntegerForKey("tutorial_was_shown", 0))
	{
		delegate()->gameConfigurationScreenTutorial();
		CCUserDefault::sharedUserDefault()->setIntegerForKey("tutorial_was_shown", 1);
	}
	
	chooseSmallUnitsLayer_->setAvatar(units_->avatars().at((int) chooseAvatarLayer_->leftAvatarType()));
	chooseSmallUnitsLayer2_->setAvatar(units_->avatars().at((int) chooseAvatarLayer_->rightAvatarType()));
	
	chooseBigUnitsLayer_->setAvatar(units_->avatars().at((int) chooseAvatarLayer_->leftAvatarType()));
	chooseBigUnitsLayer2_->setAvatar(units_->avatars().at((int) chooseAvatarLayer_->rightAvatarType()));
	
	loadingIndicatorLayer_->hide();
}


void GameConfigurationScreen::onExit()
{
	NavigationMenuScreen::onExit();
	
//	if (onlineGame_ && onlineGame_->delegate() == this)
//		onlineGame_->setDelegate(0);
		
	loadingIndicatorLayer_->hide();
}

void GameConfigurationScreen::setType(GameConfigurationScreenType type)
{
	gameType_ = type;
	chooseAvatarLayer_->setType(gameType_);
	
	if (screenLogo_)
	{
		bg_->removeChild(screenLogo_, true);
	}
	
	switch (gameType_)
	{
		case GameConfigurationScreenType_SinglePlayer:
		{
			screenLogo_ = CCSprite::createWithSpriteFrameName("lb_single_player.png");
			chooseBigUnitsLayer_->showPlayButtonGroup();
			chooseSmallUnitsLayer_->setPlayerCaptionEnabled(false);
			chooseBigUnitsLayer_->setPlayerCaptionEnabled(false);
			
			showLayer(chooseAvatarLayer_, false);
		}
		break;
			
		case GameConfigurationScreenType_TwoPlayers:
		{
			screenLogo_ = CCSprite::createWithSpriteFrameName("lb_two_players.png");
			chooseBigUnitsLayer_->showNextButtonGroup();
			chooseSmallUnitsLayer_->setPlayerCaptionEnabled(true);
			chooseBigUnitsLayer_->setPlayerCaptionEnabled(true);
			
			showLayer(chooseAvatarLayer_, false);
		}
		break;
		
		case GameConfigurationScreenType_OnlineGame:
		{
//			screenLogo_ = CCSprite::createWithSpriteFrameName("lb_online_game.png");
//			chooseBigUnitsLayer_->showPlayButtonGroup();
//			chooseSmallUnitsLayer_->setPlayerCaptionEnabled(false);
//			chooseBigUnitsLayer_->setPlayerCaptionEnabled(false);
//			
//			//PlayerConfig config = onlineGame_->playerConfig(onlineGame_->userIndex());
//			if (onlineGame_->isUserPlayReady())
//			{
//				waitingForOpponentLayer_->setOpponent(onlineGame_->opponent());
//				showLayer(waitingForOpponentLayer_, false);
//			}
//			else
//			{
//				showLayer(chooseAvatarLayer_, false);
//			}
		}
		break;
	}
	
	screenLogo_->setAnchorPoint(CCPoint(0.5, 1));
	//screenLogo_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(20)));
	screenLogo_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(10)));
	bg_->addChild(screenLogo_);
}

//void GameConfigurationScreen::setOnlineGame(OnlineGame* game)
//{
//	if (onlineGame_ && onlineGame_->delegate() == this)
//		onlineGame_->setDelegate(0);
//	
//	Session::sharedSession().setActiveGame(game, Session::OperationCallback());
//
//	onlineGame_ = game;
//	onlineGame_->setDelegate(this);
//	setType(GameConfigurationScreenType_OnlineGame);
//	
//	PlayerConfig user_config = game->playerConfig(game->userIndex());
//	chooseAvatarLayer_->setLeftAvatarType(user_config.avatar() ? (ChooseAvatarArmyType)user_config.avatar()->index() :
//		(game->userIndex() == 0 ? ChooseAvatarArmyType_Pirates : ChooseAvatarArmyType_Ninjas), false);
//		
//	PlayerConfig opponent_config = game->playerConfig(game->opponentIndex());
//	chooseAvatarLayer_->setRightAvatarType(opponent_config.avatar() ? (ChooseAvatarArmyType)opponent_config.avatar()->index() :
//		(game->opponentIndex() == 0 ? ChooseAvatarArmyType_Pirates : ChooseAvatarArmyType_Ninjas), false);
//}

//void GameConfigurationScreen::gameConfigurationUpdated(OnlineGame* game)
//{
//	if (game == onlineGame_)
//	{
//		chooseAvatarLayer_->setRightAvatarType(game->playerConfig(game->opponentIndex()).avatar() ?
//			((ChooseAvatarArmyType)game->playerConfig(game->opponentIndex()).avatar()->index()) : ChooseAvatarArmyType_Ninjas,
//			true);
//	}
//}
//
//void GameConfigurationScreen::gameStartPlayingFailed(monsters::OnlineGame *game, const monsters::Error &error)
//{
//	CCAlertBox::show("Network Problems", error.description(), "Dismiss");
//	showLayer(chooseBigUnitsLayer_, true);
//	//chooseBigUnitsLayer_->hideLoadingIndicator();
//}
//
//void GameConfigurationScreen::gameStartedPlaying(OnlineGame* game)
//{
//	if (this->isRunning() && game->state() == OnlineGame::State_Playing)
//	{
//		game->setNotifyPlay(false);
//		delegate()->gameConfigurationScreenStartOnlineGame(game);
//	}
//}

void GameConfigurationScreen::chooseAvatarLayerNext()
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	showLayer(chooseSmallUnitsLayer_, true, false);
}

void GameConfigurationScreen::chooseAvatarLayerBack()
{
	if (gameType_ == GameConfigurationScreenType_OnlineGame)
	{
		SoundManager::sharedSoundManager()->playEffect("button_click");
		alertOnlineBack_.show();
	}
	else
	{
		delegate()->gameConfigurationScreenBackToMenu();
	}
}

//void GameConfigurationScreen::gameResigned(monsters::OnlineGame *game, const monsters::User &byUser)
//{
//	if (!byUser.isLocal())
//		CCAlertBox::show(byUser.name(), "Resigned from the game", "Dismiss");
//	
//	//loadingIndicatorLayer_->hide();
//	delegate()->gameConfigurationScreenBackToMenu();
//}
//
//void GameConfigurationScreen::gameResigningFailed(OnlineGame* game, const Error& error)
//{
//	loadingIndicatorLayer_->hide();
//	CCAlertBox::show("Network Problems", error.description(), "Dismiss");
//}

void GameConfigurationScreen::alertBoxDidDismiss(cocos2d::CCAlertBox *alertBox, int buttonIndex)
{
	if (alertBox == &alertOnlineBack_)
	{
//		if (buttonIndex != alertBox->cancelButtonIndex())
//		{
//			loadingIndicatorLayer_->show();
//			onlineGame_->resign();
//		}
//		else
		{
			delegate()->gameConfigurationScreenBackToMenu();
		}
	}
}

void GameConfigurationScreen::waitingForOpponentCancel()
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	alertOnlineBack_.show();
}

void GameConfigurationScreen::chooseAvatarLayerSelectAvatar(ChooseAvatarArmyType type, bool left_avatar_flag)
{
	if (left_avatar_flag)
	{
		if (gameType_ == GameConfigurationScreenType_OnlineGame)
		{
//			PlayerConfig p;
//			p.setAvatar(units_->avatars().at((int)type));
//			onlineGame_->setPlayerConfigForUser(p);
		}
	
		chooseSmallUnitsLayer_->setAvatar(units_->avatars().at( (int) type ));
		chooseBigUnitsLayer_->setAvatar(units_->avatars().at( (int) type ));
	}
	else
	{
		chooseSmallUnitsLayer2_->setAvatar(units_->avatars().at( (int) type ));
		chooseBigUnitsLayer2_->setAvatar(units_->avatars().at( (int) type ));
	}
}

void GameConfigurationScreen::chooseSmallUnitsLayerBack(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	
	if (gameType_ == GameConfigurationScreenType_SinglePlayer)
		showLayer(chooseAvatarLayer_, true, true);
	else if (sender == chooseSmallUnitsLayer_)
		showLayer(chooseAvatarLayer_, true, true);
	else
		showLayer(chooseBigUnitsLayer_, true, true);
}

void GameConfigurationScreen::chooseSmallUnitsLayerNext(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	
	if (gameType_ == GameConfigurationScreenType_SinglePlayer)
		showLayer(chooseBigUnitsLayer_, true, false);
	else if (sender == chooseSmallUnitsLayer_)
		showLayer(chooseBigUnitsLayer_, true, false);
	else
		showLayer(chooseBigUnitsLayer2_, true, false);
}

void GameConfigurationScreen::chooseBigUnitsLayerBack(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	
	if (gameType_ == GameConfigurationScreenType_SinglePlayer)
		showLayer(chooseSmallUnitsLayer_, true, true);
	else if (sender == chooseBigUnitsLayer_)
		showLayer(chooseSmallUnitsLayer_, true, true);
	else
		showLayer(chooseSmallUnitsLayer2_, true, true);
}

void GameConfigurationScreen::chooseBigUnitsLayerNext(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	
	if (sender == chooseBigUnitsLayer_ && gameType_ == GameConfigurationScreenType_TwoPlayers)
	{
		//next player small units
		showLayer(chooseSmallUnitsLayer2_, true, false);
	}
	else
	{
		//play
		if (gameType_ == GameConfigurationScreenType_SinglePlayer)
		{
			chooseSmallUnitsLayer2_->selectRandomUnits();
			chooseBigUnitsLayer2_->selectRandomUnits();
		}
		
		PlayerConfig p1;
		p1.setAvatar(units_->avatars().at((int)chooseAvatarLayer_->leftAvatarType()));
		p1.setLargeUnitAtIndex(0, p1.avatar()->largeUnits().at(chooseBigUnitsLayer_->firstUnitIndex()));
		p1.setLargeUnitAtIndex(1, p1.avatar()->largeUnits().at(chooseBigUnitsLayer_->secondUnitIndex()));
		p1.setNormalUnitAtIndex(0, p1.avatar()->normalUnits().at(chooseSmallUnitsLayer_->redUnitIndex()));
		p1.setNormalUnitAtIndex(1, p1.avatar()->normalUnits().at(chooseSmallUnitsLayer_->greenUnitIndex()));
		p1.setNormalUnitAtIndex(2, p1.avatar()->normalUnits().at(chooseSmallUnitsLayer_->blueUnitIndex()));
		
		PlayerConfig p2;
		if (gameType_ != GameConfigurationScreenType_OnlineGame)
		{
			p2.setAvatar(units_->avatars().at((int)chooseAvatarLayer_->rightAvatarType()));
			p2.setLargeUnitAtIndex(0, p2.avatar()->largeUnits().at(chooseBigUnitsLayer2_->firstUnitIndex()));
			p2.setLargeUnitAtIndex(1, p2.avatar()->largeUnits().at(chooseBigUnitsLayer2_->secondUnitIndex()));
			p2.setNormalUnitAtIndex(0, p2.avatar()->normalUnits().at(chooseSmallUnitsLayer2_->redUnitIndex()));
			p2.setNormalUnitAtIndex(1, p2.avatar()->normalUnits().at(chooseSmallUnitsLayer2_->greenUnitIndex()));
			p2.setNormalUnitAtIndex(2, p2.avatar()->normalUnits().at(chooseSmallUnitsLayer2_->blueUnitIndex()));
		}
		
		if (gameType_ == GameConfigurationScreenType_SinglePlayer)
		{
			delegate_->gameConfigurationScreenStartSinglePlayerGame(p1, p2);
		}
		else if (gameType_ == GameConfigurationScreenType_TwoPlayers)
		{
			delegate_->gameConfigurationScreenStartTwoPlayersGame(p1, p2);
		}
		else if (gameType_ == GameConfigurationScreenType_OnlineGame)
		{
//			onlineGame_->startPlaying(p1);
//			waitingForOpponentLayer_->setOpponent(onlineGame_->opponent());
//			showLayer(waitingForOpponentLayer_, true);
		}
	}
}
