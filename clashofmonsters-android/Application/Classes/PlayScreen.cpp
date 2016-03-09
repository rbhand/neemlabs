//
//  PlayScreen.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "PlayScreen.h"
#include "GameFieldNode.h"
#include "Utils.h"
#include "GameHUDLayer.h"
#include "PlayScreenProcessingLayer.h"
//#include "CCAlertBox.h"
#include "GoogleAnalytics.h"
#include "XTLayer.h"

#include <iostream>

#define GAMEFIELD_AND_BG_ANIMATION_TAG	42
#define GAMEFIELD_POPUP_TAG             43


using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;

namespace monsters
{
	class GameLayer : public XTLayer, public IGameDelegate, public IGameHUDLayerDelegate, public INextRoundLayerDelegate,
		public IGameFieldNodeDelegate//, public IOnlineGameDelegate
	{
		friend class PlayScreen;
	
	public:
		bool init()
		{
			if (!CCLayer::init())
				return false;
			
			bg_ = CCSprite::create(
				cocos2d_utils::textureName("Level_0-Background.jpg").c_str());
			bg_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height));
			bg_->setAnchorPoint(CCPoint(0.5, 1));
			//bg->setScale(0.9);
			addChild(bg_);
			
			field_ = new GameFieldNode;
			field_->init();
			field_->autorelease();
			field_->setContentSize(getContentSize());
			field_->setAnchorPoint(CCPoint(0, 0));
			field_->setPosition(CCPoint(0, 0));
			field_->setDelegate(this);
			addChild(field_);
			
			setTouchEnabled(true);
			
			megaAttackFormation_ = 0;
			
			return true;
		}
		
		CREATE_FUNC(GameLayer);
		
		void setGame(Game* game)
		{
			game_ = game;
			field_->setGame(game);
			game_->setDelegate(this);
		}
		
//		void setOnlineGame(const boost::shared_ptr<OnlineGame>& onlineGame)
//		{
//			if (onlineGame_ && onlineGame_->delegate() == this)
//				onlineGame_->setDelegate(0);
//			onlineGame_ = onlineGame;
//			if (onlineGame_)
//				onlineGame_->setDelegate(this);
//		}
		
//		void processNextReceivedTurn()
//		{
//			if (onlineGame_)
//			{
//				PlayerAction turn = onlineGame_->popNextUnprocessedTurn();
//				if (turn.type() != PlayerActionType_None)
//				{
//					game_->applyAction(turn);
//					
//					auto_ptr<pb::Message> msg(game_->serialized());
//					onlineGame_->updateGameData(msg->serialized());
//				}
//			}
//		}
//		
//		void gameReceivedNewTurns(OnlineGame* game)
//		{
//			if (game == onlineGame_.get() && isRunning())
//			{
//				game->setNotifyOpponentTurn(false);
//				if (!field_->isProcessing() && !processingRoundEvents_ && !loadingGame_)
//				{
//					processNextReceivedTurn();
//				}
//			}
//		}
		
//		void sendQueuedTurn()
//		{
//			PlayerAction action = queuedTurn_;
//			queuedTurn_ = PlayerAction();
//			processing_->setVisible(false);
//			
//			rollbackRoundTimeWhenSendFailed_ = game_->roundTime();
//			onlineGame_->sendTurn(action);
//		}
//		
//		void gameDidSendTurn(OnlineGame* game)
//		{
//			if (game == onlineGame_.get())
//			{
//				if (!queuedTurn_.isNone())
//				{
//					// has queued turn, send it after small delay
//					this->runAction(CCSequence::create(CCDelayTime::create(1.0),
//						CCCallFunc::create(this, callfunc_selector(GameLayer::sendQueuedTurn)),
//						0));
//				}
//				else
//				{
//					auto_ptr<pb::Message> msg(game_->serialized());
//					onlineGame_->updateGameData(msg->serialized());
//				}
//			}
//		}
//		
//		void gameSendTurnFailed(OnlineGame* game, const Error& error)
//		{
//			if (game == onlineGame_.get())
//			{
//				// reset game
//				queuedTurn_ = PlayerAction();
//				processing_->setVisible(false);
//				onlineGame_->setLocalPlayerStateRoundTime(rollbackRoundTimeWhenSendFailed_);
//				
//				loadAfterSendFailedFlag_ = true;
//				
//				CCAlertBox::show("Network Problems", error.description(), "Dismiss");
//				
//				pb::Message msg;
//				onlineGame_->data()->rewind();
//				msg.deserialize(*onlineGame_->data());
//				game_->loadRemoteGame(msg, onlineGame_->localPlayerState(), Session::sharedSession().units());
//			}
//		}
//		
//		void gameResigned(monsters::OnlineGame *game, const monsters::User &byUser)
//		{
//			if (!byUser.isLocal())
//				CCAlertBox::show(byUser.name(), "Resigned from the game", "Dismiss");
//			
//			processing_->setVisible(false);
//			delegate_->playScreenMenu();
//		}

