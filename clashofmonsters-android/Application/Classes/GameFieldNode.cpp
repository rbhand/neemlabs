//
//  GameFieldNode.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "GameFieldNode.h"
#include "Utils.h"
#include "CCRemoveFromParentAction.h"

#define kGameFieldNodeIntervalVertical     cocos2d_utils::ct(12)
#define kGameFieldNodePaddingVertical      cocos2d_utils::ct(24)

#define kUnitNodeMovementSpeedFaster        (250.0)
#define kUnitNodeMovementSpeedSlower        (150.0)
//#define kUnitNodeMovementSpeed         (40.0)

using namespace monsters;
using namespace cocos2d;
using namespace cocos2d_utils;
using namespace std;

float movementAnimationTime(CCPoint p1, CCPoint p2, float speed)
{
	return ccpDistance(p1, p2) / ct(speed * 2.0 + (float)rand() / (float)RAND_MAX * 20.0);
}


bool GameFieldNode::init()
{
	delegate_ = 0;
	
	flipPlayers_ = false;
	
	targetAfterUpdateFormations_ = 0;
	
	bgCanMove_ = CCSprite::createWithSpriteFrameName("bg_can_move_unit.png");
	bgCanMove_->setVisible(false);
	addChild(bgCanMove_, 1);
	
	bgCannotMove_ = CCSprite::createWithSpriteFrameName("bg_cannot_move_unit.png");
	bgCannotMove_->setVisible(false);
	addChild(bgCannotMove_, 1);
	
	return true;
}

void GameFieldNode::showMoveIndicator(bool canMove, int col, int player, const Unit* unit)
{
	hideMoveIndicator();
	
	CCSprite* node = canMove ? bgCanMove_ : bgCannotMove_;
	
	if (normalizedPlayer(player) == 0)
	{
		node->setAnchorPoint(ccp(0.5, 1));
		node->setPosition(ccp(padX_ + cellSize_.width * (col + 0.5 * (float)unit->width()),
			getContentSize().height * 0.5));
	}
	else
	{
		node->setAnchorPoint(ccp(0.5, 0));
		node->setPosition(ccp(padX_ + cellSize_.width * (col + 0.5 * (float)unit->width()),
			getContentSize().height * 0.5));
	}
	
//	node->setPosition(CCPoint(padX_ + cellSize_.width * (col + 0.5 * (float)unit->width()), padY_ +
//		(cellSize_.height * kFieldHeight) + (player == 1 ?
//			(kGameFieldNodeIntervalVertical + cellSize_.height * (kFieldHeight / 2)) :
//			(-cellSize_.height * (kFieldHeight / 2)))));
	
	node->setScaleX(unit->width());
	
	node->setVisible(true);
}

void GameFieldNode::hideMoveIndicator()
{
	bgCanMove_->setVisible(false);
	bgCannotMove_->setVisible(false);
}

void GameFieldNode::clear()
{
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		i->second->removeFromParentAndCleanup(true);
	}
	
	processing_ = false;
	processingLeft_ = 0;
	unitNodes_.clear();
}

void GameFieldNode::setContentSize(const cocos2d::CCSize size)
{
	CCNode::setContentSize(size);
	
	int y_offset = 0;
	if (cocos2d_utils::isIPhone())
	{
		y_offset = (getContentSize().height - 480);
	}
	
	float cell_h = (getContentSize().height - y_offset - kGameFieldNodePaddingVertical * 2 - kGameFieldNodeIntervalVertical) /
		(kFieldHeight * 2);
	cellSize_ = CCSizeMake(cell_h, cell_h);
	padY_ = (getContentSize().height -
		(cellSize_.height * kFieldHeight * 2.0 + kGameFieldNodeIntervalVertical)) * 0.5;
	padX_ = (getContentSize().width - (cellSize_.width * kFieldWidth)) * 0.5;
}

int GameFieldNode::normalizedPlayer(int player) const
{
	return flipPlayers_ ? ((player + 1) % 2) : player;
}

cocos2d::CCPoint GameFieldNode::positionForCoordinate(int player, const CellCoordinate& c, UnitClass uc) const
{
	CCPoint pos(padX_ + cellSize_.width * (c.col + 0.5), padY_ +
		(cellSize_.height * kFieldHeight) + (normalizedPlayer(player) == 1 ?
			(kGameFieldNodeIntervalVertical + cellSize_.height * (c.row + 0.5)) :
			(-cellSize_.height * (c.row + 0.5))));
	
	if (uc == UnitClass_Large2 || uc == UnitClass_Large4)
	{
		pos.y += (normalizedPlayer(player) == 1) ?
			(cellSize_.width * 0.5) :
			(-cellSize_.width * 0.5);
	}
	
	if (uc == UnitClass_Large4)
	{
		pos.x += cellSize_.width * 0.5;
	}
	
	return CCPoint(floorf(pos.x), floorf(pos.y));
}

