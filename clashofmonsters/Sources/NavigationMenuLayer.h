//
//  NavigationMenuLayer.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/11/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	enum NavigationMenuLayerLayout
	{
		NavigationMenuLayerLayout_Default = 0,
		NavigationMenuLayerLayout_Internal = 1
	};
	
	//BASIC LAYER OF ONLINE GAME SCREEN
	class NavigationMenuLayer: public cocos2d::CCLayer
	{
	public:
		virtual bool init(NavigationMenuLayerLayout layout = NavigationMenuLayerLayout_Default);
		void setMenuEnabled(bool enabled);
		
		void showPlayButton(bool lock_menu = true);
		void hidePlayButton(bool lock_menu = true);
		void showPlayButtonWithoutLock();
		void hidePlayButtonWithoutLock();
		
		virtual void onShow() {};
		virtual void onHide() {};
		virtual void onShowDone() {};
		virtual void onHideDone() {};
		
		void showLoadingIndicator();
		void hideLoadingIndicator();
		
		void showPlayButtonGroup();
		void showNextButtonGroup();
		
		void enableMenu() { setMenuEnabled(true); };
	
	protected:
		CCMenu* menu() const { return menu_; }
		
		void disableMenu(float mseconds);
		
		CCMenuItemSprite* btnCancel() const { return btnCancel_; };
		CCMenuItemSprite* btnPlay() const { return btnPlay_;};
		CCMenuItemSprite* btnBack() const { return btnBack_; };
		CCMenuItemSprite* btnNext() const { return btnNext_;};
		CCSprite* loadingIndicator() const { return loadingIndicator_; }
		
		virtual void cancelButtonPressed() {};
		virtual void playButtonPressed() {};
		virtual void backButtonPressed() {};
		virtual void nextButtonPressed() {};
		
		void setActionNextEnabled(bool enabled);
		
		virtual void onEnter();
		
	private:
		CCMenu* menu_;
		
		CCMenuItemSprite* btnCancel_;
		CCMenuItemSprite* btnPlay_;
		
		CCMenuItemSprite* btnBack_;
		CCMenuItemSprite* btnNext_;
		
		CCSprite* loadingIndicator_;
		
	};
	
	class NavigationMenuScreen: public cocos2d::CCScene
	{
	public:
		 bool init();
		
	protected:
		void showLayer(NavigationMenuLayer* layer, bool animated = false, bool reversed = false, float time = 0.25, float offset = 0);
		void enableCurrentLayer();
		void hideDoneLastLayer();
		
		NavigationMenuLayer* currentLayer() const { return currentLayer_; }
		
	private:
		NavigationMenuLayer* currentLayer_;
		NavigationMenuLayer* lastLayer_;
	};
	
}