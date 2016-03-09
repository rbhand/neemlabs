//
//  OnlineGameFindByNameLayer.h
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "NavigationMenuLayer.h"
#include "FriendsListLayer.h"
#include "LoadingIndicatorLayer.h"
#include "Session.h"

#include "CCEditBox.h"

//using namespace cocos2d;
//using namespace cocos2d_utils;

namespace monsters
{
	//FIND BY NAME LAYER
	class IOnlineGameFindByNameLayerDelegate
	{
	public:
		virtual void findByNameLayerBackToMenu() = 0;
	};
	
	
	class OnlineGameFindByNameLayer : public NavigationMenuLayer, public CCEditBoxDelegate, public IFriendsListLayerDelegate
	{
	public:
		bool init();
		
		void setDelegate(IOnlineGameFindByNameLayerDelegate* delegate) { delegate_ = delegate; }
		IOnlineGameFindByNameLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(OnlineGameFindByNameLayer);
		
		void friendsListUserSelectedAtIndex(int index);
		void reloadData();
		
	private:
		//void back();
		void backButtonPressed();
		void playButtonPressed();
		
		void editBoxEditingDidBegin(CCEditBox* editBox);
		void editBoxEditingDidEnd(CCEditBox* editBox);
		void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
		void editBoxReturn(CCEditBox* editBox);
		
		void onShow();
		void onHide();
		
		void onFriendsListRetrieved(const std::vector<User>& friends, const Error& error);
		void onInviteSentCompleted(const Error& error);
		
	private:
		IOnlineGameFindByNameLayerDelegate* delegate_;
		
		CCEditBox* textField_;
		FriendsListLayer* friendsListLayer_;
		LoadingIndicatorLayer* loadingIndicatorLayer_;
		CCLabelTTF* lbNoUsersFound_;
		
		bool isShowing_;
	};
}