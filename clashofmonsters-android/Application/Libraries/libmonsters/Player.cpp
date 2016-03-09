//
//  Player.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "Player.h"
#include "SerializeUnitsIndex.h"
#include "DeserializeUnitsIndex.h"

#include <set>
#include "cocos2d.h"
#include <memory>
#include <stdlib.h>

using namespace std;
using namespace monsters;
using namespace pb;

PlayerConfig::PlayerConfig() :
	avatar_(0)
{
	for (int i=0; i<UnitColor_Count; ++i)
		normalUnits_[i] = 0;
	
	for (int i=0; i<kLargeUnitSlots; ++i)
		largeUnits_[i] = 0;
}

bool PlayerConfig::operator==(const PlayerConfig& config) const
{
	if (avatar_ != config.avatar_)
		return false;

	for (int i=0; i<UnitColor_Count; ++i)
	{
		if (normalUnits_[i] != config.normalUnits_[i])
			return false;
	}
	
	for (int i=0; i<kLargeUnitSlots; ++i)
	{
		if (largeUnits_[i] != config.largeUnits_[i])
			return false;
	}
	
	return true;
}

bool PlayerConfig::operator!=(const PlayerConfig& config) const
{
	return !(*this == config);
}

bool PlayerConfig::isFull() const
{
	for (int i=0; i<UnitColor_Count; ++i)
	{
		if (!normalUnits_[i])
			return false;
	}
	
	for (int i=0; i<kLargeUnitSlots; ++i)
	{
		if (!largeUnits_[i])
			return false;
	}
	
	return true;
}

PlayerConfig::PlayerConfig(const pb::Message& m, const GameUnits& units)
{
	bool ok;
	int avatar_index = m.getFieldValueInt32(2, &ok);
	
	if (ok && avatar_index < (int)units.avatars().size())
	{
		avatar_ = units.avatars().at(avatar_index);
		
		for (int i=0; i<UnitColor_Count; ++i)
		{
			uint32_t unit_index = m.getFieldValueUInt32AtIndex(3, i, &ok);
			if (ok && unit_index < (uint32_t)avatar_->normalUnits().size())
				normalUnits_[i] = avatar_->normalUnits().at(unit_index);
			else
				normalUnits_[i] = 0;
		}
		
		for (int i=0; i<kLargeUnitSlots; ++i)
		{
			uint32_t unit_index = m.getFieldValueUInt32AtIndex(4, i, &ok);
			if (ok && unit_index < (uint32_t)avatar_->largeUnits().size())
				largeUnits_[i] = avatar_->largeUnits().at(unit_index);
			else
				largeUnits_[i] = 0;
		}
	}
	else
	{
		avatar_ = 0;
	}
}

pb::Message* PlayerConfig::serialized() const
{
	Message* m = new Message();
	
	*m << varint::ui(1, 1); // version
	if (avatar_)
	{
		*m << varint::i(2, avatar_->index());
		
		for (int i=0; i<UnitColor_Count; ++i)
		{
			if (normalUnits_[i])
				*m << varint::ui(3, normalUnits_[i]->index());
		}
		
		for (int i=0; i<kLargeUnitSlots; ++i)
		{
			if (largeUnits_[i])
				*m << varint::ui(4, largeUnits_[i]->index());
		}
	}
	else
	{
		*m << varint::i(2, -1);
	}
	
	return m;
}

bool PlayerConfig::hasSelectedNormalUnits() const
{
	for (int i=0; i<UnitColor_Count; ++i)
	{
		if (normalUnits_[i])
			return true;
	}
	
	return false;
}

bool PlayerConfig::hasSelectedLargeUnits() const
{
	for (int i=0; i<kLargeUnitSlots; ++i)
	{
		if (largeUnits_[i])
			return true;
	}
	
	return false;
}


Player::Player()
{
	
}