//		void gameResigningFailed(OnlineGame* game, const Error& error)
//		{
//			processing_->setVisible(false);
//			CCAlertBox::show("Network Problems", error.description(), "Dismiss");
//		}
		
		void sendRemoteTurn(const PlayerAction& turn)
		{
//			if (!onlineGame_->isSendingTurn())
//			{
//				rollbackRoundTimeWhenSendFailed_ = game_->roundTime();
//				onlineGame_->sendTurn(turn);
//			}
//			else
//			{
//				// queue turn
//				queuedTurn_ = turn;
//				processing_->setTitle("Sending turn...");
//				processing_->setVisible(true);
//			}
		}
		
		void updateBackground()
		{
			bg_->setTexture(CCSprite::create(
				cocos2d_utils::textureName(CCString::createWithFormat("Level_%d-Background.jpg", game_->stageBackgroundIndex())->
				getCString()).c_str())->getTexture());
		}
		
		void clear()
		{
			field_->clear();
			draggingNode_ = 0;
			shootingAttackFormation_ = 0;
			hasRoundEvents_ = false;
			nextRoundDelayed_ = false;
			loadingGame_ = false;
			loadAfterSendFailedFlag_ = false;
			processingRoundEvents_ = false;
			
			hints_.clear();
			
			CCArray* children = this->getChildren();
			
			int i = 0;
			while (i < children->count())
			{
				CCNode* n = dynamic_cast<CCNode*>(children->objectAtIndex(i));
				if (CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(n))
				{
					//std::cout << label->getString();
					label->removeFromParentAndCleanup(true);
				}
				else if (CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(n))
				{
					//std::cout << label->getString();
					label->removeFromParentAndCleanup(true);
				}
				else if (n && n->getTag() == GAMEFIELD_POPUP_TAG)
				{
					n->removeFromParentAndCleanup(true);
				}
				else
				{
					i++;
				}
			}
		}
		
		bool canMakeTurns() const
		{
			return !field_->isProcessing() &&
				!processingRoundEvents_ &&
				!loadingGame_ &&
				queuedTurn_.isNone() &&
				game_->activePlayer().type() == PlayerType_Local;
		}
		
		void processingDidStart()
		{
			//gameHUD_->setEndTurnEnabled(false);
		}
		
		void processingDidFinish()
		{
			//gameHUD_->setEndTurnEnabled(true);
			
			if (nextRoundDelayed_)
			{
				nextRoundDelayed_ = false;
				showNextRoundLayer(callfunc_selector(GameLayer::procceedRoundDidStart));
			}
			
			if (hasRoundEvents_)
			{
				hasRoundEvents_ = false;
				game_->nextRoundEvents();
			}
			else
			{
				game_->nextAITurn();
				//processNextReceivedTurn();
			}
			
			gameHUD_->setTurnsLeft(game_->activePlayerIndex(), game_->activePlayer().turnsLeft());
			gameHUD_->setTroopsLeft(game_->activePlayerIndex(), game_->activePlayer().troopsInReservoir());
			
			showAllHints(hints_);
			hints_.clear();
		}
		
		void updateMagicHUD(int player)
		{
			gameHUD_->setMagic(player, game_->player(player).magic());
			//gameHUD_->setMagicButtonVisible(player, game_->player(player).canCastMagic());
			gameHUD_->setMagicButtonVisible(player, false);
		}
		
		void loadAllUnits(bool animated)
		{
			for (int p=0; p<2; ++p)
			{
				for (int c=0; c<kFieldWidth; ++c)
				{
					for (int r=0; r<kFieldHeight; ++r)
					{
						const Unit *unit = game_->player(p).unit(c, r);
						if (unit)
							field_->addNodeForUnit(unit, animated);
					}
				}
			}
		}
		
		void updateBasicHUD()
		{
			for (int i=0; i<2; ++i)
			{
				const Player& player = game_->player(i);
				gameHUD_->setHealth(i, player.health());
				gameHUD_->setTroopsLeft(i, player.troopsInReservoir());
				updateMagicHUD(i);
			}
		}
		
		void updateAllHUD()
		{
			for (int pi=0; pi<2; ++pi)
			{
				gameHUD_->setTroopsLeft(pi, game_->player(pi).troopsInReservoir());
			}
			
			gameHUD_->setCallTroopsVisible(game_->activePlayerIndex(),
				game_->activePlayer().type() == PlayerType_Local);
			gameHUD_->setTurnsLeftVisible(game_->activePlayerIndex(), true);
			gameHUD_->setEndTurnVisible(game_->activePlayerIndex(),
				game_->activePlayer().type() == PlayerType_Local);
			gameHUD_->setTimeLeftVisible(game_->activePlayerIndex(),
				game_->activePlayer().type() == PlayerType_Local);
			
			gameHUD_->setTurnsLeft(game_->activePlayerIndex(), game_->activePlayer().turnsLeft(),
				true);
			gameHUD_->setTimeLeft(game_->activePlayerIndex(), game_->timeLeftSec());
		}
		
		void hideActiveHUD()
		{
			for (int pi=0; pi<2; ++pi)
			{
				gameHUD_->setCallTroopsVisible(pi, false);
				gameHUD_->setTurnsLeftVisible(pi, false);
				gameHUD_->setEndTurnVisible(pi, false);
				gameHUD_->setTimeLeftVisible(pi, false);
			}
		}
		
		void configureFlipPlayers()
		{
			bool flip = game_->type() == GameType_RemoteClient &&
				game_->player(0).type() == PlayerType_Remote;
			field_->setFlipPlayers(flip);
			gameHUD_->setFlipPlayers(flip);
		}
		
		void configureAvatars()
		{
			if (game_->type() == GameType_Local)
			{
				gameHUD_->setAvatarType(0, game_->player(1).type() == PlayerType_AI ?
						AvatarNodeType_None : AvatarNodeType_1stPlayer);
				gameHUD_->setAvatarType(1, game_->player(1).type() == PlayerType_AI ?
					AvatarNodeType_AI : AvatarNodeType_2ndPlayer);
			}
			else
			{
//				gameHUD_->setAvatarUrl(0, game_->player(0).type() == PlayerType_Local ?
//						Session::sharedSession().user().avatarUrl() : onlineGame_->opponent().avatarUrl());
//				gameHUD_->setAvatarUrl(1, game_->player(1).type() == PlayerType_Local ?
//						Session::sharedSession().user().avatarUrl() : onlineGame_->opponent().avatarUrl());
			}
		}
		
		void gameDidStart()
		{
			clear();
			
			configureFlipPlayers();
			loadAllUnits(true);
			updateBasicHUD();
			configureAvatars();
			updateBackground();
		}
		
		void proceedGameDidLoad()
		{
			updateAllHUD();
			loadingGame_ = false;
			//processNextReceivedTurn();
		}
		
		void gameDidLoad()
		{
			bool do_not_show_turn = loadAfterSendFailedFlag_;
			
			clear();
			
			configureFlipPlayers();
			loadAllUnits(false);
			updateBasicHUD();
			configureAvatars();
			updateBackground();
			
			loadingGame_ = true;
			
			hideActiveHUD();
			
			if (!do_not_show_turn)
			{
				showNextRoundLayer(callfunc_selector(GameLayer::proceedGameDidLoad));
			}
			else
			{
				proceedGameDidLoad();
			}
		}
		
		void gameOver()
		{
			delegate_->playScreenGameOver();
			//delegate_->playScreenMenu();
			//showAlert("Game Over", createWithFormat("Player %d won!", game_->winnerIndex()+1).c_str(), "Dismiss");
		}
		
		void showRemoveAtPoint(const CCPoint& pos)
		{
			CCSprite* lb_cascade = CCSprite::createWithSpriteFrameName("lb_special.png");
			lb_cascade->setPosition(pos);
			
			lb_cascade->setScale(0.5);
			lb_cascade->setOpacity(0);
			lb_cascade->runAction(CCFadeIn::create(0.4));
			lb_cascade->runAction(CCScaleTo::create(0.4, 1.0));
			
			lb_cascade->setTag(GAMEFIELD_POPUP_TAG);
			lb_cascade->runAction(CCSequence::create(
				CCMoveBy::create(0.8, ccp(0, ct(20))),
				CCFadeOut::create(0.4),
				0
			));
			addChild(lb_cascade);
		}
		
		void showSpecialAbilityAttackAtPoint(const CCPoint& pos)
		{
			CCSprite* lb_cascade = CCSprite::createWithSpriteFrameName("lb_special.png");
			lb_cascade->setPosition(pos);
			
			lb_cascade->setScale(0.5);
			lb_cascade->setOpacity(0);
			lb_cascade->runAction(CCFadeIn::create(0.4));
			lb_cascade->runAction(CCScaleTo::create(0.4, 1.0));
			
			lb_cascade->setTag(GAMEFIELD_POPUP_TAG);
			lb_cascade->runAction(CCSequence::create(
				CCMoveBy::create(0.8, ccp(0, ct(20))),
				CCFadeOut::create(0.4),
				0
			));
			addChild(lb_cascade);
		}
		
		void showCascadeAtPoint(const CCPoint& pos)
		{
			SoundManager::sharedSoundManager()->playEffect("game_cascade");
			
			CCSprite* lb_cascade = CCSprite::createWithSpriteFrameName("lb_cascade.png");
			lb_cascade->setPosition(pos);
			
			lb_cascade->setScale(0.5);
			lb_cascade->setOpacity(0);
			lb_cascade->runAction(CCFadeIn::create(0.4));
			lb_cascade->runAction(CCScaleTo::create(0.4, 1.0));
			
			lb_cascade->setTag(GAMEFIELD_POPUP_TAG);
			lb_cascade->runAction(CCSequence::create(
				CCMoveBy::create(0.8, ccp(0, ct(20))),
				CCFadeOut::create(0.4),
				0
			));
			addChild(lb_cascade);
		}
		
		void showComboAtPoint(const CCPoint& pos)
		{
			SoundManager::sharedSoundManager()->playEffect("game_combo");
		
			CCSprite* lb_cascade = CCSprite::createWithSpriteFrameName("lb_combo.png");
			lb_cascade->setPosition(pos);
			
			lb_cascade->setScale(0.5);
			lb_cascade->setOpacity(0);
			lb_cascade->runAction(CCFadeIn::create(0.4));
			lb_cascade->runAction(CCScaleTo::create(0.4, 1.0));
			
			lb_cascade->setTag(GAMEFIELD_POPUP_TAG);
			lb_cascade->runAction(CCSequence::create(
				CCMoveBy::create(0.8, ccp(0, ct(20))),
				CCFadeOut::create(0.4),
				0
			));
			addChild(lb_cascade, 10000);
		}
		
		void showSyncedAttackAtPoint(const CCPoint& pos)
		{
			SoundManager::sharedSoundManager()->playEffect("game_sync");
			
			CCSprite* lb_cascade = CCSprite::createWithSpriteFrameName("lb_sync.png");
			lb_cascade->setPosition(pos);
			
			lb_cascade->setScale(0.5);
			lb_cascade->setOpacity(0);
			lb_cascade->runAction(CCFadeIn::create(0.4));
			lb_cascade->runAction(CCScaleTo::create(0.4, 1.0));
			
			lb_cascade->setTag(GAMEFIELD_POPUP_TAG);
			lb_cascade->runAction(CCSequence::create(
				CCMoveBy::create(0.8, ccp(0, ct(20))),
				CCFadeOut::create(0.4),
				0
			));
			addChild(lb_cascade, 10000);
		}
		
		void showHitAtPoint(int hits, const CCPoint& pos, bool up)
		{
			SoundManager::sharedSoundManager()->playEffect("game_hit");
			
			CCLabelBMFont* lb_cascade = CCLabelBMFont::create(createWithFormat("%d", hits).c_str(), cocos2d_utils::textureName("badaboom.fnt").c_str());
			lb_cascade->setPosition(pos);
			lb_cascade->setVisible(false);
			lb_cascade->setAnchorPoint(ccp(0.5, up ? 0 : 1));
			lb_cascade->setScale(0.5);
			
			lb_cascade->setTag(GAMEFIELD_POPUP_TAG);
			lb_cascade->runAction(CCSequence::create(
				CCShow::create(),
				CCScaleTo::create(0.5, 1),
				CCMoveBy::create(0.5, ccp(0, ct(up ? 10 : -10))),
				CCDelayTime::create(0.3),
				CCFadeOut::create(0.3),
				0
			));
			addChild(lb_cascade, 10000);
		}
		
		void roundDidStart(const std::list<AttackFormation*>& synced)
		{
			SoundManager::sharedSoundManager()->stopAllEffects();
			
			syncedAttacks_ = synced;
			processingRoundEvents_ = true;
			
			hideActiveHUD();
			
			if (field_->isProcessing())
				nextRoundDelayed_ = true;
			else
				showNextRoundLayer(callfunc_selector(GameLayer::procceedRoundDidStart));
		}
		
		void procceedRoundDidStart()
		{
			updateMagicHUD(game_->activePlayerIndex());
			
			field_->hideMoveIndicator();
			field_->updateMarkers();
			
			processingRoundEvents_ = true;
			if (field_->isProcessing())
				hasRoundEvents_ = true;
			else
				game_->nextRoundEvents();
				
			for (std::list<AttackFormation*>::const_iterator i=syncedAttacks_.begin(), e=syncedAttacks_.end(); i!=e; ++i)
			{
				showSyncedAttackAtPoint(field_->positionForUnit((*i)->units().at(0)));
			}
		}
		
		void showNextRoundLayer(SEL_CallFunc proceed)
		{
			if (game_->type() == GameType_Local)
			{
				if (game_->activePlayer().type() == PlayerType_Local)
					nextRoundLayer_->setPlayerName(createWithFormat("Player %d's", game_->activePlayerIndex()+1).c_str());
				else
					nextRoundLayer_->setPlayerName(createWithFormat("Computer's", game_->activePlayerIndex()+1).c_str());
			}
			else
			{
//				if (game_->activePlayer().type() == PlayerType_Local)
//					nextRoundLayer_->setPlayerName((Session::sharedSession().user().name() + "'s").c_str());
//				else
//					nextRoundLayer_->setPlayerName((onlineGame_->opponent().name() + "'s").c_str());
			}
			
			nextRoundLayer_->runAction(CCSequence::create(
				CCShow::create(),
				CCFadeIn::create(0.2),
				CCDelayTime::create(1.2),
				CCFadeOut::create(0.4),
				CCHide::create(),
				CCCallFunc::create(this, proceed),
				0
			));
		}
		
		void roundTimeout()
		{
			// TODO
		}
		
		void afterAtackFormationKilled()
		{
//			if (!event_.targetKilled)
//			{
				//afterAttackTargetKilled();
				processNextRoundEvent();
//			}
		}
		
		void afterAttackTargetKilled()
		{
			//processNextRoundEvent();
		}
		
		void killAttackingFormationAndProceed()
		{
			if (event_.objectKilled)
			{
				if (megaAttackFormation_ == event_.object.attackFormation)
				{
					megaAttackFormation_ = 0;
					stopMegaAttackAnimation();
				}
				
				field_->killFormation(event_.object.attackFormation, this,
					callfunc_selector(GameLayer::afterAtackFormationKilled));
					
				if (event_.target.type == RoundEventObjectType_Player)
				{
					gameHUD_->setHealth(game_->inactivePlayerIndex(),
						game_->player(game_->inactivePlayerIndex()).health());
				}
			}
			else
			{
				processNextRoundEvent();
			}
		}
		
		void killEventTarget(SEL_CallFunc selector)
		{
			if (event_.targetKilled)
			{
				if (event_.target.type == RoundEventObjectType_AttackFormation)
				{
					int* value = new int(1/*event_.target.unit->attackFormation()->baseUnit()->unitType()->isLarge()*/);
					this->runAction(CCSequence::create(CCDelayTime::create(0.2),
						CCCallFuncND::create(this, callfuncND_selector(GameLayer::playKillEventSound), (void*) value),
						0));
					
					field_->killFormation(event_.target.attackFormation, this, selector);
				}
				else if (event_.target.type == RoundEventObjectType_Unit)
				{
					int* value = new int(event_.target.unit->isWall() ? 2 : 0 /*(event_.target.unit->unitType()->isLarge())*/);
					this->runAction(CCSequence::create(CCDelayTime::create(0.2),
						CCCallFuncND::create(this, callfuncND_selector(GameLayer::playKillEventSound), (void*) value),
						0));
					
					field_->killUnit(event_.target.unit, this, selector);
				}
			}
			else if (event_.targetTemporaryKilled)
			{
				int* value = new int(1);
				this->runAction(CCSequence::create(CCDelayTime::create(0.2),
					CCCallFuncND::create(this, callfuncND_selector(GameLayer::playKillEventSound), (void*) value),
					0));
				
				if (event_.target.type == RoundEventObjectType_AttackFormation)
				{
					field_->nodeForUnit(event_.target.attackFormation->baseUnit())->runTemporaryKilledAnimation(this, selector);
				}
				else if (event_.target.type == RoundEventObjectType_Unit)
				{
					field_->nodeForUnit(event_.target.unit)->runTemporaryKilledAnimation(this, selector);
				}
			}
		}
		
		void playKillEventSound(CCNode* sender, void* data)
		{
			int* sound_choice = (int*)data;
			
			switch (*sound_choice)
			{
				case 0:
					SoundManager::sharedSoundManager()->playEffect("game_normal_killed");
				break;
				
				case 1:
					SoundManager::sharedSoundManager()->playEffect("game_large_killed");
				break;
				
				case 2:
					SoundManager::sharedSoundManager()->playEffect("game_wall_killed");
				break;
			}
		}
		
		void killEventObjectsAndProceed()
		{
			if (event_.targetKilled || event_.targetTemporaryKilled)
			{
				killEventTarget(callfunc_selector(GameLayer::afterAttackTargetKilled));
			}
			else if (event_.target.type == RoundEventObjectType_AttackFormation)
			{
				field_->nodeForUnit(event_.target.attackFormation->units().at(0))->updateMarkers();
			}
			
			if (!shootingAttackFormation_ && !hittingAttackFormation_)
			{
				killAttackingFormationAndProceed();
			}
		}
		
		void showAttackHits()
		{
			if (event_.hits)
			{
				if (event_.target.type == RoundEventObjectType_Player)
				{
					showHitAtPoint(event_.hits,
						field_->positionForPlayer(game_->inactivePlayerIndex(), event_.object.attackFormation->col()),
						field_->normalizedPlayer(game_->inactivePlayerIndex()) == 0);
				}
				else
				{
					showHitAtPoint(event_.hits, field_->positionForCoordinate(game_->inactivePlayerIndex(),
						event_.coord), field_->normalizedPlayer(game_->inactivePlayerIndex()) == 0);
				}
			}
		}
		
		void doSpecialRemove()
		{
			if (event_.target.type == RoundEventObjectType_Unit)
			{
				field_->killUnit(event_.target.unit,
					this, callfunc_selector(GameLayer::afterSpecialAbilityRemove));
			}
			else if (event_.target.type == RoundEventObjectType_AttackFormation)
			{
				field_->killFormation(event_.target.attackFormation,
					this, callfunc_selector(GameLayer::afterSpecialAbilityRemove));
			}
		}
		
		void stopMegaAttackAnimation()
		{
			megaAttackHighightLayer_->setVisible(false);
			
			bg_->stopActionByTag(GAMEFIELD_AND_BG_ANIMATION_TAG);
			bg_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height));
			
			field_->stopActionByTag(GAMEFIELD_AND_BG_ANIMATION_TAG);
			field_->setPosition(CCPoint(0, 0));
		}
		
		void startMegaAttackAnimation()
		{
			CCSequence* move_bg = CCSequence::create(
				CCMoveTo::create(0.1, CCPoint(getContentSize().width * 0.5 - ct(4), getContentSize().height * 1 )),
				CCMoveTo::create(0.1, CCPoint(getContentSize().width * 0.5 + ct(4), getContentSize().height * 1 )),
				0
			);
			
			CCSequence* move_field = CCSequence::create(
				CCMoveTo::create(0.1, CCPoint(-ct(4), field_->getPosition().y )),
				CCMoveTo::create(0.1, CCPoint(ct(4), field_->getPosition().y )),
				0
			);
			
			CCAction* bg_action = CCRepeatForever::create(move_bg);
			bg_action->setTag(GAMEFIELD_AND_BG_ANIMATION_TAG);
			
			CCAction* field_action = CCRepeatForever::create(move_field);
			field_action->setTag(GAMEFIELD_AND_BG_ANIMATION_TAG);
			
			bg_->runAction(bg_action);
			field_->runAction(field_action);
			
			megaAttackHighightLayer_->setColor(ccc3(megaAttackFormation_->baseUnit()->unitType()->viewConfig().megaAttackConfig.red,
				megaAttackFormation_->baseUnit()->unitType()->viewConfig().megaAttackConfig.green, megaAttackFormation_->baseUnit()->unitType()->viewConfig().megaAttackConfig.blue));
			megaAttackHighightLayer_->setVisible(true);
		}
		
		void afterAdvance()
		{
			showAttackHits();
			killEventObjectsAndProceed();
		}
		
		void afterAlign()
		{
			processNextRoundEvent();
		}
		
		void afterSpecialAbilityRemove()
		{
			processNextRoundEvent();
		}
		
		void afterSpecialAbilityAttackKill()
		{
			
		}
		
		void afterSpecialAbilityAttack()
		{
			if (event_.target.type == RoundEventObjectType_AttackFormation &&
				!event_.targetKilled)
			{
				field_->nodeForUnit(event_.target.attackFormation->units().at(0))->updateMarkers();
			}
		
			processNextRoundEvent();
		}
		
		void afterShootingAttack()
		{
			shootingAttackFormation_ = 0;
			//processNextRoundEvent();
			killAttackingFormationAndProceed();
		}
		
		void afterHittingAttack()
		{
			hittingAttackFormation_ = 0;
			processNextRoundEvent();
		}
		
		void afterRegenerated()
		{
			processNextRoundEvent();
		}
		
		void killEventObjectsWithShowingHitsAndProceed()
		{
			killEventObjectsAndProceed();
			showAttackHits();
			processNextRoundEvent();
		}
		
		void processNextRoundEvent()
		{
			gameHUD_->setTroopsLeft(game_->activePlayerIndex(), game_->activePlayer().troopsInReservoir());
			gameHUD_->setTroopsLeft(game_->inactivePlayerIndex(), game_->inactivePlayer().troopsInReservoir());
			updateMagicHUD(game_->activePlayerIndex());
			
			if (roundEvents_.empty())
			{
				if (!shootingAttackFormation_ && !hittingAttackFormation_) // delay next events if we are currently shooting
					game_->nextRoundEvents();
			}
			else
			{
				event_ = roundEvents_.front();
				roundEvents_.erase(roundEvents_.begin());
				
				if (!megaAttackFormation_ && event_.type == RoundEventType_Attack && event_.object.attackFormation->baseUnit()->unitType()->viewConfig().megaAttackConfig.megaAttack)
				{
					megaAttackFormation_ = event_.object.attackFormation;
					startMegaAttackAnimation();
				}
				
				if ((event_.type == RoundEventType_Attack || event_.type == RoundEventType_SpecialAbility_AttackShooting) &&
					event_.object.type == RoundEventObjectType_AttackFormation)
				{
					// attacking formation is charging
					if (event_.object.attackFormation->baseUnit()->unitType()->viewConfig().shootingAttackConfig.type ==
							ShootingAttackViewConfig::Type_None &&
						event_.type != RoundEventType_SpecialAbility_AttackShooting)
					{
						// foot soldiers attack
						if (event_.target.type != RoundEventObjectType_Player)
						{
							field_->advanceFormation(event_.object.attackFormation,
								AdvanceFormationTarget(event_.coord, game_->inactivePlayerIndex()),
								this, callfunc_selector(GameLayer::afterAdvance));
						}
						else
						{
							field_->advanceFormation(event_.object.attackFormation,
								AdvanceFormationTarget(game_->inactivePlayerIndex(),
									event_.object.attackFormation->units().front()->coord().col),
								this, callfunc_selector(GameLayer::afterAdvance));
						}
					}
					else
					{
						// shooting attack
						if (!shootingAttackFormation_)
						{
							shootingAttackFormation_ = event_.object.attackFormation;
							
							// just started
							RoundEvent max_attack_event = event_;
							for (list<RoundEvent>::iterator i=roundEvents_.begin(), e=roundEvents_.end(); i!=e; ++i)
							{
								if (i->type == event_.type &&
									i->object.type == RoundEventObjectType_AttackFormation &&
									i->object.attackFormation == event_.object.attackFormation)
								{
									if (i->target.type == RoundEventObjectType_Player)
									{
										max_attack_event = *i;
										break;
									}
									else
									{
										if (i->coord.row > max_attack_event.coord.row)
											max_attack_event = *i;
									}
								}
							}
							
							if (event_.type == RoundEventType_SpecialAbility_AttackShooting)
								showSpecialAbilityAttackAtPoint(field_->positionForUnit(shootingAttackFormation_->baseUnit()));
							
							field_->runShootingAttack(event_.object.attackFormation, max_attack_event.target,
								event_.type == RoundEventType_SpecialAbility_AttackShooting,
								this, callfunc_selector(GameLayer::afterShootingAttack));
							
							runAction(CCSequence::create(
								CCDelayTime::create(0.3),
								CCCallFunc::create(this, callfunc_selector(GameLayer::killEventObjectsWithShowingHitsAndProceed)),
								0
							));
//							killEventObjectsAndProceed();
//							showAttackHits();
//							processNextRoundEvent();
						}
						else
						{
							// already started
							killEventObjectsAndProceed();
							showAttackHits();
							processNextRoundEvent();
						}
					}
				}
				else if (event_.type == RoundEventType_Align)
				{
					field_->alignUnits(this, callfunc_selector(GameLayer::afterAlign));
				}
				else if (event_.type == RoundEventType_SpecialAbility_Remove)
				{
					field_->nodeForUnit(event_.object.attackFormation->baseUnit())->runSingleSpecialAnimation();
					showRemoveAtPoint(field_->positionForUnit(event_.object.attackFormation->baseUnit()));
					field_->nodeForUnit(event_.object.attackFormation->baseUnit())->playSound(UnitNodeSound_Special);
					
					runAction(CCSequence::create(
						CCDelayTime::create(0.4),
						CCCallFunc::create(this, callfunc_selector(GameLayer::doSpecialRemove)),
						0
					));
				}
				else if (event_.type == RoundEventType_SpecialAbility_Attack)
				{
					showSpecialAbilityAttackAtPoint(field_->positionForUnit(event_.object.attackFormation->units().at(0)));
					
					field_->nodeForUnit(event_.object.attackFormation->baseUnit())->playSound(UnitNodeSound_Special);
					
					if (event_.target.type == RoundEventObjectType_Player)
					{
						showHitAtPoint(event_.hits,
							field_->positionForPlayer(game_->inactivePlayerIndex(), event_.object.attackFormation->col()),
							field_->normalizedPlayer(game_->inactivePlayerIndex()) == 0);
					}
					else
					{
						showHitAtPoint(event_.hits, field_->positionForCoordinate(game_->inactivePlayerIndex(),
							event_.coord), field_->normalizedPlayer(game_->inactivePlayerIndex()) == 0);
					}
					
					if (event_.targetKilled || event_.targetTemporaryKilled)
					{
						killEventTarget(callfunc_selector(GameLayer::afterSpecialAbilityAttackKill));
					}
					
					field_->nodeForUnit(event_.object.attackFormation->baseUnit())->runSingleSpecialAnimation(this,
						callfunc_selector(GameLayer::afterSpecialAbilityAttack));
				}
				else if (event_.type == RoundEventType_SpecialAbility_SlowAdjascentCharges)
				{
					showSpecialAbilityAttackAtPoint(field_->positionForUnit(event_.target.attackFormation->baseUnit()));
					field_->showSlowMarker(event_.target.attackFormation);
					field_->nodeForUnit(event_.target.attackFormation->baseUnit())->updateMarkers();
					//field_->nodeForUnit(event_.target.attackFormation->baseUnit())->playSound(UnitNodeSound_Special);
					processNextRoundEvent();
				}
				else if (event_.type == RoundEventType_SpecialAbility_HitAllChargingUnits)
				{
					if (!hittingAttackFormation_)
					{
						// first time
						hittingAttackFormation_ = event_.object.attackFormation;
						showSpecialAbilityAttackAtPoint(field_->positionForUnit(event_.object.attackFormation->baseUnit()));
						field_->nodeForUnit(event_.object.attackFormation->baseUnit())->runSingleSpecialAnimation(this,
							callfunc_selector(GameLayer::afterHittingAttack));
						field_->nodeForUnit(event_.object.attackFormation->baseUnit())->playSound(UnitNodeSound_Special);
						
						field_->showHitChargingMarker(event_.target.attackFormation);
						
						killEventObjectsAndProceed();
						showAttackHits();
						processNextRoundEvent();
					}
					else
					{
						field_->showHitChargingMarker(event_.target.attackFormation);
						
						// already started
						killEventObjectsAndProceed();
						showAttackHits();
						processNextRoundEvent();
					}
				}
				else if (event_.type == RoundEventType_SpecialAbility_Regenerate)
				{
					showSpecialAbilityAttackAtPoint(field_->positionForUnit(event_.object.attackFormation->baseUnit()));
					UnitNode* node = field_->nodeForUnit(event_.object.attackFormation->baseUnit());
					node->runRegeneratedAnimation(this, callfunc_selector(GameLayer::afterRegenerated));
					node->playSound(UnitNodeSound_Special);
				}
			}
		}
		
		void roundEvents(const std::list<RoundEvent>& events)
		{
			roundEvents_ = events;
			
			if (!roundEvents_.empty())
			{
				processNextRoundEvent();
			}
			else
			{
				// Round events finished
				if (processingRoundEvents_)
				{
					game_->nextAITurn();
				}
				
				processingRoundEvents_ = false;
				
//				if (onlineGame_)
//				{
//					auto_ptr<pb::Message> msg(game_->serialized());
//					onlineGame_->updateGameData(msg->serialized());
//				}
				
				updateAllHUD();
				//processNextReceivedTurn();
			}
		}
		
		void showAllHints(const std::list<ProcessFormationsHint>& hints)
		{
			for (list<ProcessFormationsHint>::const_iterator i=hints.begin(), e=hints.end(); i!=e; ++i)
			{
				if (i->type == ProcessFormationsHint::Combo)
					showComboAtPoint(field_->positionForCoordinate(i->player, i->coord));
				else if (i->type == ProcessFormationsHint::Sync)
					showSyncedAttackAtPoint(field_->positionForCoordinate(i->player, i->coord));
				else if (i->type == ProcessFormationsHint::Cascade)
					showCascadeAtPoint(field_->positionForCoordinate(i->player, i->coord));
			}
		}
		
		void unitWasRemoved(const Unit* unit, const std::list<ProcessFormationsHint>& hints)
		{
			field_->removeUnitAndAlign(unit);
			updateMagicHUD(game_->activePlayerIndex());
			
			hints_ = hints;
		}
		
		void unitWasMoved(const Unit* unit, const CellCoordinate& newCoord,
			const std::list<ProcessFormationsHint>& hints)
		{
			field_->moveUnitAndRearrange(unit, newCoord);
//			gameHUD_->setTurnsLeft(game_->activePlayerIndex(), game_->activePlayer().turnsLeft());
//			gameHUD_->setTroopsLeft(game_->activePlayerIndex(), game_->activePlayer().troopsInReservoir());
			updateMagicHUD(game_->activePlayerIndex());
			
			hints_ = hints;
		}
		
		void troopsWereCalled()
		{
			for (int c=0; c<kFieldWidth; ++c)
			{
				for (int r=0; r<kFieldHeight; ++r)
				{
					const Unit *unit = game_->activePlayer().unit(c, r);
					if (unit && !field_->nodeForUnit(unit))
						field_->addNodeForUnit(unit, true);
				}
			}
			
//			gameHUD_->setTurnsLeft(game_->activePlayerIndex(), game_->activePlayer().turnsLeft());
//			gameHUD_->setTroopsLeft(game_->activePlayerIndex(), game_->activePlayer().troopsInReservoir());
		}
		
		void callTroops(int player)
		{
			if (!canMakeTurns())
				return;
				
			game_->callTroops();
			gameHUD_->setTroopsLeft(game_->activePlayerIndex(), game_->activePlayer().troopsInReservoir());
		}
		
		void endRound(int player)
		{
			if (!canMakeTurns())
				return;
			
			game_->endRound();
		}
		
		void magic(int player)
		{
			if (!canMakeTurns())
				return;
			
		}
		
		void startDragging()
		{
			CCNode* new_parent = this->getParent();
			draggingNode_->stopAllActions();
			draggingNode_->hideShadow();
			draggingNode_->setPosition(ccpAdd(new_parent->convertToNodeSpace(lastTouchPoint_), ccp(0, ct(20))));
			draggingNode_->retain();
			draggingNode_->removeFromParentAndCleanup(false);
			new_parent->addChild(draggingNode_);
			draggingNode_->autorelease();
			
			isDragging_ = true;
			SoundManager::sharedSoundManager()->playEffect("game_start_dragging");
		}

		void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
		{
			XTLayer::ccTouchesBegan(pTouches, pEvent);
			
			if (!canMakeTurns())
				return;
			
			CCTouch *pTouch = reinterpret_cast<CCTouch*>(pTouches->anyObject());
			CCPoint p = field_->convertTouchToNodeSpace(pTouch);
			//UnitNode* n = field_->unitNodeAtPosition(p);
			
			int player;
			CellCoordinate coord = field_->cellCoordinateForPosition(p, &player);
			if (player == game_->activePlayerIndex())
			{
				const Unit* unit = game_->activePlayer().lastUnitInColumn(coord.col);
				UnitNode* n = 0;
				if (unit)
					n = field_->nodeForUnit(unit);
					
				if (n && game_->canMoveUnit(n->unit()))
				{
					draggingNode_ = n;
					isDragging_ = false;
					touchStartedAt_ = time_;
					dragFirstPoint_ = lastTouchPoint_ =
						CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
				}
			}
			
//			if (n)
//				cout << "col: " << n->unit()->coord().col << " row: " << n->unit()->coord().row << " unit: " << n->unit() << endl;
//			else
//				cout << "missed" << endl;
			
		}
		
		void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
		{
			XTLayer::ccTouchesMoved(pTouches, pEvent);
			
			if (!canMakeTurns())
				return;
			
			CCTouch *pTouch = reinterpret_cast<CCTouch*>(pTouches->anyObject());
			
			if (draggingNode_)
			{
				lastTouchPoint_ = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
				
				if (isDragging_)
				{
					draggingNode_->setPosition(ccpAdd(draggingNode_->getParent()->convertTouchToNodeSpace(pTouch),
						ccp(0, ct(20))));
				}
				else if (ccpDistance(dragFirstPoint_, lastTouchPoint_) >= 6)
				{
					startDragging();
				}
				
				if (isDragging_)
				{
					CellCoordinate c = field_->cellCoordinateForPosition(field_->convertToNodeSpace(lastTouchPoint_));
					
					if (c.col < 0 || c.col >= kFieldWidth)
						field_->hideMoveIndicator();
					else if (c.col == draggingNode_->unit()->coord().col)
						field_->hideMoveIndicator();
					else
						field_->showMoveIndicator(game_->activePlayer().rowToMoveUnit(c.col, draggingNode_->unit()) != -1,
							c.col, game_->activePlayerIndex(), draggingNode_->unit());
				}
					
			}
		}
		
		void finalizeMoveNodeBackToField(CCNode* node)
		{
			node->retain();
			node->removeFromParentAndCleanup(false);
			field_->addChild(node);
			node->autorelease();
			
			UnitNode* unit_node = dynamic_cast<UnitNode*>(node);
			unit_node->showShadow();
		}
		
		void moveNodeBackToField(UnitNode* node)
		{
			node->runAction(CCSequence::create(
				CCMoveTo::create(0.1, node->getParent()->convertToNodeSpace(
					field_->convertToWorldSpace(field_->positionForUnit(node->unit())))),
				CCCallFuncN::create(this, callfuncN_selector(GameLayer::finalizeMoveNodeBackToField)),
				0
			));
		}
		
		void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
		{
			XTLayer::ccTouchesEnded(pTouches, pEvent);
			
			if (!canMakeTurns())
				return;
			
			CCTouch *pTouch = reinterpret_cast<CCTouch*>(pTouches->anyObject());
			CCPoint p = field_->convertTouchToNodeSpace(pTouch);
			UnitNode* n = field_->unitNodeAtPosition(p);
			
			if (isDragging_)
			{
				// move node back
				CellCoordinate c = field_->cellCoordinateForPosition(p);
				
				if (c.col < 0 || c.col >= kFieldWidth || 
					c.col == draggingNode_->unit()->coord().col ||
					game_->activePlayer().rowToMoveUnit(c.col, draggingNode_->unit()) == -1)
				{
					moveNodeBackToField(draggingNode_);
				}
				else
				{
					game_->moveUnit(draggingNode_->unit(), c.col);
				}
				
				isDragging_ = false;
				SoundManager::sharedSoundManager()->playEffect("game_end_dragging");
				field_->hideMoveIndicator();
			}
			
			// TODO: Double tap
			// if (pTouch->getTapCount() == 2 && n &&
			// 	n->unit()->player() == game_->activePlayerIndex())
			// {
			// 	game_->removeUnit(n->unit());
			// }
			
			draggingNode_ = 0;
		}
		
		void xtDoubleTapGesture(CCPoint p)
		{
			if (!canMakeTurns())
				return;
			
			UnitNode* n = field_->unitNodeAtPosition(p);
			if (n && n->unit()->player() == game_->activePlayerIndex())
			{
				game_->removeUnit(n->unit());
			}
		}

		void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
		{
			XTLayer::ccTouchesCancelled(pTouches, pEvent);
			
			if (isDragging_)
			{
				// move node back
				moveNodeBackToField(draggingNode_);
				isDragging_ = false;
				field_->hideMoveIndicator();
			}
			
			draggingNode_ = 0;
		}

		void onEnter()
		{
			CCLayer::onEnter();
			
			time_ = 0;
			scheduleUpdate();
		}
		
		void update(float dt)
		{
			CCLayer::update(dt);
			
			if (queuedTurn_.isNone() && !menuShown_ && !processing_->isVisible() &&
				!processingRoundEvents_ && !nextRoundLayer_->isVisible()) // paused while there is a queued turn, menu shown or processing operation
			{
				time_ += dt;
				
				if (draggingNode_ && !isDragging_ && (time_ - touchStartedAt_) > 0.8)
				{
					startDragging();
				}
				
				game_->update(dt * 1000.0f);
//				if (onlineGame_)
//					onlineGame_->setLocalPlayerStateRoundTime(game_->roundTime());
				
				if (game_->timeLeftSec() > 4)
				{
					playerTimeOut_ = false;
				}
				else if (game_->timeLeftSec() < 4 && !playerTimeOut_)
				{
					playerTimeOut_ = true;
					SoundManager::sharedSoundManager()->playEffect("game_time_out");
				}
				
				gameHUD_->setTimeLeft(game_->activePlayerIndex(), game_->timeLeftSec());
			}
		}
		
		void setDelegate(IPlayScreenDelegate* delegate)
		{
			delegate_ = delegate;
		}
		
		void setTouchEnabled(bool enabled)
		{
			CCLayer::setTouchEnabled(enabled);
			if (gameHUD_)
				gameHUD_->setTouchEnabled(enabled);
		}
		
		GameFieldNode* GameField()
		{
			return field_;
		}
		
		void setMenuShown(bool shown)
		{
			menuShown_ = shown;
		}
		
		
	private:
		
		
	private:
		Game* game_;
		//boost::shared_ptr<OnlineGame> onlineGame_;
		CCSprite* bg_;
		GameFieldNode* field_;
		GameHUDLayer* gameHUD_;
		cocos2d::CCLayerColor* megaAttackHighightLayer_;
		NextRoundLayer* nextRoundLayer_;
		PlayScreenProcessingLayer* processing_;
		
		CCPoint dragFirstPoint_;
		CCPoint lastTouchPoint_;
		float touchStartedAt_;
		UnitNode* draggingNode_;
		bool isDragging_;
		bool hasRoundEvents_;
		bool processingRoundEvents_;
		bool loadingGame_;
		bool nextRoundDelayed_;
		bool menuShown_;
		
		float time_;
		
		list<RoundEvent> roundEvents_;
		RoundEvent event_;
		
		AttackFormation* shootingAttackFormation_;
		AttackFormation* hittingAttackFormation_;
		
		IPlayScreenDelegate* delegate_;
		
		list<AttackFormation*> syncedAttacks_;
		list<ProcessFormationsHint> hints_;
		AttackFormation* megaAttackFormation_;
		int playersCount_;
		bool playerTimeOut_;
		
		PlayerAction queuedTurn_;
		uint64_t rollbackRoundTimeWhenSendFailed_;
		bool loadAfterSendFailedFlag_;
	};
	
}


