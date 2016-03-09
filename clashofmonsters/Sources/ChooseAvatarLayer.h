//
//  ChooseAvatarLayer.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "NavigationMenuLayer.h"
#include "GameConfigurationScreenTypes.h"

namespace monsters
{
	class IChooseAvatarLayerDelegate
	{
	public:
		virtual void chooseAvatarLayerNext() = 0;
		virtual void chooseAvatarLayerBack() = 0;
		virtual void chooseAvatarLayerSelectAvatar(ChooseAvatarArmyType type, bool left_avatar_flag) = 0;
	};
	
	class ChooseAvatarLayer: public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IChooseAvatarLayerDelegate* delegate) { delegate_ = delegate; }
		IChooseAvatarLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(ChooseAvatarLayer);
		
		void setType(GameConfigurationScreenType type);
		void setLeftAvatarType(ChooseAvatarArmyType type, bool animated);
		void setRightAvatarType(ChooseAvatarArmyType type, bool animated);
		
		void onShow();
		void onShowDone();
		void onHide();
		void onHideDone();
		
		ChooseAvatarArmyType leftAvatarType() const { return leftAvatarType_; };
		ChooseAvatarArmyType rightAvatarType() const { return rightAvatarType_; };
		
	private:
		void visit();
	
		void nextButtonPressed();
		void backButtonPressed();
		
		void setTitleText(const char* text);
		
		void leftAvatarButtonPressed();
		void rightAvatarButtonPressed();
		void showButton(CCMenuItemSprite* button, bool from_right_flag, bool animated = true);
		void hideButton(CCMenuItemSprite* button, bool from_right_flag, bool animated = true);
		
		void disableLeftMenu(float mseconds);
		void enableLeftMenu();
		void disableRightMenu(float mseconds);
		void enableRightMenu();
		
	private:
		IChooseAvatarLayerDelegate* delegate_;
		
		CCMenu* commonMenu_;
		CCMenu* leftMenu_;
		CCMenu* rightMenu_;
		
		float avatarButtonsBaseY_;
		
		GameConfigurationScreenType gameType_;
		ChooseAvatarArmyType leftAvatarType_;
		ChooseAvatarArmyType rightAvatarType_;
		
		CCLabelTTF* lb_title_;
		CCMenuItemSprite* leftButtonNinja_;
		CCSprite* leftHighlightNinja_;
		CCMenuItemSprite* rightButtonNinja_;
		CCSprite* rightHighlightNinja_;
		CCMenuItemSprite* leftButtonPirate_;
		CCSprite* leftHighlightPirate_;
		CCMenuItemSprite* rightButtonPirate_;
		CCSprite* rightHighlightPirate_;
	};
}
