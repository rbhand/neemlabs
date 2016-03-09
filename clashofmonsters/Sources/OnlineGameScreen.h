//
//  OnlineGameScreen.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/5/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "NavigationMenuLayer.h"
#include "OnlineGameWithFriendLayer.h"
#include "OnlineGameFindByNameLayer.h"

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	
	//WAITING FOR OPPONENT LAYER
	class IOnlineGameWaitingLayerDelegate
	{
	public:
		virtual void waitingLayerBackToMenu() = 0;
		virtual void waitingLayerGameConfigurationScreen() = 0;
	};
	
	class OnlineGameWaitingLayer : public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IOnlineGameWaitingLayerDelegate* delegate) { delegate_ = delegate; }
		IOnlineGameWaitingLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(OnlineGameWaitingLayer);
		
		//void showLoadingIndicator();
		//void hideLoadingIndicator();
		
		void onShow();
		void onHide();
		
	private:
		void cancelButtonPressed();
		void playButtonPressed();
		
	private:
		IOnlineGameWaitingLayerDelegate* delegate_;
	};
	
	
	
	//RANDOM OPPONENT LAYER
	class IOnlineGameRandomLayerDelegate
	{
	public:
		virtual void randomLayerBackToMenu() = 0;
		virtual void randomLayerShowWaitingLayer() = 0;
	};
	
	
	class OnlineGameRandomLayer : public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IOnlineGameRandomLayerDelegate* delegate) { delegate_ = delegate; }
		IOnlineGameRandomLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(OnlineGameRandomLayer);
		
		//void showLoadingIndicator();
		//void hideLoadingIndicator();
		
		void onShow();
		void onHide();
		
	private:
		void cancelButtonPressed();
		void playButtonPressed();
		
	private:
		IOnlineGameRandomLayerDelegate* delegate_;
		
		//CCSprite* loadingIndicator_;
	};
	
	
	
	
	//SELECT TYPE LAYER
	class IOnlineGameSelectTypeLayerDelegate
	{
	public:
		virtual void selectTypeLayerBackToMenu() = 0;
		virtual void selectTypeLayerRandom() = 0;
		virtual void selectTypeLayerFindByName() = 0;
		virtual void selectTypeLayerWithFriend() = 0;
	};
	
	
	class OnlineGameSelectTypeLayer : public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IOnlineGameSelectTypeLayerDelegate* delegate) { delegate_ = delegate; }
		IOnlineGameSelectTypeLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(OnlineGameSelectTypeLayer);
		
	private:
		void random();
		void findByName();
		void withFriend();
		void back();
		
		IOnlineGameSelectTypeLayerDelegate* delegate_;
		
	};
	
	
	
	
	//ONLINE GAME SCREEN
	class IOnlineGameScreenDelegate
	{
	public:
		virtual void onlineGameScreenBackToMenu() = 0;
		virtual void onlineGameScreenGameConfigurationScreen() = 0;
		virtual void onlineGameScreenGameProceedToGame(OnlineGame* game) = 0;
	};
	
	
	class OnlineGameScreen : public NavigationMenuScreen,
		private IOnlineGameSelectTypeLayerDelegate,
		private IOnlineGameRandomLayerDelegate,
		private IOnlineGameFindByNameLayerDelegate,
		private IOnlineGameWithFriendLayerDelegate,
		private IOnlineGameWaitingLayerDelegate
	{
	public:
		virtual bool init();
		virtual ~OnlineGameScreen();
	
		void setDelegate(IOnlineGameScreenDelegate* delegate) { delegate_ = delegate; }
		IOnlineGameScreenDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(OnlineGameScreen);
		
		void selectTypeLayerBackToMenu();
		void selectTypeLayerRandom();
		void selectTypeLayerFindByName();
		void selectTypeLayerWithFriend();
		void showWaitingForOpponentLayer();
		
		void randomLayerBackToMenu();
		void randomLayerShowWaitingLayer();
		
		void findByNameLayerBackToMenu();
		
		void withFriendLayerBackToMenu();
		
		void waitingLayerBackToMenu();
		void waitingLayerGameConfigurationScreen();
		
		void reloadData();
	
	/*private:
		void showLayer(NavigationMenuLayer* layer, bool animated = false, bool reversed = false);
		void enableCurrentLayer();*/
		
	private:
		void onStartRandomGame(OnlineGame* game, const Error& error);
		void onEnter();
	
	private:
		IOnlineGameScreenDelegate* delegate_;
		
		//NavigationMenuLayer* currentLayer_;
		OnlineGameSelectTypeLayer* selectTypeLayer_;
		OnlineGameRandomLayer* randomOpponentLayer_;
		OnlineGameFindByNameLayer* findByNameLayer_;
		OnlineGameWithFriendLayer* withFriendLayer_;
		OnlineGameWaitingLayer* waitingForOpponentLayer_;
	};
}


