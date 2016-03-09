//
//  W2FacebookFriendInviter.h
//

#import <Foundation/Foundation.h>

typedef void(^W2InviteFacebookFriendBlock)(BOOL invited, NSError* error);

@class Facebook;
@interface W2FacebookFriendInviter : NSObject
{
	Facebook* facebook_;
	NSString* userId_;
	W2InviteFacebookFriendBlock completion_;
	BOOL running_;
}

+ (W2FacebookFriendInviter*)inviteWithFacebook:(Facebook*)facebook
	userId:(NSString*)userId completion:(W2InviteFacebookFriendBlock)completion;

@property (readonly, nonatomic) BOOL isRunning;

@end
