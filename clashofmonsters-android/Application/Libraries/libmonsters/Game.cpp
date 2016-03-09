//
//  Game.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "Game.h"

#ifndef MN_SERVER
#include "Session.h"
#endif

#include <iostream>
#include <set>
#include <algorithm>
#include "cocos2d.h"
#include <memory>
#include <stdlib.h>

using namespace monsters;
using namespace w2f2;
using namespace std;

#ifndef MN_SERVER
using namespace f17y::utils;
#endif

namespace monsters
{
	// pb::Message* serialized_monotonic_timer(const f17y::utils::MonotonicTimer& t)
	// {
	// 	using namespace pb;
	// 	Message* m = new Message();
	// 	*m << varint::b(0, t.isWorking())
	// 		<< varint::ui(1, t.totalTime());
	// 	
	// 	return m;
	// };
	
	#ifndef MN_SERVER
	class AIJob : public f17y::utils::BackgroundJob
	{
	public:
		AIJob(AI& ai, const Player& player) :
			ai_(ai), player_(player)
		{
		}
		
		void main()
		{
			action_ = ai_.nextTurn(player_);
		}
		
		PlayerAction action() const
		{
			return action_;
		}
		
	private:
		AI& ai_;
		const Player& player_;
		PlayerAction action_;
	};
	#endif
	
	class DummyGameDelegate : public IGameDelegate
	{
	public:
		void gameDidStart() { }
		void gameDidLoad() { }
		void roundDidStart(const std::list<AttackFormation*>& synced) { }
		void roundEvents(const std::list<RoundEvent>& events) { }
		void roundTimeout() { }
		void unitWasRemoved(const Unit* unit,
			const std::list<ProcessFormationsHint>& combos) { }
		void unitWasMoved(const Unit* unit, const CellCoordinate& newCoord,
			const std::list<ProcessFormationsHint>& combos) { }
		void troopsWereCalled() { }
		void gameOver() { }
		void sendRemoteTurn(const PlayerAction& turn) { }
		
		static DummyGameDelegate instance;
	};
}

DummyGameDelegate DummyGameDelegate::instance;

Game::Game() :
	state_(GameState_Inactive),
	delegate_(&DummyGameDelegate::instance)
		//, units_(units)
{
	players_[0].index_ = 0;
	players_[1].index_ = 1;
}

bool Game::isEqualToGame(const Game& game) const
{
	if (state_ != game.state_)
		return false;
		
	for (int i=0; i<2; ++i)
	{
		if (!players_[i].isEqualToPlayer(game.players_[i]))
			return false;
	}
	
	if (activePlayer_ != game.activePlayer_)
		return false;
	
	if (winner_ != game.winner_)
		return false;
		
	if (round_ != game.round_)
		return false;
	
	if (randomGenerator_ != game.randomGenerator_)
		return false;
	
	if (stageBackgroundIndex_ != game.stageBackgroundIndex_)
		return false;
		
	if (turnsWereMade_ != game.turnsWereMade_)
		return false;
	
	return true;
}

pb::Message* Game::serialized() const
{
	using namespace pb;
	
	Message* m = new Message();
	
	*m << varint::ui(1, state_);
	
	if (state_ != GameState_Inactive)
	{
		for (int i=0; i<2; ++i)
			*m << msg(2, players_[i].serialized());
	
		*m << varint::ui(3, activePlayer_)
			<< varint::ui(4, winner_)
			<< varint::i(5, round_);
	
		*m << msg(6, randomGenerator_.serialized());
		
		*m << varint::i(7, stageBackgroundIndex_);
		*m << varint::b(8, turnsWereMade_);
	}
	
	return m;
}

bool Game::deserialize(const pb::Message& msg, int localPlayerIndex, const GameUnits& units)
{
	clear();
	state_ = (GameState)msg.getFieldValueUInt32(1);
	
	if (state_ != GameState_Inactive)
	{
		if (msg.countValues(2) != 2)
			return false;
		
		for (int i=0; i<2; ++i)
		{
			if (!msg.getFieldValueMessageAtIndex(2, i) ||
				!players_[i].deserialize(*msg.getFieldValueMessageAtIndex(2, i), i == localPlayerIndex ?
					PlayerType_Local : PlayerType_Remote, units))
			{
				return false;
			}
		}
	
		activePlayer_ = msg.getFieldValueUInt32(3);
		winner_ = msg.getFieldValueUInt32(4);
		round_ = msg.getFieldValueInt32(5);
	
		if (!msg.getFieldValueMessage(6) ||
				!randomGenerator_.deserialize(*msg.getFieldValueMessage(6)))
			return false;
			
		stageBackgroundIndex_ = msg.getFieldValueInt32(7);
		turnsWereMade_ = msg.getFieldValueUInt32(8);
	}
	
	return true;
}

