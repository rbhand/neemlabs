//
//  GameHUDLayer.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/1/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "BarNode.h"
#include "AvatarNode.h"

namespace monsters
{
	enum GameConfigurationScreenType
	{
		GameConfigurationScreenType_SinglePlayer = 0,
		GameConfigurationScreenType_TwoPlayers = 1,
		GameConfigurationScreenType_OnlineGame = 2
	};
	
	class IGameHUDLayerDelegate
	{
	public:
		virtual void callTroops(int player) = 0;
		virtual void endRound(int player) = 0;
		virtual void magic(int player) = 0;
	};

	class GameHUDLayer : public cocos2d::CCLayer
	{
	public:
		bool init();
		
		void setFlipPlayers(bool flipPlayers) { flipPlayers_ = flipPlayers; }
		
		void setHealth(int player, int health);
		void setMagic(int player, int magic);
		void setTroopsLeft(int player, int troops);
		void setTurnsLeft(int player, int turns, bool doNotAnimate=false);
		void setTimeLeft(int player, int time);
		
		void setCallTroopsVisible(int player, bool visible);
		void setEndTurnVisible(int player, bool visible);
		void setTurnsLeftVisible(int player, bool visible);
		void setMagicButtonVisible(int player, bool visible);
		void setTimeLeftVisible(int player, bool visible);
		
		void setAvatarType(int player, AvatarNodeType type);
		void setAvatarUrl(int player, const std::string& url);
		
		//void setEndTurnEnabled(bool enabled, int player=-1);
		
		void setDelegate(IGameHUDLayerDelegate* delegate) { delegate_ = delegate; }
		
		CREATE_FUNC(GameHUDLayer);
		
		void setTouchEnabled(bool enabled);
		
	private:
		void callTroops(cocos2d::CCNode* sender);
		void endRound(cocos2d::CCNode* sender);
		void magic(cocos2d::CCNode* sender);
		
		int normalizedPlayer(int player) const;
	
	private:
		struct 
		{
			BarNode* healthBar_;
			BarNode* magicBar_;
			cocos2d::CCLabelBMFont* outHealthLeft_;
			cocos2d::CCLabelTTF* outTroopsLeft_;
			cocos2d::CCSprite* bgTurnsLeft_;
			cocos2d::CCLabelTTF* outTurnsLeft_;
			cocos2d::CCMenuItemSprite* btnCallTroops_;
			cocos2d::CCMenuItemSprite* btnEndTurn_;
			cocos2d::CCMenuItemSprite* btnMagic_;
			cocos2d::CCSprite* bgTimeLeft_;
			cocos2d::CCLabelTTF* outTimeLeft_;
			
			//cocos2d::CCSprite* bgAvatar_;
			AvatarNode* outAvatar_;
			int previousTurnsLeft_;
		}
		players_[2];
		
		cocos2d::CCMenu* menu_;
		
		IGameHUDLayerDelegate* delegate_;
		bool flipPlayers_;
	};
}
