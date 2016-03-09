//
//  OnlineGameFindByNameLayer.cpp
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "OnlineGameFindByNameLayer.h"
#include "Session.h"
#include "Utils.h"
#include "CCAlertBox.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;
using namespace cocos2d::extension;


//FIND BY NAME LAYER
bool OnlineGameFindByNameLayer::init()
{
	if (!NavigationMenuLayer::init(NavigationMenuLayerLayout_Internal))
	{
		return false;
	}
	
	isShowing_ = false;
	
	this->btnBack()->setPosition(CCPoint(getContentSize().width * 0.5, btnBack()->getContentSize().height * 0.5));
	this->btnPlay()->setPosition(this->btnCancel()->getPosition());
	
	btnCancel()->setVisible(false);
	btnNext()->setVisible(false);
	btnBack()->setVisible(true);
	
	CCLabelTTF* lb_subtitle = CCLabelTTF::create(" FIND PLAYER BY NAME ", "BadaBoom BB", ct(22.0));
	lb_subtitle->setAnchorPoint(CCPoint(0.5, 1));
	lb_subtitle->setColor(ccc3(52, 87, 113));
	lb_subtitle->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(85)));
	
	//textField_ = CCTextFieldTTF::textFieldWithPlaceHolder("ENTER NAME", CCSize(ct(240), ct(25)), kCCTextAlignmentLeft, "BadaBoom BB", ct(22.0));
	
	CCScale9Sprite* textFieldSprite = CCScale9Sprite::createWithSpriteFrameName("bg_textfield.png", CCRectMake(8, 8, 247, 64));
	
	textField_ = CCEditBox::create(CCSizeMake(ct(300), ct(50)), textFieldSprite);
	textField_->setAnchorPoint(CCPoint(0.5, 1));
	//textField_->setColor(ccc3(255, 255, 255));
	textField_->setPosition(CCPoint(getContentSize().width * 0.5, lb_subtitle->getPositionY() - lb_subtitle->getContentSize().height - ct(10) ));
	textField_->setReturnType(kKeyboardReturnTypeSearch);
	textField_->setFont("BadaBoom BB", ct(24));
	textField_->setFontColor(ccWHITE);
	textField_->setPlaceHolder("Enter Player Name");
	textField_->setDelegate(this);
	textField_->setText("");
	
	addChild(lb_subtitle);
	addChild(textField_);
	
	
	CCSize friends_list_size;
	CCPoint friends_list_position;
	if (cocos2d_utils::isIPhone5())
		friends_list_size = CCSizeMake(300, 340);
	else if (cocos2d_utils::isIPhone())
		friends_list_size = CCSizeMake(300, 250);
	else
		friends_list_size = CCSizeMake(600, 570);
	
	/*if (cocos2d_utils::isIPhone5())
	{
		friends_list_posistion = CCPoint(getContentSize().width * 0.5 - ct(300) * 0.5,
			getContentSize().height * 0.25 - ct(60));
	}
	else
	{
		friends_list_posistion = CCPoint(getContentSize().width * 0.5 - ct(300) * 0.5,
			getContentSize().height * 0.25 - ct(35));
	}*/
	friends_list_position = CCPoint(getContentSize().width * 0.5 - friends_list_size.width * 0.5,
			getContentSize().height - lb_subtitle->getPositionY() - ct(30));
	//friends_list_size = CCSizeMake(ct(300), friends_list_position.y - this->btnCancel()->getPositionY());
	
	friendsListLayer_ = FriendsListLayer::create(friends_list_size, friends_list_position);
	friendsListLayer_->setDelegate(this);
	
	addChild(friendsListLayer_);
	
	loadingIndicatorLayer_ = LoadingIndicatorLayer::create();
	loadingIndicatorLayer_->setLinkedNodes(CCArray::create(textField_, friendsListLayer_, 0));
	addChild(loadingIndicatorLayer_);
	
	
	lbNoUsersFound_ = CCLabelTTF::create(" NO USERS FOUND ", "BadaBoom BB", ct(24.0));
	lbNoUsersFound_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbNoUsersFound_->setColor(ccWHITE);
	lbNoUsersFound_->setPosition(CCPoint(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
	lbNoUsersFound_->setVisible(false);
	addChild(lbNoUsersFound_);
	
	return true;
}

void OnlineGameFindByNameLayer::backButtonPressed()
{
	delegate_->findByNameLayerBackToMenu();
	//showPlayButton();
}

void OnlineGameFindByNameLayer::playButtonPressed()
{
	//hidePlayButton();
}

void OnlineGameFindByNameLayer::editBoxEditingDidBegin(CCEditBox* editBox)
{
	lbNoUsersFound_->setVisible(false);
}

void OnlineGameFindByNameLayer::editBoxEditingDidEnd(CCEditBox* editBox)
{
	if (strlen(editBox->getText()) > 0)
	{
		showLoadingIndicator();
		friendsListLayer_->setVisible(false);
		Session::sharedSession().findUsersByName(editBox->getText(),
			(Session::GetUsersListCallback(this,
				(Session::GetUsersListCallbackSelector)&OnlineGameFindByNameLayer::onFriendsListRetrieved)));
	}
}

void OnlineGameFindByNameLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	
}

void OnlineGameFindByNameLayer::editBoxReturn(CCEditBox* editBox)
{
	
}


void OnlineGameFindByNameLayer::onShow()
{
	isShowing_ = true;
	this->reloadData();
}

void OnlineGameFindByNameLayer::onFriendsListRetrieved(const std::vector<User>& friends, const Error& error)
{
	hideLoadingIndicator();
	friendsListLayer_->setFriendsList(friends);
	friendsListLayer_->setVisible((friends.size() > 0));
	lbNoUsersFound_->setVisible(!friendsListLayer_->isVisible());
}

void OnlineGameFindByNameLayer::onHide()
{
	isShowing_ = false;
}

void OnlineGameFindByNameLayer::onInviteSentCompleted(const Error& error)
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

void OnlineGameFindByNameLayer::friendsListUserSelectedAtIndex(int index)
{
	loadingIndicatorLayer_->setCaption("Sending invite...");
	loadingIndicatorLayer_->show();

	User user = friendsListLayer_->friends().at(index);
	Session::sharedSession().inviteUserToGame(user, Session::OperationCallback(this,
		(Session::OperationCallbackSelector)&OnlineGameFindByNameLayer::onInviteSentCompleted));
}

void OnlineGameFindByNameLayer::reloadData()
{
	if (isShowing_)
		friendsListLayer_->reloadData();
}
