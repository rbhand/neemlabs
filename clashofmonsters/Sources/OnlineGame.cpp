//
//  OnlineGame.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "OnlineGame.h"
#include "Session.h"
#include "JSONVariant.h"
#include <sstream>

using namespace monsters;
using namespace w2f2;
using namespace std;
using namespace pb;

OnlineGame::State OnlineGame::parseState(const std::string& str)
{
	if (str == "configuration")
		return State_Configuration;
	else if (str == "playing")
		return State_Playing;
	else if (str == "over")
		return State_Over;
	else if (str == "expired")
		return State_Expired;
	else if (str == "resigned")
		return State_Resigned;
		
	return State_None;
}

OnlineGame::OnlineGame(const w2f2::JSONVariant& json, const User& user, const std::map<std::string, User>& opponents,
	const GameUnits& units) : delegate_(0), sendingTurn_(0), notifyPlay_(false), notifyOpponentTurn_(false)
{
	identifier_ = json.objectForKey("id").asString();
	state_ = parseState(json.objectForKey("state").asString());
	
	localPlayerState_.playerIndex = -1;
	localPlayerState_.roundTime = 0;
	
	string player_0_id = json.objectForKey("player_0_id").asString();
	if (player_0_id == user.identifier())
	{
		localPlayerState_.playerIndex = 0;
	}
	else
	{
		std::map<std::string, User>::const_iterator i = opponents.find(player_0_id);
		if (i != opponents.end())
			opponent_ = i->second;
	}
	
	string player_1_id = json.objectForKey("player_1_id").asString();
	if (player_1_id == user.identifier())
	{
		localPlayerState_.playerIndex = 1;
	}
	else
	{
		std::map<std::string, User>::const_iterator i = opponents.find(player_1_id);
		if (i != opponents.end())
			opponent_ = i->second;
	}
	
	for (int i=0; i<2; ++i)
	{
		stringstream ss;
		ss << "player_" << i << "_config";
		
		string player_config = json.objectForKey(ss.str()).asString();
		if (player_config.size())
		{
			Buffer b;
			b.writeBase64(player_config.c_str(), player_config.size());
			b.rewind();
			
			Message msg;
			if (msg.deserialize(b))
				playerConfigs_[i] = PlayerConfig(msg, units);
		}
	}
	
	if (state_ == State_Configuration)
		userPlayReady_ = json.objectForKey("user_play_ready").asBoolean();
	else
		userPlayReady_ = true;


	w2f2::JSONVariant data = json.objectForKey("data");
	parseGameData(data);
}

void OnlineGame::parseGameData(const w2f2::JSONVariant& data)
{
	string full_state = data.objectForKey("full_state").asString();
	if (full_state.size())
	{
		data_.reset(new Buffer());
		data_->writeBase64(full_state.c_str(), full_state.size());
		data_->rewind();
	}
	
	w2f2::JSONVariant turns = data.objectForKey("turns");
	int finalized_turn_id = data.objectForKey("finalized_turn_id").asInteger();
	for (int i=0; i<turns.objectsCount(); ++i)
	{
		int turn_id = turns.objectAtIndex(i).objectForKey("id").asInteger();
		string turn_data = turns.objectAtIndex(i).objectForKey("data").asString();
		if (turn_data.size())
		{
			pb::Buffer turn_buffer;
			turn_buffer.writeBase64(turn_data.c_str(), turn_data.size());
			turn_buffer.rewind();
			
			Message msg;
			if (msg.deserialize(turn_buffer))
			{
				PlayerAction turn;
				if (turn.deserialize(msg))
				{
					if (turn_id <= finalized_turn_id)
						processedTurns_.push_back(turn);
					else
						turns_.push_back(turn);
				}
			}
		}
	}
}

void OnlineGame::applyProcessedTurns()
{
	if (processedTurns_.size())
	{
		Game game;
		Message msg;
		if (msg.deserialize(*data_) &&
			game.loadServerGame(msg, Session::sharedSession().units()))
		{
			for (vector<PlayerAction>::const_iterator i=processedTurns_.begin(), e=processedTurns_.end();
				i!=e; ++i)
			{
				game.applyAction(*i);
			}
			
			auto_ptr<Message> s_msg(game.serialized());
			data_.reset(s_msg->serialized());
			data_->rewind();
		}
	}
	processedTurns_.clear();
}

void OnlineGame::opponentConfigurationUpdated(const PlayerConfig& config)
{
	playerConfigs_[opponentIndex()] = config;
	if (delegate_)
		delegate_->gameConfigurationUpdated(this);
}

void OnlineGame::onConfigureCompleted(const Error& error)
{
	
}

int OnlineGame::gameRound() const
{
	if (data_)
	{
		Game game;
		Message msg;
		data_->rewind();
		if (msg.deserialize(*data_) &&
			game.loadServerGame(msg, Session::sharedSession().units()))
		{
			data_->rewind();
			return game.round();
		}
		
		data_->rewind();
	}
	
	return -1;
}

void OnlineGame::setPlayerConfigForUser(const PlayerConfig& config)
{
	playerConfigs_[userIndex()] = config;
	Session::sharedSession().configureOnlineGame(this, config);
}

void OnlineGame::onPlayCompleted(const Error& error, const w2f2::JSONVariant& data)
{
	if (error.isOk())
	{
		userPlayReady_ = true;
		
		if (!data.isNull())
		{
			parseGameData(data);
			state_ = State_Playing;
		}
		
		if (delegate_)
			delegate_->gameStartedPlaying(this);
	}
	else if (delegate_)
	{
		delegate_->gameStartPlayingFailed(this, error);
	}
}

void OnlineGame::resign()
{
	Session::sharedSession().resignOnlineGame(this);
}

void OnlineGame::onResignCompleted(const Error& error)
{
	if (error.isOk())
	{
		state_ = State_Resigned;
		if (delegate_)
			delegate_->gameResigned(this, Session::sharedSession().user());
	}
	else
	{
		if (delegate_)
			delegate_->gameResigningFailed(this, error);
	}
}

void OnlineGame::opponentResigned()
{
	state_ = State_Resigned;
	if (delegate_)
		delegate_->gameResigned(this, opponent_);
}

void OnlineGame::startPlaying(const PlayerConfig& config)
{
	if (state_ == State_Configuration)
	{
		playerConfigs_[userIndex()] = config;
		Session::sharedSession().playOnlineGame(this, config);
	}
}

void OnlineGame::sendTurn(const PlayerAction& turn)
{
	assert(state_ == State_Playing);
	assert(!sendingTurn_);
	
	sendingTurn_ = Session::sharedSession().sendTurnOnlineGame(this, turn);
}

PlayerAction OnlineGame::popNextUnprocessedTurn()
{
	if (turns_.size())
	{
		PlayerAction a = turns_.front();
		turns_.erase(turns_.begin());
		return a;
	}
	
	return PlayerAction();
}

void OnlineGame::onSendTurnCompleted(w2f2::OperationHandle operation, const Error& error)
{
	if (operation == sendingTurn_)
	{
		sendingTurn_ = 0;
		if (error.isOk())
		{
			if (delegate_)
				delegate_->gameDidSendTurn(this);
		}
		else
		{
			if (delegate_)
				delegate_->gameSendTurnFailed(this, error);
		}
	}
}

void OnlineGame::opponentMadeTurn(const PlayerAction& turn)
{
	turns_.push_back(turn);
	if (delegate_)
		delegate_->gameReceivedNewTurns(this);
}