bool Player::isEqualToPlayer(const Player& player) const
{
	if (index_ != player.index_)
		return false;
		
	if (health_ != player.health_)
		return false;
		
	if (magic_ != player.magic_)
		return false;
		
	if (troopsInReservoir_ != player.troopsInReservoir_)
		return false;
		
	if (largeTroopsInReservoir_ != player.largeTroopsInReservoir_)
		return false;
		
	if (config_ != player.config_)
		return false;
		
	if (randomGenerator_ != player.randomGenerator_)
		return false;
		
	list<AttackFormation*> other_formations(player.attackFormations_.begin(), player.attackFormations_.end());
	for (list<AttackFormation*>::const_iterator i=attackFormations_.begin(), e=attackFormations_.end();
		i!=e; ++i)
	{
		bool found = false;
		for (list<AttackFormation*>::iterator oi=other_formations.begin(), oe=other_formations.end();
			oi!=oe; ++oi)
		{
			if ((**oi).isEqualToFormation(**i))
			{
				found = true;
				other_formations.erase(oi);
				break;
			}
		}
		
		if (!found)
			return false;
	}
	
	if (field_ != player.field_)
		return false;

	return true;
}

pb::Message* Player::serialized() const
{
	using namespace pb;
	Message* m = new Message();
	
	*m
		<< varint::ui(1, index_)
		
		<< varint::i(3, health_)
		<< varint::i(4, magic_)
		<< varint::i(5, troopsInReservoir_)
		<< varint::i(6, largeTroopsInReservoir_)
		<< varint::i(7, turnsLeft_)
	
		<< msg(8, config_.serialized())
		<< msg(9, randomGenerator_.serialized());
		
	//map<Unit*, int> ids_by_unit;
	SerializeUnitsIndex ids_by_unit;
	
	for (list<AttackFormation*>::const_iterator i=attackFormations_.begin(), e=attackFormations_.end();
		i!=e; ++i)
	{
		*m << msg(10, (*i)->serialized(ids_by_unit));
	}
	
	*m << msg(11, field_.serialized(ids_by_unit));
	
	// serialize units
	ids_by_unit.serialize(*m, 12, attackFormations_);
	
	return m;
}

bool Player::deserialize(const pb::Message& msg, PlayerType type, const GameUnits& units)
{
	index_ = msg.getFieldValueUInt32(1);
	type_ = type;
	
	health_ = msg.getFieldValueInt32(3);
	magic_ = msg.getFieldValueInt32(4);
	troopsInReservoir_ = msg.getFieldValueInt32(5);
	largeTroopsInReservoir_ = msg.getFieldValueInt32(6);
	turnsLeft_ = msg.getFieldValueInt32(7);
	
	config_ = PlayerConfig(*msg.getFieldValueMessage(8), units);
	if (!msg.getFieldValueMessage(9) ||
			!randomGenerator_.deserialize(*msg.getFieldValueMessage(9)))
		return false;
		
	DeserializeUnitsIndex units_index;
	
	// deseralize units itself
	if (!units_index.deserialize(msg, 12, units))
		return false;
	
	// deserialize field
	if (!msg.getFieldValueMessage(11) ||
			!field_.deserialize(*msg.getFieldValueMessage(11), units_index))
		return false;
	
	// deserialize attack formations
	for (int i=0; i<msg.countValues(10); ++i)
	{
		AttackFormation* atack_formation = new AttackFormation();
		if (!msg.getFieldValueMessageAtIndex(10, i) ||
				!atack_formation->deserialize(*msg.getFieldValueMessageAtIndex(10, i), units_index))
		{
			return false;
		}
		else
		{
			attackFormations_.push_back(atack_formation);
		}
	}
	
	return true;
}

void Player::clear()
{
	for (list<AttackFormation*>::iterator i=attackFormations_.begin(), e=attackFormations_.end();
		i!=e; ++i)
	{
		delete *i;
	}
	attackFormations_.clear();

	field_.clear();
	
	randomGenerator_.srand(time(0) + rand());
}

Unit* Player::unit(const CellCoordinate& c)
{
	return field_.unit(c);
}

const Unit* Player::unit(const CellCoordinate& c) const
{
	return field_.unit(c);
}

const Unit* Player::occupyingUnit(const CellCoordinate& c) const
{
	return field_.occupyingUnit(c);
}

Unit* Player::occupyingUnit(const CellCoordinate& c)
{
	return field_.occupyingUnit(c);
}

bool Player::isCellEmpty(const CellCoordinate& c, const Unit* except) const
{
	return field_.isCellEmpty(c, except);
}

int Player::lastFreeRow(int col, const Unit* except) const
{
	for (int r=kFieldHeight-1; r>=0; --r)
	{
		if (!isCellEmpty(CellCoordinate(col, r), except))
			return (r < (kFieldHeight - 1)) ? (r+1) : -1;
	}
	
	return 0;
}

