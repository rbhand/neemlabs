//
//  UnitNode.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "UnitNode.h"
#include "Utils.h"
#include "SoundManager.h"

#include <assert.h>


#define MONSTERS_UNIT_NODE_ANIMATION_TAG   0

//#define SHOW_POSITION_MARKER	1

using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;

bool UnitNode::init()
{
	unit_ = 0;
	sprite_ = 0;
	shadow_ = 0;
	test_cross_ = 0;
	formationMarkerSprite_ = 0;
	state_ = UnitNodeState_Normal;
	animation_ = UnitNodeAnimation_None;
	reverseAttackAnimation_ = false;
	chargeAnimationStarted_ = false;
	lastActiveSoundId_ = 0;
	
	return true;
}


void UnitNode::onExit()
{
	stopSound();
	
	CCNode::onExit();
}

void UnitNode::removeFromParentAndCleanup(bool cleanup)
{
	stopSound();
	
	CCNode::removeFromParentAndCleanup(cleanup);
}

std::string UnitNode::frameNameForUnit(const Unit* unit, const char* animation, int frame)
{
	return createWithFormat("avatar_%d_%s_%d_%s_%s_%d.png",
		unit->unitType()->avatar()->index(),
		unit->unitType()->isLarge() ? "large" : "normal",
		unit->unitType()->index(),
		unit->color() == UnitColor_Red ? "red" : (unit->color() == UnitColor_Green ? "green" : "blue"),
		animation,
		frame);
}

cocos2d::CCAnimation* UnitNode::animation(const std::string& animationName, float interval, bool reverse) const
{
	CCArray* frames = CCArray::createWithCapacity(8);
	int i = 0;
	while (CCSpriteFrame* sf = CCSpriteFrameCache::sharedSpriteFrameCache()->
		spriteFrameByName(frameNameForUnit(unit_, animationName.c_str(), i).c_str()))
	{
		CCAnimationFrame* af = new CCAnimationFrame();
		af->initWithSpriteFrame(sf, 0.1, 0);
		frames->addObject(af);
		af->autorelease();
		++i;
	}
	
	if (reverse)
	{
		for (i=frames->count()-2; i>=0; --i)
		{
			CCAnimationFrame* of = dynamic_cast<CCAnimationFrame*>(frames->objectAtIndex(i));
			CCAnimationFrame* af = new CCAnimationFrame();
			af->initWithSpriteFrame(of->getSpriteFrame(), of->getDelayUnits(), 0);
			frames->addObject(af);
			af->autorelease();
		}
	}
	
	return CCAnimation::create(frames, interval, 1);
}

CCAnimation* UnitNode::idleAnimation() const
{
	return animation("idle", 1.0, true);
}

CCAnimation* UnitNode::walkAnimation() const
{
	return animation("move", 0.6, true);
}

CCAnimation* UnitNode::attackAnimation() const
{
	bool reverse = unit()->unitType()->viewConfig().loopAttackAnimation;
	
	if (reverseAttackAnimation_)
		return animation("attack_back", (1.2 + ((float)rand() / (float)RAND_MAX) * 0.3), reverse);
	else
		return animation("attack", (1.2 + ((float)rand() / (float)RAND_MAX) * 0.3), reverse);
}

cocos2d::CCAnimation* UnitNode::specialAnimation() const
{
	bool reverse = unit()->unitType()->viewConfig().loopSpecialAnimation;

	if (reverseAttackAnimation_ && CCSpriteFrameCache::sharedSpriteFrameCache()->
			spriteFrameByName(frameNameForUnit(unit_, "special_back", 0).c_str()))
		return animation("special_back", 1.2, reverse);
	else
		return animation("special", 0.7, reverse);
}

CCAnimation* UnitNode::chargeParticleAnimation() const
{
	float interval = 1.0;
	CCArray* frames = CCArray::createWithCapacity(4);
	
	int i = 0;
	while (CCSpriteFrame* sf = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(createWithFormat("unit_charge_particle_%d.png", i).c_str()))
	{
		CCAnimationFrame* af = new CCAnimationFrame();
		af->initWithSpriteFrame(sf, 0.1, 0);
		frames->addObject(af);
		af->autorelease();
		i++;
	}
	
	return CCAnimation::create(frames, interval + ((float)rand() / (float)RAND_MAX) * 0.5, 1);
}

