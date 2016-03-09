//
//  GameConfig.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "Fixed.h"

namespace monsters
{
	static const int kLargeUnitSlots = 2;
	static const int kFieldWidth = 7;
	static const int kFieldHeight = 6;
	
	static const int kMaxFormationDepth = 3;
	
	static const int kInitialTurns = 3;
	static const int kInitialHealth = 100;
	static const int kMaxTroops = 30;
	static const int kMaxLargeTroops = 6;
	
	static const int kWallLevels = 2;
	
	// Max magic
	static const int kMaxMagic = 100;
	static const int kMagicPerCombo = 20;
	static const int kMagicPerCascade = 10;
	static const int kMagicPerSyncedAttack = 10;
	
	static const w2f2::fixed64_t kSyncedAttackStrengthCoefficient = "0.1";
	static const w2f2::fixed64_t kComboAttackStrengthCoefficient = "1.2";
	
	static const int kTimeForRoundSeconds = 30;
	
	static const int kMaxUnitClassOnField[] = 
	{
		kMaxTroops,
		2,
		1
	};
}
