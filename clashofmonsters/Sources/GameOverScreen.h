//
//  GameOverScreen.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/19/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "NavigationMenuLayer.h"
#include "Game.h"
//#include "OnlineGame.h"
#include "GameUnits.h"
#include "Unit.h"
#include "Player.h"
#include "FacebookSharing.h"

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	class IGameOverScreenDelegate
	{
	public:
		virtual void gameOverScreenBackToMenu() = 0;
		virtual void gameOverScreenReplay() = 0;
	//	virtual void gameOverScreenPostToFacebook() = 0;
	};
	
	
	class GameOverScreen : public CCScene
	{
	public:
		virtual bool init();
		
		void setDelegate(IGameOverScreenDelegate* delegate) { delegate_ = delegate; }
		IGameOverScreenDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(GameOverScreen);
		
		void setGame(Game* game);
		//void setOnlineGame(OnlineGame* onlineGame);
		
	private:
		void onEnter();
		void onExit();
		void showFacebookButton();
		void facebook();
		void didFacebookShare(bool published, const Error& error);
		void menu();
		void replay();
		void updatePointsCounter(float dt);
		void showUnlockedUnits();
		void hideUnlockedUnitsHUD();
		
	private:
		IGameOverScreenDelegate* delegate_;
		
		Game* game_;
		//OnlineGame* onlineGame_;
		
		CCMenu* menu_;
		CCLayer* bg_;
		CCLabelTTF* lbTitle_;
		CCLabelTTF* lbEarned_;
		CCSprite* lbLoss_;
		CCLabelTTF* lbLoginIntoFacebook_;
		CCLabelBMFont* outPoints_;
		CCLabelBMFont* lbPoints_;
		CCLabelBMFont* outAccumulatedPoints_;
		CCLabelTTF* lbAccumulatedPoints_;
		CCMenuItemSprite* btnMenu_;
		CCMenuItemSprite* btnReplay_;
		CCLabelTTF* lbShareToFacebook_;
		CCSprite* lb5000XP_;
		CCMenuItemSprite* btnPostToFacebook_;
		
		CCLabelTTF* lbUnlocked_;
		CCLabelTTF* lbUnlockedUnitName_;
		CCSprite* iconUnlockedUnit_;
		
		
		int currentPointsNumber_;
		int updateToPointsNumber_;
		int currentTotalPointsNumber_;
		bool facebookButtonWasShown_;
	};
}
