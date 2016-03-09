//
//  OnlineNotification.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/20/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "OnlineNotification.h"

using namespace monsters;
using namespace boost;

const User& OnlineNotification::user() const
{
	if (user_.isValid())
		return user_;
	if (game_ && game_->opponent().isValid())
		return game_->opponent();
	if (invite_ && invite_->opponent().isValid())
		return invite_->opponent();
	return user_;
}

pb::Message* OnlineNotification::serialized(OnlineNotification::Type type, const User& user)
{
	using namespace pb;
	
	Message* m = new Message();
	*m << varint::ui(1, type) << msg(2, user.serialized());
	return m;
}

OnlineNotification OnlineNotification::deserialized(pb::Message& msg)
{
	OnlineNotification n;
	n.type_ = (OnlineNotification::Type)msg.getFieldValueUInt32(1);
	n.user_ = User(msg.getFieldValueMessage(2));
	return n;
}
