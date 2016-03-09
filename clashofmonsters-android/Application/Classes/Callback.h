//
//  Callback.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 3/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

namespace w2f2
{
	typedef void* OperationHandle;

	template <typename SelectorT>
	class Callback
	{
	public:
		Callback() :
			target_(0), selector_(0)
		{
		}
	
		Callback(cocos2d::CCObject* target, SelectorT selector) :
			target_(target), selector_(selector)
		{
			target_->retain();
		}
		
		Callback(const Callback& c) :
			target_(c.target_), selector_(c.selector_)
		{
			if (target_)
				target_->retain();
		}
		
		Callback& operator=(const Callback& c)
		{
			if (target_)
				target_->release();
		
			target_ = c.target_;
			selector_ = c.selector_;
			
			if (target_)
				target_->retain();
			
			return *this;
		}
		
		~Callback()
		{
			if (target_)
				target_->release();
		}
		
		cocos2d::CCObject* target() const
		{
			return target_;
		}
		
		SelectorT selector() const
		{
			return selector_;
		}
		
		void call() const
		{
			if (!target_)
				return;
			(target_->*selector_)();
		}
		
		bool isNull() const
		{
			return target_ == 0;
		}
		
		template <typename P1>
		void call(P1 param1) const
		{
			if (!target_)
				return;
			(target_->*selector_)(param1);
		}
		
		template <typename P1, typename P2>
		void call(P1 param1, P2 param2) const
		{
			if (!target_)
				return;
			(target_->*selector_)(param1, param2);
		}
		
		template <typename P1, typename P2, typename P3>
		void call(P1 param1, P2 param2, P3 param3) const
		{
			if (!target_)
				return;
			(target_->*selector_)(param1, param2, param3);
		}
		
	private:
		cocos2d::CCObject* target_;
		SelectorT selector_;
	};
}
