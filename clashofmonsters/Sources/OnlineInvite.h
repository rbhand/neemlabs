//
//  OnlineInvite.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "User.h"
#include <string>
#include <map>

namespace w2f2 { class JSONVariant; }

namespace monsters
{
	class OnlineInvite
	{
	public:
		OnlineInvite(const User& sender, const User& receiver, const std::string& identifier);
		OnlineInvite(const w2f2::JSONVariant& json, const std::map<std::string, User>& users);
	
		const std::string& identifier() const { return identifier_; }
		const User& sender() const { return sender_; }
		const User& receiver() const { return receiver_; }
		const User& opponent() const { return sender_.isLocal() ? receiver_ : sender_; }
		
		bool isRandom() const { return !receiver_.isValid(); }
	
	private:
		std::string identifier_;
		User sender_;
		User receiver_;
	};
}
