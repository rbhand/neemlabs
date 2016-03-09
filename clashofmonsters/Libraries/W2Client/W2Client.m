//
//  TRClient.m
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2Client.h"

@implementation W2Client

@synthesize signatureParameters = signatureParameters_, port = port_, securePort = securePort_,
	mainQueue = mainQueue_, overrideSecure = overrideSecure_;

- (id)initWithHost:(NSString*)host pathPrefix:(NSString*)pathPrefix
{
	if ((self = [super init]))
	{
		host_ = host;
		pathPrefix_ = pathPrefix;
		mainQueue_ = [[W2ClientRequestQueue alloc] init];
		signatureType_ = W2ClientSignatureType_None;
		signatureParameters_ = [[W2ClientOrderedParameters alloc] init];
		port_ = 80;
		securePort_ = 443;
	}
	
	return self;
}

- (void)setSignatureKey:(NSString*)signatureKey type:(W2ClientSignatureType)signatureType
{
	signatureType_ = signatureType;
	signatureKey_ = signatureKey;
}

- (W2ClientRequest*)newRequestWithMethod:(NSString*)method path:(NSString*)path
	params:(W2ClientOrderedParameters*)params secure:(BOOL)secure longPolling:(BOOL)longPolling
	sign:(BOOL)sign
{
	NSMutableString* url_string = [NSMutableString stringWithFormat:@"%@://%@:%d%@/%@",
		secure ? @"https" : @"http", host_,
		secure ? securePort_ : port_,
		pathPrefix_ ? ([pathPrefix_ hasPrefix:@"/"] ? pathPrefix_ :
			[@"/" stringByAppendingString:pathPrefix_]) : @"",
		[path hasPrefix:@"/"] ? [path substringFromIndex:1] : path];
		
	BOOL first_param = YES;
	if (sign)
	{
		for (W2ClientParameter* param in signatureParameters_.array)
		{
			for (id value in [param allValues])
			{
				[url_string appendFormat:@"%@%@=%@",
					first_param ? @"?" : @"&",
					W2URLEscapedString(param.key),
					W2URLEscapedString([value description])
				];
				
				first_param = NO;
			}
		}
	}
	
	if ([method isEqualToString:@"GET"])
	{
		for (W2ClientParameter* param in params.array)
		{
			for (id value in [param allValues])
			{
				if ([value isKindOfClass:[NSString class]] ||
					[value isKindOfClass:[NSNumber class]])
				{
					[url_string appendFormat:@"%@%@=%@",
						first_param ? @"?" : @"&",
						W2URLEscapedString(param.key),
						W2URLEscapedString([value description])
					];
					first_param = NO;
				}
				else if ([value isKindOfClass:[NSArray class]])
				{
					[url_string appendFormat:@"%@%@=",
						first_param ? @"?" : @"&",
						W2URLEscapedString(param.key)];
					BOOL first_value = YES;
					for (NSString* value_a in (NSArray*)value)
					{
						if ([value isKindOfClass:[NSString class]] ||
							[value isKindOfClass:[NSNumber class]])
						{
							[url_string appendFormat:@"%@%@=",
								first_value ? @"" : @",",
								W2URLEscapedString([value_a description])];
							first_value = NO;
						}
						else
						{
							[NSException raise:NSInvalidArgumentException
								format:@"%@ is not supported here (only strings and numbers)",
									NSStringFromClass([value class])];
						}
					}
					first_param = NO;
				}
				else
				{
					[NSException raise:NSInvalidArgumentException
						format:@"sending %@ as request parameter is not supported",
							NSStringFromClass([value class])];
				}
			}
		}
	}
	
	if (signatureType_ != W2ClientSignatureType_None && sign)
	{
		NSString* ts_string = [NSString stringWithFormat:@"%x", (unsigned int)time(0)];
		[url_string appendFormat:@"%@%@=%@",
			first_param ? @"?" : @"&",
			W2URLEscapedString(@"ts"),
			W2URLEscapedString(ts_string)
		];
		first_param = NO;
	}
	
	W2ClientRequest* req = [[W2ClientRequest alloc] initWithMethod:method url:[NSURL URLWithString:url_string]
		params:([method isEqualToString:@"POST"] || [method isEqualToString:@"PUT"])? params : nil
		flags:longPolling ? kW2ClientRequestFlag_LongPolling : kW2ClientRequestFlag_None
		signatureType:sign ? signatureType_ : W2ClientSignatureType_None
		signatureKey:signatureKey_];
	
	return req;
}

