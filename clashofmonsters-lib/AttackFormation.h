//
//  AttackFormation.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/4/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "Unit.h"
#include <vector>
#include <map>

#include "protobuf/Message.h"

namespace monsters
{
	class Player;
	class Game;
	class SerializeUnitsIndex;
	class DeserializeUnitsIndex;
	class AIContext;
	/**
	 * Attacking formation of units.
	 */
	class AttackFormation
	{
		friend class Player;
		friend class Game;
		friend class AIContext;
	
	public:
		AttackFormation();
		~AttackFormation();
		
		int col() const { return units_.at(0)->coord().col; }
		int width() const { return units_.at(0)->width(); }
		const std::vector<Unit*>& units() const { return units_; }
		const std::vector<Unit*>& absorbedUnits() const { return absorbedUnits_; }
		int strength() const;
		int roundsToCharge() const { return roundsToCharge_; }
		int combos() const { return combos_; }
		
		pb::Message* serialized(SerializeUnitsIndex& idsByUnit) const;
		bool deserialize(const pb::Message& msg, const DeserializeUnitsIndex& deserializeUnitsIndex);
		
		w2f2::fixed64_t totalStrength() const { return totalStrength_; };
		w2f2::fixed64_t subtractedStrength() const { return subtractedStrength_; };
		
		const Unit* baseUnit() const { return units_.at(0); }
		Unit* baseUnit() { return units_.at(0); }
		
		bool canRegenerate() const;
		
		// Methods for debugging
		bool isEqualToFormation(const AttackFormation& formation) const;
		
	private:
		static void serializeUnits(const std::vector<Unit*>& units, SerializeUnitsIndex& idsByUnit, pb::Message& m, pb::tag_t tag);
		bool deserializeUnits(std::vector<Unit*>& units, const DeserializeUnitsIndex& deserializeUnitsIndex,
			const pb::Message& m, pb::tag_t tag);
		static bool compareUnits(const std::vector<Unit*>& units1, const std::vector<Unit*>& units2);
		
	private:
		std::vector<Unit*> units_;
		std::vector<Unit*> absorbedUnits_;
		int	roundsToCharge_;
		int	totalRoundsToCharge_;
		int totalStrength_;
		int subtractedStrength_;
		int combos_;
	};
}
