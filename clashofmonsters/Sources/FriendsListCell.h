//
//  FriendsListCell.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "CCTableView.h"
#include "AvatarNode.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d_utils;


namespace monsters
{
	class FriendsListCell : public CCTableViewCell
	{
	public:
		bool init();
		
		CREATE_FUNC(FriendsListCell);
		
		static CCSize const cellSize();
		
		void highLightCell();
		void unhighLightCell();
		void setName(std::string name);
		void setAvatarUrl(std::string url);
		void setInvited(bool invited);
		bool isInvited() const { return invited_; };
		void update();
		
	private:
		CCSprite* bg_;
		CCSprite* bgHighlight_;
		CCLabelTTF* outName_;
		AvatarNode* outAvatar_;
		CCSprite* lbInviteSent_;
		
		bool invited_;
	};
}