bool PlayScreenMenuLayer::init()
{
	if (!cocos2d::CCLayerColor::init())
		return false;
	
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPoint(0, 0));
	addChild(menu);
	
	CCMenuItemSprite* btn_resume = menuItemSpriteHighlighted("btn_resume.png", this,
		menu_selector(PlayScreenMenuLayer::resume));
	btn_resume->setAnchorPoint(CCPoint(0.5, 0.5));
	btn_resume->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5 + btn_resume->getContentSize().height * 2.25));
	menu->addChild(btn_resume);
	
	CCMenuItemSprite* btn_settings = menuItemSpriteHighlighted("btn_playscreen_settings.png", this,
		menu_selector(PlayScreenMenuLayer::settings));
	btn_settings->setAnchorPoint(CCPoint(0.5, 0.5));
	btn_settings->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5 + btn_settings->getContentSize().height * 0.75));
	menu->addChild(btn_settings);
	
	CCMenuItemSprite* btn_tutorial = menuItemSpriteHighlighted("btn_tutorial.png", this,
		menu_selector(PlayScreenMenuLayer::tutorial));
	btn_tutorial->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5 - btn_tutorial->getContentSize().height * 0.75));
	menu->addChild(btn_tutorial);
	
	CCMenuItemSprite* btn_quit = menuItemSpriteHighlighted("btn_quit.png", this,
		menu_selector(PlayScreenMenuLayer::quit));
	btn_quit->setAnchorPoint(CCPoint(0.5, 0.5));
	btn_quit->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5 - btn_settings->getContentSize().height * 2.25));
	menu->addChild(btn_quit);
	
	return true;
}

