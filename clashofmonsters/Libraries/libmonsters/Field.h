//
//  Field.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "GameTypes.h"
#include "Unit.h"

#include "protobuf/Message.h"

namespace monsters
{
	class SerializeUnitsIndex;
	class DeserializeUnitsIndex;
	class Field
	{
	public:
		Field();
		Field(const Field& field);
		
		void clear(bool sync=true);
		
		Unit* unit(int col, int row) const { return unit(CellCoordinate(col, row)); }
		Unit* unit(const CellCoordinate& coord) const;
		Unit* occupyingUnit(const CellCoordinate& c) const;
		bool isCellEmpty(const CellCoordinate& c, const Unit* except) const;
		bool isCellEmpty(const CellCoordinate& c) const { return isCellEmpty(c, 0); }
		
		void setUnit(const CellCoordinate& c, Unit* unit);
		void setUnit(int col, int row, Unit* unit) { setUnit(CellCoordinate(col, row), unit); }
		
		void alignTroops();
		
		CellCoordinate coord(const Unit* unit) const;
		
		pb::Message* serialized(SerializeUnitsIndex& idsByUnit) const;
		bool deserialize(const pb::Message& msg, const DeserializeUnitsIndex& deserializeUnitsIndex);
		
		bool operator==(const Field& field) const;
		bool operator!=(const Field& field) const;
		
	private:
		bool syncWithUnits_;
		Unit* field_[kFieldWidth][kFieldHeight];
	};
}
