//
//  W2FacebookSharing.m
//
//  Created by Vitaliy Ivanov on 5/7/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#import "W2FacebookSharing.h"

#import <FacebookSDK/FacebookSDK.h>
#import <Social/Social.h>
//#import <Parse/Parse.h>

@implementation W2FacebookSharing

@synthesize link, picture, name, caption, description;

//- (FBAppCall*)publishWithShareDialogCompletion:(void(^)(BOOL published, NSError* error))completion
//{
//	// Set up the dialog parameters
//	FBShareDialogParams *params = [[FBShareDialogParams alloc] init];
//	params.link = self.link;
//	params.picture = self.picture;
//	params.name = self.name;
//	params.caption = self.caption;
//	params.description = self.description;
//
//	// Set this flag on to enable the Share Dialog beta feature
//	[FBSettings enableBetaFeature:FBBetaFeaturesShareDialog];
//
//	return [FBDialogs presentShareDialogWithParams:params clientState:nil handler:
//		^(FBAppCall *call, NSDictionary *results, NSError *error)
//		{
//			if (error)
//			{
//				completion(NO, error);
//			}
//			else
//			{
//				// Check if cancel info is returned and log the event
//				if (results[@"completionGesture"] && [results[@"completionGesture"] isEqualToString:@"cancel"])
//				{
//					NSLog(@"User canceled story publishing");
//					completion(NO, nil);
//				}
//				else
//				{
//					// If the post went through show a success message
//					completion(YES, nil);
//				}
//			}
//		}
//	];
//}


