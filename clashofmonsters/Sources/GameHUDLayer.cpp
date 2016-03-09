//
//  GameHUDLayer.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/1/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "GameHUDLayer.h"
#include "Game.h"
#include "Utils.h"

using namespace monsters;
using namespace cocos2d;
using namespace cocos2d_utils;

bool GameHUDLayer::init()
{
	if (!CCLayer::init())
		return false;
	
	delegate_ = 0;
	
	menu_ = CCMenu::create();
	menu_->setPosition(0, 0);
	
	addChild(menu_);
	
	for (int i=0; i<2; ++i)
	{
		
		float ch = getContentSize().height;
		
		players_[i].magicBar_ = BarNode::barNodeWithSprites(
			CCSprite::createWithSpriteFrameName("bg_magic_bar_empty.png"),
			CCSprite::createWithSpriteFrameName("bg_magic_bar_filled.png"));
		players_[i].magicBar_->setAnchorPoint(CCPoint(1, 0.5));
		players_[i].magicBar_->setDirection(i == 0 ? BarNodeDirection_Up : BarNodeDirection_Down);
		players_[i].magicBar_->setPosition(getContentSize().width - ct(16), ch * (0.25 + i * 0.5));
		players_[i].magicBar_->setVisible(false);
		
		addChild(players_[i].magicBar_);
		
		
		
		players_[i].bgTurnsLeft_ = CCSprite::createWithSpriteFrameName("bg_turns_left.png");
		players_[i].bgTurnsLeft_->setAnchorPoint(CCPoint(0.5, 0.5));
		players_[i].bgTurnsLeft_->setPosition(CCPoint(ct(3) + players_[i].bgTurnsLeft_->getContentSize().width * 0.5, getContentSize().height * 0.5 +
			((i == 0) ? -players_[i].bgTurnsLeft_->getContentSize().height : players_[i].bgTurnsLeft_->getContentSize().height)));
		addChild(players_[i].bgTurnsLeft_);
		
		players_[i].outTurnsLeft_ = CCLabelTTF::create("3", "BadaBoom BB", ct(20));
		players_[i].outTurnsLeft_->setColor(ccc3(255, 255, 255));
		players_[i].outTurnsLeft_->setPosition(CCPoint(players_[i].bgTurnsLeft_->getContentSize().width * 0.5,
			players_[i].bgTurnsLeft_->getContentSize().height * 0.5));
		players_[i].bgTurnsLeft_->addChild(players_[i].outTurnsLeft_);
		
		
		players_[i].btnCallTroops_ = menuItemSpriteHighlightedWithDisabledState("btn_call_troops.png", this,
			menu_selector(GameHUDLayer::callTroops));
		players_[i].btnCallTroops_->setAnchorPoint(CCPoint(0, 0.5));
		players_[i].btnCallTroops_->setPosition(ct(3),
			(i == 0) ? ct(0) + players_[i].btnCallTroops_->getContentSize().height * 0.5
				: getContentSize().height - ct(0) - players_[i].btnCallTroops_->getContentSize().height * 0.5);
		menu_->addChild(players_[i].btnCallTroops_);
		
		players_[i].outTroopsLeft_ = CCLabelTTF::create("25", "BadaBoom BB", ct(22));
		players_[i].outTroopsLeft_->setColor(ccc3(255, 255, 255));
		players_[i].outTroopsLeft_->setPosition(CCPoint(ct(21),
			players_[i].btnCallTroops_->getContentSize().height * 0.5));
		players_[i].btnCallTroops_->addChild(players_[i].outTroopsLeft_);
		
		players_[i].btnEndTurn_ = menuItemSpriteHighlighted("btn_end_turn.png", this,
			menu_selector(GameHUDLayer::endRound));
		players_[i].btnEndTurn_->setAnchorPoint(CCPoint(0, 0.5));
		players_[i].btnEndTurn_->setPosition(ct(0) + players_[i].btnCallTroops_->getContentSize().width,
			players_[i].btnCallTroops_->getPosition().y);
		menu_->addChild(players_[i].btnEndTurn_);
		
		
//		players_[i].bgAvatar_ = CCSprite::createWithSpriteFrameName("bg_player_avatar.png");
//		players_[i].bgAvatar_->setAnchorPoint(CCPoint(0.5, 0.5));
//		players_[i].bgAvatar_->setPosition(CCPoint(getContentSize().width - players_[i].bgAvatar_->getContentSize().width * 0.5,
//			(i == 0) ? players_[i].bgAvatar_->getContentSize().height * 0.5 : (getContentSize().height - players_[i].bgAvatar_->getContentSize().height * 0.5) ));
//		addChild(players_[i].bgAvatar_);
		
		players_[i].outAvatar_ = AvatarNode::create(AvatarNodeSize_Small);
		players_[i].outAvatar_->setAnchorPoint(CCPoint(0.5, 0.5));
		players_[i].outAvatar_->setPosition(CCPoint(getContentSize().width - players_[i].outAvatar_->getContentSize().width * 0.5,
			(i == 0) ? players_[i].outAvatar_->getContentSize().height * 0.5 : (getContentSize().height - players_[i].outAvatar_->getContentSize().height * 0.5) ));
		addChild(players_[i].outAvatar_);

		
		players_[i].bgTimeLeft_ = CCSprite::createWithSpriteFrameName("bg_time_left.png");
		players_[i].bgTimeLeft_->setAnchorPoint(CCPoint(0.5, 0.5));
		players_[i].bgTimeLeft_->setPosition(CCPoint(
			(players_[i].btnEndTurn_->getPosition().x + players_[i].btnEndTurn_->getContentSize().width * 0.5 + players_[i].outAvatar_->getPosition().x) * 0.5 ,
			players_[i].btnEndTurn_->getPosition().y ));
		addChild(players_[i].bgTimeLeft_);
		
		players_[i].outTimeLeft_ = CCLabelTTF::create("3", "BadaBoom BB", ct(20));
		players_[i].outTimeLeft_->setColor(ccc3(255, 255, 255));
		players_[i].outTimeLeft_->setAnchorPoint(ccp(0.5, 0.5));
		players_[i].outTimeLeft_->setPosition(CCPoint(players_[i].bgTimeLeft_->getContentSize().width * 0.5,
			players_[i].bgTimeLeft_->getContentSize().height * 0.5 ));
		players_[i].bgTimeLeft_->addChild(players_[i].outTimeLeft_);
		
		
		players_[i].btnMagic_ = menuItemSpriteHighlighted("btn_magic.png", this,
			menu_selector(GameHUDLayer::endRound));
		players_[i].btnMagic_->setAnchorPoint(CCPoint(1, i));
		players_[i].btnMagic_->setPosition(getContentSize().width - ct(32),
			(i == 0) ? ct(2) : (getContentSize().height - ct(2)));
		menu_->addChild(players_[i].btnMagic_);
		
		
		players_[i].healthBar_ = BarNode::barNodeWithSprites(
			CCSprite::createWithSpriteFrameName("bg_health_bar_empty.png"),
			CCSprite::createWithSpriteFrameName("bg_health_bar_filled.png"));
		players_[i].healthBar_->setAnchorPoint(CCPoint(1, 0.5));
		players_[i].healthBar_->setDirection(i == 0 ? BarNodeDirection_Up : BarNodeDirection_Down);

		//float ch = getContentSize().height;
		//players_[i].healthBar_->setPosition(getContentSize().width - ct(1), ch * (0.25 + i * 0.5) + ( (i == 0) ? ct(7) : -ct(7) ));
		players_[i].healthBar_->setPosition(getContentSize().width - ct(1),
			((i == 0) ? players_[i].outAvatar_->getPosition().y + players_[i].outAvatar_->getContentSize().height * 0.5 + ct(2) + players_[i].healthBar_->getContentSize().height * 0.5 :
			players_[i].outAvatar_->getPosition().y - players_[i].outAvatar_->getContentSize().height * 0.5 - ct(2) - players_[i].healthBar_->getContentSize().height * 0.5)
			);
		
		addChild(players_[i].healthBar_);
		
		
		players_[i].outHealthLeft_ = CCLabelBMFont::create(createWithFormat(" %d ", 0).c_str(), cocos2d_utils::textureName("Fonts/badaboom.fnt").c_str());
		players_[i].outHealthLeft_->setAnchorPoint(CCPoint(1, 0.5));
		players_[i].outHealthLeft_->setPosition(getContentSize().width - ct(4),
			((i == 0) ? players_[i].outAvatar_->getPosition().y + players_[i].outAvatar_->getContentSize().height * 0.5 + players_[i].outHealthLeft_->getContentSize().height * 0.5 :
			players_[i].outAvatar_->getPosition().y - players_[i].outAvatar_->getContentSize().height * 0.5 - players_[i].outHealthLeft_->getContentSize().height * 0.5)
			);
		
		addChild(players_[i].outHealthLeft_);
	}
	
	return true;
}

