//
//  OnlineGameWithFriendLayer.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/27/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "NavigationMenuLayer.h"
#include "FriendsListLayer.h"
#include "Session.h"
#include "LoadingIndicatorLayer.h"

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	//PLAY WITH FRIEND LAYER
	class IOnlineGameWithFriendLayerDelegate
	{
	public:
		virtual void withFriendLayerBackToMenu() = 0;
	};
	
	
	class OnlineGameWithFriendLayer : public NavigationMenuLayer, public IFriendsListLayerDelegate
	{
	public:
		bool init();
		
		void setDelegate(IOnlineGameWithFriendLayerDelegate* delegate) { delegate_ = delegate; }
		IOnlineGameWithFriendLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(OnlineGameWithFriendLayer);
		
		void reloadData();
		
	private:
		//void back();
		void backButtonPressed();
		void inviteButtonPressed();
		
		void onShow();
		void onFriendsListRetrieved(const std::vector<User>& friends, const Error& error);
		void onHide();
		
		void friendsListUserSelectedAtIndex(int index);
		void onInviteSentCompleted(const Error& error);
		void didFacebookInvite(bool invited, const Error& error);
		
	private:
		IOnlineGameWithFriendLayerDelegate* delegate_;
		
		FriendsListLayer* friendsListLayer_;
		LoadingIndicatorLayer* loadingIndicatorLayer_;
		CCLabelTTF* lbNoUsersFound_;
		CCMenuItemSprite* btnInvite_;
		
		bool isShowing_;
	};
}