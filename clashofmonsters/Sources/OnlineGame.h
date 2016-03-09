//
//  OnlineGame.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "Callback.h"
#include "User.h"
#include "Buffer.h"
#include "Game.h"
#include "Error.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>
#include <map>

namespace w2f2 { class JSONVariant; }

namespace monsters
{
	class Session;
	class OnlineGame;
	class IOnlineGameDelegate
	{
	public:
		virtual void gameConfigurationUpdated(OnlineGame* game) { }
		virtual void gameStartPlayingFailed(OnlineGame* game, const Error& error) { }
		virtual void gameDidSendTurn(OnlineGame* game) { }
		virtual void gameSendTurnFailed(OnlineGame* game, const Error& error) { }
		virtual void gameStartedPlaying(OnlineGame* game) { }
		virtual void gameReceivedNewTurns(OnlineGame* game) { }
		virtual void gameOver(OnlineGame* game) { }
		virtual void gameResigned(OnlineGame* game, const User& byUser) { }
		virtual void gameResigningFailed(OnlineGame* game, const Error& error) { }
		virtual void gameExpired(OnlineGame* game) { }
	};
	
	class OnlineGame
	{
		friend class Session;
	
	public:
		enum State
		{
			State_None = 0,
			State_Configuration = 1,
			State_Playing = 2,
			State_Over = 3,
			State_Expired = 4,
			State_Resigned = 5
		};
	
	public:
		OnlineGame(const w2f2::JSONVariant& json, const User& user, const std::map<std::string, User>& opponents,
			const GameUnits& units);
		
		State state() const { return state_; }
		const std::string& identifier() const { return identifier_; }
		const User& opponent() const { return opponent_; }
		int userIndex() const { return localPlayerState_.playerIndex; }
		int opponentIndex() const { return (userIndex() + 1) % 2; }
		bool isActive() const { return state_ == State_Playing || state_ == State_Configuration; }
		
		const PlayerConfig& playerConfig(int player) const { return playerConfigs_[player]; }
		bool isUserPlayReady() const { return userPlayReady_; }
		void setPlayerConfigForUser(const PlayerConfig& config);
		void startPlaying(const PlayerConfig& config);
		void sendTurn(const PlayerAction& turn);
		bool isSendingTurn() const { return sendingTurn_ != 0; }
		void resign();
		
		int gameRound() const;
		
		IOnlineGameDelegate* delegate() const { return delegate_; }
		void setDelegate(IOnlineGameDelegate* delegate) { delegate_ = delegate; }
		
		boost::shared_ptr<pb::Buffer> data() const { return data_; }
		const LocalPlayerState& localPlayerState() const { return localPlayerState_; }
		void setLocalPlayerStateRoundTime(uint64_t roundTime) { localPlayerState_.roundTime = roundTime; }
		
		PlayerAction popNextUnprocessedTurn();
		void updateGameData(pb::Buffer* data) { processedTurns_.clear(); data_.reset(data); }
		
		void applyProcessedTurns();
		
		void setNotifyPlay(bool notifyPlay) { notifyPlay_ = notifyPlay; }
		bool isNotifyPlay() const { return notifyPlay_; }
		void setNotifyOpponentTurn(bool notifyOpponentTurn) { notifyOpponentTurn_ = notifyOpponentTurn; }
		bool isNotifyOpponentTurn() const { return notifyOpponentTurn_; }
		
	protected:
		void opponentConfigurationUpdated(const PlayerConfig& config);
		void opponentMadeTurn(const PlayerAction& turn);
		void opponentResigned();
		
		void onConfigureCompleted(const Error& error);
		void onPlayCompleted(const Error& error, const w2f2::JSONVariant& data);
		void onResignCompleted(const Error& error);
		void onSendTurnCompleted(w2f2::OperationHandle operation, const Error& error);
		
	private:
		void parseGameData(const w2f2::JSONVariant& json);
		static State parseState(const std::string& str);
		
	private:
		State state_;
		std::string identifier_;
		User opponent_;
		
		IOnlineGameDelegate* delegate_;
		PlayerConfig playerConfigs_[2];
		bool userPlayReady_;
		w2f2::OperationHandle sendingTurn_;
		
		bool notifyPlay_;
		bool notifyOpponentTurn_;
		
		boost::shared_ptr<pb::Buffer> data_;
		std::vector<PlayerAction> processedTurns_;
		std::vector<PlayerAction> turns_;
		LocalPlayerState localPlayerState_;
	};
}
