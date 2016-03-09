//
//  SerializeUnitsIndex.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 5/9/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "protobuf/Message.h"
#include <map>
#include <list>

namespace monsters
{
	class Unit;
	class AttackFormation;
	class SerializeUnitsIndex
	{
	public:
		int indexForUnit(const Unit* unit);
		void serialize(pb::Message& msg, pb::tag_t tag, const std::list<AttackFormation*>& attackFormations);
	
	private:
		std::map<const Unit*, int> idsByUnit_;
	};
}
