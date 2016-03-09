//
//  RoundEvent.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/5/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "Unit.h"

namespace monsters
{
	enum RoundEventObjectType
	{
		RoundEventObjectType_None = 0,
		RoundEventObjectType_AttackFormation = 1,
		RoundEventObjectType_Unit = 2,
		RoundEventObjectType_Player = 3
	};
	
	struct RoundEventObject
	{
		RoundEventObject() : unit(0), type(RoundEventObjectType_None) { }
		RoundEventObject(Unit* aUnit) : type(RoundEventObjectType_Unit), unit(aUnit) { }
		RoundEventObject(AttackFormation* anAttackFormation) : type(RoundEventObjectType_AttackFormation),
			attackFormation(anAttackFormation) { }
		RoundEventObject(int aPlayer) : type(RoundEventObjectType_Player),
			player(aPlayer) { }
	
		RoundEventObjectType type;
		union
		{
			Unit* unit;
			AttackFormation* attackFormation;
			int player;
		};
	};
	
	enum RoundEventType
	{
		RoundEventType_None = 0,

		RoundEventType_Attack = 1,
		RoundEventType_Align = 2,
		
		RoundEventType_SpecialAbility_Remove = 3,
		RoundEventType_SpecialAbility_Attack = 4,
		RoundEventType_SpecialAbility_AttackShooting = 5,
		RoundEventType_SpecialAbility_SlowAdjascentCharges = 6,
		RoundEventType_SpecialAbility_HitAllChargingUnits = 7,
		RoundEventType_SpecialAbility_Regenerate = 8
	};
	
	struct RoundEvent
	{
		RoundEvent(RoundEventType aType) : type(aType), objectKilled(false), targetKilled(false), targetTemporaryKilled(false),
			hits(0) { }
		RoundEvent() : type(RoundEventType_None), objectKilled(false), targetKilled(false), targetTemporaryKilled(false),
			hits(0) { }
		
		RoundEventType type;
		RoundEventObject object;
		RoundEventObject target;
		bool objectKilled;
		bool targetKilled;
		bool targetTemporaryKilled;
		CellCoordinate coord;
		int hits;
	};
}
