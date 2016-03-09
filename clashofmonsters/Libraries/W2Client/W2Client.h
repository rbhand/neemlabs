//
//  TRClient.h
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientUtils.h"
#import "W2ClientRequest.h"
#import "W2ClientRequestQueue.h"
#import "W2ClientOrderedParameters.h"

@interface W2Client : NSObject
{
	NSString* host_;
	NSString* pathPrefix_;
	short port_;
	short securePort_;
	BOOL overrideSecure_;
	
	W2ClientSignatureType signatureType_;
	NSString* signatureKey_;
	W2ClientOrderedParameters* signatureParameters_;
	
	W2ClientRequestQueue* mainQueue_;
}

- (id)initWithHost:(NSString*)host pathPrefix:(NSString*)pathPrefix;

- (void)setSignatureKey:(NSString*)signatureKey type:(W2ClientSignatureType)signatureType;

- (W2ClientRequest*)get:(NSString*)path delegate:(id)delegate;
- (W2ClientRequest*)get:(NSString*)path completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)get:(NSString*)path target:(id)target selector:(SEL)selector;
- (W2ClientRequest*)get:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)get:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector;
- (W2ClientRequest*)get:(NSString*)path params:(NSDictionary*)params completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)getSecure:(NSString*)path delegate:(id)delegate;
- (W2ClientRequest*)getSecure:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)getLong:(NSString*)path delegate:(id)delegate;
- (W2ClientRequest*)getLong:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)getLong:(NSString*)path target:(id)target selector:(SEL)selector;

- (W2ClientRequest*)post:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)post:(NSString*)path params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)postSecure:(NSString*)path params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)postLong:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)postSecure:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)post:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector;
- (W2ClientRequest*)postLong:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector;
- (W2ClientRequest*)postSecure:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector;

- (W2ClientRequest*)put:(NSString*)path data:(NSData*)data delegate:(id)delegate;
- (W2ClientRequest*)put:(NSString*)path data:(NSData*)data completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)put:(NSString*)path data:(NSData*)data contentType:(NSString*)contentType
	completion:(W2ClientRequestCompletionBlock)completion;

- (W2ClientRequest*)get:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)get:(NSString*)path sign:(BOOL)sign
	completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)get:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params completion:(W2ClientRequestCompletionBlock)completion;
- (W2ClientRequest*)postSecure:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)postSecure:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params target:(id)target selector:(SEL)selector;
- (W2ClientRequest*)postSecure:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params completion:(W2ClientRequestCompletionBlock)completion;

- (W2ClientRequest*)post:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params delegate:(id)delegate;
- (W2ClientRequest*)post:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params target:(id)target selector:(SEL)selector;
- (W2ClientRequest*)post:(NSString*)path sign:(BOOL)sign params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion;

@property (readonly, nonatomic) W2ClientOrderedParameters* signatureParameters;
@property (readwrite, nonatomic) short port;
@property (readwrite, nonatomic) short securePort;
@property (readwrite, nonatomic) BOOL overrideSecure;

@property (readonly, nonatomic) W2ClientRequestQueue* mainQueue;

@end