const Unit* Player::lastUnitInColumn(int col) const
{
	for (int r=kFieldHeight-1; r>=0; --r)
	{
		if (const Unit* u = occupyingUnit(CellCoordinate(col, r)))
			return u;
	}
	
	return 0;
}

int Player::rowToPlace(int col, UnitClass uc, const Unit* except) const
{
	CC_ASSERT(col >= 0 && col < kFieldWidth);
	
	// check is too big to be placed in the last col
	if (col == (kFieldWidth-1) && uc == UnitClass_Large4)
		return -1;
	
	int last_row = lastFreeRow(col, except);
	if (uc == UnitClass_Normal)
		return last_row;
			
	if (last_row != -1)
	{
		if (uc == UnitClass_Large2 && last_row < (kFieldHeight - 1))
			return last_row;
			
		if (uc == UnitClass_Large4)
		{
			while (last_row < kFieldHeight - 1)
			{
				if (isCellEmpty(CellCoordinate(col+1, last_row), except))
					return last_row;
				
				++last_row;
			}
		}
	}
	
	return -1;
}

int Player::rowToPlaceAnywhere(int col, UnitClass uc, const Unit* except) const
{
	CC_ASSERT(col >= 0 && col < kFieldWidth);
	
	for (int r=0; r<(kFieldHeight - getUnitDepth(uc) - 1); ++r)
	{
		bool can_place = true;
		for (int c=col; can_place && c<(col+getUnitWidth(uc)); ++c)
		{
			for (int row=r; can_place && row<(r+getUnitDepth(uc)); ++row)
			{
				if (!isCellEmpty(CellCoordinate(c, row)))
				{
					can_place = false;
				}
			}
		}
		
		if (can_place)
		{
			return r;
		}
	}
	
	return -1;
}

bool Player::canPlace(UnitClass uc) const
{
	for (int c=0; c<kFieldWidth; ++c)
	{
		if (rowToPlace(c, uc) != -1)
			return true;
	}
	
	return false;
}

bool PlayerConfig::hasUnitClass(UnitClass uc) const
{
	switch (uc)
	{
		case UnitClass_Normal:
			return true;
		case UnitClass_Large2:
			return (largeUnits_[0] && largeUnits_[0]->unitClass() == UnitClass_Large2) ||
				(largeUnits_[1] && largeUnits_[1]->unitClass() == UnitClass_Large2);
		case UnitClass_Large4:
			return (largeUnits_[0] && largeUnits_[0]->unitClass() == UnitClass_Large4) ||
				(largeUnits_[1] && largeUnits_[1]->unitClass() == UnitClass_Large4);
	}
	
	return false;
}

UnitType* Player::randomLargeUnitTypeForClass(UnitClass uc)
{
	int rnd = randomGenerator_.randInt() % kLargeUnitSlots;
	for (int i=0; i<kLargeUnitSlots; ++i)
	{
		int index = (rnd + i) % kLargeUnitSlots;
		if (config_.largeUnitAtIndex(index) && config_.largeUnitAtIndex(index)->unitClass() == uc)
			return config_.largeUnitAtIndex(index);
	}
	
	return 0;
}

void Player::alignTroops()
{
	field_.alignTroops();
}

void Player::removeUnit(const Unit* unit)
{
	CC_ASSERT(unit == field_.unit(unit->coord()));
	
	//CellCoordinate unit_coord = unit->coord();
	Unit* unit_r = field_.unit(unit->coord());
	field_.setUnit(unit->coord(), 0);
	
	putUnitBackToReservoir(unit_r, true);
	//delete unit_r;
	
	alignTroops();
}

bool Player::canMoveUnit(const Unit* unit) const
{
	if (unit->isWallOrAttack())
		return false;
	
	for (int c=0; c<unit->width(); ++c)
	{
		for (int r=unit->coord().row+unit->depth(); r<kFieldHeight; ++r)
		{
			if (!isCellEmpty(CellCoordinate(unit->coord().col + c, r)))
				return false;
		}
	}
	
	return true;
}

void Player::moveUnit(const Unit* unit, int col)
{
	int row = rowToMoveUnit(col, unit);
	
	CC_ASSERT(row != -1);
	CC_ASSERT(field_.unit(unit->coord()) == unit);
	
	field_.setUnit(unit->coord(), 0);
	
	CC_ASSERT(field_.unit(col, row) == 0);
	
	field_.setUnit(col, row, const_cast<Unit*>(unit));
	field_.unit(col, row)->coord_ = CellCoordinate(col, row);
}

