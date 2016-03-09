//
//  W2FacebookFriendInviter.m
//
//  Created by Vitaliy Ivanov on 11/19/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2FacebookFriendInviter.h"
#import <FacebookSDK/FacebookSDK.h>
#import <FacebookSDK/Facebook.h>

@interface W2FacebookFriendInviter() <FBDialogDelegate>

@end


@implementation W2FacebookFriendInviter

@synthesize isRunning = running_;

- (id)initWithFacebook:(Facebook*)facebook userId:(NSString*)userId
	completion:(W2InviteFacebookFriendBlock)completion
{
	if ((self = [super init]))
	{
		facebook_ = facebook;
		userId_ = userId;
		completion_ = [completion copy];
	}
	
	return self;
}

- (void)dialogDidComplete:(FBDialog *)dialog
{
	completion_(YES, nil);
}

- (void)dialogDidNotComplete:(FBDialog *)dialog
{
	completion_(NO, nil);
}

- (void)dialog:(FBDialog*)dialog didFailWithError:(NSError *)error
{
	completion_(NO, error);
}

- (void)run
{
	running_ = YES;
	[facebook_ dialog:@"apprequests"
		andParams:[NSMutableDictionary dictionaryWithDictionary:@{
			@"to": userId_,
			@"message": NSLocalizedString(@"Let's play a Game of Words!", @"")
		}]
		andDelegate:self];
}

+ (W2FacebookFriendInviter*)inviteWithFacebook:(Facebook*)facebook
	userId:(NSString*)userId completion:(W2InviteFacebookFriendBlock)completion
{
	W2FacebookFriendInviter* inviter = [[W2FacebookFriendInviter alloc] initWithFacebook:facebook
		userId:userId completion:completion];
	[inviter run];
	return inviter;
}

@end
