//
//  W2VkontakteFriendInviter.m
//  RussianScrabble
//
//  Created by Vitaliy Ivanov on 11/20/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2VkontakteFriendInviter.h"
#import "NSURLConnection+Blocks.h"
#import "W2ClientUtils.h"
#import "JSONKit.h"

@implementation W2VkontakteFriendInviter

+ (void)inviteUserId:(NSString*)userId accessToken:(NSString*)accessToken
	text:(NSString*)text link:(NSString*)link image:(NSString*)image
	completion:(W2InviteVkontakteFriendBlock)completion
{
	[NSURLConnection asyncRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:[NSString
		stringWithFormat:@"https://api.vk.com/method/wall.post?owner_id=%@&message=%@&attachments=%@,%@&access_token=%@",
		userId,
		W2URLEscapedString(text),
		W2URLEscapedString(link),
		image,
		accessToken]]]
	success:^(NSData* data, NSURLResponse* response)
	{
		NSDictionary* error_dict = W2GetDictionaryValue([data objectFromJSONData], @"error");
		if (!error_dict)
		{
			completion(YES, nil);
		}
		else
		{
			completion(NO, [[NSError alloc] initWithDomain:@"Vkontakte"
				code:[W2GetNumberValue(error_dict, @"error_code") intValue]
				userInfo:@{
					NSLocalizedDescriptionKey: W2GetStringValue(error_dict, @"error_msg")
				}]);
		}
	}
	failure:^(NSData* data, NSError* error)
	{
		completion(NO, error);
	}];
}

@end
