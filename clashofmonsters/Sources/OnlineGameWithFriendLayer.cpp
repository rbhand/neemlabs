//
//  OnlineGameWithFriendLayer.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/27/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "OnlineGameWithFriendLayer.h"
#include "Utils.h"
#include "Session.h"
#include "CCAlertBox.h"
#include "FacebookSharing.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;


//PLAY WITH FRIEND LAYER
bool OnlineGameWithFriendLayer::init()
{
	if (!NavigationMenuLayer::init(NavigationMenuLayerLayout_Internal))
	{
		return false;
	}
	
	isShowing_ = false;
	
	this->btnBack()->setPosition(CCPoint(getContentSize().width * 0.5, btnBack()->getContentSize().height * 0.5));
//	this->btnPlay()->setPosition(this->btnCancel()->getPosition());

	btnInvite_ = menuItemSpriteHighlighted("btn_invite.png", this,
		menu_selector(OnlineGameWithFriendLayer::inviteButtonPressed));
	btnInvite_->setPosition(CCPoint(getContentSize().width * 0.5, btnBack()->getPositionY() + btnInvite_->getContentSize().height));
	menu()->addChild(btnInvite_);
	
	btnCancel()->setVisible(false);
	btnNext()->setVisible(false);
	btnBack()->setVisible(true);
	
	CCLabelTTF* lb_subtitle = CCLabelTTF::create(" FRIENDS ", "BadaBoom BB", ct(22.0));
	lb_subtitle->setAnchorPoint(CCPoint(0.5, 1));
	lb_subtitle->setColor(ccc3(52, 87, 113));
	lb_subtitle->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(85)));
	
	CCSize friends_list_size;
	CCPoint friends_list_posistion;
	if (cocos2d_utils::isIPhone5())
		friends_list_size = CCSizeMake(ct(300), ct(350));
	else if (cocos2d_utils::isIPhone())
		friends_list_size = CCSizeMake(ct(300), ct(260));
	else
		friends_list_size = CCSizeMake(ct(300), ct(280));
	
	if (cocos2d_utils::isIPhone5())
	{
		friends_list_posistion = CCPoint(getContentSize().width * 0.5 - ct(300) * 0.5,
			getContentSize().height * 0.25 - ct(35));
	}
	else if (cocos2d_utils::isIPhone())
	{
		friends_list_posistion = CCPoint(getContentSize().width * 0.5 - ct(300) * 0.5,
			getContentSize().height * 0.25 - ct(10));
	}
	else
	{
		friends_list_posistion = CCPoint(getContentSize().width * 0.5 - ct(300) * 0.5,
			getContentSize().height * 0.25 - ct(10));
	}
	friendsListLayer_ = FriendsListLayer::create(friends_list_size, friends_list_posistion);
	friendsListLayer_->setDelegate(this);
	
	addChild(lb_subtitle);
	addChild(friendsListLayer_);
	
	loadingIndicatorLayer_ = LoadingIndicatorLayer::create();
	loadingIndicatorLayer_->setLinkedNodes(CCArray::create(friendsListLayer_, 0));
	addChild(loadingIndicatorLayer_);
	
	lbNoUsersFound_ = CCLabelTTF::create(" NO USERS FOUND ", "BadaBoom BB", ct(24.0));
	lbNoUsersFound_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbNoUsersFound_->setColor(ccWHITE);
	lbNoUsersFound_->setPosition(CCPoint(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
	lbNoUsersFound_->setVisible(false);
	addChild(lbNoUsersFound_);
	
	return true;
}


void OnlineGameWithFriendLayer::backButtonPressed()
{
	delegate_->withFriendLayerBackToMenu();
}


void OnlineGameWithFriendLayer::inviteButtonPressed()
{
	FacebookSharing::sharedSharing().facebookInvite(FacebookSharing::InviteCallback(this,
		(FacebookSharing::InviteCallbackSelector)&OnlineGameWithFriendLayer::didFacebookInvite));
}


void OnlineGameWithFriendLayer::didFacebookInvite(bool invited, const Error& error)
{
	if (!error.isOk())
	{
		CCAlertBox::show("Invite Failed", error.description(), "Dismiss");
	}
	else if (invited)
	{
		CCAlertBox::show("Friends Invited", "We have sent notification to your friends!", "Dismiss");
	}
}


void OnlineGameWithFriendLayer::onShow()
{
	isShowing_ = true;
	showLoadingIndicator();
	friendsListLayer_->setVisible(false);
	Session::sharedSession().getFriendsList(Session::GetUsersListCallback(this, (Session::GetUsersListCallbackSelector)&OnlineGameWithFriendLayer::onFriendsListRetrieved));
}


void OnlineGameWithFriendLayer::onFriendsListRetrieved(const std::vector<User>& friends, const Error& error)
{
	hideLoadingIndicator();
	friendsListLayer_->setFriendsList(friends);
	friendsListLayer_->setVisible((friends.size() > 0));
	lbNoUsersFound_->setVisible(!friendsListLayer_->isVisible());
}

void OnlineGameWithFriendLayer::onHide()
{
	isShowing_ = false;
}

void OnlineGameWithFriendLayer::onInviteSentCompleted(const Error& error)
{
	loadingIndicatorLayer_->hide();
	
	if (error.isOk())
	{
		friendsListLayer_->reloadData();
	}
	else
	{
		CCAlertBox::show("Network Problems", error.description(), "Dismiss");
	}
}

void OnlineGameWithFriendLayer::friendsListUserSelectedAtIndex(int index)
{
	loadingIndicatorLayer_->setCaption("Sending invite...");
	loadingIndicatorLayer_->show();

	User user = friendsListLayer_->friends().at(index);
	Session::sharedSession().inviteUserToGame(user, Session::OperationCallback(this,
		(Session::OperationCallbackSelector)&OnlineGameWithFriendLayer::onInviteSentCompleted));
}

void OnlineGameWithFriendLayer::reloadData()
{
	if (isShowing_)
		friendsListLayer_->reloadData();
}

