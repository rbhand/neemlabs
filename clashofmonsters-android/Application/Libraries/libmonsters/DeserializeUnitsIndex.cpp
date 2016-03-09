//
//  DeserializeUnitsIndex.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 5/13/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "DeserializeUnitsIndex.h"
#include "Unit.h"
#include <iostream>

using namespace monsters;
using namespace std;

Unit* DeserializeUnitsIndex::unitForIndex(int index) const
{
	if (index >= 0 && index < (int)units_.size())
		return units_[index];
	
	return 0;
}

bool DeserializeUnitsIndex::deserialize(const pb::Message& msg, pb::tag_t tag, const GameUnits& units)
{
	map<Unit*, int> absorber_wall_indices;
	units_ = vector<Unit*>(msg.countValues(tag), (Unit*)0);
	for (int i=msg.countValues(tag)-1; i>=0; --i)
	{
		int absorber_wall_index = -1;
		Unit* unit = new Unit(*msg.getFieldValueMessageAtIndex(tag, i), units, *this, &absorber_wall_index);
		
		if (absorber_wall_index != -1)
			absorber_wall_indices.insert(pair<Unit*, int>(unit, absorber_wall_index));
		
		if (unit->isValid())
		{
			units_[i] = unit;
		}
		else
		{
			cerr << "ERROR: Deserialization of unit failed" << endl;
		}
	}
	
	for (vector<Unit*>::iterator i=units_.begin(), e=units_.end(); i!=e; ++i)
	{
		map<Unit*, int>::const_iterator it=absorber_wall_indices.find(*i);
		if (it != absorber_wall_indices.end())
		{
			(*i)->absorberWall_ = unitForIndex(it->second);
			if (!(*i)->absorberWall_)
			{
				cerr << "ERROR: Deserialization of unit failed (absorber wall not found)" << endl;
			}
		}
	}
	
	return true;
}
