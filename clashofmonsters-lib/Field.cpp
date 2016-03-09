//
//  Field.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "Field.h"
#include "SerializeUnitsIndex.h"
#include "DeserializeUnitsIndex.h"
#include <string.h>

using namespace monsters;
using namespace std;

Field::Field() :
	syncWithUnits_(true)
{
	for (int c=0; c<kFieldWidth; ++c)
	{
		for (int r=0; r<kFieldHeight; ++r)
		{
			field_[c][r] = 0;
		}
	}
}

Field::Field(const Field& field) :
	syncWithUnits_(false)
{
	memcpy(field_, field.field_, sizeof(field.field_));
}

bool Field::operator==(const Field& field) const
{
	for (int x=0; x<kFieldWidth; ++x)
	{
		for (int y=0; y<kFieldHeight; ++y)
		{
			bool has1 = field_[x][y] != 0;
			bool has2 = field.field_[x][y] != 0;
			
			if (has1 != has2 || (has1 && !field_[x][y]->isEqualToUnit(*(field.field_[x][y]))))
				return false;
		}
	}
	
	return true;
}

bool Field::operator!=(const Field& field) const
{
	return !(*this == field);
}

pb::Message* Field::serialized(SerializeUnitsIndex& idsByUnit) const
{
	using namespace pb;
	Message* m = new Message();
	
	for (int x=0; x<kFieldWidth; ++x)
	{
		for (int y=0; y<kFieldHeight; ++y)
		{
			if (field_[x][y])
			{
				Message* unit_m = new Message();
				
				*unit_m
					<< varint::i(1, x)
					<< varint::i(2, y)
					<< varint::i(3, idsByUnit.indexForUnit(field_[x][y]));
				
				*m << msg(1, unit_m);
			}
		}
	}
	
	return m;
}

bool Field::deserialize(const pb::Message& msg, const DeserializeUnitsIndex& deserializeUnitsIndex)
{
	for (int c=0; c<kFieldWidth; ++c)
	{
		for (int r=0; r<kFieldHeight; ++r)
		{
			field_[c][r] = 0;
		}
	}

	for (int i=0; i<msg.countValues(1); ++i)
	{
		pb::Message* unit_m = msg.getFieldValueMessageAtIndex(1, i);
		if (unit_m)
		{
			int x = unit_m->getFieldValueInt32(1);
			int y = unit_m->getFieldValueInt32(2);
			int index = unit_m->getFieldValueInt32(3);
			Unit* unit = deserializeUnitsIndex.unitForIndex(index);
			
			if (!unit)
				return false;
				
			field_[x][y] = unit;
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

void Field::clear(bool sync)
{
	for (int c=0; c<kFieldWidth; ++c)
	{
		for (int r=0; r<kFieldHeight; ++r)
		{
			if (field_[c][r])
			{
				if (syncWithUnits_ && sync)
					delete field_[c][r];
				
				field_[c][r] = 0;
			}
		}
	}
}

void Field::alignTroops()
{
	bool moved;
	do
	{
		moved = false;
		
		for (int c=0; !moved && c<kFieldWidth; ++c)
		{
			for (int r=0; r<kFieldHeight; ++r)
			{
				if (Unit* unit = field_[c][r])
				{
					int nr = r-1;
					bool can_move = true;
					
					while (can_move && nr >= 0)
					{
						for (int i=0; i<unit->width(); ++i)
						{
							if (!isCellEmpty(CellCoordinate(c+i, nr)))
							{
								can_move = false;
								break;
							}
						}
						
						if (can_move)
							--nr;
					}
					
					if ((nr + 1) < r)
					{
						field_[c][nr+1] = field_[c][r];
						if (syncWithUnits_)
							field_[c][nr+1]->coord_.row = nr+1;
						field_[c][r] = 0;
						
						moved = true;
						break;
					}
				}
			}
		}
	}
	while (moved);
}

void Field::setUnit(const CellCoordinate& c, Unit* unit)
{
	assert(c.col >= 0 && c.col < kFieldWidth &&
		c.row >= 0 && c.row < kFieldHeight);
	
	field_[c.col][c.row] = unit;
}

Unit* Field::unit(const CellCoordinate& c) const
{
	assert(c.col >= 0 && c.col < kFieldWidth &&
		c.row >= 0 && c.row < kFieldHeight);
	
	return field_[c.col][c.row];
}

Unit* Field::occupyingUnit(const CellCoordinate& c) const
{
	if (c.col >= 0 && c.col < kFieldWidth &&
		c.row >= 0 && c.row < kFieldHeight)
	{
		if (field_[c.col][c.row])
			return field_[c.col][c.row];
		
		// check top for 2 or 4 sized large unit
		if (c.row > 0 && field_[c.col][c.row-1] && 
				(field_[c.col][c.row-1]->unitType()->unitClass() == UnitClass_Large2 ||
				field_[c.col][c.row-1]->unitType()->unitClass() == UnitClass_Large4))
			return field_[c.col][c.row-1];
			
		// check left for 4 sized large unit
		if (c.col > 0 && field_[c.col-1][c.row] && 
				field_[c.col-1][c.row]->unitType()->unitClass() == UnitClass_Large4)
			return field_[c.col-1][c.row];
			
		// check left top for 4 sized large unit
		if (c.row > 0 && c.col > 0 && field_[c.col-1][c.row-1] && 
				field_[c.col-1][c.row-1]->unitType()->unitClass() == UnitClass_Large4)
			return field_[c.col-1][c.row-1];
	}
	
	return 0;
}

CellCoordinate Field::coord(const Unit* unit) const
{
	for (int c=0; c<kFieldWidth; ++c)
	{
		for (int r=0; r<kFieldHeight; ++r)
		{
			if (field_[c][r] == unit)
				return CellCoordinate(c, r);
		}
	}
	
	return CellCoordinate();
}

bool Field::isCellEmpty(const CellCoordinate& c, const Unit* except) const
{
//	assert(c.col >= 0 && c.col < kFieldWidth &&
//		c.row >= 0 && c.row < kFieldHeight);
	
	const Unit* u = occupyingUnit(c);
	return u == 0 || u == except;
}

