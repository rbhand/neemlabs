//
//  FriendsListCell.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "FriendsListCell.h"

using namespace cocos2d;
using namespace monsters;

bool FriendsListCell::init()
{
	setContentSize(cellSize());
	invited_ = false;
	
	float avatar_padding = 0;
	
	bg_ = CCSprite::createWithSpriteFrameName("bg_table_cell.png");
	bg_->setAnchorPoint(CCPoint(0, 0.5));
	bg_->setPosition(CCPoint(0, getContentSize().height * 0.5));
	
	bgHighlight_ = CCSprite::createWithSpriteFrameName("bg_table_cell.png");
	bgHighlight_->setAnchorPoint(CCPoint(0, 0.5));
	bgHighlight_->setPosition(CCPoint(0, getContentSize().height * 0.5));
	bgHighlight_->setVisible(false);
	bgHighlight_->setColor(ccBLACK);
	
	outAvatar_ = AvatarNode::create(AvatarNodeSize_Large);
	outAvatar_->setAvatarType(AvatarNodeType_None);
	outAvatar_->setAnchorPoint(CCPoint(0, 0.5));
	avatar_padding = (bg_->getContentSize().height - outAvatar_->getContentSize().height)/2;
	outAvatar_->setPosition(CCPoint(avatar_padding, getContentSize().height * 0.5));
	
	lbInviteSent_ = CCSprite::createWithSpriteFrameName("lb_check_mark.png");
	lbInviteSent_->setAnchorPoint(CCPoint(1, 0.5));
	lbInviteSent_->setPosition(CCPoint(getContentSize().width - avatar_padding * 0.5, getContentSize().height * 0.5));
	lbInviteSent_->setVisible(false);
		
	outName_ = CCLabelTTF::create(" ", "HelveticaNeue-CondensedBlack", ct(22.0));
	outName_->setAnchorPoint(CCPoint(0, 0.5));
	//outName_->setColor(ccc3(52, 87, 113));
	outName_->setColor(ccWHITE);
	outName_->setPosition(CCPoint(outAvatar_->getPosition().x + outAvatar_->getContentSize().width + avatar_padding,
		getContentSize().height * 0.5));
	outName_->setDimensions(CCSizeMake(this->cellSize().width - avatar_padding * 3 - outAvatar_->getContentSize().width - lbInviteSent_->getContentSize().width,
		outName_->getContentSize().height));
	outName_->setHorizontalAlignment(kCCTextAlignmentLeft);
	
	
	addChild(bg_);
	addChild(bgHighlight_);
	addChild(outName_);
	addChild(outAvatar_);
	addChild(lbInviteSent_);
	
	return true;
}


CCSize const FriendsListCell::cellSize()
{
	return CCSizeMake(ct(300), ct(80));
}

void FriendsListCell::highLightCell()
{
	if (invited_)
		return;
	
	bgHighlight_->setVisible(true);
	//bg_->setVisible(false);
}

void FriendsListCell::unhighLightCell()
{
	//bg_->setVisible(true);
	bgHighlight_->setVisible(false);
}

void FriendsListCell::setName(std::string name)
{
	outName_->setString(CCString::createWithFormat("%s", name.c_str())->getCString());
}

void FriendsListCell::setAvatarUrl(std::string url)
{
	outAvatar_->setAvatarUrl(url);
}

void FriendsListCell::setInvited(bool invited)
{
	invited_ = invited;
	this->update();
}

void FriendsListCell::update()
{
	lbInviteSent_->setVisible(invited_);
}

