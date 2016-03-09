//
//  Update.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "Update.h"
#include "JSONVariant.h"

using namespace w2f2;
using namespace monsters;
using namespace std;

Update::Type Update::parseType(const std::string& str)
{
	if (str == "game_started")
		return Type_GameStarted;
	else if (str == "game_configuration")
		return Type_GameConfiguration;
	else if (str == "game_playing")
		return Type_GamePlaying;
	else if (str == "game_turn")
		return Type_GameTurn;
	else if (str == "game_over")
		return Type_GameOver;
	else if (str == "game_resigned")
		return Type_GameResigned;
	else if (str == "chat_message")
		return Type_ChatMessage;
	else if (str == "friend_request")
		return Type_FriendRequest;
	else if (str == "friend_added")
		return Type_FriendAdded;
	else if (str == "invite_received")
		return Type_InviteReceived;
	else if (str == "invite_declined")
		return Type_InviteDeclined;
	else if (str == "invite_withdrawn")
		return Type_InviteWithdrawn;
	else if (str == "game_will_expire")
		return Type_GameWillExpire;
	else if (str == "game_expired")
		return Type_GameExpired;
	
	return Type_None;
}

Update::Update(const w2f2::JSONVariant& json, const User& user, const std::map<std::string, User>& opponents,
	const GameUnits& units)
{
	type_ = parseType(json.objectForKey("type").asString());
	gameId_ = json.objectForKey("game_id").asString();
	inviteId_ = json.objectForKey("invite_id").asString();
	
	std::map<std::string, User>::const_iterator i = opponents.find(json.objectForKey("user_id").asString());
	if (i != opponents.end())
		user_ = i->second;
	
	string data = json.objectForKey("data").asString();
	if (!data.size())
		data = json.objectForKey("turn").asString();
	if (!data.size())
		data = json.objectForKey("config").asString();
	
	if (data.size())
	{
		data_.writeBase64(data.c_str(), data.size());
		data_.rewind();
	}
	
	playData_ = json.objectForKey("play_data");
		
	date_ = dateFromISO8601(json.objectForKey("date").asString());
	
	JSONVariant game = json.objectForKey("game");
	if (!game.isNull())
	{
		game_.reset(new OnlineGame(game, user, opponents, units));
	}
}
