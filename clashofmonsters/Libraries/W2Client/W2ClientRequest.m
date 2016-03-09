//
//  TRClientRequest.m
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientRequest.h"
#import "JSONKit.h"
#import "W2ClientRequestQueue.h"
#import "W2ClientUtils.h"
#import "NSString+HMAC.h"
#import "NSData+Base64.h"

@interface W2ClientRequestQueue (W2ClientRequest_Shared)

- (void)requestDidFinish:(W2ClientRequest*)request;

@end


@implementation W2ClientRequestDidFinishInvocation

+ (W2ClientRequestDidFinishInvocation*)invocationWithTarget:(id)target selector:(SEL)selector
{
	W2ClientRequestDidFinishInvocation* inv = [[W2ClientRequestDidFinishInvocation alloc] init];
	inv->invoke_ = [NSInvocation invocationWithMethodSignature:[target methodSignatureForSelector:selector]];
	[inv->invoke_ setTarget:target];
	[inv->invoke_ setSelector:selector];
	return inv;
}

- (void)dealloc
{
}

+ (W2ClientRequestDidFinishInvocation*)invocationWithCompletion:(W2ClientRequestCompletionBlock)completion
{
	W2ClientRequestDidFinishInvocation* inv = [[W2ClientRequestDidFinishInvocation alloc] init];
	inv->completion_ = [completion copy];
	return inv;
}

- (void)requestDidFinish:(W2ClientRequest*)request
{
	if (invoke_)
	{
		[invoke_ setArgument:&request atIndex:2];
		[invoke_ invoke];
	}
	
	if (completion_)
	{
		completion_(request);
	}
}

@end


@interface W2ClientRequest () <NSURLConnectionDelegate>

@end

#define kW2ClientFormDataBoundary    @"0xKhTmLbOuNdArY"
#define kW2ConnectTimeout            10.0
#define kW2ConnectLongTimeout        (2*60.0)
//(5*60.0)

@implementation W2ClientRequest

@synthesize state = state_, error = error_, url = url_,
	response = response_, data = data_, flags = flags_,
	delegate = delegate_;

- (id)initWithMethod:(NSString*)method url:(NSURL*)url params:(W2ClientOrderedParameters*)params
	flags:(int)flags signatureType:(W2ClientSignatureType)signatureType signatureKey:(NSString*)signatureKey
{
	if ((self = [super init]))
	{
		NSAssert(([method isEqualToString:@"GET"] || [method isEqualToString:@"POST"] ||
			[method isEqualToString:@"PUT"] || [method isEqualToString:@"DELETE"]), @"");
	
		method_ = method;
		url_ = url;
		params_ = params;
		flags_ = flags;
		state_ = W2ClientRequestState_Inactive;
		
		signatureType_ = signatureType;
		signatureKey_ = signatureKey;
	}
	
	return self;
}

- (void)dealloc
{
}

- (void)cancel
{
	if (state_ == W2ClientRequestState_Working)
	{
		[conn_ cancel];
		[queue_ cancel:self];
	}
	else if (state_ == W2ClientRequestState_Queued)
	{
		[queue_ cancel:self];
	}
	
	state_ = W2ClientRequestState_Cancelled;
}

//- (void)run:(W2ClientRequestQueue*)queue
//{
//	queue_ = queue;
//	state_ = W2ClientRequestState_Queued;
//}

- (NSString*)contentTypeForPOSTRequest
{
//	return [NSString stringWithFormat:@"multipart/form-data; boundary=%@",
//		kW2ClientFormDataBoundary];
	if ([method_ isEqualToString:@"POST"])
		return @"application/x-www-form-urlencoded";
	else if ([method_ isEqualToString:@"PUT"])
		return [params_ parameterValueForKey:@"__Content-Type"];
	
	return @"";
}

- (void)appendKey:(NSString*)key value:(NSString*)value toHTTPPostData:(NSMutableData*)body
	first:(BOOL)first
{
//	[body appendData:[[NSString stringWithFormat:@"\r\n--%@\r\n", kW2ClientFormDataBoundary]
//		dataUsingEncoding:NSUTF8StringEncoding]];
//	[body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"\r\n\r\n",
//		key] dataUsingEncoding:NSUTF8StringEncoding]];
//	[body appendData:[value dataUsingEncoding:NSUTF8StringEncoding]];

	[body appendData:[[NSString stringWithFormat:@"%@%@=%@",
		first ? @"" : @"&", W2URLEscapedString(key), W2URLEscapedString([value description])]
			dataUsingEncoding:NSUTF8StringEncoding]];
}

