//
//  PlayerAction.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "GameTypes.h"
#include "protobuf/Message.h"

namespace monsters
{
	enum PlayerActionType
	{
		PlayerActionType_None = 0,
		PlayerActionType_RemoveUnit = 1,
		PlayerActionType_MoveUnit = 2,
		PlayerActionType_CallTroops = 3,
		PlayerActionType_EndRound = 4,
		PlayerActionType_TimerExpired = 5
	};
	
	class PlayerAction
	{
	public:
		PlayerAction() : type_(PlayerActionType_None), to_(-1), playerIndex_(-1) { }
	
		static PlayerAction actionRemoveUnit(CellCoordinate atCoord)
			{ return PlayerAction(PlayerActionType_RemoveUnit, atCoord, -1); }
		static PlayerAction actionMoveUnit(CellCoordinate atCoord, int to)
			{ return PlayerAction(PlayerActionType_MoveUnit, atCoord, to); }
		static PlayerAction actionCallTroops()
			{ return PlayerAction(PlayerActionType_CallTroops); }
		static PlayerAction actionEndRound()
			{ return PlayerAction(PlayerActionType_EndRound); }
		static PlayerAction actionTimerExpired()
			{ return PlayerAction(PlayerActionType_TimerExpired); }
		
		PlayerActionType type() const { return type_; }
		CellCoordinate coord() const { return coord_; }
		int to() const { return to_; }
		
		void setPlayerIndex(int playerIndex) { playerIndex_ = playerIndex; }
		int playerIndex() const { return playerIndex_; }
		
		bool isNone() const { return type_ == PlayerActionType_None; }
		
		pb::Message* serialized() const;
		bool deserialize(const pb::Message& msg);
		
		static PlayerAction deserialized(const std::string& base64);
		std::string serializedBase64() const;
		
	private:
		PlayerAction(PlayerActionType type) :
			type_(type), to_(-1), playerIndex_(-1) { }
		PlayerAction(PlayerActionType type, CellCoordinate coord, int to) :
			type_(type), coord_(coord), to_(to), playerIndex_(-1) { }
		
	private:
		PlayerActionType type_;
		CellCoordinate coord_;
		int to_;
		int playerIndex_;
	};
}