void PlayScreenMenuLayer::settings()
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate()->playScreenMenuLayerSettings();
}

void PlayScreenMenuLayer::tutorial()
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate()->playScreenMenuLayerTutorial();
}

void PlayScreenMenuLayer::quit()
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate()->playScreenMenuLayerQuit();
}

void PlayScreenMenuLayer::resume()
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate()->playScreenMenuLayerResume();
}

PlayScreen::PlayScreen()
// : alertOnlineBack_("Online Game",
// 	"Do you want to resign from the game completely or temporary quit to main menu?",
// 	"Main Menu", "Resign", this)
{
	
}

bool PlayScreen::init()
{
	if (!CCScene::init())
		return false;
		
	delegate_ = 0;
		
	gameLayer_ = GameLayer::create();
	addChild(gameLayer_, 10);
	
	gameLayer_->megaAttackHighightLayer_ = CCLayerColor::create(ccc4(0, 0, 0, 32));
	gameLayer_->megaAttackHighightLayer_->setVisible(false);
	addChild(gameLayer_->megaAttackHighightLayer_, 20);
	
	gameLayer_->gameHUD_ = GameHUDLayer::create();
	gameLayer_->gameHUD_->setDelegate(gameLayer_);
	addChild(gameLayer_->gameHUD_, 50);
	
	gameLayer_->nextRoundLayer_ = NextRoundLayer::create();
	gameLayer_->nextRoundLayer_->setDelegate(gameLayer_);
	gameLayer_->nextRoundLayer_->setVisible(false);
	
	gameLayer_->processing_ = PlayScreenProcessingLayer::create();
	gameLayer_->processing_->setVisible(false);
	
	menu_ = CCMenu::create();
	menu_->setPosition(CCPoint(0, 0));
	addChild(menu_, 40);
	
	CCMenuItemSprite* btn_menu = menuItemSpriteHighlighted("btn_menu.png", this,
		menu_selector(PlayScreen::menu));
	btn_menu->setAnchorPoint(CCPoint(1, 0.5));
	btn_menu->setPosition(CCPoint(getContentSize().width, getContentSize().height * 0.5));
	menu_->addChild(btn_menu);
	
	CCSprite* normal_sprite = CCSprite::createWithSpriteFrameName("btn_show_strength.png");
	CCSprite* selected_sprite = CCSprite::createWithSpriteFrameName("btn_show_strength.png");
	selected_sprite->setColor(ccc3(120, 120, 120));
	CCMenuItemSpriteTouch* btn_show_strength = CCMenuItemSpriteTouch::create(normal_sprite, selected_sprite,
		this, menu_selector(PlayScreen::hideStrengthLayer));
	btn_show_strength->setAnchorPoint(CCPoint(0, 0.5));
	btn_show_strength->setPosition(CCPoint(0, getContentSize().height * 0.5));
	btn_show_strength->setTouchSelector(menu_selector(PlayScreen::showStrengthLayer));
	menu_->addChild(btn_show_strength);
	
	popupBgLayer_ = new CCLayerColor();
	popupBgLayer_->initWithColor(ccc4(0, 0, 0, 192));
	popupBgLayer_->setVisible(false);
	popupBgLayer_->setPosition(CCPoint(0, 0));
	popupBgLayer_->autorelease();
	addChild(popupBgLayer_, 60);
	
	menuLayer_ = PlayScreenMenuLayer::create();
	setMenuVisible(false);
	menuLayer_->setPosition(CCPoint(0, 0));
	menuLayer_->setDelegate(this);
	addChild(menuLayer_, 70);
	
	settingsLayer_ = SettingsLayer::create();
	settingsLayer_->setVisible(false);
	settingsLayer_->setDelegate(this);
	
	addChild(gameLayer_->nextRoundLayer_, 80);
	addChild(settingsLayer_, 90);
	addChild(gameLayer_->processing_, 100);
	
	return true;
}