- (W2ClientRequest*)get:(NSString*)path delegate:(id)delegate
{
	return [self get:path params:nil delegate:delegate];
}

- (W2ClientRequest*)get:(NSString*)path target:(id)target selector:(SEL)selector
{
	return [self get:path params:nil delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)get:(NSString*)path completion:(W2ClientRequestCompletionBlock)completion
{
	return [self get:path params:nil delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)get:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"GET" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:NO longPolling:NO sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)get:(NSString*)path params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion
{
	return [self get:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)get:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector;
{
	return [self get:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)getSecure:(NSString*)path delegate:(id)delegate
{
	return [self getSecure:path params:nil delegate:delegate];
}

- (W2ClientRequest*)getSecure:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"GET" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:overrideSecure_ ? NO : YES longPolling:NO sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)getLong:(NSString*)path delegate:(id)delegate
{
	return [self getLong:path params:nil delegate:delegate];
}

- (W2ClientRequest*)getLong:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"GET" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:NO longPolling:YES sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)getLong:(NSString*)path target:(id)target selector:(SEL)selector
{
	return [self getLong:path delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)post:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"POST" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:NO longPolling:NO sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)postLong:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"POST" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:NO longPolling:YES sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}


- (W2ClientRequest*)postSecure:(NSString*)path params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"POST" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:overrideSecure_ ? NO : YES longPolling:NO sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)post:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector
{
	return [self post:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)post:(NSString*)path params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion
{
	return [self post:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)postSecure:(NSString*)path params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion
{
	return [self postSecure:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)postLong:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector
{
	return [self postLong:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)postSecure:(NSString*)path params:(NSDictionary*)params target:(id)target selector:(SEL)selector
{
	return [self postSecure:path params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)put:(NSString*)path data:(NSData*)data delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"PUT" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:@{ @"__data": data }]
		secure:NO longPolling:NO sign:YES];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)put:(NSString*)path data:(NSData*)data completion:(W2ClientRequestCompletionBlock)completion
{
	return [self put:path data:data delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)put:(NSString*)path data:(NSData*)data contentType:(NSString*)contentType
	completion:(W2ClientRequestCompletionBlock)completion
{
	W2ClientRequest* req = [self newRequestWithMethod:@"PUT" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:@{
			@"__data": data,
			@"__Content-Type": contentType
		}]
		secure:NO longPolling:NO sign:YES];
	req.delegate = [W2ClientRequestDidFinishInvocation invocationWithCompletion:completion];
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)get:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"GET" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:NO longPolling:NO sign:sign];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)get:(NSString*)path sign:(BOOL)sign
	completion:(W2ClientRequestCompletionBlock)completion
{
	return [self get:path sign:sign params:nil delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)get:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params completion:(W2ClientRequestCompletionBlock)completion
{
	return [self get:path sign:sign params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)post:(NSString*)path sign:(BOOL)sign params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"POST" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:NO longPolling:NO sign:sign];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)post:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params target:(id)target selector:(SEL)selector
{
	return [self post:path sign:sign params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)post:(NSString*)path sign:(BOOL)sign params:(NSDictionary*)params
	completion:(W2ClientRequestCompletionBlock)completion
{
	return [self post:path sign:sign params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

- (W2ClientRequest*)postSecure:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params delegate:(id)delegate
{
	W2ClientRequest* req = [self newRequestWithMethod:@"POST" path:path
		params:[[W2ClientOrderedParameters alloc] initWithDictionary:params]
		secure:overrideSecure_ ? NO : YES longPolling:NO sign:sign];
	req.delegate = delegate;
	[mainQueue_ enqueue:req];
	return req;
}

- (W2ClientRequest*)postSecure:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params target:(id)target selector:(SEL)selector
{
	return [self postSecure:path sign:sign params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithTarget:target selector:selector]];
}

- (W2ClientRequest*)postSecure:(NSString*)path sign:(BOOL)sign
	params:(NSDictionary*)params completion:(W2ClientRequestCompletionBlock)completion
{
	return [self postSecure:path sign:sign params:params delegate:[W2ClientRequestDidFinishInvocation
		invocationWithCompletion:completion]];
}

@end
