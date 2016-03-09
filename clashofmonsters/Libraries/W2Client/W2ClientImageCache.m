//
//  W2ClientImageCache.m
//  Trivia
//
//  Created by Vitaliy Ivanov on 8/6/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientImageCache.h"

@interface W2ClientImage : NSObject
{
	NSURL* url_;
	UIImage* image_;
	NSDate* lastRequest_;
}

@property (strong, readwrite) NSURL* url;
@property (strong, readwrite) UIImage* image;
@property (strong, readwrite) NSDate* lastRequest;

@end

@implementation W2ClientImage
@synthesize url = url_, image = image_, lastRequest = lastRequest_;
@end


@interface W2ClientImageCache()

- (W2Client*)client;
- (void)imageRequestDidFinish:(W2ClientImageRequest*)request;

- (UIImage*)imageEntryForURL:(NSURL*)url;

@end


@interface W2ClientImageRequest() <W2ClientRequestDelegate>
@property (readwrite) UIImage* image;
@end


@implementation W2ClientImageRequest

@synthesize delegate = delegate_, image = image_, fromCache = fromCache_;

- (id)initWithURL:(NSURL*)url cache:(W2ClientImageCache*)cache
{
	if ((self = [super init]))
	{
		request_ = [[W2ClientRequest alloc] initWithMethod:@"GET" url:url params:nil
			flags:kW2ClientRequestFlag_None signatureType:W2ClientSignatureType_None signatureKey:nil];
		request_.delegate = self;
		cache_ = cache;
	}
	
	return self;
}

- (NSURL*)url
{
	return request_.url;
}

- (void)run
{
	image_ = [cache_ imageEntryForURL:request_.url];
	
	if (!image_)
		[[cache_ client].mainQueue enqueue:request_];
	else
	{
		fromCache_ = YES;
		dispatch_async(dispatch_get_main_queue(), ^{
			[delegate_ clientImageRequestDidLoad:self];
		});
	}
}

- (void)cancel
{
	[request_ cancel];
}

- (void)requestDidFinish:(W2ClientRequest*)request
{
	if (!request_.error)
	{
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
			self.image = [UIImage imageWithData:request_.data];
			
			dispatch_async(dispatch_get_main_queue(), ^{
				[cache_ imageRequestDidFinish:self];
				[delegate_ clientImageRequestDidLoad:self];
			});
		});
	}
	else
	{
		[cache_ imageRequestDidFinish:self];
		[delegate_ clientImageRequestDidLoad:self];
	}
}

- (NSError*)error
{
	return request_.error;
}

- (void)dealloc
{
	request_.delegate = nil;
	[request_ cancel];
}

@end


@implementation W2ClientImageCache

- (id)initWithClient:(W2Client*)client
{
	if ((self = [super init]))
	{
		client_ = client;
		images_ = [[NSMutableDictionary alloc] init];
		maxImages_ = 100;
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didReceiveMemoryWarning)
			name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
	}
	
	return self;
}

- (void)didReceiveMemoryWarning
{
	[images_ removeAllObjects];
}

- (W2Client*)client
{
	return client_;
}

- (UIImage*)imageEntryForURL:(NSURL*)url
{
	W2ClientImage* img = [images_ objectForKey:url];
	if (img)
	{
		last_ = url;
		img.lastRequest = [NSDate date];
	}
	
	return img.image;
}

- (void)imageRequestDidFinish:(W2ClientImageRequest*)request
{
	if (request.image)
	{
		if (last_ && [images_ count] == maxImages_)
		{
			[images_ removeObjectForKey:last_];
		}
	
		W2ClientImage* img = [[W2ClientImage alloc] init];
		img.image = request.image;
		img.lastRequest = [NSDate date];
		img.url = request.url;
		last_ = request.url;
		
		[images_ setObject:img forKey:request.url];
	}
}

- (BOOL)isURLStringInCache:(NSString*)urlString
{
	return images_[[NSURL URLWithString:urlString]] != nil;
}

- (W2ClientImageRequest*)getFromURLString:(NSString*)urlString
	delegate:(id<W2ClientImageRequestDelegate>)delegate
{
	W2ClientImageRequest* req = [[W2ClientImageRequest alloc]
			initWithURL:[NSURL URLWithString:urlString] cache:self];
	req.delegate = delegate;
	[req run];
	return req;
}

- (W2ClientImageRequest*)getFromURL:(NSURL*)url
	delegate:(id<W2ClientImageRequestDelegate>)delegate
{
	W2ClientImageRequest* req = [[W2ClientImageRequest alloc]
			initWithURL:url cache:self];
	req.delegate = delegate;
	[req run];
	return req;
}

- (void)deleteURL:(NSURL*)url
{
	[images_ removeObjectForKey:url];
}

@end