bool Game::loadServerGame(const pb::Message& msg, const GameUnits& units)
{
	type_ = GameType_Server;
	return deserialize(msg, -1, units);
}

#ifndef MN_SERVER
bool Game::loadRemoteGame(const pb::Message& msg, const LocalPlayerState& localPlayerState,
	const GameUnits& units)
{
	type_ = GameType_RemoteClient;
	if (deserialize(msg, localPlayerState.playerIndex, units))
	{
		roundTime_ = localPlayerState.roundTime;
		
		if (round_ == 0 && !turnsWereMade_)
		{
			delegate_->gameDidStart();
			delegate_->roundDidStart(list<AttackFormation*>());
		}
		else
		{
			delegate_->gameDidLoad();
		}
		
		return true;
	}
	
	return false;
}
#endif

void Game::clear()
{
	for (int i=0; i<2; ++i)
	{
		players_[i].clear();
	}
	//gameDuration_ = 0;
	roundTime_ = 0;
	
	#ifndef MN_SERVER
	aiThinking_ = false;
	#endif
}

void Game::startRound()
{
	players_[activePlayer_].turnsLeft_ = kInitialTurns;
	if (activePlayer_ == 0)
		++round_;
	
	syncedAttacks_.clear();
	
	for (list<AttackFormation*>::iterator i=players_[activePlayer_].attackFormations().begin(),
		e=players_[activePlayer_].attackFormations().end(); i!=e; ++i)
	{
		(*i)->roundsToCharge_--;
		if ((*i)->roundsToCharge_ == 0)
			syncedAttacks_.insert(*i);
	}
	
	formationsToProcessSpecialAbilities_.clear();
	list<AttackFormation*> synced;
	for (list<AttackFormation*>::iterator i=players_[activePlayer_].attackFormations().begin(),
		e=players_[activePlayer_].attackFormations().end(); i!=e; ++i)
	{
		Unit* base_unit = (*i)->units().at(0);
		SpecialAbility sa = base_unit->unitType()->specialAbility();
		if (specialAbilityAddsEventsBeforeTurn(sa) &&
				((*i)->roundsToCharge() > 0 || specialAbilityAddsEventsBeforeAttack(sa)))
		{
			formationsToProcessSpecialAbilities_.push_back(*i);
		}
		
		if ((*i)->roundsToCharge_ == 0 && syncedAttacks_.size() > 1)
			synced.push_back(*i);
	}
	
	delegate_->roundDidStart(synced);
	
	troopsAligned_ = false;
	if (activePlayer_ == 0 && round_ == 0)
		troopsAligned_ = true;
	
	if (type_ == GameType_Server) // auto process events on server
	{
		while (nextRoundEvents() > 0)
		{
		}
	}
	
//	nextRoundEvents();
}

void Game::sendTurnIfNeeded(const PlayerAction& action)
{
	if (activePlayer().type() == PlayerType_Local && type_ == GameType_RemoteClient)
	{
		PlayerAction turn = action;
		turn.setPlayerIndex(activePlayer_);
		delegate_->sendRemoteTurn(turn);
	}
}

void Game::removeUnit(const Unit* unit)
{
	if (unit->attackFormation() /*|| unit->isWall()*/)
		return;
		
	CC_ASSERT(unit->player() == activePlayer_);
	//CC_ASSERT(!unit->isWall());
	CC_ASSERT(!unit->attackFormation());
	
	sendTurnIfNeeded(PlayerAction::actionRemoveUnit(unit->coord()));
	
	turnsWereMade_ = true;
	
	players_[activePlayer_].turnsLeft_--;
	players_[activePlayer_].removeUnit(unit);
	
	std::list<ProcessFormationsHint> hints;
	int new_formations = players_[activePlayer_].processFormations(round_, false, &hints, true);
	int cascades = new_formations;
	players_[activePlayer_].turnsLeft_ += cascades;
	players_[activePlayer_].addMagic(cascades * kMagicPerCascade);
	
	delegate_->unitWasRemoved(unit, hints);
	delete unit;
	
	checkEndRound();
}

bool Game::canMoveUnit(const Unit* unit) const
{
	return (unit->player() == activePlayer_ &&
		activePlayer().canMoveUnit(unit));
}

