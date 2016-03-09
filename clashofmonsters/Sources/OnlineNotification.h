//
//  OnlineNotification.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/17/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "OnlineGame.h"
#include "OnlineInvite.h"
#include "User.h"
#include "Message.h"

#include <list>
#include <boost/shared_ptr.hpp>

namespace monsters
{
	class OnlineNotification
	{
	public:
		enum Type
		{
			Type_None = 0,
			Type_GameStarted = 1,
			Type_GameResigned = 2,
			Type_GameExpired = 3,
			Type_InviteReceived = 4,
			Type_InviteDeclined = 5,
			Type_FriendAdded = 6,
			Type_GamePlaying = 7,
			Type_GameOpponentTurn = 8
		};
		
	public:
		OnlineNotification() : type_(Type_None) { }
		OnlineNotification(Type type, const boost::shared_ptr<OnlineGame>& game) :
			type_(type), game_(game) { }
		OnlineNotification(Type type, const boost::shared_ptr<OnlineInvite>& invite) :
			type_(type), invite_(invite) { }
		OnlineNotification(Type type, const User& user) :
			type_(type), user_(user) { }
		
		static pb::Message* serialized(OnlineNotification::Type type, const User& user);
		static OnlineNotification deserialized(pb::Message& msg);
		
		Type type() const { return type_; }
		boost::shared_ptr<OnlineGame> game() const { return game_; }
		boost::shared_ptr<OnlineInvite> invite() const { return invite_; }
		const User& user() const;
		
	private:
		Type type_;
		boost::shared_ptr<OnlineGame> game_;
		boost::shared_ptr<OnlineInvite> invite_;
		User user_;
	};
}
