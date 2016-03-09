//
//  W2ClientRequestQueue.h
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientRequest.h"

@interface W2ClientRequestQueue : NSObject
{
	NSMutableArray* queued_;
	NSMutableSet* running_;
	NSMutableSet* longRunning_;
}

- (void)enqueue:(W2ClientRequest*)request;
- (void)cancel:(W2ClientRequest*)request;

- (void)clear;

@end