- (NSDictionary*)parseURLParams:(NSString *)query
{
    NSArray *pairs = [query componentsSeparatedByString:@"&"];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    for (NSString *pair in pairs)
	{
        NSArray *kv = [pair componentsSeparatedByString:@"="];
        NSString *val =
        [[kv objectAtIndex:1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];

        [params setObject:val forKey:[kv objectAtIndex:0]];
    }
    return params;
}

- (void)presentWebDialogWithCompletion:(void(^)(BOOL published, NSError* error))completion
{
//	// Invoke the dialog
//	[FBDialogs presentShareDialogWithLink:self.link ? self.link : nil
//		name:self.name ? self.name : @""
//		caption:self.caption ? self.caption : @""
//		description:self.description ? self.description : @""
//		picture:self.picture ? self.picture : nil clientState:nil
//		handler:^(FBAppCall *call, NSDictionary *results, NSError *error)
//	{
//		if (error)
//		{
//			// Error launching the dialog or publishing a story.
//			completion(NO, error);
//		}
//		else
//		{
//			
//		}
//	}];

	[FBWebDialogs presentFeedDialogModallyWithSession:nil
		parameters: @{
			@"name": self.name ? self.name : @"",
			@"caption": self.caption ? self.caption : @"",
			@"description": self.description ? self.description : @"",
			@"link": self.link ? [self.link absoluteString] : @"",
			@"picture": self.picture ? [self.picture absoluteString] : @"",
			@"app_id": [[NSBundle mainBundle] objectForInfoDictionaryKey:@"FacebookAppID"]
		}
		handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
		{
			if (error)
			{
				// Error launching the dialog or publishing a story.
				completion(NO, error);
			}
			else
			{
				if (result == FBWebDialogResultDialogNotCompleted)
				{
					// User clicked the "x" icon
					NSLog(@"User canceled story publishing.");
					completion(NO, nil);
				}
				else
				{
					NSDictionary *urlParams = [self parseURLParams:[resultURL query]];
					if (![urlParams valueForKey:@"post_id"])
					{
						 // User clicked the Cancel button
                         NSLog(@"User canceled request.");
						 completion(NO, nil);
					}
					else
					{
						// User clicked the Share button
						completion(YES, nil);
					}
				}
			}
		}
	];
}

- (void)shareWithCompletion:(void(^)(BOOL published, NSError* error))completion
{
	SLComposeViewController* composer = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
	[composer addURL:self.link];
	[composer setInitialText:self.description];
	[composer addImage:[UIImage imageNamed:@"iTunesArtwork"]];
	[((UIWindow*)[UIApplication sharedApplication].windows[0]).rootViewController presentViewController:composer animated:YES completion:^
	{
	}];
	[composer setCompletionHandler:^(SLComposeViewControllerResult result)
	{
		if (result == SLComposeViewControllerResultDone)
			completion(YES, nil);
		else
			completion(NO, nil);
	}];
	return;

//	BOOL displayedNativeDialog = [FBDialogs presentOSIntegratedShareDialogModallyFrom:[[UIApplication sharedApplication].windows[0] rootViewController]
//		initialText:self.description
//		image:[UIImage imageNamed:@"iTunesArtwork"]
//		url:self.link
//		handler:^(FBOSIntegratedShareDialogResult result, NSError *error)
//	{
//		if (result == FBOSIntegratedShareDialogResultSucceeded)
//		{
//			completion(YES, nil);
//		}
//		else
//		{
//			completion(NO, [[NSError alloc] initWithDomain:@"W2FacebookSharing"
//				code:error.fberrorCategory userInfo:@{ NSLocalizedDescriptionKey: error.fberrorUserMessage }]);
//		}
//	}];

	//if (!displayedNativeDialog)
	{
		[FBSession openActiveSessionWithPublishPermissions:@[ @"publish_actions" ]
			defaultAudience:FBSessionDefaultAudienceEveryone allowLoginUI:YES
			completionHandler:^(FBSession *session, FBSessionState status, NSError *error)
			{
				if (status == FBSessionStateOpen ||
					status == FBSessionStateOpenTokenExtended)
				{
					[self presentWebDialogWithCompletion:completion];
				}
				else if (status == FBSessionStateClosedLoginFailed)
				{
					if (![error.userInfo[@"com.facebook.sdk:ErrorLoginFailedReason"]
						isEqualToString:@"com.facebook.sdk:UserLoginCancelled"])
					{
						completion(NO, error);
					}
					else
					{
						completion(NO, nil);
					}
				}
			}
		];
	}
}


- (void)presentWebApprequestDialogWithCompletion:(void(^)(BOOL published, NSError* error))completion
{
	// Invoke the dialog
	[FBWebDialogs presentRequestsDialogModallyWithSession:nil
		message: @"Hey man, check this new game!"
		title: @"Invite to PiratesVSNinjas game"
		parameters: @{
			/*@"name": self.name ? self.name : @"",
			@"caption": self.caption ? self.caption : @"",
			@"description": self.description ? self.description : @"",
			@"link": self.link ? [self.link absoluteString] : @"",
			@"picture": self.picture ? [self.picture absoluteString] : @""*/
		}
		handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
		{
			if (error)
			{
				// Error launching the dialog or publishing a story.
				completion(NO, error);
			}
			else
			{
				if (result == FBWebDialogResultDialogNotCompleted)
				{
					// User clicked the "x" icon
					NSLog(@"User canceled story publishing.");
					completion(NO, nil);
				}
				else
				{
					NSDictionary *urlParams = [self parseURLParams:[resultURL query]];
					if (![urlParams valueForKey:@"request"])
					{
						 // User clicked the Cancel button
                         NSLog(@"User canceled request.");
						 completion(NO, nil);
					}
					else
					{
						// User clicked the Share button
						completion(YES, nil);
					}
				}
			}
		}
	];
}


- (void)inviteWithCompletion:(void(^)(BOOL published, NSError* error))completion
{
	[FBSession openActiveSessionWithPublishPermissions:@[ @"publish_actions" ]
		defaultAudience:FBSessionDefaultAudienceEveryone allowLoginUI:YES
		completionHandler:^(FBSession *session, FBSessionState status, NSError *error)
		{
			if (status == FBSessionStateOpen ||
				status == FBSessionStateOpenTokenExtended)
			{
				[self presentWebApprequestDialogWithCompletion:completion];
			}
			else if (status == FBSessionStateClosedLoginFailed)
			{
				if (![error.userInfo[@"com.facebook.sdk:ErrorLoginFailedReason"]
					isEqualToString:@"com.facebook.sdk:UserLoginCancelled"])
				{
					completion(NO, error);
				}
				else
				{
					completion(NO, nil);
				}
			}
		}
	];
}

@end