- (NSData*)bodyDataForPOSTRequest
{
	NSMutableData* body = [NSMutableData dataWithCapacity:512];
	BOOL first = YES;
	
	for (W2ClientParameter* param in params_.array)
	{
		if ([param.value isKindOfClass:[NSArray class]])
		{
			for (id value in (NSArray*)param.value)
			{
				[self appendKey:param.key value:value
					toHTTPPostData:body first:first];
				first = NO;
			}
		}
		else
		{
			[self appendKey:param.key value:param.value
				toHTTPPostData:body first:first];
			first = NO;
		}
	}
	
//	[body appendData:[[NSString stringWithFormat:@"\r\n--%@--\r\n", kW2ClientFormDataBoundary]
//		dataUsingEncoding:NSUTF8StringEncoding]];
	
	return body;
}

- (void)startWithQueue:(W2ClientRequestQueue*)queue
{
	state_ = W2ClientRequestState_Working;
	queue_ = queue;
	
	W2CLog(@"(W2Client) Request STARTED: %@", url_);
	W2CLog(@"  method: %@", method_);
	if ([params_.array count])
	{
		W2CLog(@"  params:");
		for (W2ClientParameter* param in params_.array)
		{
			W2CLog(@"    \"%@\" = \"%@\"", param.key, param.value);
		}
	}
	
	NSData* body_data = nil;
	if ([method_ isEqualToString:@"POST"])
	{
		body_data = [self bodyDataForPOSTRequest];
	}
	else if ([method_ isEqualToString:@"PUT"])
	{
		body_data = [params_ parameterValueForKey:@"__data"];
	}
	
	if (signatureType_ != W2ClientSignatureType_None)
	{
		if (signatureKey_)
		{
			// calculate signature
			NSString* signature_string = [[url_ path]
				stringByAppendingFormat:@"?%@", [url_ query]];
			NSString* sign = W2URLEscapedString([signature_string HMACWithSecret:signatureKey_]);
			
			url_ = [NSURL URLWithString:[NSString stringWithFormat:@"%@%@sign=%@", [url_ absoluteString],
				[url_ query] ? @"&" : @"?", sign]];
		}
		else
		{
			// report error
			error_ = [[NSError alloc] initWithDomain:kW2ClientErrorDomain
				code:kW2ClientErrorCode_UnexpectedBehavior userInfo:nil];
			[self didFinish];
			return;
		}
	}
	
	NSMutableURLRequest* url_request = [NSMutableURLRequest requestWithURL:url_
		cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:kW2ConnectLongTimeout];
	[url_request setHTTPMethod:method_];
	[url_request setHTTPShouldHandleCookies:NO];
	
	if (body_data)
	{
		[url_request setHTTPBody:body_data];
		[url_request addValue:[self contentTypeForPOSTRequest]
			forHTTPHeaderField: @"Content-Type"];
	}
	
	conn_ = [[NSURLConnection alloc] initWithRequest:url_request delegate:self
		startImmediately:NO];
//	if (!doNotUseRunLoopCommonModes_)
//		[conn_ scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];

	[conn_ start];
}

//- (BOOL)connection:(NSURLConnection *)connection
//	canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace
//{
//	return YES;
//}
//
//- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
//{
//	[challenge.sender useCredential:[NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust] forAuthenticationChallenge:challenge];
//	[challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge];
//}

- (void)connection:(NSURLConnection*)connection didReceiveResponse:(NSURLResponse*)response
{
	long long length = [response expectedContentLength];
	if (length == NSURLResponseUnknownLength)
		data_ = [[NSMutableData alloc] init];
	else
		data_ = [[NSMutableData alloc] initWithCapacity:(int)[response expectedContentLength]];
	
	httpCode_ = [((NSHTTPURLResponse*)response) statusCode];
	isJson_ = [[[((NSHTTPURLResponse*)response) allHeaderFields] objectForKey:@"Content-Type"]
		isEqualToString:@"application/json"];
}

- (void)connection:(NSURLConnection*)connection didReceiveData:(NSData*)data
{
	[data_ appendData:data];
}