//
int GameFieldNode::zOrderForCoordinate(int player, const CellCoordinate& c) const
{
	int z = 0;
	if (normalizedPlayer(player) == 0)
		z = /*c.col + */c.row * kFieldWidth + (kFieldWidth * kFieldHeight) + 10;
	else
		z = (/*c.col + */(kFieldHeight - c.row - 1) * kFieldWidth) + 10;
			
	//cout << "p: " << player << "; row: " << c.row << "; col: " << c.col << " = " << z << endl;
	
	return z;
}

int GameFieldNode::zOrderForUnit(const Unit* unit) const
{
	if (unit->unitType()->isLarge())
	{
		if (unit->unitType()->viewConfig().shadowType == UnitShadowType_Small)
		{
			if (normalizedPlayer(unit->player()) == 0)
				return zOrderForCoordinate(unit->player(), CellCoordinate(unit->coord().col, unit->coord().row+1));
			else
				return zOrderForCoordinate(unit->player(), unit->coord());
		}
		else
		{
			if (normalizedPlayer(unit->player()) == 1)
				return zOrderForCoordinate(unit->player(), CellCoordinate(unit->coord().col, unit->coord().row+1));
			else
				return zOrderForCoordinate(unit->player(), unit->coord());
		}
	}
	else
	{
		return zOrderForCoordinate(unit->player(), unit->coord());
	}
}

void GameFieldNode::afterUpdateFormations()
{
	updateMarkers();

	if (targetAfterUpdateFormations_)
	{
		(targetAfterUpdateFormations_->*selectorAfterUpdateFormations_)();
	}
	else
	{
		processing_ = false;
		delegate_->processingDidFinish();
	}
}

void GameFieldNode::eraseUnitNode(CCNode* node)
{
	UnitNode* un = dynamic_cast<UnitNode*>(node);
	CC_ASSERT(un);
	unitNodes_.erase(un->unit());
}

void GameFieldNode::afterUpdateFormationsForUnit(CCNode* node)
{
	UnitNode* unit_node = dynamic_cast<UnitNode*>(node);
	unit_node->setAnimation(UnitNodeAnimation_Idle);
}

void GameFieldNode::updateFormationsAnimated(CCObject* target, cocos2d::SEL_CallFunc selector)
{
	targetAfterUpdateFormations_ = target;
	selectorAfterUpdateFormations_ = selector;
	
	addMissingUnits();

	float max_time = 0.2;
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		if (const Unit* absorber = i->first->absorber())
		{
			i->second->hideShadow();
			i->second->runAction(CCSequence::create(
				CCSpawn::create(
					CCMoveTo::create(0.3, positionForUnit(absorber)),
					CCFadeOut::create(0.3),
					0
				),
				CCCallFuncN::create(this, callfuncN_selector(GameFieldNode::eraseUnitNode)),
				CCRemoveFromParentAction::create(),
				0
			));
			if (max_time < 0.3) max_time = 0.3;
		}
		else
		{
			CCPoint np = positionForCoordinate(i->second->unit()->player(), i->second->unit()->coord(),
				i->second->unit()->unitClass());
				
			if (i->second->getPosition().x != np.x || i->second->getPosition().y != np.y)
			{
				reorderChild(i->second, zOrderForUnit(i->second->unit()));
				
				i->second->setAnimation(UnitNodeAnimation_Walk);
				
				float t;
				i->second->runAction(CCSequence::create(
					CCMoveTo::create(t = (ccpDistance(i->second->getPosition(), np) / ct(kUnitNodeMovementSpeedFaster)), np),
					CCCallFuncN::create(this, callfuncN_selector(GameFieldNode::afterUpdateFormationsForUnit)),
					0
				));
				
				if (t > max_time)
					max_time = t;
			}
		}
		
		if (UnitNode::stateForUnit(i->second->unit()) != i->second->state())
		{
			i->second->setState(UnitNode::stateForUnit(i->second->unit()), true);
		}
	}
	
	runAction(CCSequence::create(
		CCDelayTime::create(max_time + 0.1),
		CCCallFunc::create(this, callfunc_selector(GameFieldNode::afterUpdateFormations)),
		0
	));
}

void GameFieldNode::afterRemoveAndAlign()
{
	updateFormationsAnimated(0, 0);
}

void GameFieldNode::afterRemoveAndAlignForUnit(CCNode* node)
{
	UnitNode* unit_node = dynamic_cast<UnitNode*>(node);
	unit_node->setAnimation(UnitNodeAnimation_Idle);
}

