//
//  W2ClientImageCache.h
//  Trivia
//
//  Created by Vitaliy Ivanov on 8/6/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2Client.h"

@class W2ClientImageRequest;
@protocol W2ClientImageRequestDelegate

- (void)clientImageRequestDidLoad:(W2ClientImageRequest*)request;

@end


@class W2ClientImageCache;
@interface W2ClientImageRequest : NSObject
{
	UIImage* image_;
	W2ClientRequest* request_;
	W2ClientImageCache* cache_;
	BOOL fromCache_;
	__unsafe_unretained id<W2ClientImageRequestDelegate> delegate_;
}

- (void)cancel;

@property (readwrite, assign, nonatomic) id<W2ClientImageRequestDelegate> delegate;
@property (readonly, nonatomic) NSError* error;
@property (readonly, nonatomic) NSURL* url;
@property (readonly) UIImage* image;
@property (readonly) BOOL fromCache;

@end


@interface W2ClientImageCache : NSObject
{
	W2Client* client_;
	NSMutableDictionary* images_;
	int maxImages_;
	NSURL* last_;
}

- (id)initWithClient:(W2Client*)client;

- (BOOL)isURLStringInCache:(NSString*)urlString;

- (void)deleteURL:(NSURL*)url;
- (W2ClientImageRequest*)getFromURLString:(NSString*)urlString
	delegate:(id<W2ClientImageRequestDelegate>)delegate;
- (W2ClientImageRequest*)getFromURL:(NSURL*)url
	delegate:(id<W2ClientImageRequestDelegate>)delegate;

@end