void UnitNode::setState(UnitNodeState state, bool animated)
{
	if (state_ != state)
	{
		if (formationMarkerSprite_)
		{
			formationMarkerSprite_->removeFromParentAndCleanup(true);
			formationMarkerSprite_ = 0;
		}
		
		if (outRoundsToCharge_)
		{
			outRoundsToCharge_->removeFromParentAndCleanup(true);
			outRoundsToCharge_ = 0;
		}
		
		if (outHealthLeft_)
		{
			outHealthLeft_->removeFromParentAndCleanup(true);
			outHealthLeft_ = 0;
		}
		
		if (state == UnitNodeState_WallLevel0 || state == UnitNodeState_WallLevel1)
		{
			SoundManager::sharedSoundManager()->playEffectOneAtTime(CCString::createWithFormat("avatar_%d_wall",
				unit_->unitType()->avatar()->index())->getCString(), 1);
			
			formationMarkerSprite_ = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("avatar_%d_wall_%d.png",
				unit_->unitType()->avatar()->index(),
				(state - UnitNodeState_WallLevel0))->getCString());
			
			shadow_->setVisible(false);
			sprite_->setVisible(false);
		}
		else if (state == UnitNodeState_AttackFormation)
		{
//			switch (unit_->unitClass())
//			{
//				case UnitClass_Normal:
//					formationMarkerSprite_ = CCSprite::createWithSpriteFrameName("attack_marker_normal.png");
//				break;
//				
//				case UnitClass_Large2:
//					formationMarkerSprite_ = CCSprite::createWithSpriteFrameName("attack_marker_large2.png");
//				break;
//				
//				case UnitClass_Large4:
//					formationMarkerSprite_ = CCSprite::createWithSpriteFrameName("attack_marker_large4.png");
//				break;
//			}
			
			shadow_->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
				shadowSpriteFrameName(true).c_str()));
			shadow_->setOpacity(200);
			shadow_->runAction(CCRepeatForever::create(CCSequence::create(
				CCFadeTo::create(0.8, 128),
				CCFadeTo::create(0.8, 200),
				0
			)));
			
			updateMarkers();
		}
		
		if (state != UnitNodeState_AttackFormation)
		{
			shadow_->stopAllActions();
			shadow_->setOpacity(255);
			shadow_->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
				shadowSpriteFrameName(false).c_str()));
		}
		
		if (formationMarkerSprite_)
		{
			addChild(formationMarkerSprite_);
		}
		
		if (animated && formationMarkerSprite_)
		{
			formationMarkerSprite_->runAction(CCFadeIn::create(0.2));
		}
		
		state_ = state;
	}
}

