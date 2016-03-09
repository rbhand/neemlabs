//
//  SettingsScreen.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/4/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "SettingsScreen.h"
#include "SimpleAudioEngine.h"
#include "Utils.h"
#include "GoogleAnalytics.h"

using namespace cocos2d;
using namespace cocos2d_utils;
using namespace CocosDenshion;
using namespace monsters;



bool SettingsLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPoint(0, 0));
	
	btnSoundOnOf_ = menuItemSpriteHighlighted("btn_sound_on.png", this,
		menu_selector(SettingsLayer::soundOnOf));
	btnSoundOnOf_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5 + ct(70)));
	menu->addChild(btnSoundOnOf_);
	
	btnMusicOnOf_ = menuItemSpriteHighlighted("btn_music_on.png", this,
		menu_selector(SettingsLayer::musicOnOf));
	btnMusicOnOf_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	menu->addChild(btnMusicOnOf_);
	
	CCMenuItemSprite* btn_back = menuItemSpriteHighlighted("btn_back.png", this,
		menu_selector(SettingsLayer::back));
	btn_back->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.25 - ct(70)));
	menu->addChild(btn_back);
	
	addChild(menu);
	
	return true;
}


void SettingsLayer::back(CCObject* sender)
{
	delegate_->settingsLayerBack();
}

void SettingsLayer::soundOnOf(CCObject* sender)
{
	settings_->setSoundEnabled(!settings_->isSoundEnabled());
	
	delegate_->settingsLayerSoundOnOf();
	
	updateSoundButton();
}

void SettingsLayer::musicOnOf(CCObject* sender)
{
	settings_->setMusicEnabled(!settings_->isMusicEnabled());
	
	delegate_->settingsLayerMusicOnOf();
	
	updateMusicButton();
}

void SettingsLayer::updateSoundButton()
{
	if (settings_->isSoundEnabled())
		menuItemSpriteHighlightedChangeSprite(btnSoundOnOf_, "btn_sound_on.png");
	else
		menuItemSpriteHighlightedChangeSprite(btnSoundOnOf_, "btn_sound_off.png");
}

void SettingsLayer::updateMusicButton()
{
	if (settings_->isMusicEnabled())
		menuItemSpriteHighlightedChangeSprite(btnMusicOnOf_, "btn_music_on.png");
	else
		menuItemSpriteHighlightedChangeSprite(btnMusicOnOf_, "btn_music_off.png");
}




bool SettingsScreen::init()
{
	if (!CCScene::init())
	{
		return false;
	}
	
	returnToMenu_ = true;
	
	CCLayer* bg = CCLayer::create();
	
	CCSprite* bg_menu = CCSprite::create(cocos2d_utils::textureName("Textures/Menu-Background.jpg").c_str());
	bg_menu->setScale(getContentSize().height / bg_menu->getContentSize().height);
	bg_menu->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	bg_menu->setAnchorPoint(CCPoint(0.5, 0.5));
	bg->addChild(bg_menu);
	
	settingsLayer_ = SettingsLayer::create();
	settingsLayer_->setDelegate(this);
	
	addChild(bg);
	addChild(settingsLayer_);
	
	return true;
}

void SettingsScreen::onEnter()
{
	CCScene::onEnter();
	
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Settings");
	
	settingsLayer_->updateSoundButton();
	settingsLayer_->updateMusicButton();
}

void SettingsScreen::settingsLayerBack()
{
	delegate()->settingsScreenBack();
}

void SettingsScreen::settingsLayerSoundOnOf()
{
	delegate()->settingsScreenSoundOnOf();
}

void SettingsScreen::settingsLayerMusicOnOf()
{
	delegate()->settingsScreenMusicOnOf();
}