void GameFieldNode::removeUnitAndAlign(const Unit* unit)
{
	processing_ = true;
	delegate_->processingDidStart();

	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		if (i->second->unit() == unit)
		{
			i->second->runAction(CCSequence::create(
				CCFadeOut::create(0.1),
				CCRemoveFromParentAction::create(),
				0
			));
			
			unitNodes_.erase(i);
			
			break;
		}
	}
	
	float max_time = 0.1;
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		CCPoint np = positionForCoordinate(i->second->unit()->player(), i->second->unit()->coord(),
			i->second->unit()->unitClass());
		if (i->second->getPosition().x != np.x || i->second->getPosition().y != np.y)
		{
			reorderChild(i->second, zOrderForUnit(i->second->unit()));
		
			i->second->setAnimation(UnitNodeAnimation_Walk);
		
			float t;
			i->second->runAction(CCSequence::create(
				CCMoveTo::create(t = (ccpDistance(i->second->getPosition(), np) / ct(kUnitNodeMovementSpeedFaster)), np),
				CCCallFuncN::create(this, callfuncN_selector(GameFieldNode::afterRemoveAndAlignForUnit)),
				0
			));
			
			if (t > max_time)
				max_time = t;
		}
	}
	
	runAction(CCSequence::create(
		CCDelayTime::create(max_time + 0.1),
		CCCallFunc::create(this, callfunc_selector(GameFieldNode::afterRemoveAndAlign)),
		0
	));
}

void GameFieldNode::updateMarkers()
{
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		i->second->updateMarkers();
	}
}

void GameFieldNode::afterMove(CCNode* node)
{
	UnitNode* unit_node = dynamic_cast<UnitNode*>(node);
	unit_node->setAnimation(UnitNodeAnimation_Idle);

	updateFormationsAnimated(0, 0);
}

void GameFieldNode::moveUnitAndRearrange(const Unit* unit, const CellCoordinate& newCoord)
{
	processing_ = true;
	delegate_->processingDidStart();
	
	map<const Unit*, UnitNode*>::iterator it = unitNodes_.find(unit);
	CC_ASSERT(it != unitNodes_.end());
	
	UnitNode* n = it->second;
	
	reorderChild(it->second, zOrderForUnit(it->second->unit()));
	
	if (n->getParent() == this)
	{
		// moved not by dragging
		
		CCPoint np1 = n->getPosition();
		np1.y = positionForCoordinate(unit->player(), CellCoordinate(0, 0), unit->unitClass()).y +
			getContentSize().height * 0.5 * (normalizedPlayer(unit->player()) == 0 ? -1 : 1);
		
		CCPoint np2 = np1;
		np2.x = positionForCoordinate(unit->player(), newCoord, unit->unitClass()).x;
		
		CCPoint np3 = positionForCoordinate(unit->player(), newCoord, unit->unitClass());
		
		n->setAnimation(UnitNodeAnimation_Walk);
		n->runAction(CCSequence::create(
			CCMoveTo::create(movementAnimationTime(n->getPosition(), np1, kUnitNodeMovementSpeedSlower), np1),
			CCMoveTo::create(0.05, np2),
			CCMoveTo::create(movementAnimationTime(np2, np3, kUnitNodeMovementSpeedSlower), np3),
			CCCallFuncN::create(this, callfuncN_selector(GameFieldNode::afterMove)),
			0
		));
	}
	else
	{
		n->retain();
		n->removeFromParent();
		addChild(n);
		n->autorelease();
		
		n->showShadow();
		
		CCPoint np = positionForCoordinate(unit->player(), newCoord, unit->unitClass());
		n->setAnimation(UnitNodeAnimation_Walk);
		n->setPosition(ccpAdd(positionForCoordinate(unit->player(), CellCoordinate(newCoord.col, 0), unit->unitClass()),
			ccp(0, getContentSize().height * 0.5 * (normalizedPlayer(unit->player()) == 0 ? -1 : 1))));
		n->runAction(CCSequence::create(
			CCMoveTo::create(movementAnimationTime(np, n->getPosition(), kUnitNodeMovementSpeedFaster), np),
			CCCallFuncN::create(this, callfuncN_selector(GameFieldNode::afterMove)),
			0
		));
	}
	
//	CCPoint np = positionForCoordinate(unit->player(), newCoord, unit->unitClass());
//	n->runAction(CCSequence::create(
//		CCMoveTo::create(0.1, np),
//		0
//	));
//	
//	runAction(CCSequence::create(
//		CCDelayTime::create(0.1),
//		CCCallFunc::create(this, callfunc_selector(GameFieldNode::afterMove)),
//		0
//	));
}

cocos2d::CCPoint GameFieldNode::positionForPlayer(int player, int col) const
{
	CCPoint np = positionForCoordinate(player, CellCoordinate(col, 0));
	np.y = normalizedPlayer(player) == 0 ? 0 : getContentSize().height;
	return np;
}

