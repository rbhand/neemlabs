//
//  UnitNode.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "BarNode.h"

namespace monsters
{
	enum UnitNodeState
	{
		UnitNodeState_Normal = 0,
		UnitNodeState_AttackFormation = 1,
		UnitNodeState_WallLevel0 = 10,
		UnitNodeState_WallLevel1 = 11
	};
	
	enum UnitNodeAnimation
	{
		UnitNodeAnimation_None = 0,
		UnitNodeAnimation_Idle = 1,
		UnitNodeAnimation_Walk = 2,
		UnitNodeAnimation_Attack = 3,
		UnitNodeAnimation_Special = 4
	};
	
	enum UnitNodeSound
	{
		UnitNodeSound_None = 0,
		UnitNodeSound_Attack = 1,
		UnitNodeSound_Special = 2
	};

	class UnitNode : public cocos2d::CCNode, public cocos2d::CCRGBAProtocol
	{
	public:
		bool init();
		void removeFromParentAndCleanup(bool cleanup);
		
		void setUnit(const Unit* unit);
		void setAnimation(UnitNodeAnimation animation);
		void setReverseAttackAnimation(bool reverse) { reverseAttackAnimation_ = reverse; }
		const Unit* unit() const { return unit_; }
		
		UnitNodeState state() const { return state_; }
		void setState(UnitNodeState state, bool animated);
		void updateMarkers();
		
		GLubyte getOpacity(void) { return sprite_->getOpacity(); }
		void setOpacity(GLubyte opacity);
		void setColor(const cocos2d::ccColor3B& color);
		const cocos2d::ccColor3B& getColor(void) { return sprite_->getColor(); }
		void setOpacityModifyRGB(bool bValue) { sprite_->setOpacityModifyRGB(bValue); }
		bool isOpacityModifyRGB(void) { return sprite_->isOpacityModifyRGB(); }
		
		const cocos2d::ccColor3B& getDisplayedColor(void) { return sprite_->getDisplayedColor(); }
		GLubyte getDisplayedOpacity(void) { return sprite_->getDisplayedOpacity(); }
		bool isCascadeColorEnabled(void) { return sprite_->isCascadeColorEnabled(); }
		void setCascadeColorEnabled(bool cascadeColorEnabled) { sprite_->setCascadeColorEnabled(cascadeColorEnabled); }
		void updateDisplayedColor(const cocos2d::ccColor3B& color) { sprite_->updateDisplayedColor(color); }
		bool isCascadeOpacityEnabled(void) { return sprite_->isCascadeOpacityEnabled(); }
		void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) { return sprite_->setCascadeOpacityEnabled(cascadeOpacityEnabled); }
		void updateDisplayedOpacity(GLubyte opacity) { sprite_->updateDisplayedOpacity(opacity); }
		
		static UnitNodeState stateForUnit(const Unit* unit);
		
		cocos2d::CCAnimation* idleAnimation() const;
		cocos2d::CCAnimation* walkAnimation() const;
		cocos2d::CCAnimation* attackAnimation() const;
		cocos2d::CCAnimation* specialAnimation() const;
		cocos2d::CCAnimation* chargeParticleAnimation() const;
		
		cocos2d::CCSize getSpriteSize() const { return sprite_->getContentSize(); }
		
		static std::string frameNameForUnit(const Unit* unit, const char* animation, int frame);
		
		void showShadow();
		void hideShadow();
		
		void shrinkShadowAnimated(float time);
		void runSingleAttackAnimation();
		void runSingleSpecialAnimation();
		void runSingleSpecialAnimation(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		
		void runTemporaryKilledAnimation(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		void runRegeneratedAnimation(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		
		void playSound(UnitNodeSound sound, float time = 5);
		void stopSound();
		
	private:
		cocos2d::CCAnimation* animation(const std::string& animationName, float interval,
			bool reverse) const;
		cocos2d::CCSprite* createChargeParticle();
		std::string shadowSpriteFrameName(bool highlighted) const;
		
		void afterSingleSpecialAnimation();
		void afterRegenerated();
		void onExit();
		
	private:
		const Unit* unit_;
		cocos2d::CCSprite* sprite_;
		cocos2d::CCSprite* spriteTemporaryKilled_;
		cocos2d::CCSprite* shadow_;
		cocos2d::CCSprite* test_cross_;
		cocos2d::CCSprite* formationMarkerSprite_;
		BarNode* outHealthLeft_;
		cocos2d::CCLabelTTF* outRoundsToCharge_;
		bool chargeAnimationStarted_;
		UnitNodeState state_;
		UnitNodeAnimation animation_;
		bool reverseAttackAnimation_;
		unsigned int lastActiveSoundId_;
	};
}