void Game::moveUnit(const Unit* unit, int col)
{
	CC_ASSERT(canMoveUnit(unit));
	
	sendTurnIfNeeded(PlayerAction::actionMoveUnit(unit->coord(), col));
	
	turnsWereMade_ = true;
	
	players_[activePlayer_].turnsLeft_--;
	players_[activePlayer_].moveUnit(unit, col);
	
	CellCoordinate coord_after_move = unit->coord();
	
	std::list<ProcessFormationsHint> hints;
	int new_formations = players_[activePlayer_].processFormations(round_, false, &hints, false);
	int cascades = max(0, new_formations-1);
	players_[activePlayer_].turnsLeft_ += cascades;
	players_[activePlayer_].addMagic(cascades * kMagicPerCascade);
	
	delegate_->unitWasMoved(unit, coord_after_move, hints);
	
	checkEndRound();
}

void Game::callTroops()
{
	if (activePlayer().troopsInReservoir() > 0)
	{
		sendTurnIfNeeded(PlayerAction::actionCallTroops());
	
		turnsWereMade_ = true;
		
		players_[activePlayer_].fillTroopsFromReservoir();
		players_[activePlayer_].turnsLeft_--;
		
		delegate_->troopsWereCalled();
		
		checkEndRound();
	}
}

void Game::endRound()
{
	sendTurnIfNeeded(PlayerAction::actionEndRound());
	doEndRound();
}

void Game::doEndRound()
{
	turnsWereMade_ = true;

	roundTime_ = 0;
	
	activePlayer_ = (activePlayer_ + 1) % 2;
	startRound();
}

int Game::attackStrength(AttackFormation* attacker, int strength, RoundEventType attackType)
{
	switch (attackType)
	{
		case RoundEventType_Attack: return attacker->strength();
		default: return strength;
	}
}

list<RoundEvent> Game::attackUnits(const std::list<Unit*>& units, RoundEventType attackType, AttackFormation* attacker,
	bool attackPlayer)
{
	Player& dp = players_[inactivePlayerIndex()];
	AttackFormation* f = attacker;
	
	list<RoundEvent> events;
	int strength = 0;
		
	if (attackType == RoundEventType_SpecialAbility_Attack ||
		attackType == RoundEventType_SpecialAbility_AttackShooting)
	{
		strength = f->units().at(0)->unitType()->specialAbilityParam();
		if (attacker->combos() > 1)
			strength = (strength * kComboAttackStrengthCoefficient * attacker->combos()).ceilInt();
	}
	
	std::list<Unit*> units_to_attack = units;
	while (units_to_attack.size())
	{
		Unit* du = units_to_attack.front();
		
		RoundEvent event(attackType);
		event.object = attacker;
			
		if (du->attackFormation_)
		{
			// defensive unit is part of the formation, count formation as a whole
			event.hits = min(attackStrength(f, strength, attackType), du->attackFormation_->strength());
			event.target = RoundEventObject(du->attackFormation_);
			event.coord = du->coord();
			
			du->attackFormation_->subtractedStrength_ = du->attackFormation_->subtractedStrength_ + event.hits;
			
			if (attackType == RoundEventType_Attack)
				f->subtractedStrength_ += event.hits;
			else
				strength -= event.hits;
			
			CC_ASSERT(du->attackFormation()->strength() >= 0);
			CC_ASSERT(f->strength() >= 0);
			
			if (du->attackFormation()->strength() == 0)
			{
				if (du->attackFormation()->canRegenerate())
				{
					event.targetTemporaryKilled = true;
					
					du->attackFormation()->units().at(0)->isTemporaryKilled_ = true;
					du->attackFormation()->units().at(0)->regeneratedCount_++;
					du->attackFormation()->subtractedStrength_ = 0;
				}
				else
				{
					event.targetKilled = true;
					
					formationsToDelete_.insert(du->attackFormation_);
					
					for (vector<Unit*>::const_iterator i=du->attackFormation_->units().begin(), e=du->attackFormation_->units().end();
						i!=e; ++i)
					{
						dp.putUnitBackToReservoir(*i, false);
					}
					
					// remove formation
					dp.attackFormations_.erase(find(dp.attackFormations_.begin(), dp.attackFormations_.end(),
						du->attackFormation_));
					
					// remove all units
					for (vector<Unit*>::iterator ui=du->attackFormation_->units_.begin(),
						ue = du->attackFormation_->units_.end(); ui!=ue; ++ui)
					{
						dp.field_.setUnit((*ui)->coord(), 0);
						unitsToDelete_.insert((*ui));
					}
				}
			}
			
			for (vector<Unit*>::iterator ui=du->attackFormation_->units_.begin(),
				ue = du->attackFormation_->units_.end(); ui!=ue; ++ui)
			{
				units_to_attack.remove(*ui);
			}
		}
		else if (du->isWall_)
		{
			// wall
			event.hits = min((int)attackStrength(f, strength, attackType), du->wallStrength());
			event.target = RoundEventObject(du);
			event.coord = du->coord();
			
			du->wallStrength_ -= event.hits;
			
			if (attackType == RoundEventType_Attack)
				f->subtractedStrength_ += event.hits;
			else
				strength -= event.hits;
			
			CC_ASSERT(du->wallStrength() >= 0);
			CC_ASSERT(f->strength() >= 0);
			
			if (du->wallStrength() == 0)
			{
				event.targetKilled = true;
				
				dp.field_.setUnit(du->coord(), 0);
				dp.putUnitBackToReservoir(du, false);
				
				unitsToDelete_.insert(du);
			}
		}
		else
		{
			// idle unit
			event.hits = min(attackStrength(f, strength, attackType), du->unitType()->defensive());
			event.target = RoundEventObject(du);
			event.coord = du->coord();
			
			if (attackType == RoundEventType_Attack)
				f->subtractedStrength_ += event.hits;
			else
				strength -= event.hits;
			
			event.targetKilled = true;
			
			CC_ASSERT(f->strength() >= 0);
			
			if (event.targetKilled)
			{
				dp.putUnitBackToReservoir(du, false);
				
				dp.field_.setUnit(du->coord(), 0);
				unitsToDelete_.insert(du);
			}
		}
		
		if (attackType == RoundEventType_Attack && f->strength() == 0)
		{
			event.objectKilled = true;
		}
		
		units_to_attack.remove(du);
		events.push_back(event);
		
		if (event.objectKilled || attackStrength(f, strength, attackType) == 0)
			break;
	}
	
	if (attackStrength(f, strength, attackType) > 0 && attackPlayer)
	{
		RoundEvent event(attackType);
		event.object = RoundEventObject(f);
		event.target = RoundEventObject(inactivePlayerIndex());
		event.hits = attackStrength(f, strength, attackType);
		if (attackType == RoundEventType_Attack)
			event.objectKilled = true;
		events.push_back(event);
		
		players_[inactivePlayerIndex()].health_ = max(players_[inactivePlayerIndex()].health_ - event.hits, 0);
	}

	return events;
}