int Player::rowToMoveUnit(int col, const Unit* unit) const
{
	return rowToPlace(col, unit->unitClass(), unit);
}

AttackFormation* Player::processComboFormation(AttackFormation* formation)
{
	AttackFormation* combo_formation = 0;
	Unit* unit = formation->units_.at(0);
	if (unit->coord().row > 0)
	{
		Unit* front_unit = occupyingUnit(CellCoordinate(unit->coord().col, unit->coord().row-1));
		combo_formation = front_unit ? front_unit->attackFormation_ : 0;
		if (combo_formation &&
			combo_formation->units().at(0)->unitType() == formation->units().at(0)->unitType() &&
			combo_formation->units().at(0)->color() == formation->units().at(0)->color()
			/* && combo_formation->roundsToCharge_ == formation->roundsToCharge_*/)
		{
			// combo found
			for (vector<Unit*>::iterator i=formation->units_.begin(), e=formation->units_.end();
				i!=e; ++i)
			{
				Unit* u = *i;
				field_.setUnit(u->coord(), 0);
				u->attackFormation_ = combo_formation;
				combo_formation->absorbedUnits_.push_back(u);
				combo_formation->totalStrength_ = (combo_formation->totalStrength_ * kComboAttackStrengthCoefficient).ceilInt();
			}
			
			for (vector<Unit*>::iterator i=formation->absorbedUnits_.begin(), e=formation->absorbedUnits_.end();
				i!=e; ++i)
			{
				Unit* u = *i;
				u->attackFormation_ = combo_formation;
				combo_formation->absorbedUnits_.push_back(u);
				combo_formation->totalStrength_ = (combo_formation->totalStrength_ * kComboAttackStrengthCoefficient).ceilInt();
			}
			
			addMagic(kMagicPerCombo);
			combo_formation->roundsToCharge_ = std::min(combo_formation->roundsToCharge_, formation->roundsToCharge_);
			combo_formation->combos_++;
		}
		else
		{
			combo_formation = 0;
		}
	}
	
	if (combo_formation)
	{
		formation->absorbedUnits_.clear();
	}
	
	return combo_formation;
}

