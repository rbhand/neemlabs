//
//  Unit.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "Unit.h"
#include "AttackFormation.h"
#include "SerializeUnitsIndex.h"
#include "DeserializeUnitsIndex.h"
#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace monsters;
using namespace std;

Unit::~Unit()
{
	for (list<Unit*>::iterator i=absorbedUnits_.begin(), e=absorbedUnits_.end();
		i!=e; ++i)
	{
		delete *i;
	}
	
	cout << "deleted Unit: " << (void*)this << endl;
}

Unit::Unit(const pb::Message& msg, const GameUnits& units, const DeserializeUnitsIndex& deserializeUnitsIndex,
	int* out_absorberWallIndex) :
	unitType_(0), attackFormation_(0)
{
	player_ = msg.getFieldValueInt32(1);
	coord_.row = msg.getFieldValueInt32(2);
	coord_.col = msg.getFieldValueInt32(3);
	
	uint32_t avatar_index = msg.getFieldValueUInt32(4);
	if (avatar_index < units.avatars().size())
	{
		Avatar* avatar = units.avatars().at(avatar_index);
		bool is_large = msg.getFieldValueUInt32(5);
		uint32_t unit_type_index = msg.getFieldValueUInt32(6);
		
		const std::vector<UnitType*>& unit_types = is_large ?
			avatar->largeUnits() : avatar->normalUnits();
		
		if (unit_type_index < unit_types.size())
			unitType_ = unit_types[unit_type_index];
		else // invalid unit type
			return;
	}
	
	color_ = (UnitColor)msg.getFieldValueUInt32(7);
	
	isWall_ = msg.getFieldValueUInt32(8);
	isTemporaryKilled_ = msg.getFieldValueUInt32(9);
	weightless_ = msg.getFieldValueUInt32(10);
	
	wallStrength_ = msg.getFieldValueInt32(11);
	wallLevel_ = msg.getFieldValueInt32(12);
	regeneratedCount_ = msg.getFieldValueInt32(13);
	
	if (msg.hasValue(15))
	{
		absorberWall_ = deserializeUnitsIndex.unitForIndex(msg.getFieldValueInt32(15));
		if (!absorberWall_)
		{
			*out_absorberWallIndex = (int)msg.getFieldValueInt32(15);
			return;
		}
	}
	else
	{
		absorberWall_ = 0;
	}
	
	for (int i=0; i<msg.countValues(16); ++i)
	{
		Unit* unit = deserializeUnitsIndex.unitForIndex(msg.getFieldValueInt32AtIndex(16, i));
		if (unit)
		{
			absorbedUnits_.push_back(unit);
		}
		else
		{
			// error
			unitType_ = 0;
			return;
		}
	}
}

static bool compareUnits(const std::list<Unit*>& units1, const std::list<Unit*>& units2)
{
	list<Unit*> other_units(units2.begin(), units2.end());
	for (list<Unit*>::const_iterator i=units1.begin(), e=units1.end();
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

bool Unit::isEqualToUnit(const Unit& unit) const
{
	if (player_ != unit.player_)
		return false;
	
	if (coord_.row != unit.coord_.row)
		return false;
		
	if (coord_.col != unit.coord_.col)
		return false;
		
	if (unitType_ != unit.unitType_)
		return false;
		
	if (color_ != unit.color_)
		return false;
		
	if (isWall_ != unit.isWall_)
		return false;
		
	if (isTemporaryKilled_ != unit.isTemporaryKilled_)
		return false;
		
	if (weightless_ != unit.weightless_)
		return false;
		
	if (wallStrength_ != unit.wallStrength_)
		return false;
		
	if (wallLevel_ != unit.wallLevel_)
		return false;
		
	if (regeneratedCount_ != unit.regeneratedCount_)
		return false;
	
	if (absorberWall_ && unit.absorberWall_)
	{
		// Possible endless loop
		//if (!absorberWall_->isEqualToUnit(*unit.absorberWall_))
		//	return false;
	}
	else if ((!absorberWall_ && unit.absorberWall_) || (absorberWall_ && !unit.absorberWall_))
	{
		return false;
	}
	
	if (!compareUnits(absorbedUnits_, unit.absorbedUnits_))
		return false;
	
	return true;
}

pb::Message* Unit::serialized(SerializeUnitsIndex& idsByUnit, int attackFormationIndex) const
{
	using namespace pb;
	Message* m = new Message();
	
	*m
		<< varint::i(1, player_)
		<< varint::i(2, coord_.row)
		<< varint::i(3, coord_.col)
		
		<< varint::ui(4, unitType_->avatar()->index())
		<< varint::b(5, unitType_->isLarge())
		<< varint::ui(6, unitType_->index())
		<< varint::ui(7, color_)
		
		<< varint::b(8, isWall_)
		<< varint::b(9, isTemporaryKilled_)
		<< varint::b(10, weightless_)
		
		<< varint::i(11, wallStrength_)
		<< varint::i(12, wallLevel_)
		<< varint::i(13, regeneratedCount_)
		
		<< varint::i(14, attackFormationIndex);
		
	if (absorberWall_)
	{
		*m << varint::i(15, idsByUnit.indexForUnit(absorberWall_));
	}
	
	for (list<Unit*>::const_iterator i=absorbedUnits_.begin(), e=absorbedUnits_.end(); i!=e; ++i)
	{
		*m << varint::i(16, idsByUnit.indexForUnit(*i));
	}
	
	
	return m;
}

bool Unit::containsCell(const CellCoordinate& c) const
{
	if (coord() == c)
		return true;
	
	if (unitType()->unitClass() == UnitClass_Large2 ||
		unitType()->unitClass() == UnitClass_Large4)
	{
		if (CellCoordinate(coord().col, coord().row + 1) == c)
			return true;
		
		if (unitType()->unitClass() == UnitClass_Large4 &&
			(CellCoordinate(coord().col + 1, coord().row) == c ||
			CellCoordinate(coord().col + 1, coord().row + 1) == c))
		{
			return true;
		}
	}
	
	return false;
}

int Unit::indexInAttackFormation() const
{
	if (attackFormation_)
	{
		std::vector<Unit*>::const_iterator it = find(attackFormation_->units().begin(),
			attackFormation_->units().end(), this);
		assert(it != attackFormation_->units().end());
		return it - attackFormation_->units().begin();
	}
	
	return -1;
}

bool Unit::isAbsorbed() const
{
	if (attackFormation_)
	{
		std::vector<Unit*>::const_iterator it = find(attackFormation_->absorbedUnits().begin(),
			attackFormation_->absorbedUnits().end(), this);
		return (it != attackFormation_->absorbedUnits().end());
	}
	
	return absorberWall_ != 0;
}

const Unit* Unit::absorber() const
{
	if (isAbsorbed())
	{
		return attackFormation_ ? attackFormation_->units().at(0) :
			absorberWall_;
	}
	
	return 0;
}

int Unit::troopsWeight() const
{
	if (weightless_)
		return 0;
	
	switch (unitType()->unitClass())
	{
		case UnitClass_Normal: return 1;
		case UnitClass_Large2: return 2;
		case UnitClass_Large4: return 4;
	}
	
	return 0;
}

int Unit::width() const
{
	return unitType()->width();
}

int Unit::depth() const
{
	return unitType()->depth();
}
