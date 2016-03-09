//
//  AttackFormation.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/4/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "AttackFormation.h"
#include "SerializeUnitsIndex.h"
#include "DeserializeUnitsIndex.h"
#include <iostream>

using namespace monsters;
using namespace std;

AttackFormation::AttackFormation() :
	combos_(1)
{
}

AttackFormation::~AttackFormation()
{
	for (vector<Unit*>::iterator i=absorbedUnits_.begin(), e=absorbedUnits_.end();
		i!=e; ++i)
	{
		delete *i;
	}
	
	//cout << "deleted AttackFormation: " << (void*)this << endl;
}

int AttackFormation::strength() const
{
	int s = std::max(0, totalStrength_ / 2 + ((totalStrength_ / 2) * (totalRoundsToCharge_ - roundsToCharge_)) /
		(totalRoundsToCharge_) - subtractedStrength_);
	if (baseUnit()->regeneratedCount() > 0)
		s /= 4;
	return s;
}

void AttackFormation::serializeUnits(const std::vector<Unit*>& units, SerializeUnitsIndex& idsByUnit, pb::Message& m, pb::tag_t tag)
{
	using namespace pb;
	
	for (vector<Unit*>::const_iterator i=units.begin(), e=units.end(); i!=e; ++i)
	{
		m << varint::i(tag, idsByUnit.indexForUnit(*i));
	}
}

bool AttackFormation::deserializeUnits(std::vector<Unit*>& units, const DeserializeUnitsIndex& deserializeUnitsIndex, const pb::Message& m, pb::tag_t tag)
{
	for (int i=0; i<m.countValues(tag); ++i)
	{
		Unit* unit = deserializeUnitsIndex.unitForIndex(m.getFieldValueInt32AtIndex(tag, i));
		unit->attackFormation_ = this;
		if (unit)
			units.push_back(unit);
		else
			return false;
	}
	
	return true;
}

bool AttackFormation::compareUnits(const std::vector<Unit*>& units1, const std::vector<Unit*>& units2)
{
	list<Unit*> other_units(units2.begin(), units2.end());
	for (vector<Unit*>::const_iterator i=units1.begin(), e=units1.end();
		i!=e; ++i)
	{
		bool found = false;
		for (list<Unit*>::iterator oi=other_units.begin(), oe=other_units.end();
			oi!=oe; ++oi)
		{
			if ((**oi).isEqualToUnit(**i))
			{
				found = true;
				other_units.erase(oi);
				break;
			}
		}
		
		if (!found)
			return false;
	}
	
	return true;
}


pb::Message* AttackFormation::serialized(SerializeUnitsIndex& idsByUnit) const
{
	using namespace pb;
	Message* m = new Message();
	
	*m
		<< varint::i(1, roundsToCharge_)
		<< varint::i(2, totalRoundsToCharge_)
		<< varint::i(3, totalStrength_)
		<< varint::i(4, subtractedStrength_)
		<< varint::i(5, combos_);
	
	serializeUnits(units_, idsByUnit, *m, 6);
	serializeUnits(absorbedUnits_, idsByUnit, *m, 7);
	
	return m;
}

bool AttackFormation::isEqualToFormation(const AttackFormation& formation) const
{
	if (roundsToCharge_ != formation.roundsToCharge_)
		return false;
		
	if (totalRoundsToCharge_ != formation.totalRoundsToCharge_)
		return false;
		
	if (totalStrength_ != formation.totalStrength_)
		return false;
		
	if (subtractedStrength_ != formation.subtractedStrength_)
		return false;
		
	if (combos_ != formation.combos_)
		return false;
		
	if (!compareUnits(units_, formation.units_))
		return false;
		
	if (!compareUnits(absorbedUnits_, formation.absorbedUnits_))
		return false;

	return true;
}

bool AttackFormation::deserialize(const pb::Message& msg, const DeserializeUnitsIndex& deserializeUnitsIndex)
{
	roundsToCharge_ = msg.getFieldValueInt32(1);
	totalRoundsToCharge_ = msg.getFieldValueInt32(2);
	totalStrength_ = msg.getFieldValueInt32(3);
	subtractedStrength_ = msg.getFieldValueInt32(4);
	combos_ = msg.getFieldValueInt32(5);
	
	if (!deserializeUnits(units_, deserializeUnitsIndex, msg, 6))
		return false;
	
	if (!deserializeUnits(absorbedUnits_, deserializeUnitsIndex, msg, 7))
		return false;
	
	return true;
}

bool AttackFormation::canRegenerate() const
{
	return baseUnit()->unitType()->specialAbility() == SpecialAbility_Regenerate &&
		baseUnit()->regeneratedCount() < baseUnit()->unitType()->specialAbilityParam();
}
