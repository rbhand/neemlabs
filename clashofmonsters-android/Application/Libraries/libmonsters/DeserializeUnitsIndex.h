//
//  DeserializeUnitsIndex.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 5/13/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "protobuf/Message.h"
#include <vector>

namespace monsters
{
	class Unit;
	class GameUnits;
	class AttackFormation;
	class DeserializeUnitsIndex
	{
	public:
		Unit* unitForIndex(int index) const;
		bool deserialize(const pb::Message& msg, pb::tag_t tag, const GameUnits& units);
	
	private:
		std::vector<Unit*> units_;
	};
}
