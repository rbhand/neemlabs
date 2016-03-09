//
//  GameConfigurationScreenTypes.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

namespace monsters
{
	enum GameConfigurationScreenType
	{
		GameConfigurationScreenType_SinglePlayer = 0,
		GameConfigurationScreenType_TwoPlayers = 1,
		GameConfigurationScreenType_OnlineGame = 2
	};
	
	enum ChooseAvatarArmyType
	{
		ChooseAvatarArmyType_None = -1,
		ChooseAvatarArmyType_Pirates = 0,
		ChooseAvatarArmyType_Ninjas = 1
	};
}
