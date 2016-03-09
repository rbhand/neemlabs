//
//  GameUnits.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "GameUnits.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdlib.h>

//#include "CCFileUtils.h"
#include "csvpp/csvpp.h"
//#include <json/reader.h>

using namespace monsters;
using namespace std;

struct UnitTypeViewConfigPair
{
	const char unitName[64];
	UnitTypeViewConfig config;
};

static UnitTypeViewConfigPair kUnitTypeViewConfigMap[] = {
	{ "Dragon",
		{
			{ 0, 0 }, { 0, -0.1 }, UnitShadowType_Large, false,
			{ true, true }, // attack and special animations
			{ false }, //megattack settings
			{ ShootingAttackViewConfig::Type_Framed, "dragon_attack_particle", 0.4, 1.2, 255, { 0, 0.5 }, 2, 1, {0, 0, 0} },
			{ ShootingAttackViewConfig::Type_Throw, "dragon_special_particle", 0.4, 1.3, 255, { 0, 0.5 }, 2, 1, {0, 0, 0} }
		}
	},
	{ "Crane",
		{
			{ 0, 0.08 }, { 0, -0.5 }, UnitShadowType_Small, true,
			{ true, false, false }, // attack and special animations
			{ false }, //megattack settings
			{ ShootingAttackViewConfig::Type_None },
		}
	},
	{ "Spider",
		{
			{ 0, -0.05 }, { 0, -0.11 }, UnitShadowType_Large, false,
			{ false }, // attack and special animations
			{ false }, //megattack settings
			{ ShootingAttackViewConfig::Type_Throw, "spider_attack_particle", 0.5, 1.0, 255, { 0, 0 } }
		}
	},
	{ "Komodo Dragon",
		{
			{ 0, 0 }, { 0, -0.1 }, UnitShadowType_Large, false,
			{ true, true }, // attack and special animations
			{ true, 255, 140, 0 }, //megattack settings
			{ ShootingAttackViewConfig::Type_Framed, "dragon_attack_particle", 0.4, 1.4, 255, { 0, 0.5 }, 2, 1, {255, 175, 0} },
			{ ShootingAttackViewConfig::Type_Throw, "dragon_special_particle", 0.4, 1.3, 255, { 0, 0.5 }, 2, 1, {0, 0, 0} }
		}
	},
	{ "Black Widow Spider",
		{
			{ 0, 0 }, { 0, -0.12 }, UnitShadowType_Large, false,
			{ false }, // attack and special animations
			{ true, 105, 105, 105 }, //megattack settings
			{ ShootingAttackViewConfig::Type_Throw, "spider_attack_particle", 0.5, 1.4, 255 }
		}
	},
	
	{ "Ninja with stars", { { 0.004, 0.008 }, { -0.011, -0.25 }, UnitShadowType_Small, false, { false },
		{ ShootingAttackViewConfig::Type_None } } },
	{ "Ninja with stick", { { 0.045, 0.05 }, { -0.039, -0.29 }, UnitShadowType_Small, false, { false },
		{ ShootingAttackViewConfig::Type_None } } },
	
	{ "Octopus",
		{
			{ 0, 0 }, { 0, -0.0 }, UnitShadowType_Large, false,
			{ false, false, true },
			{ false }, //megattack settings
			{ ShootingAttackViewConfig::Type_Framed, "octopus_attack_particle", 0.4, 1.2, 255, { 0, 0 }, 2, 1, {0, 0, 0} }
		}
	},
	{ "Skeleton",
		{
			{ -0.021, 0.05 }, { 0.016, -0.49 }, UnitShadowType_Small, false,
			{ false },
			{ false }, //megattack settings
			{ ShootingAttackViewConfig::Type_None }
		}
	},
	{ "Croc",
		{
			{ 0, 0 }, { 0, -0.15 }, UnitShadowType_Alligator, false,
			{ true, false, true },
			{ false }, //megattack settings
			{ ShootingAttackViewConfig::Type_None }
		}
	},
	{ "Giant Octopus",
		{
			{ 0, 0.05 }, { 0.01, -0.15 }, UnitShadowType_Large, false,
			{ false, false, true },
			{ true, 199, 22, 133 }, //megattack settings
			{ ShootingAttackViewConfig::Type_Framed, "octopus_attack_particle", 0.4, 1.4, 255, { 0, 0 }, 2, 1, {155, 50, 255} }
		}
	},
	{ "Captain Skeleton",
		{
			{ -0.025, 0 }, { 0.015, -0.48 }, UnitShadowType_Small, false,
			{ false },
			{ true, 0, 0, 0 }, //megattack settings
			{ ShootingAttackViewConfig::Type_None }
		}
	},
	{ "Pirate with gun",   { { 0.01, 0.05 }, { -0.01, -0.28 }, UnitShadowType_Small, false, { false },
		{ ShootingAttackViewConfig::Type_None } } },
	{ "Pirate with sword", { { -0.06, 0 }, { 0.055, -0.23 }, UnitShadowType_Small, false, { false },
		{ ShootingAttackViewConfig::Type_None } } },
};