void GameFieldNode::advanceFormation(const AttackFormation* formation, const AdvanceFormationTarget& moveTo,
	cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	CellCoordinate first_coord = formation->units().front()->coord();
	
	float max_time = 0;
	
	for (vector<Unit*>::const_iterator ui=formation->units().begin(),
		ue = formation->units().end(); ui!=ue; ++ui)
	{
		map<const Unit*, UnitNode*>::iterator it = unitNodes_.find(*ui);
		CC_ASSERT(it != unitNodes_.end());
		UnitNode* node = it->second;
		
//		reorderChild(it->second, zOrderForCoordinate(it->second->unit()->player(),
//			it->second->unit()->coord()));

		reorderChild(it->second, 1000);

		int actual_row = moveTo.coord.row - 1;
		int actual_player = actual_row < 0 ? ((moveTo.player + 1) % 2) : moveTo.player;
		int actual_row_normalized = (actual_row < 0) ? (abs(actual_row) - 1)  : actual_row;
		CCPoint np = positionForCoordinate(actual_player,
			CellCoordinate(node->unit()->coord().col,//moveTo.coord.col - (first_coord.col - node->unit()->coord().col),
				actual_row_normalized +
				(actual_row >= 0 ?
					(first_coord.row - node->unit()->coord().row) :
					(node->unit()->coord().row - first_coord.row))),
			node->unit()->unitClass());
	
		if (moveTo.type == AdvanceFormationTarget::Type_Player)
		{
			np.y = -cellSize_.height * (first_coord.row - node->unit()->coord().row +
				(node->unit()->depth() - 1.0) / 2.0 );
			
			if (normalizedPlayer(moveTo.player) == 1)
				np.y = getContentSize().height - np.y;
		}
								
		if (node->getPosition().x != np.x || node->getPosition().y != np.y)
		{
			float t;
			node->setAnimation(UnitNodeAnimation_Attack);
			
			if (node->unit()->unitType()->viewConfig().hideShadowWhenAttack)
				node->hideShadow();
			
//			if (node->unit()->unitType()->viewConfig().shootingAttack)
//			{
//				/*node->runAction(CCSequence::create(
//					CCDelayTime::create(t = (ccpDistance(node->getPosition(), np) / ct(kUnitNodeMovementSpeed))),
//					CCFadeIn::create(0.2),
//					0
//				));*/
//			}
//			else
//			{
				node->runAction(CCSequence::create(
					CCMoveTo::create(t = (ccpDistance(node->getPosition(), np) / ct(kUnitNodeMovementSpeedSlower)),
						np),
						0
				));
//			}

			node->playSound(UnitNodeSound_Attack);

			if (t > max_time)
				max_time = t;
		}
	}
	
	runAction(CCSequence::create(
		CCDelayTime::create(max_time),
		CCCallFunc::create(target, selector),
		0
	));
}

void GameFieldNode::killFormation(const AttackFormation* formation,
	cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	float max_time = 0.2;
	for (vector<Unit*>::const_iterator ui=formation->units().begin(),
		ue = formation->units().end(); ui!=ue; ++ui)
	{
		map<const Unit*, UnitNode*>::iterator it = unitNodes_.find(*ui);
		CC_ASSERT(it != unitNodes_.end());
		UnitNode* node = it->second;
		node->shrinkShadowAnimated(max_time);
		node->runAction(CCSequence::create(
			CCSpawn::create(
				CCFadeOut::create(max_time),
				CCScaleTo::create(max_time, 0.1),
				CCMoveBy::create(max_time, ccp(0, -cellSize_.width * 0.5)),
				0
			),
			CCRemoveFromParentAction::create(),
			0
		));
		unitNodes_.erase(it);
	}
	
	runAction(CCSequence::create(
		CCDelayTime::create(max_time),
		CCCallFunc::create(target, selector),
		0
	));
}

void GameFieldNode::killUnit(const Unit* unit,
	cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	map<const Unit*, UnitNode*>::iterator it = unitNodes_.find(unit);
	CC_ASSERT(it != unitNodes_.end());
	
	float time = 0.2;
	UnitNode* node = it->second;
	node->shrinkShadowAnimated(time);
	node->runAction(CCSequence::create(
		CCSpawn::create(
			CCFadeOut::create(time),
			CCScaleTo::create(time, 0.1),
			CCMoveBy::create(time, ccp(0, -cellSize_.width * 0.5)),
			0
		),
		CCRemoveFromParentAction::create(),
		CCCallFunc::create(target, selector),
		0
	));
	unitNodes_.erase(it);
}

void GameFieldNode::afterAlign()
{
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		i->second->setAnimation(UnitNodeAnimation_Idle);
	}

	updateFormationsAnimated(targetAfterUpdateFormations_, selectorAfterUpdateFormations_);
}

