//
//  GameUnits.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <assert.h>

#include "UnitTypeViewConfig.h"

namespace monsters
{
	class GameUnits;

	enum UnitClass
	{
		UnitClass_Normal = 0,
		UnitClass_Large2 = 1,
		UnitClass_Large4 = 2
	};
	
	int getUnitWidth(UnitClass uc);
	int getUnitDepth(UnitClass uc);
	
	enum SpecialAbility
	{
		SpecialAbility_None = 0,
	
		SpecialAbility_RemoveUnmade = 1,
		SpecialAbility_RemoveAny = 2,
		
		SpecialAbility_Regenerate = 3,
		
		SpecialAbility_AttackFirst = 4,
		SpecialAbility_AttackThroughWalls = 5,
		SpecialAbility_AttackAll = 6,
		
		SpecialAbility_SlowAdjascentCharges = 7,
		
		SpecialAbility_HitAllChargingUnits = 8
	};
	
	bool specialAbilityAddsEventsBeforeTurn(SpecialAbility ability);
	bool specialAbilityAddsEventsBeforeAttack(SpecialAbility ability);
	
	class Avatar;
	class UnitType
	{
		friend class GameUnits;
		
	public:
		UnitType(UnitClass unitClass, int defensive, int offensive, int cost, int roundsToCharge,
				SpecialAbility specialAbility, int specialAbilityParam,
				const UnitTypeViewConfig& viewConfig, const char* name, const char* description) :
			unitClass_(unitClass), defensive_(defensive), offensive_(offensive), cost_(cost), roundsToCharge_(roundsToCharge),
			name_(name), description_(description),
			specialAbility_(specialAbility), specialAbilityParam_(specialAbilityParam),
			viewConfig_(viewConfig)
		{
		}
		
		UnitClass unitClass() const { return unitClass_; }
		bool isLarge() const { return unitClass_ == UnitClass_Large2 || unitClass_ == UnitClass_Large4; }
		int offensive() const { return offensive_; }
		int defensive() const { return defensive_; }
		int cost() const { return cost_; }
		int roundsToCharge() const { return roundsToCharge_; }
		const std::string& name() const { return name_; }
		const std::string& description() const { return description_; }
		const Avatar* avatar() const { return avatar_; }
		
		SpecialAbility specialAbility() const { return specialAbility_; }
		int specialAbilityParam() const { return specialAbilityParam_; }
		
		int index() const { return index_; }
		int width() const;
		int depth() const;
		
		UnitTypeViewConfig viewConfig() const { return viewConfig_; }
		
	private:
		UnitClass unitClass_;
		int defensive_;
		int offensive_;
		int cost_;
		int roundsToCharge_;
		std::string name_;
		std::string description_;
		int index_;
		Avatar* avatar_;
		SpecialAbility specialAbility_;
		int specialAbilityParam_;
		UnitTypeViewConfig viewConfig_;
	};

	class Avatar
	{
		friend class GameUnits;
		
	public:
		Avatar(const std::vector<UnitType*>& normalUnits, const std::vector<UnitType*>& largeUnits, int wallStrength,
				int wallStrengthX2, const std::string& name) :
			normalUnits_(normalUnits), largeUnits_(largeUnits), name_(name)
		{
			wallStrength_[0] = wallStrength;
			wallStrength_[1] = wallStrengthX2;
		}
		~Avatar();
		
		const std::vector<UnitType*>& normalUnits() const
			{ return normalUnits_; }
		const std::vector<UnitType*>& largeUnits() const
			{ return largeUnits_; }
			
		int index() const { return index_; }
		int wallStrength(int level) const { assert(level >= 0 && level <= 1); return wallStrength_[level]; }
		
	private:
		std::vector<UnitType*> normalUnits_;
		std::vector<UnitType*> largeUnits_;
		std::string name_;
		int index_;
		int wallStrength_[2];
	};
	
	class GameUnits
	{
	public:
		GameUnits(const std::string& piratesConfigPath, const std::string& ninjaConfigPath);
		~GameUnits();
		
		const std::vector<Avatar*>& avatars() const { return avatars_; }
		
	private:
		void setupIndices();
		
	private:
		//std::string configBasePath_;
		std::vector<Avatar*> avatars_;
	};
}
