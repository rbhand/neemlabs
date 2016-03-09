//
//  AI.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "PlayerAction.h"
#include "Player.h"
#include <set>

namespace monsters
{
	class AIContext
	{
	public:
		AIContext(const AIContext& ctx);
		AIContext(const Field& field);
		~AIContext();
		
		void removeUnit(const CellCoordinate& coord);
		void moveUnit(const CellCoordinate& coord, int col);
		int lastFreeRow(int col, const Unit* except=0) const;
		int lastOccupiedRow(int col) const;
		int rowToMoveUnit(int col, const Unit* unit) const;
		
		int score() const;
		int depth() const { return depth_; }
		
		const Field& field() const { return field_; }
		Field& field() { return field_; }
		
		PlayerAction firstAction() const { return actions_.front(); }
		const std::list<PlayerAction>& actions() const { return actions_; }
	
		bool unitIsWall(const Unit* unit) const;
		bool unitIsInAttackFormation(const Unit* unit) const;
		bool unitIsWallOrAttack(const Unit* unit) const;
		const AttackFormation* unitAttackFormation(const Unit* unit) const;
		
		void pushAction(const PlayerAction& action) { actions_.push_back(action); }
		
		
	private:
		int processFormations();
		bool isCellEmpty(const CellCoordinate& c, const Unit* except=0) const;
		int rowToPlace(int col, UnitClass uc, const Unit* except=0) const;
		int rowToPlaceAnywhere(int col, UnitClass uc, const Unit* except=0) const;
		void arrangeTroops();
	
	private:
		Field field_;
		std::list<AttackFormation*> newAttackFormations_;
		std::set<const Unit*> walls_;
		std::list<PlayerAction> actions_;
		int score_;
		int depth_;
	};
	
	class AI
	{
	public:
		AI();
		
		PlayerAction nextTurn(const Player& player);
		
	private:
		void traverseTurns(const AIContext& context);
		
	private:
		PlayerAction bestAction_;
		int bestScore_;
	};
}
