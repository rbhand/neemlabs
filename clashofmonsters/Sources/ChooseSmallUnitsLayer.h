//
//  ChooseSmallUnitsLayer.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "GameUnits.h"
#include "NavigationMenuLayer.h"
#include "GameConfigurationScreenTypes.h"

namespace monsters
{
	class IChooseSmallUnitsLayerDelegate
	{
	public:
		virtual void chooseSmallUnitsLayerBack(CCObject* sender) = 0;
		virtual void chooseSmallUnitsLayerNext(CCObject* sender) = 0;
	};
	
	class ChooseSmallUnitsLayer: public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IChooseSmallUnitsLayerDelegate* delegate) { delegate_ = delegate; }
		IChooseSmallUnitsLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(ChooseSmallUnitsLayer);
		
		//void onShow();
		//void onHide();
		
		void setPlayerCaptionEnabled(bool enabled);
		void setPlayerCaption(const char* caption);
		
		void setAvatar(Avatar* avatar);
		void selectRandomUnits();
		
		int redUnitIndex() const { return redUnitIndex_; };
		int greenUnitIndex() const { return greenUnitIndex_; };
		int blueUnitIndex() const { return blueUnitIndex_; };
		
	private:
		void cancelButtonPressed();
		void playButtonPressed();
		void backButtonPressed();
		void nextButtonPressed();
		
		void selectRedUnit(CCObject* sender);
		void selectGreenUnit(CCObject* sender);
		void selectBlueUnit(CCObject* sender);
		
	private:
		IChooseSmallUnitsLayerDelegate* delegate_;
		Avatar* avatar_;
		
		float unitsButtonsBaseY_;
		
		CCMenu* unitsMenu_;
		CCLayer* unitsCaptions_;
		CCLabelTTF* playerCaption_;
		
		std::vector<CCMenuItemSprite*> redButtons_;
		std::vector<CCMenuItemSprite*> greenButtons_;
		std::vector<CCMenuItemSprite*> blueButtons_;
		
		int redUnitIndex_;
		int greenUnitIndex_;
		int blueUnitIndex_;
	};

}
