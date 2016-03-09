//
//  Game.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "RandomGenerator.h"
#include "GameConstants.h"
#include "GameTypes.h"
#include "GameUnits.h"
#include "Player.h"
#include "Unit.h"
#include "RoundEvent.h"
#include "PlayerAction.h"

#ifndef MN_SERVER
#include "AI.h"
#include "BackgroundJob.h"
//#include "MonotonicTimer.h"
#endif

#include "protobuf/Message.h"

#include <list>
#include <set>

namespace monsters
{
	class IGameDelegate
	{
	public:
		virtual void gameDidStart() = 0;
		virtual void gameDidLoad() = 0;
		virtual void roundDidStart(const std::list<AttackFormation*>& synced) = 0;
		virtual void roundEvents(const std::list<RoundEvent>& events) = 0;
		virtual void roundTimeout() = 0;
		virtual void unitWasRemoved(const Unit* unit, const std::list<ProcessFormationsHint>& combos) = 0;
		virtual void unitWasMoved(const Unit* unit, const CellCoordinate& newCoord,
			const std::list<ProcessFormationsHint>& combos) = 0;
		virtual void troopsWereCalled() = 0;
		virtual void gameOver() = 0;
		
		virtual void sendRemoteTurn(const PlayerAction& turn) = 0;
	};
	
	enum GameType
	{
		GameType_Local = 0,
		GameType_Server = 1,
		GameType_RemoteClient = 2
	};
	
	enum GameState
	{
		GameState_Inactive = 0,
		GameState_Configuring = 1,
		GameState_Playing = 2,
		GameState_Over = 3
	};
	
	class IRemoteGameConnection
	{
		
	};
	
	class IServerGameConnection
	{
		
	};
	
	#ifndef MN_SERVER
	struct LocalPlayerState
	{
		int playerIndex;
		uint64_t roundTime;
	};
	#endif
	
	class AIJob;
	class Game
		#ifndef MN_SERVER
			: public f17y::utils::IBackgroundJobDelegate
		#endif
	{
	public:
		Game();
		
		void startTwoPlayersGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config);
		void startSinglePlayerGame(const PlayerConfig& player1Config, const PlayerConfig& aiConfig);
		void restartGame();
		
		bool startGameServer(const PlayerConfig& player1Config, const PlayerConfig& player2Config);
		bool loadServerGame(const pb::Message& msg, const GameUnits& units);
		
		#ifndef MN_SERVER
		bool loadRemoteGame(const pb::Message& msg, const LocalPlayerState& localPlayerState, const GameUnits& units);
		#endif
		
		#ifndef MN_SERVER
		void update(uint64_t dt);
		#endif
		
		void removeUnit(const Unit* unit);
		void moveUnit(const Unit* unit, int col);
		void callTroops();
		void endRound();
		bool applyAction(const PlayerAction& action);
		
		/// Processes round events in batches and reports them to delegate.
		/// Returns number of new events. No need to call on server.
		int nextRoundEvents();
		
		/// Called by delegate when it becomes ready to process next turn.
		/// Only needed in single player game, no need to call on server.
		void nextAITurn();
		
		bool canMoveUnit(const Unit* unit) const;
		
		#ifndef MN_SERVER
		int timeLeftSec() const;
		uint64_t roundTime() const { return roundTime_; }
		#endif
		
		const Player& player(int index) const { return players_[index]; }
		int activePlayerIndex() const { return activePlayer_; }
		int inactivePlayerIndex() const { return (activePlayer_ + 1) % 2; }
		const Player& activePlayer() const { return players_[activePlayer_]; }
		const Player& inactivePlayer() const { return players_[inactivePlayerIndex()]; }
		
		IGameDelegate* delegate() const { return delegate_; }
		void setDelegate(IGameDelegate* delegate) { delegate_ = delegate; }
		
		int winnerIndex() const { return winner_; }
		int stageBackgroundIndex() const { return stageBackgroundIndex_; }
		
		pb::Message* serialized() const;
		
		GameState state() const { return state_; }
		GameType type() const { return type_; }
		int round() const { return round_; }
		int playerPoints(int index) const { return ((winner_ == index) ? 20000 : 0) + round_ * ( 555 + round_ ); /*+ (int)(gameDuration_ / 60.0)*/; };
		
		// Methods for debugging
		bool loadServerGame(const std::string& base64, const GameUnits& units);
		std::string serializedBase64() const;
		bool isEqualToGame(const Game& game) const;
		
	private:
		void clear();
		void startRound();
		void checkEndRound();
		
		int attackStrength(AttackFormation* attacker, int strength, RoundEventType attackType);
		std::list<RoundEvent> attackUnits(const std::list<Unit*>& units, RoundEventType attackType, AttackFormation* attacker,
			bool attackPlayer);
		
		void startNewGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config,
			PlayerType player1Type, PlayerType player2Type);
		bool deserialize(const pb::Message& msg, int localPlayerIndex, const GameUnits& units);
		
		bool canMoveUnit(const Unit* unit, int col) const;
		
		void doEndRound();
		void sendTurnIfNeeded(const PlayerAction& action);
		
		#ifndef MN_SERVER
		void onBackgroundJobCompleted(f17y::utils::BackgroundJob* job);
		#endif
		
	private:
		GameType type_;
		GameState state_;
		Player players_[2];
		int activePlayer_;
		int winner_;
		int stageBackgroundIndex_;
		
		bool turnsWereMade_;
		int round_;
		
		RandomGenerator randomGenerator_;
		
		uint64_t roundTime_;
//		uint64_t gameDuration_;
		
		#ifndef MN_SERVER
//		f17y::utils::MonotonicTimer roundTimer_;
//		f17y::utils::MonotonicTimer gameTimer_;
		#endif
		
		IGameDelegate* delegate_;
		
		// temporary variables, only used while processing events, i.e. shouldn't be serialized
		std::set<AttackFormation*> syncedAttacks_;
		bool troopsAligned_;
		std::set<Unit*> unitsToDelete_;
		std::set<AttackFormation*> formationsToDelete_;
		std::list<AttackFormation*> formationsToProcessSpecialAbilities_;
		
		// AI variables, shouldn't be serialized
		#ifndef MN_SERVER
		bool aiThinking_;
		uint64_t aiWillEndThinkingAt_;
		AI ai_;
		AIJob* aiJob_;
		#endif
	};
}