int GameHUDLayer::normalizedPlayer(int player) const
{
	return flipPlayers_ ? ((player + 1) % 2) : player;
}

//void GameHUDLayer::setPlayersCount(int count)
//{
//	playersCount_ = count;
//	
//	if (players_[0].outAvatar_)
//		players_[0].outAvatar_->removeFromParentAndCleanup(true);
//		
//	if (players_[1].outAvatar_)
//		players_[1].outAvatar_->removeFromParentAndCleanup(true);
//		
//		
//	players_[0].outAvatar_ = CCSprite::createWithSpriteFrameName("player1_avatar.png");
//	players_[0].outAvatar_->setAnchorPoint(CCPoint(0.5, 0.5));
//	players_[0].outAvatar_->setPosition(CCPoint(players_[0].bgAvatar_->getContentSize().width * 0.5, players_[0].bgAvatar_->getContentSize().height * 0.5));
//	
//	if (count == 2)
//		players_[1].outAvatar_ = CCSprite::createWithSpriteFrameName("player2_avatar.png");
//	else
//		players_[1].outAvatar_ = CCSprite::createWithSpriteFrameName("comp_avatar.png");
//	
//	players_[1].outAvatar_->setAnchorPoint(CCPoint(0.5, 0.5));
//	players_[1].outAvatar_->setPosition(CCPoint(players_[1].bgAvatar_->getContentSize().width * 0.5, players_[1].bgAvatar_->getContentSize().height * 0.5));
//	
//	players_[0].bgAvatar_->addChild(players_[0].outAvatar_);
//	players_[1].bgAvatar_->addChild(players_[1].outAvatar_);
//	
//	players_[0].previousTurnsLeft_ = -1;
//	players_[1].previousTurnsLeft_ = -1;
//}

