//
//  Update.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "User.h"
#include "Buffer.h"
#include "Date.h"
#include "OnlineGame.h"
#include "JSONVariant.h"

#include "boost_essentials.h"

#include <string>
#include <map>

namespace w2f2 { class JSONVariant; }

namespace monsters
{
	class Update
	{
	public:
		enum Type
		{
			Type_None = 0,
		
			Type_GameStarted = 1,
			Type_GameConfiguration = 2,
			Type_GamePlaying = 3,
			Type_GameTurn = 4,
			Type_GameOver = 5,
			Type_GameResigned = 6,
			Type_ChatMessage = 7,
			Type_FriendRequest = 8,
			Type_FriendAdded = 9,
			Type_InviteReceived = 10,
			Type_InviteDeclined = 11,
			Type_InviteWithdrawn = 12,
			Type_GameWillExpire = 13,
			Type_GameExpired = 14
		};
	
	public:
		Update(const w2f2::JSONVariant& json, const User& user, const std::map<std::string, User>& opponents,
			const GameUnits& units);
		
		const std::string& gameId() const { return gameId_; }
		const std::string& inviteId() const { return inviteId_; }
		const User& user() const { return user_; }
		const pb::Buffer& data() const { return data_; }
		Type type() const { return type_; }
		w2f2::Date date() const { return date_; }
		const w2f2::JSONVariant& playData() const { return playData_; }
		boost::shared_ptr<OnlineGame> game() const { return game_; }
		
	private:
		static Type parseType(const std::string& str);
	
	private:
		std::string gameId_;
		std::string inviteId_;
		pb::Buffer data_;
		Type type_;
		User user_;
		w2f2::Date date_;
		boost::shared_ptr<OnlineGame> game_;
		w2f2::JSONVariant playData_;
	};
}