int Game::nextRoundEvents()
{
	list<RoundEvent> events;
	
	//cout << "nextRoundEvents: " << players_[activePlayer_].attackFormations().size() << endl;
	
	set<AttackFormation*> special_abilities_processed;
	
	// Special abilities
	bool stop = false;
	for (list<AttackFormation*>::iterator i=formationsToProcessSpecialAbilities_.begin(),
		e=formationsToProcessSpecialAbilities_.end(); i!=e && !stop; ++i)
	{
		Player& ap = players_[activePlayerIndex()];
		Player& dp = players_[inactivePlayerIndex()];
	
		Unit* base_unit = (*i)->units().at(0);
		SpecialAbility ability = base_unit->unitType()->specialAbility();
		int param = base_unit->unitType()->specialAbilityParam();
		
		special_abilities_processed.insert(*i);
		
		switch (ability)
		{
			case SpecialAbility_RemoveAny:
			case SpecialAbility_RemoveUnmade:
			{
				if (ap.isUnitInFrontRow(base_unit))
				{
					vector<Unit*> other_units;
					
					for (int i=0; i<base_unit->width(); ++i)
					{
						Unit* u = dp.occupyingUnit(CellCoordinate(base_unit->coord().col + i, 0));
						
						if (u && (!u->isWallOrAttack() || ability == SpecialAbility_RemoveAny) &&
								find(other_units.begin(), other_units.end(), u) == other_units.end())
							other_units.push_back(u);
					}
					
					if (other_units.size())
					{
						Unit* du = other_units[randomGenerator_.randInt() % other_units.size()];
						
						RoundEvent event(RoundEventType_SpecialAbility_Remove);
						event.object = RoundEventObject(*i);
						event.targetKilled = true;
						
						if (!du->attackFormation())
						{
							unitsToDelete_.insert(du);
							dp.putUnitBackToReservoir(du, true);
							dp.field_.setUnit(du->coord(), 0);
							
							event.target = RoundEventObject(du);
						}
						else
						{
							formationsToDelete_.insert(du->attackFormation_);
									
							for (vector<Unit*>::const_iterator ui=du->attackFormation_->units().begin(), ue=du->attackFormation_->units().end();
								ui!=ue; ++ui)
							{
								dp.putUnitBackToReservoir(*ui, false);
							}
							
							// remove formation
							dp.attackFormations_.erase(find(dp.attackFormations_.begin(), dp.attackFormations_.end(),
								du->attackFormation_));
							
							// remove all units
							for (vector<Unit*>::iterator ui=du->attackFormation_->units_.begin(),
								ue = du->attackFormation_->units_.end(); ui!=ue; ++ui)
							{
								dp.field_.setUnit((*ui)->coord(), 0);
								unitsToDelete_.insert((*ui));
							}
							
							event.target = RoundEventObject(du->attackFormation_);
						}
							
						events.push_back(event);
					}
				}
			}
			break;
			
			case SpecialAbility_AttackFirst:
			{
				if (ap.isUnitInFrontRow(base_unit))
				{
					list<Unit*> units_to_attack;
					for (int i=0; i<base_unit->width(); ++i)
					{
						Unit* u = dp.occupyingUnit(CellCoordinate(base_unit->coord().col + i, 0));
						if (u && find(units_to_attack.begin(), units_to_attack.end(), u) == units_to_attack.end())
							units_to_attack.push_back(u);
					}
					
					list<RoundEvent> new_events = attackUnits(units_to_attack, RoundEventType_SpecialAbility_Attack, *i, false);
					events.splice(events.end(), new_events);
				}
			}
			break;
			
			case SpecialAbility_AttackAll:
			case SpecialAbility_AttackThroughWalls:
			{
				if (ap.isUnitInFrontRow(base_unit))
				{
					list<Unit*> units_to_attack;
					for (int r=0; r<kFieldHeight; ++r)
					{
						for (int i=0; i<base_unit->width(); ++i)
						{
							Unit* u = dp.occupyingUnit(CellCoordinate(base_unit->coord().col + i, r));
							if (u && find(units_to_attack.begin(), units_to_attack.end(), u) == units_to_attack.end() &&
								(!u->isWall() || ability != SpecialAbility_AttackThroughWalls))
							{
								units_to_attack.push_back(u);
							}
						}
					}
					
					list<RoundEvent> new_events = attackUnits(units_to_attack, RoundEventType_SpecialAbility_AttackShooting, *i, true);
					events.splice(events.end(), new_events);
				}
			}
			break;
			
			case SpecialAbility_HitAllChargingUnits:
			{
				for (list<AttackFormation*>::iterator di=dp.attackFormations_.begin(), de=dp.attackFormations_.end(); di!=de; ++di)
				{
					RoundEvent event(RoundEventType_SpecialAbility_HitAllChargingUnits);
					event.object = RoundEventObject(*i);
					event.target = RoundEventObject(*di);
					
					event.hits = min((int)((*i)->combos() > 1 ?
						((param * kComboAttackStrengthCoefficient * (*i)->combos()).ceilInt()) :
						param), (*di)->strength());
					
					event.coord = (*di)->baseUnit()->coord();
					
					(*di)->subtractedStrength_ += event.hits;
					
					if ((*di)->strength() == 0)
					{
						if ((*di)->canRegenerate())
						{
							event.targetTemporaryKilled = true;
							
							(*di)->units().at(0)->isTemporaryKilled_ = true;
							(*di)->units().at(0)->regeneratedCount_++;
							(*di)->subtractedStrength_ = 0;
						}
						else
						{
							event.targetKilled = true;
						
							formationsToDelete_.insert(*di);
							
							for (vector<Unit*>::const_iterator ui=(*di)->units().begin(), ue=(*di)->units().end(); ui!=ue; ++ui)
							{
								dp.putUnitBackToReservoir(*ui, false);
							}
							
							// remove all units
							for (vector<Unit*>::iterator ui=(*di)->units_.begin(), ue = (*di)->units_.end(); ui!=ue; ++ui)
							{
								dp.field_.setUnit((*ui)->coord(), 0);
								unitsToDelete_.insert((*ui));
							}
						}
					}
					
					events.push_back(event);
				}
				
				for (set<AttackFormation*>::iterator fi=formationsToDelete_.begin(), fe=formationsToDelete_.end(); fi!=fe; ++fi)
				{
					// remove formation
					list<AttackFormation*>::iterator it = find(dp.attackFormations_.begin(), dp.attackFormations_.end(), *fi);
					if (it != dp.attackFormations_.end())
						dp.attackFormations_.erase(it);
				}
				
				stop = true;
			}
			break;
			
			case SpecialAbility_SlowAdjascentCharges:
			case SpecialAbility_Regenerate:
			case SpecialAbility_None:
			break;
		}
		
		if (events.size())
		{
			for (set<AttackFormation*>::iterator i=special_abilities_processed.begin(), e=special_abilities_processed.end();
				i!=e; ++i)
			{
				formationsToProcessSpecialAbilities_.erase(find(formationsToProcessSpecialAbilities_.begin(),
					formationsToProcessSpecialAbilities_.end(), *i));
			}
		
			delegate()->roundEvents(events);
			return events.size();
		}
	}
	
//	formationsToProcessSpecialAbilities_.clear();
//	if (events.size())
//	{
//		delegate()->roundEvents(events);
//		return;
//	}
	
	
	// Attacks
	for (list<AttackFormation*>::iterator i=players_[activePlayer_].attackFormations().begin(),
		e=players_[activePlayer_].attackFormations().end(); i!=e; ++i)
	{
		AttackFormation* f = (*i);
		CC_ASSERT(f->roundsToCharge_ >= 0);
		
		if (f->roundsToCharge_ == 0)
		{	
			// formation charging
			
			set<Unit*> processed_units;
			
			Player& ap = players_[activePlayer_];
			Player& dp = players_[(activePlayer_ + 1) % 2];
			
			if (syncedAttacks_.size() > 1)
			{
				ap.addMagic(kMagicPerSyncedAttack);
				
				f->totalStrength_ = f->totalStrength_ + (f->totalStrength_ *
					(kSyncedAttackStrengthCoefficient * (int)(syncedAttacks_.size() - 1))).ceilInt();
			}
			
//			int combos = f->combos_;
//			while (combos > 1)
//			{
//				f->totalStrength_ = (float)f->totalStrength_ * (kComboAttackStrengthCoefficient * 0.5);
//				--combos;
//			}
			
			// attack
			list<Unit*> units_to_attack;
			for (int r=0; f->strength() > 0 && r<kFieldHeight; ++r)
			{
				for (int c=f->col(); f->strength() > 0 && c < (f->col() + f->width()); ++c)
				{
					Unit* du = dp.occupyingUnit(CellCoordinate(c, r));
					if (du && find(units_to_attack.begin(), units_to_attack.end(), du) == units_to_attack.end())
						units_to_attack.push_back(du);
				}
			}
			
			// append generated events
			list<RoundEvent> new_events = attackUnits(units_to_attack, RoundEventType_Attack, f, true);
			events.splice(events.end(), new_events);
			
			// special ability slow down
			if (f->baseUnit()->unitType()->specialAbility() == SpecialAbility_SlowAdjascentCharges)
			{
				set<AttackFormation*> slowed_down;
				bool stop = false;
				for (int r=0; f->strength() > 0 && r<kFieldHeight && !stop; ++r)
				{
					for (int c=f->col(); c < (f->col() + f->width()); ++c)
					{
						if (dp.occupyingUnit(CellCoordinate(c, r)))
						{
							stop = true;
							break;
						}
					}
					
					if (!stop)
					{
						// look for adjascent charges
						// left
						if (Unit* unit = f->col() > 0 ? dp.occupyingUnit(CellCoordinate(f->col() - 1, r)) : 0)
						{
							if (unit->attackFormation() &&
								find(slowed_down.begin(), slowed_down.end(), unit->attackFormation()) == slowed_down.end())
							{
								slowed_down.insert(unit->attackFormation_);
							}
						}
						
						// right
						if (Unit* unit = (f->col() + f->width()) < kFieldWidth ? dp.occupyingUnit(CellCoordinate(f->col() + f->width(), r)) : 0)
						{
							if (unit->attackFormation() &&
								find(slowed_down.begin(), slowed_down.end(), unit->attackFormation()) == slowed_down.end())
							{
								slowed_down.insert(unit->attackFormation_);
							}
						}
					}
					else
					{
						for (int c=f->col(); c < (f->col() + f->width()); ++c)
						{
							Unit* unit = dp.occupyingUnit(CellCoordinate(c, r));
							if (unit && unit->attackFormation() &&
								find(slowed_down.begin(), slowed_down.end(), unit->attackFormation()) == slowed_down.end())
							{
								slowed_down.insert(unit->attackFormation_);
							}
						}
					}
				}
				
				for (set<AttackFormation*>::iterator i=slowed_down.begin(), e=slowed_down.end(); i!=e; ++i)
				{
					int sa_param = f->baseUnit()->unitType()->specialAbilityParam();
					(*i)->roundsToCharge_ += (int)(f->combos() > 1 ?
						((sa_param * kComboAttackStrengthCoefficient * f->combos()).ceilInt()) :
						sa_param);
					
					RoundEvent event(RoundEventType_SpecialAbility_SlowAdjascentCharges);
					event.object = RoundEventObject(f);
					event.target = RoundEventObject(*i);
					event.objectKilled = true;
					
					events.push_back(event);
				}
			}
			
			formationsToDelete_.insert(f);
			
			//cout << "before erase: " << players_[activePlayer_].attackFormations().size() << endl;
			
			// remove formation
			list<AttackFormation*>::iterator it = find(ap.attackFormations_.begin(), ap.attackFormations_.end(), f);
			CC_ASSERT(it != ap.attackFormations_.end());
			ap.attackFormations_.erase(it);
			
			//cout << "after erase: " << players_[activePlayer_].attackFormations().size() << endl;
			
			// remove all units
			for (vector<Unit*>::iterator ui=f->units_.begin(),
				ue = f->units_.end(); ui!=ue; ++ui)
			{
				ap.field_.setUnit((*ui)->coord(), 0);
				unitsToDelete_.insert((*ui));
				
				ap.putUnitBackToReservoir(*ui, true);
			}
			
			delegate_->roundEvents(events);
			return events.size();
		}
	}
	
	// regenerate what can be regenerated
	for (list<AttackFormation*>::const_iterator i=inactivePlayer().attackFormations_.begin(),
		e=inactivePlayer().attackFormations_.end(); i!=e; ++i)
	{
		Unit* du = (*i)->baseUnit();
		if (du->isTemporaryKilled_)
		{
			du->isTemporaryKilled_ = false;
			
			RoundEvent event(RoundEventType_SpecialAbility_Regenerate);
			event.object = RoundEventObject(*i);
			event.coord = du->coord();
			events.push_back(event);
		}
	}
	
	if (events.size())
	{
		delegate_->roundEvents(events);
		return events.size();
	}
	
	
	if (!troopsAligned_)
	{
		for (int p=0; p<2; ++p)
		{
			players_[p].alignTroops();
			players_[p].processFormations(round_, false);
		}
		
		events.push_back(RoundEvent(RoundEventType_Align));
		
		delegate_->roundEvents(events);
		troopsAligned_ = true;
		
		return events.size();
	}
	
	delegate_->roundEvents(events);
	
	for (set<Unit*>::iterator ui=unitsToDelete_.begin(), ue=unitsToDelete_.end(); ui!=ue; ++ui)
		delete *ui;
	unitsToDelete_.clear();
	for (set<AttackFormation*>::iterator fi=formationsToDelete_.begin(), fe=formationsToDelete_.end(); fi!=fe; ++fi)
		delete *fi; 
	formationsToDelete_.clear();
	
	if (inactivePlayer().health() <= 0)
	{
		state_ = GameState_Over;
		winner_ = activePlayerIndex();
		
		#ifndef MN_SERVER
		
		/*if (winner_ == 0) //if 1st player won - award him with points
			Session::sharedSession().addExperiencePoints(playerPoints(winner_));*/
		//add xp points to 1st player
		/*if (type_ == GameType_Local)
		{
			Session::sharedSession().addExperiencePoints(playerPoints(0));
		}
		else if (type_ == GameType_RemoteClient)
		{
			Session::sharedSession().addExperiencePoints(playerPoints(0));
		}*/
		
		#endif
		
		delegate_->gameOver();
	}
	else
	{
		// control goes to player
		#ifndef MN_SERVER
		roundTime_ = 0;
		#endif
	}
	
	return 0;
}

