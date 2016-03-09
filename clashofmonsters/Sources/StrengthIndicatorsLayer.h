//
//  StrengthIndicatorsLayer.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/13/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include "GameFieldNode.h"

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	class StrengthIndicatorsLayer: public cocos2d::CCLayer
	{
	public:
		virtual bool init();
		
		CREATE_FUNC(StrengthIndicatorsLayer);
		
		void setGamefield(GameFieldNode* game_field) { gameFieldNode_ = game_field; };
		
		void fadeIn();
		void fadeOut();
		void fade(float time, bool fade_in = true);
		
		float passedTime() const { return time_; }
		
	private:
		void onEnter();
		void onExit();
		
		void update(float dt);
		
		CCPoint labelPositionForUnitNode(UnitNode* unit_node);
		CCLabelBMFont* strengthLabelForUnitNode(UnitNode* unit_node);
		CCSprite* strengthIconForUnitNode(UnitNode* unit_node);
		int iconTagForUnitNode(UnitNode* unit_node);
		int strengthForUnitNode(UnitNode* unit_node);
		void updateLabels();
		void updateIcons();
		
	private:
		GameFieldNode* gameFieldNode_;
		std::map<const UnitNode*, CCLabelBMFont*> strengthLabels_;
		std::map<const UnitNode*, CCSprite*> strengthIcons_;
		
		CCLayer* labelsLayer_;
		CCLayer* iconsLayer_;
		CCLayerColor* bgLayer_;
		
		float time_;
	};
	
}