void GameFieldNode::addMissingUnits()
{
	// add missing units (required due to stupid duplicating units rule)
	for (int pi=0; pi<2; ++pi)
	{
		const Player& p = game_->player(pi);
		for (int c=0; c<kFieldWidth; ++c)
		{
			for (int r=0; r<kFieldHeight; ++r)
			{
				if (const Unit* unit = p.unit(c, r))
				{
					if (!nodeForUnit(unit))
					{
						UnitNode* node = addNodeForUnit(unit, false);
						node->runAction(CCFadeIn::create(0.2));
					}
				}
			}
		}
	}
}

void GameFieldNode::alignUnits(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	float max_time = 0;
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		CCPoint np = positionForCoordinate(i->second->unit()->player(), i->second->unit()->coord(),
			i->second->unit()->unitClass());
		if (i->second->getPosition().x != np.x || i->second->getPosition().y != np.y)
		{
			float t;
			
			reorderChild(i->second, zOrderForUnit(i->second->unit()));
			
			i->second->setAnimation(UnitNodeAnimation_Walk);
			i->second->runAction(CCSequence::create(
				CCMoveTo::create(t = (ccpDistance(i->second->getPosition(), np) / ct(kUnitNodeMovementSpeedFaster)),
					np),
				0
			));
			
			if (t > max_time)
				max_time = t;
		}
	}
	
	addMissingUnits();
	
	targetAfterUpdateFormations_ = target;
	selectorAfterUpdateFormations_ = selector;
	
	runAction(CCSequence::create(
		CCDelayTime::create(max_time + 0.01),
		CCCallFunc::create(this, callfunc_selector(GameFieldNode::afterAlign)),
		0
	));
}

void GameFieldNode::afterShootingAttack()
{
	if (processing_)
	{
		processing_ = false;
		delegate_->processingDidFinish();
	}
}

void GameFieldNode::addNamedFrameToArray(cocos2d::CCArray* frames, const char* frameName)
{
	CCAnimationFrame* af = new CCAnimationFrame();
	af->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
		spriteFrameByName(frameName), 0.2, 0);
	frames->addObject(af);
	af->autorelease();
}