void Game::nextAITurn()
{
	//cout << "Game::nextTurn" << endl;
	
	#ifndef MN_SERVER
	if (activePlayer().type() == PlayerType_AI)
	{
		aiThinking_ = true;
		aiWillEndThinkingAt_ = roundTime_ + 1000;//500 + (rand() % 4) * 500;
	}
	#endif
}

bool Game::canMoveUnit(const Unit* unit, int col) const
{
	return canMoveUnit(unit) && (col >= 0 && col < kFieldWidth &&
		activePlayer().rowToMoveUnit(col, unit) != -1);
}

bool Game::applyAction(const PlayerAction& action)
{
	if (state_ == GameState_Playing &&
		(action.playerIndex() == -1 || activePlayer_ == action.playerIndex()))
	{
		switch (action.type())
		{
			case PlayerActionType_CallTroops:
			{
				callTroops();
				return true;
			}
			break;
			
			case PlayerActionType_EndRound:
			case PlayerActionType_TimerExpired:
			{
				doEndRound();
				return true;
			}
			break;
			
			case PlayerActionType_MoveUnit:
			{
				const Unit* unit = activePlayer().occupyingUnit(action.coord());
				if (unit && canMoveUnit(unit, action.to()))
				{
					moveUnit(unit, action.to());
					return true;
				}
			}
			break;
			
			case PlayerActionType_RemoveUnit:
			{
				const Unit* unit = activePlayer().occupyingUnit(action.coord());
				if (unit && unit->player() == activePlayer_ && !unit->attackFormation())
				{
					removeUnit(activePlayer().occupyingUnit(action.coord()));
					return true;
				}
			}
			break;
			
			case PlayerActionType_None:
			break;
		}
	}
	
	return false;
}

