//
//  AvatarNode.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/27/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "Session.h"
//#include "AvatarsCache.h"
#include <string>

#define kAvatarNodeSize_Small ct(51)
#define kAvatarImageNodeSize_Small ct(38)

#define kAvatarNodeSize_Large ct(60)
#define kAvatarImageNodeSize_Large ct(56)

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	enum AvatarNodeType
	{
		AvatarNodeType_None = 0,
		AvatarNodeType_1stPlayer = 1,
		AvatarNodeType_2ndPlayer = 2,
		AvatarNodeType_AI = 3,
		AvatarNodeType_Url = 4
	};
	
	enum AvatarNodeSize
	{
		AvatarNodeSize_Small = 0,
		AvatarNodeSize_Large = 1
	};
	
	class AvatarImageNode : public CCNode
	{
	public:
		static AvatarImageNode* create(AvatarNodeSize size = AvatarNodeSize_Small);
		bool init(AvatarNodeSize size = AvatarNodeSize_Small);
		
		//CREATE_FUNC(AvatarImageNode);
		
		void setAvatarUrl(const std::string& avatarUrl);
		void setAvatarType(AvatarNodeType type);
		void onAvatarImageLoaded(const std::string& url, cocos2d::CCSpriteFrame* spriteFrame, const Error& error);
		
	private:
		void visit();
		
	private:
		CCSprite* avatarImage_;
		std::string avatarUrl_;
		float avatarImageSize_;
	};
	
	
	class AvatarNode : public CCNode
	{
	public:
		static AvatarNode* create(AvatarNodeSize size = AvatarNodeSize_Small);
		bool init(AvatarNodeSize size = AvatarNodeSize_Small);
		
		CREATE_FUNC(AvatarNode);
		
		void setAvatarUrl(const std::string& avatarUrl);
		void setAvatarType(AvatarNodeType type);
	
	private:
		AvatarImageNode* avatarImage_;
		CCSprite* bg_;
	};
}
