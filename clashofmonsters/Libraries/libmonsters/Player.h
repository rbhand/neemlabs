//
//  Player.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "GameConstants.h"
#include "GameTypes.h"
#include "GameUnits.h"
#include "RandomGenerator.h"
#include "Unit.h"
#include "AttackFormation.h"
#include "Field.h"

#include "protobuf/Message.h"

#include <list>

namespace monsters
{
	enum PlayerType
	{
		PlayerType_Local = 0,
		PlayerType_Remote = 1,
		PlayerType_AI = 2
	};
	
	struct PlayerConfig
	{
		PlayerConfig();
		PlayerConfig(const pb::Message& message, const GameUnits& units);
	
		bool hasUnitClass(UnitClass uc) const;
		bool isValid() const { return avatar_ != 0; }
		bool isFull() const;
		
		Avatar* avatar() const { return avatar_; }
		UnitType* normalUnitAtIndex(int index) { return normalUnits_[index]; }
		UnitType* largeUnitAtIndex(int index) { return largeUnits_[index]; }
		
		bool hasSelectedNormalUnits() const;
		bool hasSelectedLargeUnits() const;
		
		void setAvatar(Avatar* avatar) { avatar_ = avatar; }
		void setNormalUnitAtIndex(int index, UnitType* unit) { normalUnits_[index] = unit; }
		void setLargeUnitAtIndex(int index, UnitType* unit) { largeUnits_[index] = unit; }
		
		pb::Message* serialized() const;
		
		// Methods for debugging
		static PlayerConfig deserialized(const std::string& base64, const GameUnits& units);
		std::string serializedBase64() const;
		bool operator==(const PlayerConfig& config) const;
		bool operator!=(const PlayerConfig& config) const;
		
	private:
		Avatar* avatar_;
		UnitType* normalUnits_[UnitColor_Count];
		UnitType* largeUnits_[kLargeUnitSlots];
	};
	
	class AI;
	class Player
	{
		friend class Game;
		friend class AI;
	
	public:
		Player();

		PlayerType type() const { return type_; }
		const PlayerConfig& config() const { return config_; }
		
		int troopsInReservoir() const { return troopsInReservoir_; }
		int turnsLeft() const { return turnsLeft_; }
		int health() const { return health_; }
		int magic() const { return magic_; }
		bool canCastMagic() const { return magic_ >= kMaxMagic; }
		
		const Unit* unit(const CellCoordinate& c) const;
		const Unit* unit(int col, int row) const { return unit(CellCoordinate(col, row)); }
		const Unit* occupyingUnit(const CellCoordinate& c) const;
		const Unit* lastUnitInColumn(int col) const;
		
		int unitClassCountOnField(UnitClass uc) const;
		
		bool isCellEmpty(const CellCoordinate& c, const Unit* except=0) const;
		
		bool canMoveUnit(const Unit* unit) const;
		int rowToMoveUnit(int col, const Unit* unit) const;
		
		bool isUnitInFrontRow(const Unit* unit, bool ignoreWalls=true) const;
		
		bool deserialize(const pb::Message& msg, PlayerType type, const GameUnits& units);
		pb::Message* serialized() const;
		
		// Methods for debugging
		bool isEqualToPlayer(const Player& player) const;
		
	protected:
		Unit* unit(const CellCoordinate& c);
		/// Return 0 if cell is not occupied by any unit
		Unit* occupyingUnit(const CellCoordinate& c);
		
		void fillTroopsFromReservoir();
		void removeUnit(const Unit* unit);
		void moveUnit(const Unit* unit, int col);
		int processFormations(int round, bool findOnly, std::list<ProcessFormationsHint>* hints = 0, bool forceCascade = false);
		
		void addMagic(int magic);
		
		void clear();
		int lastFreeRow(int col, const Unit* except=0) const;
		
		/**
		 * Returns row where unit of specified type can be placed in specified column.
		 * If not possible returns -1.
		 */
		int rowToPlace(int col, UnitClass uc, const Unit* except = 0) const;
		/**
		 * Returns row where unit of specified type can be placed in specified column.
		 * Not necessary last in column. If not possible returns -1.
		 */
		int rowToPlaceAnywhere(int col, UnitClass uc, const Unit* except = 0) const;
		
		bool canPlace(UnitClass uc) const;
		
		void putUnitBackToReservoir(const Unit* unit, bool largeTroopRestorable);
		
		std::list<AttackFormation*>& attackFormations() { return attackFormations_; }
		
	private:
		UnitType* randomLargeUnitTypeForClass(UnitClass uc);
		/// All units standing in the back will occupy the space in front of them if any
		void alignTroops();
		/// All walls and formations will be brought to front
		void arrangeTroops();
		
		AttackFormation* processComboFormation(AttackFormation* formation);
		
	private:
		int index_;
		PlayerType type_;
		Field field_;
		std::list<AttackFormation*> attackFormations_;
		
		int health_;
		int magic_;
		int troopsInReservoir_;
		int largeTroopsInReservoir_;
		int turnsLeft_;
		PlayerConfig config_;
		RandomGenerator randomGenerator_;
	};
}