void UnitNode::updateMarkers()
{
	if (unit_->attackFormation() && !unit_->isAbsorbed() &&
		unit_->indexInAttackFormation() == 0)
	{
		if (!outHealthLeft_)
		{
			outHealthLeft_ = BarNode::barNodeWithSprites(
				CCSprite::createWithSpriteFrameName("bg_unit_health_empty.png"),
				CCSprite::createWithSpriteFrameName("bg_unit_health_filled.png"), 210);
			outHealthLeft_->setAnchorPoint(CCPoint(0.5, 0.5));
			addChild(outHealthLeft_, 19);
			
			SoundManager::sharedSoundManager()->playEffect(CCString::createWithFormat(
				"avatar_%d_formation_%s",
				unit_->unitType()->avatar()->index(),
				unit_->unitType()->isLarge() ? "large" : "normal")->getCString(), false);
		}
		
		if (!outRoundsToCharge_)
		{
			outRoundsToCharge_ = CCLabelTTF::create("", "BadaBoom BB", ct(18));
			if (outHealthLeft_)
				outRoundsToCharge_->setPosition(CCPoint(outHealthLeft_->getPosition().x, outHealthLeft_->getPosition().y));
			outRoundsToCharge_->setAnchorPoint(CCPoint(0.5, 0.5));
			addChild(outRoundsToCharge_, 20);
		}
		
		outRoundsToCharge_->setString(CCString::createWithFormat(" %d ", unit_->attackFormation()->roundsToCharge())->getCString());
		outRoundsToCharge_->setColor(ccWHITE);
		outRoundsToCharge_->setVisible(unit_->attackFormation()->roundsToCharge() > 0);
		
		if (outHealthLeft_)
		{
			outHealthLeft_->setValue((unit_->attackFormation()->strength() /
				unit_->attackFormation()->totalStrength()).toFloat());
			outHealthLeft_->setVisible(unit_->attackFormation()->roundsToCharge() > 0);
		}
	}
	
	
	if (!chargeAnimationStarted_ && unit_->attackFormation() && !unit_->isAbsorbed())
	{
		chargeAnimationStarted_ = true;
		
		if (unit_->unitType()->viewConfig().shadowType == UnitShadowType_Small)
		{
			for (int i = 0; i < 4; i++)
			{
				CCSprite* particle = createChargeParticle();
				
				particle->setPosition(CCPoint(shadow_->getPosition().x - shadow_->getContentSize().width * ((float) i / 3) + shadow_->getContentSize().width * 0.5,
					shadow_->getPosition().y - shadow_->getContentSize().height * ( (i == 0 || i == 3) ? 0 : 0.6 ) ));
				
				particle->setScale((unit_->unitType()->isLarge()) ? 1.0 : 0.66);
				
				addChild(particle);
			}
		}
		else if (unit_->unitType()->viewConfig().shadowType == UnitShadowType_Alligator)
		{
			for (int i = 0; i < 4; i++)
			{
				CCSprite* particle = createChargeParticle();
				
				particle->setPosition(CCPoint(shadow_->getPosition().x - shadow_->getContentSize().width * 0.5
					 + shadow_->getContentSize().width * ((i == 1) ? 0.2 : 0 )
					 + shadow_->getContentSize().width * ((i == 2) ? 0.8 : 0 )
					 + shadow_->getContentSize().width * ((i == 3) ? 1 : 0 ),
					shadow_->getPosition().y - shadow_->getContentSize().height * ( (i == 0 || i == 3) ? 0 : 0.5 ) ));
				
				addChild(particle);
			}
		}
		else if (unit_->unitType()->viewConfig().shadowType == UnitShadowType_Large)
		{
			for (int i = 0; i < 6; i++)
			{
				CCSprite* particle = createChargeParticle();
				
				particle->setPosition(CCPoint(shadow_->getPosition().x - shadow_->getContentSize().width * 0.5
					 + shadow_->getContentSize().width * ((i == 1) ? 0.2 : 0 )
					 + shadow_->getContentSize().width * ((i == 2) ? 0.4 : 0 )
					 + shadow_->getContentSize().width * ((i == 3) ? 0.6 : 0 )
					 + shadow_->getContentSize().width * ((i == 4) ? 0.8 : 0 )
					 + shadow_->getContentSize().width * ((i == 5) ? 1 : 0 ),
					shadow_->getPosition().y
						- shadow_->getContentSize().height * ( (i == 1) ? 0.35 : 0 )
						- shadow_->getContentSize().height * ( (i == 2) ? 0.45 : 0 )
						- shadow_->getContentSize().height * ( (i == 3) ? 0.45 : 0 )
						- shadow_->getContentSize().height * ( (i == 4) ? 0.35 : 0 )
					));
				
				addChild(particle);
			}
		}
	}
}

CCSprite* UnitNode::createChargeParticle()
{
	CCSprite* particle = CCSprite::createWithSpriteFrameName("unit_charge_particle_0.png");
	particle->setAnchorPoint(CCPoint(0.5, 0));
	
	CCAction* a = CCRepeatForever::create(CCSequence::create(
		CCDelayTime::create(((float)rand() / (float)RAND_MAX) * 0.1),
		CCShow::create(),
		CCAnimate::create(chargeParticleAnimation()),
		CCHide::create(),
		CCDelayTime::create(0.1),
		0)
	);
	a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	particle->runAction(a);
	
	return particle;
}

std::string UnitNode::shadowSpriteFrameName(bool highlighted) const
{
	return CCString::createWithFormat((unit_->unitType()->viewConfig().shadowType == UnitShadowType_Small) ? "shadow_%ssmall.png" :
		((unit_->unitType()->viewConfig().shadowType == UnitShadowType_Large) ?
			"shadow_%slarge.png" : "shadow_%salligator.png"), highlighted ? "highlighted_" : "")->getCString();
}

