//
//  Unit.h
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
#include "Fixed.h"
#include <list>
#include "cocos2d.h"

#include "protobuf/Message.h"

namespace monsters
{
	enum UnitColor
	{
		UnitColor_Red = 0,
		UnitColor_Green = 1,
		UnitColor_Blue = 2
	};
	static const int UnitColor_Count = 3;
	
	class Game;
	class Field;
	class Player;
	class AttackFormation;
	class SerializeUnitsIndex;
	class DeserializeUnitsIndex;
	class Unit
	{
		friend class Game;
		friend class Player;
		friend class Field;
		friend class AttackFormation;
		friend class DeserializeUnitsIndex;
		
	public:
		Unit(const pb::Message& msg, const GameUnits& units, const DeserializeUnitsIndex& deserializeUnitsIndex,
			int* out_absorberWallIndex);
		Unit(UnitType* unitType, UnitColor color) :
			unitType_(unitType), color_(color),
			isWall_(false), isTemporaryKilled_(false), 
			weightless_(false), wallLevel_(0), regeneratedCount_(0),
			attackFormation_(0), absorberWall_(0)
		{
			CC_ASSERT(unitType_);
		}
		~Unit();
		
		int player() const { return player_; }
		const CellCoordinate& coord() const { return coord_; }
		const UnitType* unitType() const { return unitType_; }
		const AttackFormation* attackFormation() const { return attackFormation_; }
		AttackFormation* attackFormation() { return attackFormation_; }
		UnitClass unitClass() const { return unitType_->unitClass(); }
		UnitColor color() const { return color_; }
		bool containsCell(const CellCoordinate& c) const;
		bool isWallOrAttack() const { return isWall_ || attackFormation_; }
		bool isWall() const { return isWall_; }
		int indexInAttackFormation() const;
		int wallStrength() const { return wallStrength_; }
		int wallLevel() const { return wallLevel_; }
		int troopsWeight() const;
		int width() const;
		int depth() const;
		bool isTemporaryKilled() const { return isTemporaryKilled_; }
		int regeneratedCount() const { return regeneratedCount_; }
		
		bool isAbsorbed() const;
		bool isAbsorbedWall() const { return isWall() && isAbsorbed(); }
		const Unit* absorber() const;
		
		pb::Message* serialized(SerializeUnitsIndex& idsByUnit, int attackFormationIndex) const;
		
		bool isValid() const { return unitType_ != 0; }
		bool isEqualToUnit(const Unit& unit) const;
	
	private:
		int player_;
		CellCoordinate coord_;
		UnitType* unitType_;
		UnitColor color_;
		
		bool isWall_;
		bool isTemporaryKilled_;
		bool weightless_;
		int wallStrength_;
		int wallLevel_;
		int regeneratedCount_;
		
		AttackFormation* attackFormation_;
		Unit* absorberWall_;
		std::list<Unit*> absorbedUnits_;
	};
}

