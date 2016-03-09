//
//  AvatarsCache.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 3/28/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "AvatarsCache.h"

#import "W2Client.h"
#import "W2ClientUtils.h"

using namespace monsters;
using namespace w2f2;
using namespace cocos2d;

namespace monsters
{
	class AvatarsCachePrivate
	{
	public:
		__unsafe_unretained W2Client* client_;
	};
	
	class AfterAvatarAddImage : public CCObject
	{
	public:
		AfterAvatarAddImage(const std::string& url, const std::string& path, const AvatarsCache::LoadCallback& callback) :
			url_(url), path_(path), callback_(callback)
		{
			
		}
		
		void onLoad(CCTexture2D* tex)
		{
			//CCTexture2D* tex = CCTextureCache::sharedTextureCache()->textureForKey(path_.c_str());
			if (tex)
			{
				CCRect r = CCRectMake(0, 0, tex->getContentSizeInPixels().width, tex->getContentSizeInPixels().height);
				callback_.call(url_, CCSpriteFrame::createWithTexture(tex, r, false, ccp(0, 0), r.size), Error());
			}
			else
			{
				callback_.call(url_, (CCSpriteFrame*)0, Error("Failed loading image"));
			}
			
			autorelease();
		}
		
	private:
		std::string url_;
		std::string path_;
		AvatarsCache::LoadCallback callback_;
	};
}

AvatarsCache::AvatarsCache() :
	d_(new AvatarsCachePrivate())
{
	
}

AvatarsCache::~AvatarsCache()
{
	delete d_;
}

void AvatarsCache::setClient(void* client)
{
	d_->client_ = (__bridge W2Client*)client;
}

static void loadTextureFromCacheFile(const std::string& url, NSString* cachePath, const AvatarsCache::LoadCallback& callback)
{
	AfterAvatarAddImage* after_add_image = new AfterAvatarAddImage(url, [cachePath UTF8String], callback);
	CCTextureCache::sharedTextureCache()->addImageAsync([cachePath UTF8String],
		after_add_image, callfuncO_selector(AfterAvatarAddImage::onLoad));
}

void AvatarsCache::load(const std::string& url, const LoadCallback& callback)
{
	if (CCTexture2D* tex = CCTextureCache::sharedTextureCache()->textureForKey(url.c_str()))
	{
		// texture is already loaded in memory
		CCRect r = CCRectMake(0, 0, tex->getContentSizeInPixels().width, tex->getContentSizeInPixels().height);
		callback.call(url, CCSpriteFrame::createWithTexture(tex, r, false, ccp(0, 0), r.size), Error());
		return;
	}
	
	NSString* cache_path = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject]
		stringByAppendingPathComponent:W2URLEscapedString([NSString stringWithUTF8String:url.c_str()])];
		
	if ([[NSFileManager defaultManager] fileExistsAtPath:cache_path])
	{
		// file was previosly downloaded - load to memory
		loadTextureFromCacheFile(url, cache_path, callback);
		return;
	}
	
	// request file from server
	W2ClientRequest* request = [[W2ClientRequest alloc] initWithMethod:@"GET"
		url:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]] params:nil
		flags:kW2ClientRequestFlag_None signatureType:W2ClientSignatureType_None signatureKey:nil];
	
	LoadCallback the_callback = callback;
	std::string the_url = url;
	request.delegate = [W2ClientRequestDidFinishInvocation invocationWithCompletion:^(W2ClientRequest *request) {
		if (!request.error)
		{
			dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
				// save data to disk
				[request.data writeToFile:cache_path atomically:YES];
				
				dispatch_async(dispatch_get_main_queue(), ^{
					loadTextureFromCacheFile(the_url, cache_path, the_callback);
				});
			});
		}
		else
		{
			callback.call(the_url, (CCSpriteFrame*)0, Error([[request.error localizedDescription] UTF8String]));
		}
	}];
	
	[d_->client_.mainQueue enqueue:request];
	
}