void PlayScreen::onEnter()
{
	CCScene::onEnter();
	
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Play Screen");
	
	SoundManager::sharedSoundManager()->stopBackgroundMusic();
	
	this->runAction(CCSequence::create(CCDelayTime::create(0.25),
	CCCallFunc::create(this, callfunc_selector(PlayScreen::playStartupMusic)),
	CCDelayTime::create(1.0),
		CCCallFunc::create(this, callfunc_selector(PlayScreen::startBackgroundMusic)),
		0));
	
	popupBgLayer_->setVisible(false);
	setMenuVisible(false);
	settingsLayer_->setVisible(false);
	gameLayer_->setTouchEnabled(true);
	menu_->setTouchEnabled(true);
	
	strengthIndicatorsLayer_ = StrengthIndicatorsLayer::create();
	strengthIndicatorsLayer_->setGamefield(gameLayer_->GameField());
	strengthIndicatorsLayer_->retain();
	strengthIndicatorsLayer_->setTag(STRENGTH_INDICATORS_LAYER_TAG);
}

void PlayScreen::playStartupMusic()
{
	SoundManager::sharedSoundManager()->playEffect("startup_melody");
}

void PlayScreen::startBackgroundMusic()
{
	SoundManager::sharedSoundManager()->startBackgroundMusic();
}

