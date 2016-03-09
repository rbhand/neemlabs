//
//  UnitTypeViewConfig.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/26/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include <stdint.h>

namespace monsters
{
	enum UnitShadowType
	{
		UnitShadowType_Small = 0,
		UnitShadowType_Large = 1,
		UnitShadowType_Alligator = 2
	};
	
	struct ShootingAttackViewConfig
	{
		enum Type
		{
			Type_None = 0,
			Type_Framed = 1,
			Type_Throw = 2
		};
	
		Type type;
		char particleBaseName[128];
		float secondsPerAnimationUnit;
		float scaleFactor;
		uint8_t masterOpacity;
		struct
		{
			float x;
			float y;
		} particleStartOffset;
		int firstFramesToRepeat;
		int repeatCount;
		struct
		{
			unsigned char red;
			unsigned char green;
			unsigned char blue;
		} sparkColor;
	};

	struct UnitTypeViewConfig
	{
//		UnitTypeViewConfig() :
//			shadowType(UnitShadowType_Small)
//		{
//			offset.x = offset.y = 0;
//		}
	
		struct
		{
			float x;
			float y;
		} offset;
		
		struct
		{
			float x;
			float y;
		} shadowOffset;
		UnitShadowType shadowType;
		bool hideShadowWhenAttack;
		
		struct
		{
			bool loopAttackAnimation;
			bool specialAnimationIsAttackAnimation;
			bool loopSpecialAnimation;
		};
		
		struct
		{
			bool megaAttack;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
		} megaAttackConfig;

		ShootingAttackViewConfig shootingAttackConfig;
		ShootingAttackViewConfig shootingSpecialAttackConfig;
		
	};
}