void GameFieldNode::runFramedShootingAttack(AttackFormation* formation, RoundEventObject maxTarget,
	const ShootingAttackViewConfig& shootingAttackConfig, cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	if (!processing_)
	{
		processing_ = true;
		delegate_->processingDidStart();
	}
	
	Unit* base_unit = formation->units().at(0);
	CCSprite* particle = CCSprite::createWithSpriteFrameName(createWithFormat("%s_0.png",
		shootingAttackConfig.particleBaseName).c_str());
	float base_anchor_y = 0.8;
	particle->setAnchorPoint(ccp(0.5, base_anchor_y));
	addChild(particle, 1000);
	
	if (normalizedPlayer(base_unit->player()) == 0)
	{
		particle->setFlipY(true);
		particle->setAnchorPoint(ccp(particle->getAnchorPoint().x,
			1-particle->getAnchorPoint().y));
	}
	
	particle->setPosition(ccpAdd(positionForUnit(base_unit),
		ccp(shootingAttackConfig.particleStartOffset.x * nodeForUnit(base_unit)->getSpriteSize().width,
			shootingAttackConfig.particleStartOffset.y * nodeForUnit(base_unit)->getSpriteSize().height *
			(normalizedPlayer(base_unit->player()) == 0 ? 1 : -1))));
	particle->setOpacity(shootingAttackConfig.masterOpacity);
	
	float fire_distance = 0;
	
	if (maxTarget.type == RoundEventObjectType_Player)
	{
		if (normalizedPlayer(maxTarget.player) == 0)
			fire_distance = fabs(particle->getPositionY());
		else
			fire_distance = fabs(getContentSize().height - particle->getPositionY());
	}
	else if (maxTarget.type == RoundEventObjectType_Unit)
	{
		fire_distance = fabs(particle->getPositionY() - positionForUnit(maxTarget.unit).y);
	}
	else if (maxTarget.type == RoundEventObjectType_AttackFormation)
	{
		fire_distance = fabs(particle->getPositionY() - positionForUnit(maxTarget.attackFormation->units().at(0)).y);
	}
	
	particle->setScale(fire_distance / particle->getContentSize().height *
		shootingAttackConfig.scaleFactor);
	
	if (particle->getScaleX() < 0.5)
	{
		particle->setOpacity(180);
	}
	
	CCArray* frames = CCArray::createWithCapacity(8);
	
	for (int i=0; i<shootingAttackConfig.repeatCount; ++i)
	{
		for (int f=0; f<shootingAttackConfig.firstFramesToRepeat; ++f)
		{
			addNamedFrameToArray(frames, createWithFormat("%s_%d.png",
				shootingAttackConfig.particleBaseName, f).c_str());
		}
	}
	
	int i = 0;
	while (CCSpriteFrame* sf = CCSpriteFrameCache::sharedSpriteFrameCache()->
		spriteFrameByName(createWithFormat("%s_%d.png", shootingAttackConfig.particleBaseName, i).c_str()))
	{
		CCAnimationFrame* af = new CCAnimationFrame();
		af->initWithSpriteFrame(sf, 0.2, 0);
		frames->addObject(af);
		af->autorelease();
		++i;
	}
	
	particle->runAction(CCSequence::create(
		CCHide::create(),
		CCDelayTime::create(0.3),
		CCShow::create(),
		
		CCAnimate::create(CCAnimation::create(frames, shootingAttackConfig.secondsPerAnimationUnit, 1)),
		CCFadeOut::create(0.2),
		CCCallFunc::create(this, callfunc_selector(GameFieldNode::afterShootingAttack)),
		CCCallFunc::create(target, selector),
		CCRemoveFromParentAction::create(),
		0
	));
	
	
	if (shootingAttackConfig.sparkColor.red || shootingAttackConfig.sparkColor.green || shootingAttackConfig.sparkColor.blue)
	{
		CCArray* sparks_frames = CCArray::createWithCapacity(6);
		
		i = 0;
		while (CCSpriteFrame* sf = CCSpriteFrameCache::sharedSpriteFrameCache()->
				spriteFrameByName(createWithFormat("sparks_%d.png", i).c_str()))
		{
			CCAnimationFrame* af = new CCAnimationFrame();
			af->initWithSpriteFrame(sf, 0.2, 0);
			sparks_frames->addObject(af);
			af->autorelease();
			++i;
		}
		
		int sparks_number = (particle->getScaleX() < 0.5) ? 4 : 8;
		for (int i = 1; i < sparks_number + 1; ++i)
		{
			CCSprite* spark_particle = CCSprite::createWithSpriteFrameName(createWithFormat("sparks_0.png").c_str());
			spark_particle->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
			spark_particle->setColor(ccc3(shootingAttackConfig.sparkColor.red, shootingAttackConfig.sparkColor.green, shootingAttackConfig.sparkColor.blue));
			spark_particle->setScale((particle->getScale() < 0.5) ? 0.5 : (particle->getScale() < 1) ? 1 : 1.5);
			addChild(spark_particle, 1100);
			
			float offset_x = ((i % 2) ? 0.2 : -0.2) * (nodeForUnit(base_unit)->getSpriteSize().width + (nodeForUnit(base_unit)->getSpriteSize().width * 0.1 * i)) * ((particle->getScale() < 0.75) ? 1 : 1.25);
			float offset_y = fire_distance * (normalizedPlayer(base_unit->player()) == 0 ? 1 : -1) * i / sparks_number;
			
			spark_particle->setPosition(ccpAdd(positionForUnit(base_unit),
				ccp(shootingAttackConfig.particleStartOffset.x * nodeForUnit(base_unit)->getSpriteSize().width + offset_x,
					shootingAttackConfig.particleStartOffset.y * nodeForUnit(base_unit)->getSpriteSize().height *
					(normalizedPlayer(base_unit->player()) == 0 ? 1 : -1) + offset_y)));
			
			spark_particle->runAction(CCSequence::create(
				CCHide::create(),
				CCDelayTime::create(0.1),
				CCDelayTime::create(0.1 * i),
				CCShow::create(),
				
				CCFadeIn::create(0.2),
				CCAnimate::create(CCAnimation::create(sparks_frames, shootingAttackConfig.secondsPerAnimationUnit, 1)),
				CCFadeOut::create(0.2),
				CCRemoveFromParentAction::create(),
				0
			));
		}
	}
}