void PlayScreen::onExit()
{
	//SoundManager::sharedSoundManager()->stopBackgroundMusic();
	CCScene::onExit();
	
	strengthIndicatorsLayer_->removeFromParent();
	strengthIndicatorsLayer_->release();
}

void PlayScreen::setDelegate(IPlayScreenDelegate* delegate)
{
	gameLayer_->setDelegate(delegate);
	delegate_ = delegate;
}

void PlayScreen::menu()
{
	CC_ASSERT(delegate_);
	
	if (gameLayer_->processingRoundEvents_ &&
			gameLayer_->game_->type() == GameType_RemoteClient)
		return;
	
	SoundManager::sharedSoundManager()->playEffect("playscreen_menu");
	
//	string b64 = gameLayer_->game_->serializedBase64();
//	Game g;
//	g.loadServerGame(b64, Session::sharedSession().units());
	
	popupBgLayer_->setVisible(true);
	setMenuVisible(true);
	gameLayer_->setTouchEnabled(false);
	menu_->setTouchEnabled(false);
}

void PlayScreen::showStrengthLayer()
{
	if (this->getChildByTag(STRENGTH_INDICATORS_LAYER_TAG) == strengthIndicatorsLayer_)
	{
		strengthIndicatorsLayer_->stopAllActions();
	}
	else
	{
		addChild(strengthIndicatorsLayer_, 30);
		strengthIndicatorsLayer_->runAction(CCCallFunc::create(strengthIndicatorsLayer_, callfunc_selector(StrengthIndicatorsLayer::fadeIn)));
		//strengthIndicatorsLayer_->runAction(CCFadeIn::create(0.25));
	}
}