int Player::processFormations(int round, bool findOnly, std::list<ProcessFormationsHint>* hints, bool forceCascade)
{
	int count = 0;
	bool found, found_wall;
	
	set<AttackFormation*> new_attack_formations;
	set<Unit*> new_wall_centers;
	set<Unit*> new_walls;
	
	do
	{
		found = false;
		found_wall = false;
		
		// process walls
		for (int r=0; /*!found && */r<kFieldHeight; ++r)
		{
			for (int c=0; /*!found && */c<kFieldWidth; ++c)
			{
				if (Unit* unit = field_.unit(c, r))
				{
					if (!unit->isWall_ && !unit->attackFormation_ &&
						unit->unitClass() == UnitClass_Normal)
					{
						// check for wall
						int wall_width = 1;
						for (int cc=c+1; cc<kFieldWidth; ++cc)
						{
							if (Unit* unit2 = field_.unit(cc, r))
							{
								if (unit2->unitClass() == UnitClass_Normal &&
									unit2->unitType() == unit->unitType() &&
									unit2->color() == unit->color() &&
									!unit2->isWallOrAttack())
								{
									++wall_width;
								}
								else
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
						
						if (wall_width >= 3)
						{
							// we have found the wall
							if (!findOnly)
							{
								for (int cc=c; cc<(c+wall_width); ++cc)
								{
									Unit* unit2 = field_.unit(cc, r);
									unit2->isWall_ = true;
									unit2->wallStrength_ = unit2->unitType()->avatar()->wallStrength(0);
									unit2->wallLevel_ = 0;
									new_walls.insert(unit2);
								}
							}
							
							new_wall_centers.insert(field_.unit(c + wall_width/2, r));
							//found = true;
							found_wall = true;
							++count;
						}
					}
				}
			}
		}
		
		// process formations
		for (int r=0; !found && r<kFieldHeight; ++r)
		{
			for (int c=0; !found && c<kFieldWidth; ++c)
			{
				if (Unit* unit = field_.unit(c, r))
				{
					if (!unit->isWall_ && !unit->attackFormation_)
					{
						switch (unit->unitClass())
						{
							case UnitClass_Normal:
							{
								// check for attack formation
								int formation_depth = 1;
								for (int rr=r+1; rr<kFieldHeight; ++rr)
								{
									if (Unit* unit2 = field_.unit(c, rr))
									{
										if (unit2->unitClass() == UnitClass_Normal &&
											unit2->unitType() == unit->unitType() &&
											unit2->color() == unit->color() &&
											(!unit2->isWallOrAttack() || new_walls.find(unit2) != new_walls.end()))
										{
											++formation_depth;
										}
										else
										{
											break;
										}
									}
									else
									{
										break;
									}
								}
								
								if (formation_depth >= kMaxFormationDepth)
								{
									if (!findOnly)
									{
										// we have found attack formation
										
										// check if it is possible to create it
										// (it might not be possible because of stupid new rule: "it should create shooter and wall")
										int unoccupied = kFieldHeight;
										for (int rr2=0; rr2<kFieldHeight; ++rr2)
										{
											if (field_.occupyingUnit(CellCoordinate(c, rr2)))
												--unoccupied;
										}
										
										int unoccupied_required = 0;
										for (int rr=r; rr<(r + kMaxFormationDepth); ++rr)
										{
											Unit* unit2 = field_.unit(c, rr);
											if (unit2->isWall())
												++unoccupied_required;
										}
										
										if (unoccupied_required > unoccupied)
										{
											// oops, no space here for spare unit
											break;
										}
										
										AttackFormation* formation = new AttackFormation();
										formation->totalRoundsToCharge_ = formation->roundsToCharge_ = unit->unitType()->roundsToCharge();
										formation->totalStrength_ = 0;
										for (int rr=r; rr<(r + kMaxFormationDepth); ++rr)
										{
											Unit* unit2 = field_.unit(c, rr);
											
											if (unit2->isWall())
											{
												// that's the stupid case, we need to create another unit
												//CellCoordinate coord = unit2->coord_;
												unit2 = new Unit(unit2->unitType_, unit2->color());
												//unit2->coord_ = coord;
												unit2->player_ = index_;
												unit2->weightless_ = true;
												
												for (int rr2=0; rr2<kFieldHeight; ++rr2)
												{
													CellCoordinate coord(c, rr2);
													if (!field_.occupyingUnit(coord))
													{
														unit2->coord_ = coord;
														field_.setUnit(coord, unit2);
														break;
													}
												}
											}
											
											unit2->attackFormation_ = formation;
											formation->units_.push_back(unit2);
											formation->totalStrength_ += unit2->unitType()->offensive();
										}
										formation->subtractedStrength_ = 0;
										
										// check for combo
										AttackFormation* combo_formation = processComboFormation(formation);
										if (!combo_formation)
										{
											attackFormations_.push_back(formation);
											new_attack_formations.insert(formation);
										}
										else
										{
											delete formation;
											if (hints)
												hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Combo,
													combo_formation->units().at(0)->coord(), index_));
										}
									}
									
									found = true;
									++count;
								}
							}
							break;
							
							case UnitClass_Large2:
							{
								if ((r+2) < (kFieldHeight - 1) &&
									field_.unit(c, r+2) && !field_.unit(c, r+2)->unitType()->isLarge() &&
										field_.unit(c, r+2)->color() == unit->color() &&
									field_.unit(c, r+3) && !field_.unit(c, r+3)->unitType()->isLarge() &&
										field_.unit(c, r+3)->color() == unit->color())
								{
									if (!findOnly)
									{
										AttackFormation* formation = new AttackFormation();
										formation->totalRoundsToCharge_ = formation->roundsToCharge_ = unit->unitType()->roundsToCharge();
										formation->totalStrength_ = unit->unitType()->offensive();
										formation->units_.push_back(unit);
										unit->attackFormation_ = formation;
										
										for (int rr=r+2; rr<(r+4); ++rr)
										{
											Unit* unit2 = field_.unit(c, rr);
											
											if (unit2->isWall())
											{
												// that's the stupid case, we need to create another unit
												//CellCoordinate coord = unit2->coord_;
												unit2 = new Unit(unit2->unitType_, unit2->color());
												unit2->player_ = index_;
												//unit2->coord_ = coord;
												unit2->weightless_ = true;
											}
											else
											{
												field_.setUnit(c, rr, 0);
											}
											
											unit2->attackFormation_ = formation;
											formation->absorbedUnits_.push_back(unit2);
											formation->totalStrength_ += unit2->unitType()->offensive();
											
											
											putUnitBackToReservoir(unit2, true);
										}
										formation->subtractedStrength_ = 0;
										
										// check for combo
										AttackFormation* combo_formation = processComboFormation(formation);
										if (!combo_formation)
										{
											attackFormations_.push_back(formation);
											new_attack_formations.insert(formation);
										}
										else
										{
											delete formation;
											if (hints)
												hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Combo,
													combo_formation->units().at(0)->coord(), index_));
										}
									}
								
									found = true;
									++count;
								}
							}
							break;
							
							case UnitClass_Large4:
							{
								if ((r+2) < (kFieldHeight - 1) &&
									field_.unit(c, r+2) && !field_.unit(c, r+2)->unitType()->isLarge() &&
										field_.unit(c, r+2)->color() == unit->color() &&
									field_.unit(c, r+3) && !field_.unit(c, r+3)->unitType()->isLarge() &&
										field_.unit(c, r+3)->color() == unit->color() &&
									field_.unit(c+1, r+2) && !field_.unit(c+1, r+2)->unitType()->isLarge() &&
										field_.unit(c+1, r+2)->color() == unit->color() &&
									field_.unit(c+1, r+3) && !field_.unit(c+1, r+3)->unitType()->isLarge() &&
										field_.unit(c+1, r+3)->color() == unit->color())
								{
									if (!findOnly)
									{
										AttackFormation* formation = new AttackFormation();
										formation->totalRoundsToCharge_ = formation->roundsToCharge_ = unit->unitType()->roundsToCharge();
										formation->totalStrength_ = unit->unitType()->offensive();
										formation->units_.push_back(unit);
										unit->attackFormation_ = formation;
										
										for (int cc=c; cc<=(c+1); ++cc)
										{
											for (int rr=r+2; rr<(r+4); ++rr)
											{
												Unit* unit2 = field_.unit(cc, rr);
												
												if (unit2->isWall())
												{
													// that's the stupid case, we need to create another unit
													//CellCoordinate coord = unit2->coord_;
													unit2 = new Unit(unit2->unitType_, unit2->color());
													//unit2->coord_ = coord;
													unit2->player_ = index_;
													unit2->weightless_ = true;
												}
												else
												{
													field_.setUnit(cc, rr, 0);
												}
												
												unit2->attackFormation_ = formation;
												formation->absorbedUnits_.push_back(unit2);
												formation->totalStrength_ += unit2->unitType()->offensive();
												
												putUnitBackToReservoir(unit2, true);
											}
										}
										formation->subtractedStrength_ = 0;
										
										// check for combo
										AttackFormation* combo_formation = processComboFormation(formation);
										if (!combo_formation)
										{
											attackFormations_.push_back(formation);
											new_attack_formations.insert(formation);
										}
										else
										{
											delete formation;
											if (hints)
												hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Combo,
													combo_formation->units().at(0)->coord(), index_));
										}
									}
								
									found = true;
									++count;
								}
							}
							break;
						}
					}
				}
			}
		}
		
		if (count > 0 && !findOnly)
		{
			// arrange units
			arrangeTroops();
		}
	}
	while ((found || found_wall) && !findOnly);
	
	// search for synced attacks
	if (hints)
	{
		for (list<AttackFormation*>::iterator i=attackFormations_.begin(), e=attackFormations_.end(); i!=e; ++i)
		{
			if (new_attack_formations.find(*i) != new_attack_formations.end())
			{
				// recently found formation
				// check if it is synced with anything else
				bool found = false;
				for (list<AttackFormation*>::iterator i2=attackFormations_.begin(), e2=attackFormations_.end(); i2!=e2; ++i2)
				{
					if ((*i) != (*i2) && (*i)->roundsToCharge_ == (*i2)->roundsToCharge_)
					{
						found = true;
						hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Sync, (*i2)->baseUnit()->coord(), index_));
						
						set<AttackFormation*>::iterator ni = new_attack_formations.find(*i2);
						if (ni != new_attack_formations.end())
							new_attack_formations.erase(ni);
					}
				}
				
				if (found)
				{
					hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Sync, (*i)->baseUnit()->coord(), index_));
					
					set<AttackFormation*>::iterator ni = new_attack_formations.find(*i);
					if (ni != new_attack_formations.end())
						new_attack_formations.erase(ni);
				}
			}
		}
	}
	
	if (hints)
	{
		if (((new_attack_formations.size() + new_wall_centers.size()) > 1) ||
			((new_attack_formations.size() + new_wall_centers.size()) > 0 && forceCascade))
		{
			for (set<AttackFormation*>::const_iterator i=new_attack_formations.begin(),
				e=new_attack_formations.end(); i!=e; ++i)
			{
				hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Cascade,
					(*i)->baseUnit()->coord(), index_));
			}
			
			for (set<Unit*>::const_iterator i=new_wall_centers.begin(),
				e=new_wall_centers.end(); i!=e; ++i)
			{
				hints->push_back(ProcessFormationsHint(ProcessFormationsHint::Cascade,
					(*i)->coord(), index_));
			}
		}
	}
	
	return count;
}

