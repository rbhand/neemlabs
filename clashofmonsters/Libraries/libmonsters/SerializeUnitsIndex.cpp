//
//  SerializeUnitsIndex.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 5/9/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "SerializeUnitsIndex.h"
#include "Unit.h"
#include <vector>
#include <algorithm>

using namespace monsters;
using namespace std;

int SerializeUnitsIndex::indexForUnit(const Unit* unit)
{
	int index = 0;
	map<const Unit*, int>::iterator ui = idsByUnit_.find(unit);
	if (ui != idsByUnit_.end())
	{
		index = ui->second;
	}
	else
	{
		index = idsByUnit_.size();
		idsByUnit_.insert(pair<const Unit*, int>(unit, index));
	}
	
	return index;
}

void SerializeUnitsIndex::serialize(pb::Message& m, pb::tag_t tag, const std::list<AttackFormation*>& attackFormations)
{
	vector<const Unit*> units(idsByUnit_.size(), 0);
	for (map<const Unit*, int>::const_iterator i=idsByUnit_.begin(), e=idsByUnit_.end();
		i!=e; ++i)
	{
		units[i->second] = i->first;
	}
	
	int off = 0, size;
	
	do
	{
		size = idsByUnit_.size();
		
		for (vector<const Unit*>::const_iterator i=units.begin()+off, e=units.end(); i!=e; ++i)
		{
			int attack_formation_index = (*i)->attackFormation() ?
				distance(attackFormations.begin(),
					find(attackFormations.begin(), attackFormations.end(), (*i)->attackFormation())) :
				-1;
			m << pb::msg(12, (*i)->serialized(*this, attack_formation_index));
		}
		
		off = size;
		
		// looking for newely added units
		for (int new_unit_index=size; new_unit_index<(int)idsByUnit_.size(); ++new_unit_index)
		{
			for (map<const Unit*, int>::const_iterator i=idsByUnit_.begin(), e=idsByUnit_.end();
				i!=e; ++i)
			{
				if (i->second == new_unit_index)
				{
					units.push_back(i->first);
					break;
				}
			}
		}
	}
	while (size != (int)idsByUnit_.size());
}