void PlayScreen::hideStrengthLayer()
{
	if (this->getChildByTag(STRENGTH_INDICATORS_LAYER_TAG) != strengthIndicatorsLayer_)
		return;

	strengthIndicatorsLayer_->stopAllActions();
	strengthIndicatorsLayer_->runAction(CCSequence::create(CCDelayTime::create( (strengthIndicatorsLayer_->passedTime() > 1.5) ? 0.01 : (1.5 - strengthIndicatorsLayer_->passedTime())),
		CCCallFunc::create(strengthIndicatorsLayer_, callfunc_selector(StrengthIndicatorsLayer::fadeOut)),
		//CCFadeOut::create(0.25),
		CCDelayTime::create(0.25),
		CCCallFunc::create(this, callfunc_selector(PlayScreen::removeStrengthLayer)),
		0));
}


void PlayScreen::removeStrengthLayer()
{
	if (this->getChildByTag(STRENGTH_INDICATORS_LAYER_TAG) != strengthIndicatorsLayer_)
		return;
	
	strengthIndicatorsLayer_->removeFromParent();
}

void PlayScreen::setMenuVisible(bool visible)
{
	gameLayer_->setMenuShown(visible);
	menuLayer_->setVisible(visible);
}

void PlayScreen::resume()
{
	setMenuVisible(false);
	popupBgLayer_->setVisible(false);
	gameLayer_->setTouchEnabled(true);
	menu_->setTouchEnabled(true);
}