bool Player::isUnitInFrontRow(const Unit* unit, bool ignoreWalls) const
{
	for (int w=0; w<unit->width(); ++w)
	{
		for (int r=unit->coord().row-1; r>=0; --r)
		{
			const Unit* ou = occupyingUnit(CellCoordinate(r, unit->coord().col+w));
			if (ou && ou != unit && (!ou->isWall() || ignoreWalls))
				return false;
		}
	}
	
	return true;
}

void Player::arrangeTroops()
{
	set<Unit*> arranged;
	Field old_field = field_;
	field_.clear(false);
	
	// merge walls
	for (int c=0; c<kFieldWidth; ++c)
	{
		bool wall_merged;
		do
		{
			wall_merged = false;
			Unit* wall1 = 0;
			for (int r=0; !wall_merged && r<kFieldHeight; ++r)
			{
				if (old_field.unit(c, r) && old_field.unit(c, r)->isWall() &&
					old_field.unit(c, r)->wallLevel() == 0)
				{
					if (!wall1)
						wall1 = old_field.unit(c, r);
					else
					{
						wall1->absorbedUnits_.push_back(old_field.unit(c, r));
						wall1->wallLevel_ = 1;
						wall1->wallStrength_ += wall1->unitType()->avatar()->wallStrength(1) -
							wall1->unitType()->avatar()->wallStrength(0);
						wall_merged = true;
						
						putUnitBackToReservoir(old_field.unit(c, r), true);
						
						old_field.unit(c, r)->absorberWall_ = wall1;
						old_field.setUnit(c, r, 0);
					}
				}
			}
		}
		while (wall_merged);
	}
	

	// place formed units
	for (int t=0; t<2; ++t)
	{
		for (int c=0; c<kFieldWidth; ++c)
		{
			Unit* most_front_unit;
			do
			{
				most_front_unit = 0;
				for (int rr=0; rr<kFieldHeight; ++rr)
				{
					if (Unit* unit = old_field.unit(c, rr))
					{
						if (t == 0 && unit->isWall())
						{
							// walls
							if (!most_front_unit)
							{
								most_front_unit = unit;
							}
							else if (unit->wallLevel_ < most_front_unit->wallLevel_)
							{
								most_front_unit = unit;
							}
						}
						else if (t == 1 && unit->attackFormation())
						{
							// attack formations
							if (!most_front_unit)
							{
								most_front_unit = unit;
							}
							else if ((unit->attackFormation()->roundsToCharge() <
									most_front_unit->attackFormation()->roundsToCharge() &&
									unit->attackFormation()->baseUnit()->unitClass() == most_front_unit->unitClass()) ||
									(unit->attackFormation()->baseUnit()->unitClass() != UnitClass_Normal &&
										most_front_unit->unitClass() == UnitClass_Normal))
							{
								most_front_unit = unit;
							}
						}
					}
				}
				
				if (most_front_unit)
				{
					old_field.setUnit(most_front_unit->coord().col, most_front_unit->coord().row, 0);
					int new_row = rowToPlace(c, most_front_unit->unitClass());
					//CC_ASSERT(new_row != -1);
					if (new_row == -1)
						new_row = rowToPlaceAnywhere(c, most_front_unit->unitClass());
					CC_ASSERT(new_row != -1);
					
					most_front_unit->coord_.row = new_row;
					field_.setUnit(most_front_unit->coord().col, most_front_unit->coord().row, most_front_unit);
				}
			}
			while (most_front_unit);
		}
	}
	
	// place all other units
	for (int r=0; r<kFieldHeight; ++r)
	{
		for (int c=0; c<kFieldWidth; ++c)
		{
			if (Unit* unit = old_field.unit(c, r))
			{
				int new_col = c, new_row;
				for (; new_col < kFieldWidth; ++new_col)
				{
					new_row = rowToPlace(new_col, unit->unitClass());
					if (new_row == -1)
						new_row = rowToPlaceAnywhere(new_col, unit->unitClass());
					if (new_row != -1)
						break;
				}	
				
				CC_ASSERT(new_row != -1);
				
				unit->coord_.row = new_row;
				unit->coord_.col = new_col;
				field_.setUnit(unit->coord().col, unit->coord().row, unit);
			}
		}
	}
}

