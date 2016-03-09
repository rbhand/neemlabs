//
//  W2ClientRequestQueue.m
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientRequestQueue.h"

@interface W2ClientRequest (W2ClientRequestQueue_Shared)

- (void)startWithQueue:(W2ClientRequestQueue*)queue;

@end

@implementation W2ClientRequestQueue

- (id)init
{
	if ((self = [super init]))
	{
		queued_ = [[NSMutableArray alloc] init];
		running_ = [[NSMutableSet alloc] init];
		longRunning_ = [[NSMutableSet alloc] init];
	}
	
	return self;
}

- (void)enqueue:(W2ClientRequest*)request
{
	if (request.flags & kW2ClientRequestFlag_LongPolling)
	{
		[longRunning_ addObject:request];
		[request startWithQueue:self];
	}
	else
	{
		if ([running_ count] >= 3)
		{
			[queued_ addObject:request];
		}
		else
		{
			[running_ addObject:request];
			[request startWithQueue:self];
		}
	}
}

- (void)cancel:(W2ClientRequest*)request
{
	if ([queued_ containsObject:request])
		[queued_ removeObject:request];
	else
	{
		[self requestDidFinish:request];
	}
}

- (void)requestDidFinish:(W2ClientRequest*)request
{
	if (request.flags & kW2ClientRequestFlag_LongPolling)
	{
		[longRunning_ removeObject:request];
	}
	else if ([running_ containsObject:request])
	{
		if ([queued_ count])
		{
			W2ClientRequest* req = [queued_ objectAtIndex:0];
			[queued_ removeObjectAtIndex:0];
			[running_ addObject:req];
			[req startWithQueue:self];
		}
		
		[running_ removeObject:request];
	}
}

- (void)clear
{
	NSSet* long_running = [NSSet setWithSet:longRunning_];
	for (W2ClientRequest* req in long_running)
	{
		[req cancel];
	}
	
	NSSet* running = [NSSet setWithSet:running_];
	for (W2ClientRequest* req in running)
	{
		[req cancel];
	}
	
	[longRunning_ removeAllObjects];
	[running_ removeAllObjects];
	[queued_ removeAllObjects];
}

@end