void PlayScreen::setGame(Game* game)
{
	gameLayer_->setGame(game);
}

//void PlayScreen::setOnlineGame(const boost::shared_ptr<OnlineGame>& onlineGame)
//{
//	gameLayer_->setOnlineGame(onlineGame);
//}

void PlayScreen::playScreenMenuLayerSettings()
{
	settingsLayer_->setVisible(true);
	setMenuVisible(false);
}

void PlayScreen::playScreenMenuLayerTutorial()
{
	delegate()->playScreenTutorial();
}

// void PlayScreen::alertBoxDidDismiss(CCAlertBox* alertBox, int buttonIndex)
// {
// 	if (alertBox == &alertOnlineBack_)
// 	{
// 		if (buttonIndex != alertBox->cancelButtonIndex())
// 		{
// 			setMenuVisible(false);
// 			gameLayer_->processing_->setVisible(true);
// //			gameLayer_->onlineGame_->resign();
// 		}
// 		else
// 		{
// 			delegate()->playScreenMenu();
// 		}
// 	}
// }

void PlayScreen::playScreenMenuLayerQuit()
{
//	if (gameLayer_->onlineGame_)
//	{
//		alertOnlineBack_.show();
//	}
//	else
	{
		delegate()->playScreenMenu();
	}
}

void PlayScreen::playScreenMenuLayerResume()
{
	resume();
}


void PlayScreen::settingsLayerBack()
{
	settingsLayer_->setVisible(false);
	setMenuVisible(true);
}

void PlayScreen::settingsLayerSoundOnOf()
{
	delegate()->playScreenSoundOnOf();
}

void PlayScreen::settingsLayerMusicOnOf()
{
	delegate()->playScreenMusicOnOf();
}

//boost::shared_ptr<OnlineGame> PlayScreen::onlineGame() const
//{
//	return gameLayer_->onlineGame_;
//}


