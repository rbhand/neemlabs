//
//  TRClientRequest.h
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientOrderedParameters.h"
#import "W2ClientErrors.h"

typedef enum
{
	W2ClientRequestState_Inactive,
	W2ClientRequestState_Queued,
	W2ClientRequestState_Completed,
	W2ClientRequestState_Working,
	W2ClientRequestState_Cancelled
}
W2ClientRequestState;

typedef enum
{
	W2ClientSignatureType_None,
	W2ClientSignatureType_MD5_HMAC
}
W2ClientSignatureType;


@class W2ClientRequest;
typedef void (^W2ClientRequestCompletionBlock)(W2ClientRequest* request);

#define kW2ClientRequestFlag_None           0x0000
#define kW2ClientRequestFlag_LongPolling    0x0001

@class W2ClientRequest;
@protocol W2ClientRequestDelegate

- (void)requestDidFinish:(W2ClientRequest*)request;

@end

@interface W2ClientRequestDidFinishInvocation : NSObject <W2ClientRequestDelegate>
{
	NSInvocation* invoke_;
	W2ClientRequestCompletionBlock completion_;
}

+ (W2ClientRequestDidFinishInvocation*)invocationWithTarget:(id)target selector:(SEL)selector;
+ (W2ClientRequestDidFinishInvocation*)invocationWithCompletion:(W2ClientRequestCompletionBlock)completion;

@end

#define W2Client_OnFinish(target, selector) []


@class W2ClientRequestQueue;
@interface W2ClientRequest : NSObject
{
	NSString* method_;
	NSURL* url_;
	W2ClientOrderedParameters* params_;
	int flags_;
	W2ClientRequestState state_;
	NSURLConnection* conn_;
	
	NSMutableData* data_;
	int httpCode_;
	BOOL isJson_;
	NSError* error_;
	NSDictionary* response_;
	
	W2ClientSignatureType signatureType_;
	NSString* signatureKey_;
	
	__unsafe_unretained W2ClientRequestQueue* queue_;
	
	W2ClientRequestDidFinishInvocation* invoke_;
	__unsafe_unretained id delegate_;
}

- (id)initWithMethod:(NSString*)method url:(NSURL*)url params:(W2ClientOrderedParameters*)params
	flags:(int)flags signatureType:(W2ClientSignatureType)signatureType signatureKey:(NSString*)signatureKey;
	
- (void)cancel;

//- (void)run:(W2ClientRequestQueue*)queue;

@property (readonly, nonatomic) NSError* error;
@property (readonly, nonatomic) NSDictionary* response;
@property (readonly, nonatomic) NSData* data;
@property (readonly, nonatomic) int flags;
@property (readonly, nonatomic) NSURL* url;

@property (readonly, assign, nonatomic) W2ClientRequestState state;
@property (readwrite, assign, nonatomic) id delegate;

@end
