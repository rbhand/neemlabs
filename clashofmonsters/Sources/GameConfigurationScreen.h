//
//  GameConfigurationScreen.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/7/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

#include "Utils.h"
#include "GameUnits.h"
#include "Game.h"
//#include "OnlineGame.h"

#include "NavigationMenuLayer.h"
#include "CCAlertBox.h"

#include "GameConfigurationScreenTypes.h"
#include "ChooseAvatarLayer.h"
#include "ChooseBigUnitsLayer.h"
#include "ChooseSmallUnitsLayer.h"
#include "WaitingForOpponentLayer.h"
#include "LoadingIndicatorLayer.h"

namespace monsters
{
	class IGameConfigurationScreenDelegate
	{
	public:
		virtual void gameConfigurationScreenBackToMenu() = 0;
		virtual void gameConfigurationScreenStartSinglePlayerGame(const PlayerConfig& player1Config, const PlayerConfig& aiConfig) = 0;
		virtual void gameConfigurationScreenStartTwoPlayersGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config) = 0;
//		virtual void gameConfigurationScreenStartOnlineGame(OnlineGame* game) = 0;
		virtual void gameConfigurationScreenTutorial() = 0;
	};
	
	class GameConfigurationScreen : public NavigationMenuScreen,
		//private IOnlineGameDelegate,
		private IChooseAvatarLayerDelegate,
		private IChooseSmallUnitsLayerDelegate,
		private IChooseBigUnitsLayerDelegate,
		private IWaitingForOpponentLayerDelegate,
		private ICCAlertBoxDelegate
	{
	public:
		GameConfigurationScreen();
	
		virtual bool init();
		
		void setDelegate(IGameConfigurationScreenDelegate* delegate) { delegate_ = delegate; }
		IGameConfigurationScreenDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(GameConfigurationScreen);
		
//		void setOnlineGame(OnlineGame* game);
		void setType(GameConfigurationScreenType type);
		void setUnits(const GameUnits* units) { units_ = units; };
		
		GameConfigurationScreenType type() const { return gameType_; }
		
	private:
		void chooseAvatarLayerNext();
		void chooseAvatarLayerBack();
		void chooseAvatarLayerSelectAvatar(ChooseAvatarArmyType type, bool left_avatar_flag);
		
		void chooseSmallUnitsLayerBack(CCObject* sender);
		void chooseSmallUnitsLayerNext(CCObject* sender);
		
		void chooseBigUnitsLayerBack(CCObject* sender);
		void chooseBigUnitsLayerNext(CCObject* sender);
	
		void onEnter();
		void onExit();
		
//		void gameConfigurationUpdated(OnlineGame* game);
//		void gameStartedPlaying(OnlineGame* game);
//		void gameStartPlayingFailed(monsters::OnlineGame *game, const monsters::Error &error);
//		void gameResigned(monsters::OnlineGame *game, const monsters::User &byUser);
//		void gameResigningFailed(OnlineGame* game, const Error& error);
		
		void waitingForOpponentCancel();
		
		void alertBoxDidDismiss(cocos2d::CCAlertBox *alertBox, int buttonIndex);
	
	private:
		IGameConfigurationScreenDelegate* delegate_;
		const GameUnits* units_;
//		OnlineGame* onlineGame_;
		
		ChooseAvatarLayer* chooseAvatarLayer_;
		ChooseSmallUnitsLayer* chooseSmallUnitsLayer_;
		ChooseSmallUnitsLayer* chooseSmallUnitsLayer2_;
		ChooseBigUnitsLayer* chooseBigUnitsLayer_;
		ChooseBigUnitsLayer* chooseBigUnitsLayer2_;
		WaitingForOpponentLayer* waitingForOpponentLayer_;
		
		GameConfigurationScreenType gameType_;
		ChooseAvatarArmyType player1Army_;
		ChooseAvatarArmyType player2Army_;
		
		LoadingIndicatorLayer* loadingIndicatorLayer_;
		
		CCAlertBox alertOnlineBack_;
		
		CCLayer* bg_;
		CCSprite* screenLogo_;
	};
}


