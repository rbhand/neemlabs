//
//  ChooseBigUnitsLayer.h
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
	class IChooseBigUnitsLayerDelegate
	{
	public:
		virtual void chooseBigUnitsLayerBack(CCObject* sender) = 0;
		virtual void chooseBigUnitsLayerNext(CCObject* sender) = 0;
	};
	
	class ChooseBigUnitsLayer: public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IChooseBigUnitsLayerDelegate* delegate) { delegate_ = delegate; }
		IChooseBigUnitsLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(ChooseBigUnitsLayer);
		
		//void onShow();
		//void onHide();
		
		void setPlayerCaptionEnabled(bool enabled);
		void setPlayerCaption(const char* caption);
		
		void setAvatar(Avatar* avatar);
		void selectRandomUnits();
		
		int firstUnitIndex() const { return firstUnitIndex_; };
		int secondUnitIndex() const { return secondUnitIndex_; };
	
	private:
		void cancelButtonPressed();
		void playButtonPressed();
		void backButtonPressed();
		void nextButtonPressed();
		
		void selectUnit(CCObject* sender);
		void updateUnitsButtons();
		
	private:
		IChooseBigUnitsLayerDelegate* delegate_;
		Avatar* avatar_;
		
		float unitsButtonsBaseY_;
		
		CCMenu* unitsMenu_;
		CCLabelBMFont* playerCaption_;
		
		std::vector<CCMenuItemSprite*> unitsButtons_;
		
		int firstUnitIndex_;
		int secondUnitIndex_;
	};
}