#ifndef MN_SERVER
void Game::onBackgroundJobCompleted(f17y::utils::BackgroundJob* job)
{
	if (aiJob_ == job)
	{
		// make AI turn
		PlayerAction action = aiJob_->action();
		aiJob_ = 0;
		applyAction(action);
	}
	
	job->release();
}

void Game::update(uint64_t dt)
{
	roundTime_ += dt;
	if (roundTime_ >= kTimeForRoundSeconds * 1000 &&
		activePlayer().type() == PlayerType_Local &&
		!aiThinking_)
	{
		delegate_->roundTimeout();
		
		aiThinking_ = false;
		
		sendTurnIfNeeded(PlayerAction::actionTimerExpired());
		doEndRound();
	}
	else
	{
		if (aiThinking_ && roundTime_ >= aiWillEndThinkingAt_)
		{
			aiJob_ = new AIJob(ai_, activePlayer());
			aiJob_->setDelegate(this);
			aiThinking_ = false;
			aiJob_->run();
		}
	}
}
#endif

void Game::checkEndRound()
{
	CC_ASSERT(players_[activePlayer_].turnsLeft_ >= 0);
	
	if (players_[activePlayer_].turnsLeft_ == 0)
	{
		doEndRound();
	}
}

#ifndef MN_SERVER
int Game::timeLeftSec() const
{
	return max(0, (int)(kTimeForRoundSeconds - roundTime_ / 1000));
}
#endif