UnitTypeViewConfig viewConfigForUnitName(const string& name)
{
	for (int i=0; i<(int)(sizeof(kUnitTypeViewConfigMap)/sizeof(kUnitTypeViewConfigMap[0])); ++i)
	{
		if (name == kUnitTypeViewConfigMap[i].unitName)
			return kUnitTypeViewConfigMap[i].config;
	}
	
	UnitTypeViewConfig cfg = { { 0, 0 }, { UnitShadowType_Small } };
	return cfg;
}

Avatar* readAvatarConfig(const string& name)
{
    CCLOG("(PIRATES) readAvatarConfig: %s", name.c_str());
	csvpp::CSV csv;
	//unsigned long size;
	//string csv_path = name;
	//char* raw_data = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(csv_path.c_str()).c_str(), "r", &size);
	
	//std::ifstream t(csv_path.c_str());
	//std::string data((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    unsigned long len;
	char* raw_data = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(name.c_str(), "r", &len);
    std::string data(raw_data, len);
	
	// char* data = new char[size+1];
	// memcpy(data, raw_data, size);
	// data[size] = '\0';
	// delete [] raw_data;
	
	csv.set(data.c_str());
	csvpp::rows rows = csv.read();
	
	vector<UnitType*> normal_units;
	vector<UnitType*> large_units;
	
	int wall_strength = 10, wall_strength_x2 = 14;
	
	for (unsigned int i = 0; i < rows.size(); i++)
	{
		csvpp::ROW row = rows.at(i);
		
		csvpp::rowiterator it;
		// for (it = row.begin(); it != row.end(); it++)
		// 	cout << it->first << " => " << it->second << endl;
		
		string name = row["Name"];
		if (row["Name"] == "Wall strength")
		{
			wall_strength = atoi(row["Type"].c_str());
		}
		else if (row["Name"] == "Wall strength x2")
		{
			wall_strength_x2 = atoi(row["Type"].c_str());
		}
		else if (row["Name"].length())
		{
			UnitClass cls = UnitClass_Normal;
			if (row["Type"] == "Small")
				cls = UnitClass_Normal;
			else if (row["Type"] == "Large x2")
				cls = UnitClass_Large2;
			else if (row["Type"] == "Large x4")
				cls = UnitClass_Large4;
			else
				continue;
			
			int defensive = atoi(row["Defensive"].c_str());
			int offensive = atoi(row["Offensive"].c_str());
			int cost = atoi(row["Cost"].c_str());
			int turns_to_charge = atoi(row["Turns to charge"].c_str());
			
			SpecialAbility special_ability = SpecialAbility_None;
			int special_ability_param = atoi(row["Special param 1"].c_str());

			if (row["Special"] == "Remove unmade")
				special_ability = SpecialAbility_RemoveUnmade;
			else if (row["Special"] == "Regenerate")
				special_ability = SpecialAbility_Regenerate;
			else if (row["Special"] == "Remove any")
				special_ability = SpecialAbility_RemoveAny;
			else if (row["Special"] == "Attack first")
				special_ability = SpecialAbility_AttackFirst;
			else if (row["Special"] == "Attack through walls")
				special_ability = SpecialAbility_AttackThroughWalls;
			else if (row["Special"] == "Attack all")
				special_ability = SpecialAbility_AttackAll;
			else if (row["Special"] == "Slow adjascent charges")
				special_ability = SpecialAbility_SlowAdjascentCharges;
			else if (row["Special"] == "Hit all charging units")
				special_ability = SpecialAbility_HitAllChargingUnits;
					
			else if (row["Special"].size())
				cerr << "WARNING: Unknown special ability: \"" << row["Special"] << "\" for unit type \"" << row["Name"] << "\"";
				
				
//			SpecialAbility special_ability = SpecialAbility_AttackAll;
//			int special_ability_param = 10;
			
			UnitType* ut = new UnitType(cls, defensive, offensive, cost, turns_to_charge,
				special_ability, special_ability_param,
				viewConfigForUnitName(row["Name"]), row["Name"].c_str(), row["Description"].c_str());
			if (cls == UnitClass_Normal)
				normal_units.push_back(ut);
			else
				large_units.push_back(ut);
		}
	}
	
	//delete [] data;
	
	return new Avatar(normal_units, large_units, wall_strength, wall_strength_x2,
		name.c_str());
}