void GameFieldNode::runThrowShootingAttack(AttackFormation* formation, RoundEventObject maxTarget,
	const ShootingAttackViewConfig& shootingAttackConfig,
	cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
	if (!processing_)
	{
		processing_ = true;
		delegate_->processingDidStart();
	}
	
	Unit* base_unit = formation->units().at(0);
	CCSprite* particle = CCSprite::createWithSpriteFrameName(createWithFormat("%s_0.png",
		shootingAttackConfig.particleBaseName).c_str());
	float base_anchor_y = 0.8;
	particle->setAnchorPoint(ccp(0.5, base_anchor_y));
	addChild(particle, 1000);
	
	particle->setPosition(ccpAdd(positionForUnit(base_unit),
		ccp(shootingAttackConfig.particleStartOffset.x * nodeForUnit(base_unit)->getSpriteSize().width,
			shootingAttackConfig.particleStartOffset.y * nodeForUnit(base_unit)->getSpriteSize().height *
			(normalizedPlayer(base_unit->player()) == 0 ? 1 : -1))));
	particle->setOpacity(shootingAttackConfig.masterOpacity);
	
	if (normalizedPlayer(base_unit->player()) == 0)
	{
		particle->setFlipY(true);
		particle->setAnchorPoint(ccp(particle->getAnchorPoint().x,
			1-particle->getAnchorPoint().y));
	}
	
	float fire_distance = 0;
	float max_scale = 1.0;
	
	if (maxTarget.type == RoundEventObjectType_Player)
	{
		if (normalizedPlayer(maxTarget.player) == 0)
			fire_distance = fabs(particle->getPositionY());
		else
			fire_distance = fabs(getContentSize().height - particle->getPositionY());
		max_scale = 1.0;
	}
	else if (maxTarget.type == RoundEventObjectType_Unit)
	{
		fire_distance = fabs(particle->getPositionY() - positionForUnit(maxTarget.unit).y);
		max_scale = maxTarget.unit->coord().row * 0.1 + 0.2;
	}
	else if (maxTarget.type == RoundEventObjectType_AttackFormation)
	{
		fire_distance = fabs(particle->getPositionY() - positionForUnit(maxTarget.attackFormation->baseUnit()).y);
		max_scale = maxTarget.attackFormation->baseUnit()->coord().row * 0.1 + 0.2;
	}
	
	max_scale *= shootingAttackConfig.scaleFactor;
	
	particle->setScale(0.01);
	particle->runAction(CCSequence::create(
		CCHide::create(),
		CCDelayTime::create(0.3),
		CCShow::create(),
	
		CCSpawn::create(
			CCMoveBy::create(shootingAttackConfig.secondsPerAnimationUnit, ccp(0,
				normalizedPlayer(base_unit->player()) == 0 ? fire_distance : -fire_distance)),
			
			CCScaleTo::create(shootingAttackConfig.secondsPerAnimationUnit * 0.2, max_scale),
			
			CCSequence::create(
				CCFadeIn::create(0.05),
				CCDelayTime::create(shootingAttackConfig.secondsPerAnimationUnit-0.1),
				CCFadeOut::create(0.05),
				0
			),
			0
		),
		
		CCCallFunc::create(this, callfunc_selector(GameFieldNode::afterShootingAttack)),
		CCCallFunc::create(target, selector),
		CCRemoveFromParentAction::create(),
		0
	));
}

void GameFieldNode::runShootingAttack(AttackFormation* formation, RoundEventObject maxTarget, bool special,
	cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector)
{
//	ShootingAttackViewConfig cfg;
//	cfg.type = ShootingAttackViewConfig::Type_Throw;
//	cfg.firstFramesToRepeat = 2;
//	cfg.repeatCount = 0;
//	//strcpy(cfg.particleBaseName, "dragon_attack_particle");
//	strcpy(cfg.particleBaseName, "spider_attack_particle");
//	cfg.scaleFactor = 1.2;
//	cfg.secondsPerAnimationUnit = 0.4;
	
	const ShootingAttackViewConfig& cfg = !special ?
		formation->units().at(0)->unitType()->viewConfig().shootingAttackConfig :
		formation->units().at(0)->unitType()->viewConfig().shootingSpecialAttackConfig;
	
	if (!special)
	{
		nodeForUnit(formation->baseUnit())->runSingleAttackAnimation();
		nodeForUnit(formation->baseUnit())->stopSound();
		nodeForUnit(formation->baseUnit())->playSound(UnitNodeSound_Attack);
	}
	else
	{
		nodeForUnit(formation->baseUnit())->runSingleSpecialAnimation();
		nodeForUnit(formation->baseUnit())->stopSound();
		nodeForUnit(formation->baseUnit())->playSound(UnitNodeSound_Special);
	}
	
	if (cfg.type == ShootingAttackViewConfig::Type_Framed)
		runFramedShootingAttack(formation, maxTarget, cfg, target, selector);
	else if (cfg.type == ShootingAttackViewConfig::Type_Throw)
		runThrowShootingAttack(formation, maxTarget, cfg, target, selector);
	else
		CC_ASSERT(0);
}


cocos2d::CCPoint GameFieldNode::positionForCoordinate(int player, const CellCoordinate& c) const
{
	return positionForCoordinate(player, c, UnitClass_Normal);
}

cocos2d::CCPoint GameFieldNode::positionForUnit(const Unit* unit) const
{
	return positionForCoordinate(unit->player(), unit->coord(), unit->unitClass());
}

