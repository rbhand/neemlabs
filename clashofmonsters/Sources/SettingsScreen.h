//
//  SettingsScreen.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/4/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Settings.h"

namespace monsters
{
	class ISettingsLayerDelegate
	{
	public:
		virtual void settingsLayerBack() = 0;
		virtual void settingsLayerSoundOnOf() = 0;
		virtual void settingsLayerMusicOnOf() = 0;
	};
	
	class SettingsLayer : public cocos2d::CCLayer
	{
	public:
		virtual bool init();
		
		void setDelegate(ISettingsLayerDelegate* delegate) { delegate_ = delegate; }
		ISettingsLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(SettingsLayer);
		
		void setSettings(Settings* settings) { settings_ = settings; updateMusicButton(); updateSoundButton(); };
		void updateSoundButton();
		void updateMusicButton();
		
	private:
		void back(CCObject* sender);
		void soundOnOf(CCObject* sender);
		void musicOnOf(CCObject* sender);
		
	private:
		ISettingsLayerDelegate* delegate_;
		Settings* settings_;
		
		cocos2d::CCMenuItemSprite* btnSoundOnOf_;
		cocos2d::CCMenuItemSprite* btnMusicOnOf_;
	};
	
	
	
	class ISettingsScreenDelegate
	{
	public:
		virtual void settingsScreenBack() = 0;
		virtual void settingsScreenSoundOnOf() = 0;
		virtual void settingsScreenMusicOnOf() = 0;
	};
	
	class SettingsScreen : public cocos2d::CCScene,
		ISettingsLayerDelegate
	{
	public:
		virtual bool init();
		
		void setDelegate(ISettingsScreenDelegate* delegate) { delegate_ = delegate; }
		ISettingsScreenDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(SettingsScreen);
		
		bool shouldReturnToMenu() const { return returnToMenu_; };
		void setShouldReturnToMenu(bool shouldReturn) { returnToMenu_ = shouldReturn; };
		
		void setSettings(Settings* settings) { settings_ = settings; settingsLayer_->setSettings(settings); };
		Settings* getSettings() const { return settings_; };
		
		void settingsLayerBack();
		void settingsLayerSoundOnOf();
		void settingsLayerMusicOnOf();
		
	private:
		void onEnter();
		
		
		
	private:
		ISettingsScreenDelegate* delegate_;
		bool returnToMenu_;
		Settings* settings_;
		SettingsLayer* settingsLayer_;
	};
}
