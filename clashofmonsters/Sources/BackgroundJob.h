//
//  BackgroundJob.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/13/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

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
	
	class BackgroundJob
	{
	public:
		BackgroundJob() : delegate_(0) { }
		virtual ~BackgroundJob() { }
	
		void run();
		
		void setDelegate(IBackgroundJobDelegate* delegate) { delegate_ = delegate; }
		IBackgroundJobDelegate* delegate() const { return delegate_; }
	
	protected:
		virtual void main() = 0;
	
	private:
		IBackgroundJobDelegate* delegate_;
	};
}
}