void Player::putUnitBackToReservoir(const Unit* unit, bool largeTroopRestorable)
{
	troopsInReservoir_ += unit->troopsWeight();
	if (unit->unitType()->isLarge() && largeTroopRestorable)
		largeTroopsInReservoir_++;
}

void Player::addMagic(int magic)
{
	magic_ = min(magic_ + magic, kMaxMagic);
}

int Player::unitClassCountOnField(UnitClass uc) const
{
	int count = 0;
	for (int r=0; r<kFieldHeight; ++r)
	{
		for (int c=0; c<kFieldWidth; ++c)
		{
			if (field_.unit(c, r) &&
				field_.unit(c, r)->unitClass() == uc)
			{
				++count;
			}
		}
	}
	
	return count;
}

void Player::fillTroopsFromReservoir()
{
	while (troopsInReservoir_ && canPlace(UnitClass_Normal))
	{
		UnitClass uc = UnitClass_Normal;
		if (largeTroopsInReservoir_ > 0 && troopsInReservoir_ >= 4 && canPlace(UnitClass_Large4) &&
			config_.hasUnitClass(UnitClass_Large4))
		{
			int rnd = randomGenerator_.randInt() % 30;
			if (rnd < 24)
				uc = UnitClass_Normal;
			else if (rnd < 27)
				uc = UnitClass_Large2;
			else
				uc = UnitClass_Large4;
		}
		else if (largeTroopsInReservoir_ > 0 && troopsInReservoir_ >= 2  && canPlace(UnitClass_Large2) &&
			config_.hasUnitClass(UnitClass_Large2))
		{
			//uc = (UnitClass)(randomGenerator_.randInt() % 2);
			int rnd = randomGenerator_.randInt() % 30;
			if (rnd < 25)
				uc = UnitClass_Normal;
			else
				uc = UnitClass_Large2;
		}
		
		if (unitClassCountOnField(uc) >= kMaxUnitClassOnField[uc])
			uc = UnitClass_Normal;
		
		int col = randomGenerator_.randInt() % kFieldWidth;
		int row = rowToPlace(col, uc);
		while (row == -1)
		{
			col = (col + 1) % kFieldWidth;
			row = rowToPlace(col, uc);
		}
		
		UnitColor color = (UnitColor)(randomGenerator_.randInt() % UnitColor_Count);
		UnitType* ut = (uc == UnitClass_Normal) ? config_.normalUnitAtIndex(color) :
			randomLargeUnitTypeForClass(uc);
		
		// in case we can't pick large unit
		if (!ut)
		{
			uc = UnitClass_Normal;
			ut = config_.normalUnitAtIndex(color);
		}
		
		Unit* u = new Unit(ut, color);
		
		u->player_ = index_;
		u->coord_ = CellCoordinate(col, row);
		
		CC_ASSERT(!field_.unit(col, row));
		CC_ASSERT(isCellEmpty(u->coord_));
		field_.setUnit(col, row, u);
		
		if (processFormations(0, true))
		{
			// rollback
			field_.setUnit(col, row, 0);
			delete u;
		}
		else
		{
			troopsInReservoir_ -= u->troopsWeight();
		}
	}
}

PlayerConfig PlayerConfig::deserialized(const std::string& base64, const GameUnits& units)
{
	pb::Message msg;
	pb::Buffer buf;
	buf.writeBase64(base64.c_str(), base64.length());
	buf.rewind();
	msg.deserialize(buf);
	return PlayerConfig(msg, units);
}

std::string PlayerConfig::serializedBase64() const
{
	std::auto_ptr<pb::Message> msg(serialized());
	std::auto_ptr<pb::Buffer> buf(msg->serialized());
	std::vector<char> base64 = buf->toBase64();
	return std::string(base64.begin(), base64.end());
}

