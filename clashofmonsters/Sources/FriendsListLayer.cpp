//
//  FriendsListLayer.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/22/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "FriendsListLayer.h"
#include "Session.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;




FriendsListLayer* FriendsListLayer::create(CCSize tableSize, CCPoint tablePosition)
{
	FriendsListLayer* friends_list_layer = new FriendsListLayer();
    if (friends_list_layer && friends_list_layer->init(tableSize, tablePosition))
    {
        friends_list_layer->autorelease();
        return friends_list_layer;
    }
	
    CC_SAFE_DELETE(friends_list_layer);
    return NULL;
}


bool FriendsListLayer::init(CCSize tableSize, CCPoint tablePosition)
{
	if (!CCLayer::init())
	{
		return false;
	}

	tableView_ = CCTableView::create(this, tableSize);
	tableView_->setDelegate(this);
	tableView_->setDataSource(this);
	/*tableView_->setPosition(CCPoint(getContentSize().width * 0.5 - tableView_->getContentSize().width * 0.5,
		getContentSize().height * 0.25 - ct(0) ));*/
	tableView_->setPosition(tablePosition);
	tableView_->setClippingToBounds(true);
		
	addChild(tableView_);
	
	return true;
}


void FriendsListLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	User user = friends_.at(cell->getTag());
	if (Session::sharedSession().isUserInvited(user))
		return;
	
	if (delegate_)
		delegate_->friendsListUserSelectedAtIndex(cell->getTag());
}


void FriendsListLayer::tableCellHighlight(CCTableView* table, CCTableViewCell* cell)
{
	if (FriendsListCell* selected_cell = dynamic_cast<FriendsListCell*>(cell))
	{
		selected_cell->highLightCell();
	}
}


void FriendsListLayer::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell)
{
	if (FriendsListCell* selected_cell = dynamic_cast<FriendsListCell*>(cell))
	{
		selected_cell->unhighLightCell();
	}
}


CCSize FriendsListLayer::cellSizeForTable(CCTableView *table)
{
	return FriendsListCell::cellSize();
}


CCTableViewCell* FriendsListLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	FriendsListCell* cell = FriendsListCell::create();
	
	User un = friends_.at(idx);
	cell->setName(un.name());
	cell->setAvatarUrl(un.avatarUrl());
	cell->setTag(idx);
	cell->setInvited(Session::sharedSession().isUserInvited(un));
	
	return cell;
}

unsigned int FriendsListLayer::numberOfCellsInTableView(CCTableView *table)
{
	//cout << endl << "Friends list size: " << friends_.size() << endl;
	return friends_.size();
}

void FriendsListLayer::setFriendsList(const std::vector<User>& friends)
{
	friends_ = friends;
	
	tableView_->reloadData();
	
	//cout << endl << "table view x:" << tableView_->getPosition().x << " y:" << tableView_->getPosition().y << endl;
	//cout << endl << "table view width:" << tableView_->getContentSize().width << " height:" << tableView_->getContentSize().height << endl;
};

void FriendsListLayer::reloadData()
{
	tableView_->reloadData();
}


