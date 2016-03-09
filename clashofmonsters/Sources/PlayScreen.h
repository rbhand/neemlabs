//
//  PlayScreen.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#define STRENGTH_INDICATORS_LAYER_TAG 401

#include "cocos2d.h"
#include "Game.h"
//#include "OnlineGame.h"
#include "SettingsScreen.h"
#include "NextRoundLayer.h"
#include "StrengthIndicatorsLayer.h"
#include "CCMenuItemSpriteTouch.h"
#include "SoundManager.h"
#include "RandomGenerator.h"
#include "CCAlertBox.h"

namespace monsters
{
	class IPlayScreenMenuLayerDelegate
	{
	public:
		virtual void playScreenMenuLayerSettings() = 0;
		virtual void playScreenMenuLayerTutorial() = 0;
		virtual void playScreenMenuLayerQuit() = 0;
		virtual void playScreenMenuLayerResume() = 0;
	};
	
	class PlayScreenMenuLayer : public cocos2d::CCLayerColor
	{
	public:
		bool init();
		
		void setDelegate(IPlayScreenMenuLayerDelegate* delegate) { delegate_ = delegate; }
		IPlayScreenMenuLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(PlayScreenMenuLayer);
	
	private:
		void settings();
		void tutorial();
		void resume();
		void quit();
	
	private:
		IPlayScreenMenuLayerDelegate* delegate_;
	};
	
	
	
	class IPlayScreenDelegate
	{
	public:
		virtual void playScreenMenu() = 0;
		virtual void playScreenTutorial() = 0;
		virtual void playScreenGameOver() = 0;
		virtual void playScreenSettings() = 0;
		virtual void playScreenSoundOnOf() = 0;
		virtual void playScreenMusicOnOf() = 0;
	};
	
	class GameLayer;
	class PlayScreen : public cocos2d::CCScene,
		public IPlayScreenMenuLayerDelegate,
		public ISettingsLayerDelegate,
		public ICCAlertBoxDelegate
	{
	public:
		PlayScreen();
		
		virtual bool init();
		
		CREATE_FUNC(PlayScreen);
		
		void setGame(Game* game);
//		void setOnlineGame(const boost::shared_ptr<OnlineGame>& onlineGame);
//		boost::shared_ptr<OnlineGame> onlineGame() const;
		
		void setDelegate(IPlayScreenDelegate* delegate);
		IPlayScreenDelegate* delegate() const { return delegate_; }
		
		void setSettings(Settings* settings) { settings_ = settings; settingsLayer_->setSettings(settings); };
		Settings* getSettings() const { return settings_; };
		
		void playScreenMenuLayerSettings();
		void playScreenMenuLayerTutorial();
		void playScreenMenuLayerQuit();
		void playScreenMenuLayerResume();
		
		void settingsLayerBack();
		void settingsLayerSoundOnOf();
		void settingsLayerMusicOnOf();
		
	private:
		void menu();
		void showStrengthLayer();
		void hideStrengthLayer();
		void removeStrengthLayer();
		void resume();
		void onEnter();
		void onExit();
		void startBackgroundMusic();
		void playStartupMusic();
		void setMenuVisible(bool visible);
		void alertBoxDidDismiss(CCAlertBox* alertBox, int buttonIndex);
		
	private:
		Settings* settings_;
		GameLayer* gameLayer_;
		cocos2d::CCLayerColor* popupBgLayer_;
		PlayScreenMenuLayer* menuLayer_;
		SettingsLayer* settingsLayer_;
		StrengthIndicatorsLayer* strengthIndicatorsLayer_;
		IPlayScreenDelegate* delegate_;
		cocos2d::CCMenu* menu_;
		cocos2d::CCAlertBox alertOnlineBack_;
	};
}