void GameHUDLayer::callTroops(cocos2d::CCNode* sender)
{
	assert(delegate_);

	if (sender == players_[0].btnCallTroops_)
		delegate_->callTroops(0);
	else if (sender == players_[1].btnCallTroops_)
		delegate_->callTroops(1);
}

void GameHUDLayer::endRound(cocos2d::CCNode* sender)
{
	assert(delegate_);

	if (sender == players_[0].btnEndTurn_)
		delegate_->endRound(0);
	else if (sender == players_[1].btnEndTurn_)
		delegate_->endRound(1);
}

void GameHUDLayer::magic(cocos2d::CCNode* sender)
{
	assert(delegate_);

	if (sender == players_[0].btnMagic_)
		delegate_->magic(0);
	else if (sender == players_[1].btnMagic_)
		delegate_->magic(1);
}

void GameHUDLayer::setTimeLeft(int player, int time)
{
	player = normalizedPlayer(player);
	players_[player].outTimeLeft_->setString(createWithFormat(" %d:%02d ",
		time / 60, time % 60).c_str());
}

void GameHUDLayer::setTimeLeftVisible(int player, bool visible)
{
	player = normalizedPlayer(player);
	players_[player].bgTimeLeft_->setVisible(visible);
	players_[player].outTimeLeft_->setVisible(visible);
	
	if(!visible)
		players_[player].previousTurnsLeft_ = -1;
}

