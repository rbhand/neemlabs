//
//  FriendsListLayer.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/22/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "CCTableView.h"
#include "FriendsListCell.h"
#include "User.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d_utils;


namespace monsters
{
	class IFriendsListLayerDelegate
	{
	public:
		virtual void friendsListUserSelectedAtIndex(int index) = 0;
	};
	
	class FriendsListLayer : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
	{
	public:
		bool init(CCSize tableSize, CCPoint tablePosition);
		static FriendsListLayer* create(CCSize tableSize, CCPoint tablePosition);
		
		//CREATE_FUNC(FriendsListLayer);
		
		void setFriendsList(const std::vector<User>& friends);
		std::vector<User> friends() { return friends_; };
		
		void setDelegate(IFriendsListLayerDelegate* delegate) { delegate_ = delegate; }
		IFriendsListLayerDelegate* delegate() const { return delegate_;}
		
		void reloadData();
	
	private:
		void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
		void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
		void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
		
		void scrollViewDidScroll(CCScrollView* view) {};
		void scrollViewDidZoom(CCScrollView* view) {};
		
		CCSize cellSizeForTable(CCTableView *table);
		CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
		unsigned int numberOfCellsInTableView(CCTableView *table);
		
	private:
		CCTableView* tableView_;
		
		std::vector<User> friends_;
		IFriendsListLayerDelegate* delegate_;
	};
}