void Game::startNewGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config,
	PlayerType player1Type, PlayerType player2Type)
{
	state_ = GameState_Playing;
	round_ = -1;
	turnsWereMade_ = false;
	
	clear();
	randomGenerator_.srand(time(0) + rand());
	
	players_[0].config_ = player1Config;
	players_[1].config_ = player2Config;
	
	players_[0].type_ = player1Type;
	players_[1].type_ = player2Type;
	
	for (int p=0; p<2; ++p)
	{
		players_[p].health_ = kInitialHealth;
		players_[p].magic_ = 0;
		players_[p].troopsInReservoir_ = kMaxTroops;
		players_[p].largeTroopsInReservoir_ = kMaxLargeTroops;
		players_[p].fillTroopsFromReservoir();
	}
	
	if (rand() % 2 == 0)
		stageBackgroundIndex_ = player1Config.avatar()->index();
	else
		stageBackgroundIndex_ = player2Config.avatar()->index();
	
	delegate_->gameDidStart();
	
	activePlayer_ = 0;
	startRound();
}

void Game::startSinglePlayerGame(const PlayerConfig& player1Config, const PlayerConfig& aiConfig)
{
	CC_ASSERT(delegate_);
	
	type_ = GameType_Local;
	startNewGame(player1Config, aiConfig,
		PlayerType_Local, PlayerType_AI);
}