GameUnits::GameUnits(const std::string& piratesConfigPath, const std::string& ninjaConfigPath)
//	configBasePath_(configBasePath)
{
	avatars_.push_back(readAvatarConfig(piratesConfigPath));
	avatars_.push_back(readAvatarConfig(ninjaConfigPath));
	
	setupIndices();
}

void GameUnits::setupIndices()
{
	int ind = 0;
	for (vector<Avatar*>::iterator i=avatars_.begin(), e=avatars_.end(); i!=e; ++i)
	{
		(*i)->index_ = ind;
		++ind;
		
		int uind = 0;
		for (vector<UnitType*>::iterator ui=(*i)->normalUnits_.begin(), ue=(*i)->normalUnits_.end();
			ui!=ue; ++ui)
		{
			(*ui)->index_ = uind;
			(*ui)->avatar_ = (*i);
			++uind;
		}
		
		uind = 0;
		for (vector<UnitType*>::iterator ui=(*i)->largeUnits_.begin(), ue=(*i)->largeUnits_.end();
			ui!=ue; ++ui)
		{
			(*ui)->index_ = uind;
			(*ui)->avatar_ = (*i);
			++uind;
		}
	}
}

Avatar::~Avatar()
{
	for (std::vector<UnitType*>::iterator i=normalUnits_.begin(), e=normalUnits_.end(); i!=e; ++i)
	{
		delete *i;
	}
	
	for (std::vector<UnitType*>::iterator i=largeUnits_.begin(), e=largeUnits_.end(); i!=e; ++i)
	{
		delete *i;
	}
}

GameUnits::~GameUnits()
{
	for (std::vector<Avatar*>::iterator i=avatars_.begin(), e=avatars_.end(); i!=e; ++i)
	{
		delete *i;
	}
}

int monsters::getUnitWidth(UnitClass uc)
{
	switch (uc)
	{
		case UnitClass_Normal: return 1;
		case UnitClass_Large2: return 1;
		case UnitClass_Large4: return 2;
	}
	
	return 1;
}

int monsters::getUnitDepth(UnitClass uc)
{
	switch (uc)
	{
		case UnitClass_Normal: return 1;
		case UnitClass_Large2: return 2;
		case UnitClass_Large4: return 2;
	}
	
	return 1;
}

bool monsters::specialAbilityAddsEventsBeforeTurn(SpecialAbility ability)
{
	switch (ability)
	{
		case SpecialAbility_RemoveUnmade:
		case SpecialAbility_RemoveAny:
		case SpecialAbility_AttackFirst:
		case SpecialAbility_AttackAll:
		case SpecialAbility_AttackThroughWalls:
		case SpecialAbility_HitAllChargingUnits:
			return true;
		
		case SpecialAbility_Regenerate:
		case SpecialAbility_SlowAdjascentCharges:
			return false;
		
		case SpecialAbility_None:
			return false;
	}
	
	return false;
}

bool monsters::specialAbilityAddsEventsBeforeAttack(SpecialAbility ability)
{
	return false;
}

int UnitType::width() const
{
	return getUnitWidth(unitClass_);
}

int UnitType::depth() const
{
	return getUnitDepth(unitClass_);
}