CellCoordinate GameFieldNode::cellCoordinateForPosition(const cocos2d::CCPoint& pos, int* out_player) const
{
	CellCoordinate c;
	int player = 0;
	
	c.col = (pos.x - padX_) / cellSize_.width;
	if (pos.x - padX_ < 0)
		c.col = -1;
	
	if ((pos.y - padY_) > (getContentSize().height - padY_ * 2) * 0.5)
	{
		player = 1;
		c.row = ((pos.y - padY_) - (getContentSize().height - padY_ * 2) * 0.5) / cellSize_.height;
	}
	else
	{
		c.row = ((getContentSize().height - padY_ * 2) * 0.5 - (pos.y - padY_)) / cellSize_.height;
	}
	
	if (out_player)
		*out_player = normalizedPlayer(player); //denormalized player
	
	return c;
}

UnitNode* GameFieldNode::unitNodeAtPosition(const cocos2d::CCPoint& p)
{
	int player;
	CellCoordinate c = cellCoordinateForPosition(p, &player);
	if (c.col < 0)
		c.col = 0;
	if (c.col >= kFieldWidth)
		c.col = kFieldWidth - 1;
	if (c.row < 0)
		c.row = 0;
	if (c.row >= kFieldHeight)
		c.row = kFieldHeight - 1;
	
	for (std::map<const Unit*,UnitNode*>::iterator i=unitNodes_.begin(), e=unitNodes_.end();
		i!=e; ++i)
	{
		if (i->second->unit()->containsCell(c) && i->second->unit()->player() == player)
			return i->second;
	}
	
	return 0;
}

UnitNode* GameFieldNode::nodeForUnit(const Unit* unit) const
{
	map<const Unit*, UnitNode*>::const_iterator it = unitNodes_.find(unit);
	if (it != unitNodes_.end())
		return it->second;
	
	return 0;
}

void GameFieldNode::afterAddNodeAnimation(CCNode* node)
{
	--processingLeft_;
	CC_ASSERT(processingLeft_ >= 0);
	
	if (processingLeft_ == 0)
	{
		processing_ = false;
		delegate_->processingDidFinish();
	}
	
	UnitNode* unit_node = dynamic_cast<UnitNode*>(node);
	unit_node->setAnimation(UnitNodeAnimation_Idle);
}

UnitNode* GameFieldNode::addNodeForUnit(const Unit* unit, bool animated)
{
	UnitNode* node = new UnitNode();
	node->init();
	node->autorelease();
	node->setUnit(unit);
	node->setPosition(positionForUnit(unit));
	node->setReverseAttackAnimation(normalizedPlayer(unit->player()) == 0);
	addChild(node, zOrderForUnit(node->unit()));
	
	//if (cocos2d_utils::isIPhone())
	node->setScale(1.0);
	
	unitNodes_.insert(pair<const Unit*, UnitNode*>(unit, node));
	
	if (animated)
	{
		CCPoint np = node->getPosition();
		node->setPosition(ccpAdd(np, ccp(0, getContentSize().height * 0.5 * (normalizedPlayer(unit->player()) == 0 ? -1 : 1))));
		node->setAnimation(UnitNodeAnimation_Walk);
		node->runAction(CCSequence::create(
			CCMoveTo::create(ccpDistance(np, node->getPosition()) / ct(kUnitNodeMovementSpeedFaster + (float)rand() / (float)RAND_MAX * 20.0), np),
			CCCallFuncN::create(this, callfuncN_selector(GameFieldNode::afterAddNodeAnimation)),
			0
		));
		
		if (!processing_)
		{
			processingLeft_ = 0;
			processing_ = true;
			delegate_->processingDidStart();
		}
		
		++processingLeft_;
	}
	
	return node;
}

void GameFieldNode::showHitChargingMarker(AttackFormation* formation)
{
	CCSprite* marker = CCSprite::createWithSpriteFrameName("marker_hit.png");
	marker->setPosition(positionForUnit(formation->baseUnit()));
	marker->runAction(CCSequence::create(
		CCSpawn::create(
			CCFadeIn::create(0.1),
			CCRotateBy::create(0.3, 360),
			0
		),
		CCRotateBy::create(0.2, 240),
		CCSpawn::create(
			CCFadeOut::create(0.1),
			CCRotateBy::create(0.1, 120),
			0
		),
		CCRemoveFromParentAction::create(),
		0
	));
	addChild(marker, 10000);
}

void GameFieldNode::showSlowMarker(AttackFormation* formation)
{
	CCSprite* marker = CCSprite::createWithSpriteFrameName("marker_slow.png");
	marker->setPosition(positionForUnit(formation->baseUnit()));
	marker->setScale(0.01);
	marker->runAction(CCSequence::create(
		CCSpawn::create(
			CCFadeIn::create(0.2),
			CCEaseBounceIn::create(CCScaleTo::create(0.6, 1.6)),
			0
		),
		CCFadeOut::create(0.8),
		CCRemoveFromParentAction::create(),
		0
	));
	addChild(marker, 10000);
}
