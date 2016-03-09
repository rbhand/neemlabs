//
//  AvatarsCache.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 3/28/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include <string>

#include "Callback.h"
#include "Error.h"

namespace monsters
{
	class Session;
	class AvatarsCachePrivate;
	
	class AvatarsCache
	{
		friend class Session;
	
	public:
		typedef void(cocos2d::CCObject::*LoadSelector)(const std::string& url, cocos2d::CCSpriteFrame* spriteFrame, const Error& error);
		typedef w2f2::Callback<LoadSelector> LoadCallback;
	
	public:
		AvatarsCache();
		~AvatarsCache();
		
		void load(const std::string& url, const LoadCallback& callback);
		
	protected:
		void setClient(void* client);
		
	private:
		AvatarsCachePrivate* d_;
	};
}
