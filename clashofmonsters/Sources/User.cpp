//
//  User.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "User.h"
#include "Message.h"
//#include "JSONVariant.h"
#include "Session.h"

using namespace monsters;
using namespace pb;
//using namespace w2f2;

//User::User(const w2f2::JSONVariant& json)
//{
//	identifier_ = json.objectForKey("id").asString();
//	name_ = json.objectForKey("name").asString();
//	facebookId_ = json.objectForKey("fb_id").asString();
//	avatarUrl_ = json.objectForKey("avatar_url").asString();
//}

pb::Message* User::serialized() const
{
	Message* m = new Message();
	*m
		<< bytes(1, identifier_)
		<< bytes(2, name_)
		<< bytes(3, facebookId_)
		<< bytes(4, avatarUrl_);
	
	return m;
}

User::User(pb::Message* m)
{
	if (m)
	{
		identifier_ = m->getFieldValueString(1);
		name_ = m->getFieldValueString(2);
		facebookId_ = m->getFieldValueString(3);
		avatarUrl_ = m->getFieldValueString(4);
	}
}

bool User::isLocal() const
{
	return true;
//	return Session::sharedSession().user().identifier() == identifier_;
}
