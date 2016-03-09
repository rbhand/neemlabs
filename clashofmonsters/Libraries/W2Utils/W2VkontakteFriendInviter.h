//
//  W2VkontakteFriendInviter.h
//  RussianScrabble
//
//  Created by Vitaliy Ivanov on 11/20/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^W2InviteVkontakteFriendBlock)(BOOL invited, NSError* error);

@interface W2VkontakteFriendInviter : NSObject

+ (void)inviteUserId:(NSString*)userId accessToken:(NSString*)accessToken
	text:(NSString*)text link:(NSString*)link image:(NSString*)image
	completion:(W2InviteVkontakteFriendBlock)completion;

@end
