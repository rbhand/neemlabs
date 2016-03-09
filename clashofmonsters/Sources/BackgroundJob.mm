//
//  BackgroundJob.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/13/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "BackgroundJob.h"

using namespace f17y::utils;

void BackgroundJob::run()
{
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
		main();
		
		dispatch_async(dispatch_get_main_queue(), ^{
			if (delegate_)
				delegate_->onBackgroundJobCompleted(this);
		});
	});
}