void UnitNode::setUnit(const Unit *unit)
{
	unit_ = unit;
	
	removeAllChildrenWithCleanup(true);
	
//	sprite_ = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("avatar_%d_%s_%d.png",
//		unit->unitType()->avatar()->index(),
//		unit->unitType()->isLarge() ? "large" : "normal",
//		unit->unitType()->index())->getCString());
	//sprite_->setOpacity(100);
	
	sprite_ = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("avatar_%d_%s_%d_%s_idle_0.png",
		unit->unitType()->avatar()->index(),
		unit->unitType()->isLarge() ? "large" : "normal",
		unit->unitType()->index(),
		unit->color() == UnitColor_Red ? "red" : (unit->color() == UnitColor_Green ? "green" : "blue"))->getCString());
	sprite_->setPosition(CCPoint(round(unit->unitType()->viewConfig().offset.x * sprite_->getContentSize().width),
		round(unit->unitType()->viewConfig().offset.y * sprite_->getContentSize().height)));
	
	shadow_ = CCSprite::createWithSpriteFrameName(shadowSpriteFrameName(false).c_str());
	shadow_->setPosition(CCPoint(unit->unitType()->viewConfig().offset.x * sprite_->getContentSize().width + sprite_->getContentSize().width * unit->unitType()->viewConfig().shadowOffset.x,
		unit->unitType()->viewConfig().offset.y * sprite_->getContentSize().height + sprite_->getContentSize().width * unit->unitType()->viewConfig().shadowOffset.y));
	//shadow_->setVisible(false);
	
	#ifdef SHOW_POSITION_MARKER
	test_cross_ = CCSprite::createWithSpriteFrameName("test_cross.png");
	#endif
	
//	if (unit->unitType()->isLarge())
//	{
//		ccColor3B color;
//		
//		switch (unit->color())
//		{
//			case UnitColor_Red:
//				color = ccc3(247, 61, 65);
//			break;
//			
//			case UnitColor_Green:
//				color = ccc3(10, 183, 43);
//			break;
//			
//			case UnitColor_Blue:
//				color = ccc3(7, 13, 233);
//			break;
//		}
//		
//		sprite_->setColor(color);
//	}
	
	addChild(shadow_);
	if (test_cross_) addChild(test_cross_);
	addChild(sprite_);

	
	setState(stateForUnit(unit_), false);
	setAnimation(UnitNodeAnimation_Idle);
}

void UnitNode::setAnimation(UnitNodeAnimation animation)
{
	if (animation != animation_)
	{
		animation_ = animation;
		sprite_->stopActionByTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
		
		switch(animation)
		{
			case UnitNodeAnimation_None:
			break;
		
			case UnitNodeAnimation_Idle:
			{
				CCAction* a = CCRepeatForever::create(CCSequence::createWithTwoActions(
					CCDelayTime::create(((float)rand() / (float)RAND_MAX) * 0.2),
					CCAnimate::create(idleAnimation()))
				);
				a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
				sprite_->runAction(a);
			}
			break;
			
			case UnitNodeAnimation_Walk:
			{
				CCAction* a = CCRepeatForever::create(CCSequence::createWithTwoActions(
					CCDelayTime::create(((float)rand() / (float)RAND_MAX) * 0.05),
					CCAnimate::create(walkAnimation()))
				);
				a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
				sprite_->runAction(a);
			}
			break;
			
			case UnitNodeAnimation_Attack:
			{
				CCAction* a = CCRepeatForever::create(CCSequence::createWithTwoActions(
					CCDelayTime::create(((float)rand() / (float)RAND_MAX) * 0.05),
					CCAnimate::create(attackAnimation()))
				);
				a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
				sprite_->runAction(a);
				//playAttackSound();
			}
			break;
			
			case UnitNodeAnimation_Special:
			break;
		}
	}
}

void UnitNode::playSound(UnitNodeSound sound, float time)
{
	//do not play attack sound again
	if (lastActiveSoundId_ && UnitNodeSound_Attack)
		return;
	
	stopSound();
	
	if (unit_->indexInAttackFormation() == 0)
	{
		switch(sound)
		{
			case UnitNodeSound_None:
			break;
			
			case UnitNodeSound_Attack:
			{
				lastActiveSoundId_ = SoundManager::sharedSoundManager()->playEffect(CCString::createWithFormat(
				"avatar_%d_%s_%d_attack",
				unit_->unitType()->avatar()->index(),
				unit_->unitType()->isLarge() ? "large" : "normal",
				//unit_->unitType()->index())->getCString(), !unit_->unitType()->isLarge());
				unit_->unitType()->index())->getCString(), true);
			}
			break;
			
			case UnitNodeSound_Special:
			{
				lastActiveSoundId_ = SoundManager::sharedSoundManager()->playEffect(CCString::createWithFormat(
				"avatar_%d_%s_%d_special",
				unit_->unitType()->avatar()->index(),
				unit_->unitType()->isLarge() ? "large" : "normal",
				unit_->unitType()->index())->getCString(), !unit_->unitType()->isLarge());
			}
			break;
		}
		
		if (sound != UnitNodeSound_None)
		{
			this->runAction(CCSequence::create(CCDelayTime::create(time),
				CCCallFunc::create(this, callfunc_selector(UnitNode::stopSound)),
				0));
		}
	}
}

