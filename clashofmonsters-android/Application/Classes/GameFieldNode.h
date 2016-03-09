//
//  GameFieldNode.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "UnitNode.h"

#include <map>

namespace monsters
{
	class IGameFieldNodeDelegate
	{
	public:
		virtual void processingDidStart() = 0;
		virtual void processingDidFinish() = 0;
	};
	
	struct AdvanceFormationTarget
	{
		enum Type
		{
			Type_CellCoordinate,
//			Type_Unit,
			Type_Player
		};
		
//		AdvanceFormationTarget(const CellCoordinate& aCoord, const Unit* aUnit) : type(Type_Unit),
//			player(aUnit->player()), coord(aUnit->coord()), unit(aUnit) { }
		AdvanceFormationTarget(const CellCoordinate& aCoord, int aPlayer) : type(Type_CellCoordinate),
			player(aPlayer), coord(aCoord) { }
		AdvanceFormationTarget(int aPlayer, int col) : type(Type_Player),
			coord(col, 0), player(aPlayer) { }
		
		Type type;
		int player;
		CellCoordinate coord;
//		const Unit* unit;
	};

	class GameFieldNode : public cocos2d::CCNode
	{
	public:
		bool init();
		//CREATE_FUNC(GameFieldNode);
		
		void clear();
		void setFlipPlayers(bool flipPlayers) { flipPlayers_ = flipPlayers; }
		
		UnitNode* nodeForUnit(const Unit* unit) const;
		UnitNode* addNodeForUnit(const Unit* unit, bool animated);
		void removeUnitAndAlign(const Unit* unit);
		void moveUnitAndRearrange(const Unit* unit, const CellCoordinate& newCoord);
		
		void advanceFormation(const AttackFormation* formation, const AdvanceFormationTarget& moveTo,
			cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		void killFormation(const AttackFormation* formation,
			cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		void killUnit(const Unit* unit,
			cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		void alignUnits(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		
		void updateMarkers();
		
		void showMoveIndicator(bool canMove, int col, int player, const Unit* unit);
		void hideMoveIndicator();
		
		bool isProcessing() const { return processing_; }
		
		cocos2d::CCPoint positionForCoordinate(int player, const CellCoordinate& c, UnitClass uc) const;
		cocos2d::CCPoint positionForCoordinate(int player, const CellCoordinate& c) const;
		int zOrderForCoordinate(int player, const CellCoordinate& c) const;
		int zOrderForUnit(const Unit* unit) const;
		cocos2d::CCPoint positionForUnit(const Unit* unit) const;
		cocos2d::CCPoint positionForPlayer(int player, int col) const;
		CellCoordinate cellCoordinateForPosition(const cocos2d::CCPoint& pos, int* out_player=0) const;
		UnitNode* unitNodeAtPosition(const cocos2d::CCPoint& p);
		const std::map<const Unit*, UnitNode*>& unitNodes() const { return unitNodes_; };
		
		void setContentSize(const cocos2d::CCSize size);
		
		IGameFieldNodeDelegate* delegate() const { return delegate_; }
		void setDelegate(IGameFieldNodeDelegate* delegate) { delegate_ = delegate; }
		
		int normalizedPlayer(int player) const;
		
		void runShootingAttack(AttackFormation* formation, RoundEventObject maxTarget, bool special,
			cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
			
		void showSlowMarker(AttackFormation* formation);
		void showHitChargingMarker(AttackFormation* formation);
		
		void setGame(Game* game) { game_ = game; }
		Game* game() const { return game_; }
		
	private:
		void updateFormationsAnimated(CCObject* target, cocos2d::SEL_CallFunc selector);
		void afterMove(CCNode* node);
		void afterRemoveAndAlign();
		void afterRemoveAndAlignForUnit(CCNode* node);
		void afterUpdateFormations();
		void afterUpdateFormationsForUnit(CCNode* node);
		void afterAlign();
		void afterShootingAttack();
		
		void afterAddNodeAnimation(CCNode* node);
		
		void eraseUnitNode(CCNode* node);
		
		void addMissingUnits();
		
		void addNamedFrameToArray(cocos2d::CCArray* frames, const char* frameName);
		void runFramedShootingAttack(AttackFormation* formation, RoundEventObject maxTarget,
			const ShootingAttackViewConfig& shootingAttackConfig,
			cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		void runThrowShootingAttack(AttackFormation* formation, RoundEventObject maxTarget,
			const ShootingAttackViewConfig& shootingAttackConfig,
			cocos2d::CCObject* target, cocos2d::SEL_CallFunc selector);
		
	private:
		bool flipPlayers_;
		std::map<const Unit*, UnitNode*> unitNodes_;
		Game* game_;
		
		cocos2d::CCSprite* bgCanMove_;
		cocos2d::CCSprite* bgCannotMove_;
		
		cocos2d::CCSize cellSize_;
		float padX_, padY_;
		
		bool processing_;
		int processingLeft_;
		
		CCObject* targetAfterUpdateFormations_;
		cocos2d::SEL_CallFunc selectorAfterUpdateFormations_;
		
		IGameFieldNodeDelegate* delegate_;
	};
}
