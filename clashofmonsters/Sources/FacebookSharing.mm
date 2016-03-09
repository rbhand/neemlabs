//
//  FacebookSharing.cpp
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 5/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "FacebookSharing.h"
#include "W2FacebookSharing.h"

using namespace monsters;


namespace monsters
{
	class W2FacebookSharingPrivate
	{
	public:
		Error parseError(NSError* objcError) const
		{
			if (!objcError)
				return Error();
			else
				return Error([[objcError localizedDescription] UTF8String]);
		}
		
	public:
		W2FacebookSharing* facebookSharing_;
	};
}


static FacebookSharing* g_sharedSharing = 0x0;

FacebookSharing::FacebookSharing() :
	d_(new W2FacebookSharingPrivate)
{
	d_->facebookSharing_ = [[W2FacebookSharing alloc] init];
	
	#ifdef IS_PRO
		d_->facebookSharing_.link = [NSURL URLWithString:@"https://itunes.apple.com/us/app/pirates-versus-ninjas-2-player/id667904125?ls=1&mt=8"];
	#else
		d_->facebookSharing_.link = [NSURL URLWithString:@"https://itunes.apple.com/us/app/pirates-versus-ninjas-2-player/id667901021?ls=1&mt=8"];
	#endif
	
	d_->facebookSharing_.picture = [NSURL URLWithString:@"https://photos-2.dropbox.com/t/2/AAC2_wVsTAooetyV_SbTXBC_LUIWPjouNYGn4jNwXIE3FA/12/498971797/png/32x32/3/1450263600/0/2/Pirates%20versus%20Ninjas.png/ENzlr4YEGBEgAigC/PiE7xKD7x15uL-KA6fYqEKIwNNFwsThARuiEFcUmikU?size_mode=5&size=32x32"];
	d_->facebookSharing_.name = @"Pirates vs Ninjas";
	d_->facebookSharing_.description = @"Check this amazing game!";
}

FacebookSharing::~FacebookSharing()
{
	[d_->facebookSharing_ release];
	delete d_;
}

void FacebookSharing::facebookShare(const std::string& text, const ShareCallback& callback)
{
	ShareCallback share_callback = callback;
	
	cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("showStartupAds", false);
	
	d_->facebookSharing_.description = [NSString stringWithUTF8String:text.c_str()];
	[d_->facebookSharing_ shareWithCompletion:^(BOOL published, NSError *error) {
		
		share_callback.call(published ? true : false, d_->parseError(error));
		cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("showStartupAds", true);
	}];
}

void FacebookSharing::facebookInvite(const InviteCallback& callback)
{
	InviteCallback invite_callback = callback;
	
	[d_->facebookSharing_ inviteWithCompletion:^(BOOL published, NSError *error) {
		
		invite_callback.call(published ? true : false, d_->parseError(error));
		
	}];
}

FacebookSharing& FacebookSharing::sharedSharing()
{
	if (!g_sharedSharing)
	{
		g_sharedSharing = new FacebookSharing();
	}
	
	return *g_sharedSharing;
}