void Game::startTwoPlayersGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config)
{
	CC_ASSERT(delegate_);
	
	type_ = GameType_Local;
	startNewGame(player1Config, player2Config,
		PlayerType_Local, PlayerType_Local);
}

void Game::restartGame()
{
	CC_ASSERT(delegate_);
	
	type_ = GameType_Local;
	startNewGame(players_[0].config_, players_[1].config_,
		players_[0].type_, players_[1].type_);
}

bool Game::startGameServer(const PlayerConfig& player1Config, const PlayerConfig& player2Config)
{
	if (!player1Config.isFull() || !player2Config.isFull())
		return false;
	
	type_ = GameType_Server;
	startNewGame(player1Config, player2Config,
		PlayerType_Remote, PlayerType_Remote);
		
	return true;
}

bool Game::loadServerGame(const std::string& base64, const GameUnits& units)
{
	pb::Message msg;
	pb::Buffer buf;
	buf.writeBase64(base64.c_str(), base64.length());
	buf.rewind();
	msg.deserialize(buf);
	return loadServerGame(msg, units);
}

std::string Game::serializedBase64() const
{
	std::auto_ptr<pb::Message> msg(serialized());
	std::auto_ptr<pb::Buffer> buf(msg->serialized());
	std::vector<char> base64 = buf->toBase64();
	return std::string(base64.begin(), base64.end());
}