void UnitNode::stopSound()
{
	if (lastActiveSoundId_)
	{
		SoundManager::sharedSoundManager()->stopEffect(lastActiveSoundId_);
		lastActiveSoundId_ = 0;
	}
}

UnitNodeState UnitNode::stateForUnit(const Unit* unit)
{
	if (unit->isWall())
		return (UnitNodeState)(unit->wallLevel() + UnitNodeState_WallLevel0);
	else if (unit->attackFormation())
		return UnitNodeState_AttackFormation;
	return UnitNodeState_Normal;
}


void UnitNode::setOpacity(GLubyte opacity)
{
	sprite_->setOpacity(opacity);
	shadow_->setOpacity(opacity);
	if (test_cross_) test_cross_->setOpacity(opacity);
}

void UnitNode::setColor(const cocos2d::ccColor3B& color)
{
	sprite_->setColor(color);
	shadow_->setColor(color);
	if (test_cross_) test_cross_->setColor(color);
}

void UnitNode::showShadow()
{
	shadow_->setVisible(true);
	if (test_cross_) test_cross_->setVisible(true);
}

void UnitNode::hideShadow()
{
	shadow_->setVisible(false);
	if (test_cross_) test_cross_->setVisible(false);
}

void UnitNode::shrinkShadowAnimated(float time)
{
	shadow_->runAction(CCScaleTo::create(time, 0.1));
}

void UnitNode::runSingleAttackAnimation()
{
	sprite_->stopActionByTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	
	CCAction* a = CCAnimate::create(attackAnimation());
	a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	sprite_->runAction(a);
}

void UnitNode::afterSingleSpecialAnimation()
{
	setAnimation(UnitNodeAnimation_Idle);
}

void UnitNode::runSingleSpecialAnimation(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	sprite_->stopActionByTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	animation_ = UnitNodeAnimation_Special;
	
	CCAction* a = CCSequence::create(
		CCAnimate::create(unit_->unitType()->viewConfig().specialAnimationIsAttackAnimation ?
			attackAnimation() : specialAnimation()),
		CCCallFunc::create(this, callfunc_selector(UnitNode::afterSingleSpecialAnimation)),
		target ? CCCallFunc::create(target, selector) : 0,
		0
	);
	a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	sprite_->runAction(a);
}

void UnitNode::runSingleSpecialAnimation()
{
	runSingleSpecialAnimation(0, 0);
}

void UnitNode::runTemporaryKilledAnimation(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	sprite_->stopActionByTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	
	if (!spriteTemporaryKilled_)
	{
		spriteTemporaryKilled_ = CCSprite::createWithSpriteFrameName(frameNameForUnit(unit_, "special", 0).c_str());
		spriteTemporaryKilled_->setPosition(sprite_->getPosition());
		spriteTemporaryKilled_->setAnchorPoint(sprite_->getAnchorPoint());
		
		addChild(spriteTemporaryKilled_, sprite_->getZOrder());
	}
	
	outHealthLeft_->setVisible(false);
	outRoundsToCharge_->setVisible(false);
	
	spriteTemporaryKilled_->runAction(CCFadeIn::create(0.1));
	
	CCAction* a = CCSequence::create(
		CCFadeOut::create(0.2),
		CCHide::create(),
		CCCallFunc::create(target, selector),
		0
	);
	a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	sprite_->runAction(a);
}

void UnitNode::afterRegenerated()
{
	setAnimation(UnitNodeAnimation_Idle);
}

void UnitNode::runRegeneratedAnimation(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	sprite_->stopActionByTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	animation_ = UnitNodeAnimation_None;
		
	spriteTemporaryKilled_->runAction(CCSequence::create(
		CCFadeOut::create(0.1),
		CCHide::create(),
		0
	));
	
	outHealthLeft_->setVisible(true);
	outRoundsToCharge_->setVisible(true);
	
	CCAction* a = CCSequence::create(
		CCShow::create(),
		CCFadeIn::create(0.2),
		CCCallFunc::create(this, callfunc_selector(UnitNode::afterRegenerated)),
		CCCallFunc::create(target, selector),
		0
	);
	a->setTag(MONSTERS_UNIT_NODE_ANIMATION_TAG);
	sprite_->runAction(a);
}