- (void)didFinish
{
	if (!error_ && ![data_ length])
	{
		error_ = [[NSError alloc] initWithDomain:kW2ClientErrorDomain
			code:kW2ClientErrorCode_UnexpectedBehavior
			userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
				@"Server error", NSLocalizedDescriptionKey,
				nil
			]];
	}

	W2CLog(@"(W2Client) Request FINISHED: %@", url_);
	W2CLog(@"  method: %@", method_);
	
	if (error_)
		W2CLog(@"  ERROR: %@", error_);
	else if (response_)
		W2CLog(@"  RESPONSE: %@", response_);
	else if (data_)
		W2CLog(@"  DATA: %@", [[NSString alloc] initWithData:data_ encoding:NSUTF8StringEncoding]);

	
	if ([delegate_ respondsToSelector:@selector(requestDidFinish:)])
	{
		[delegate_ requestDidFinish:self];
	}
	
	[queue_ requestDidFinish:self];
}

- (void)connection:(NSURLConnection*)connection didFailWithError:(NSError*)error
{
	if ([error.domain isEqualToString:NSURLErrorDomain] && error.code == NSURLErrorTimedOut)
	{
		// retry
//		if (!(flags_ & kW2ClientRequestFlag_LongPolling))
//		{
//			NSLog(@"Connection timed out, try again");
//			[self startWithQueue:queue_];
//		}
//		else
		{
			state_ = W2ClientRequestState_Completed;
			[self didFinish];
		}
	}
	else
	{
		state_ = W2ClientRequestState_Completed;
		error_ = error;
		[self didFinish];
	}
}

- (void)connectionDidFinishLoading:(NSURLConnection*)connection
{
	if (state_ == W2ClientRequestState_Cancelled)
		return;
	
	state_ = W2ClientRequestState_Completed;
	
	if (isJson_)
	{
		response_ = [data_ objectFromJSONData];
		if (![response_ isKindOfClass:[NSDictionary class]])
		{
			response_ = nil;
			isJson_ = NO;
			
			error_ = [[NSError alloc] initWithDomain:kW2ClientErrorDomain
				code:kW2ClientErrorCode_HTTPErrorCode
				userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
					@"Server error", NSLocalizedDescriptionKey,
					nil
				]];
		}
		else
		{
			NSDictionary* error_dict = W2GetDictionaryValue(response_, @"error");
			if (error_dict)
			{
				NSMutableDictionary* user_info = [NSMutableDictionary dictionary];
				
				NSString* error_code = W2GetStringValue(error_dict, @"code");
				NSString* error_message = NSLocalizedString(W2GetStringValue(error_dict, @"message"), @"");
				if ([error_message isEqualToString:W2GetStringValue(error_dict, @"message")])
					error_message = W2GetStringValue(error_dict, @"message_en");
				
				if (error_code)
					[user_info setObject:error_code forKey:kW2ClientErrorCodeKey];
					
				if ([error_code isEqualToString:@"out_of_sync"] && state_ != W2ClientRequestState_Cancelled)
				{
					[[NSNotificationCenter defaultCenter] postNotificationName:kW2ClientRequestOutOfSyncNotification
						object:self];
				}
				else if ([error_code isEqualToString:@"double_login"] && state_ != W2ClientRequestState_Cancelled)
				{
					[[NSNotificationCenter defaultCenter] postNotificationName:kW2ClientRequestDoubleLoginNotification
						object:self];
				}
				
				if (error_message)
					[user_info setObject:error_message forKey:NSLocalizedDescriptionKey];
			
				error_ = [[NSError alloc] initWithDomain:kW2ClientErrorDomain
					code:kW2ClientErrorCode_ErrorObject
					userInfo:user_info];
			}
		}
	}

	if (!(httpCode_ >= 200 && httpCode_ <= 206))
	{
		if (!error_)
		{
			error_ = [[NSError alloc] initWithDomain:kW2ClientErrorDomain
				code:kW2ClientErrorCode_HTTPErrorCode
				userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
					@"Couldn't connect to server", NSLocalizedDescriptionKey,
					nil
				]];
		}
	}
	
	[self didFinish];
}

- (void)setDelegate:(id)aDelegate
{
	delegate_ = aDelegate;
	if ([delegate_ isKindOfClass:[W2ClientRequestDidFinishInvocation class]])
		invoke_ = delegate_;
	else
		invoke_ = nil;
}

@end
