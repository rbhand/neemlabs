//
//  BackgroundJob.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/13/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

namespace f17y
{
namespace utils
{
	class BackgroundJob;
	class IBackgroundJobDelegate
	{
	public:
		virtual void onBackgroundJobCompleted(BackgroundJob* job) = 0;
	};

	struct BackgroundJobPrivate;
	class BackgroundJob : public cocos2d::CCObject
	{
	    friend struct BackgroundJobPrivate;

	public:
	    BackgroundJob();
		virtual ~BackgroundJob();
	
		void run();
		
		void setDelegate(IBackgroundJobDelegate* delegate) { delegate_ = delegate; }
		IBackgroundJobDelegate* delegate() const { return delegate_; }
	
	protected:
		virtual void main() = 0;

	private:
	    void checkForCompletion();
	
	private:
		IBackgroundJobDelegate* delegate_;
		BackgroundJobPrivate* d_;
	};
}
}
