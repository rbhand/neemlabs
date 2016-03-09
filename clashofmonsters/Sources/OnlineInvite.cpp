//
//  OnlineInvite.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "OnlineInvite.h"
#include "JSONVariant.h"

using namespace monsters;
using namespace w2f2;

OnlineInvite::OnlineInvite(const User& sender, const User& receiver, const std::string& identifier) :
	sender_(sender), receiver_(receiver), identifier_(identifier)
{
	
}

OnlineInvite::OnlineInvite(const w2f2::JSONVariant& json, const std::map<std::string, User>& users)
{
	identifier_ = json.objectForKey("id").asString();
	
	std::map<std::string, User>::const_iterator i;
	
	i = users.find(json.objectForKey("sender").asString());
	if (i != users.end())
		sender_ = i->second;
		
	i = users.find(json.objectForKey("receiver").asString());
	if (i != users.end())
		receiver_ = i->second;
}