void GameHUDLayer::setHealth(int player, int health)
{
	player = normalizedPlayer(player);
	players_[player].healthBar_->setValue((float)health / (float)kInitialHealth);
	players_[player].outHealthLeft_->setString(createWithFormat(" %d ", health).c_str());
}

void GameHUDLayer::setMagic(int player, int magic)
{
	player = normalizedPlayer(player);
	players_[player].magicBar_->setValue((float)magic / (float)kMaxMagic);
}

void GameHUDLayer::setTroopsLeft(int player, int troops)
{
	player = normalizedPlayer(player);
	players_[player].btnCallTroops_->setEnabled(troops > 0);
	
	players_[player].outTroopsLeft_->setString(CCString::createWithFormat(" %d ", troops)->
		getCString());
	
	players_[player].outTroopsLeft_->setOpacity(troops > 0 ? 255 : 128);
//	players_[player].bgTroopsLeft_->setOpacity(troops > 0 ? 255 : 128);
				
//	players_[player].outTroopsLeft_->setVisible(players_[player].btnCallTroops_->isVisible() && troops > 0);
//	players_[player].bgTroopsLeft_->setVisible(players_[player].btnCallTroops_->isVisible() && troops > 0);
}

void GameHUDLayer::setTurnsLeft(int player, int turns, bool doNotAnimate)
{
	player = normalizedPlayer(player);
	if (players_[player].previousTurnsLeft_ <= turns && players_[player].previousTurnsLeft_ != -1 &&
		!doNotAnimate)
	{
		players_[player].outTurnsLeft_->runAction(CCSequence::create(
			CCScaleTo::create(0.25, 1.5),
			CCScaleTo::create(0.25, 1.0),
			0));
		players_[player].bgTurnsLeft_->runAction(CCSequence::create(
			CCScaleTo::create(0.25, 1.5),
			CCScaleTo::create(0.25, 1.0),
			0));
	}
	
	players_[player].outTurnsLeft_->setString(CCString::createWithFormat(" %d ", turns)->
		getCString());
		
	players_[player].previousTurnsLeft_ = turns;
}

void GameHUDLayer::setCallTroopsVisible(int player, bool visible)
{
	player = normalizedPlayer(player);
	players_[player].btnCallTroops_->setVisible(visible);
}

void GameHUDLayer::setEndTurnVisible(int player, bool visible)
{
	player = normalizedPlayer(player);
	players_[player].btnEndTurn_->setVisible(visible);
}

void GameHUDLayer::setAvatarType(int player, AvatarNodeType type)
{
	player = normalizedPlayer(player);
	players_[player].outAvatar_->setAvatarType(type);
}

void GameHUDLayer::setAvatarUrl(int player, const std::string& url)
{
	player = normalizedPlayer(player);
	players_[player].outAvatar_->setAvatarUrl(url);
}

//void GameHUDLayer::setEndTurnEnabled(bool enabled, int player)
//{
//	if (player == -1)
//	{
//		players_[0].btnEndTurn_->setEnabled(enabled);
//		players_[1].btnEndTurn_->setEnabled(enabled);
//	}
//	else
//	{
//		players_[player].btnEndTurn_->setEnabled(enabled);
//	}
//}

void GameHUDLayer::setTurnsLeftVisible(int player, bool visible)
{
	player = normalizedPlayer(player);
	players_[player].bgTurnsLeft_->setVisible(visible);
}

void GameHUDLayer::setMagicButtonVisible(int player, bool visible)
{
	player = normalizedPlayer(player);
	players_[player].btnMagic_->setVisible(visible);
}

void GameHUDLayer::setTouchEnabled(bool enabled)
{
	CCLayer::setTouchEnabled(enabled);
	menu_->setTouchEnabled(enabled);
}


