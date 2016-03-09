//
//  FacebookSharing.h
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 5/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "Callback.h"
#include "Error.h"

namespace monsters
{
	class W2FacebookSharingPrivate;
	class FacebookSharing
	{
		friend class W2FacebookSharingPrivate;
	
	public:
		typedef void(cocos2d::CCObject::*ShareCallbackSelector)(bool shared, const Error& error);
		typedef w2f2::Callback<ShareCallbackSelector> ShareCallback;
		
		typedef void(cocos2d::CCObject::*InviteCallbackSelector)(bool invited, const Error& error);
		typedef w2f2::Callback<InviteCallbackSelector> InviteCallback;
		
	public:
		FacebookSharing();
		~FacebookSharing();
		
		void facebookShare(const std::string& text, const ShareCallback& callback);
		void facebookInvite(const InviteCallback& callback);

		ShareCallback& getShareCallback() { return shareCallback_; }

		static FacebookSharing& sharedSharing();
		
	private:
		W2FacebookSharingPrivate* d_;
		ShareCallback shareCallback_;
	};
	
}