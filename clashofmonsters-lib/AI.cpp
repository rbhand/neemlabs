//
//  AI.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "AI.h"
#include <set>
#include <algorithm>
#include <iostream>

using namespace monsters;
using namespace std;


AIContext::AIContext(const Field& field) :
	field_(field), depth_(0), score_(0)
{
}

AIContext::AIContext(const AIContext& ctx) :
	field_(ctx.field_), walls_(ctx.walls_), actions_(ctx.actions_),
	depth_(ctx.depth_+1), score_(ctx.score_)
{
	for (list<AttackFormation*>::const_iterator i=ctx.newAttackFormations_.begin(),
		e=ctx.newAttackFormations_.end(); i!=e; ++i)
	{
		AttackFormation* f = new AttackFormation(*(*i));
		newAttackFormations_.push_back(f);
	}
}

AIContext::~AIContext()
{
	for (list<AttackFormation*>::iterator i=newAttackFormations_.begin(),
		e=newAttackFormations_.end(); i!=e; ++i)
	{
		delete *i;
	}
}

bool AIContext::unitIsWall(const Unit* unit) const
{
	return unit->isWall() ||
		walls_.find(unit) != walls_.end();
}

bool AIContext::unitIsInAttackFormation(const Unit* unit) const
{
	if (unit->attackFormation())
		return true;
	
	for (list<AttackFormation*>::const_iterator i=newAttackFormations_.begin(),
		e=newAttackFormations_.end(); i!=e; ++i)
	{
		if (find((*i)->units().begin(), (*i)->units().end(), unit) != (*i)->units().end())
			return true;
		if (find((*i)->absorbedUnits().begin(), (*i)->absorbedUnits().end(), unit) != (*i)->absorbedUnits().end())
			return true;
	}
	
	return false;
}

const AttackFormation* AIContext::unitAttackFormation(const Unit* unit) const
{
	if (unit->attackFormation())
		return unit->attackFormation();
	
	for (list<AttackFormation*>::const_iterator i=newAttackFormations_.begin(),
		e=newAttackFormations_.end(); i!=e; ++i)
	{
		if (find((*i)->units().begin(), (*i)->units().end(), unit) != (*i)->units().end())
			return *i;
		if (find((*i)->absorbedUnits().begin(), (*i)->absorbedUnits().end(), unit) != (*i)->absorbedUnits().end())
			return *i;
	}
	
	return 0;
}

bool AIContext::unitIsWallOrAttack(const Unit* unit) const
{
	return unitIsWall(unit) || unitIsInAttackFormation(unit);
}

int AIContext::processFormations()
{
	int count = 0;
	bool found;
	
	do
	{
		found = false;
		
		for (int r=0; !found && r<kFieldHeight; ++r)
		{
			for (int c=0; !found && c<kFieldWidth; ++c)
			{
				if (Unit* unit = field_.unit(c, r))
				{
					if (!unitIsWall(unit) && !unitIsInAttackFormation(unit))
					{
						switch (unit->unitClass())
						{
							case UnitClass_Normal:
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
											!unitIsWallOrAttack(unit2))
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
									for (int cc=c; cc<(c+wall_width); ++cc)
									{
										Unit* unit2 = field_.unit(cc, r);
										walls_.insert(unit2);
										
										score_ += unit2->unitType()->avatar()->wallStrength(0) * (3 - depth_);
										
										// TODO
//												unit2->isWall_ = true;
//												unit2->wallStrength_ = unit2->unitType()->avatar()->wallStrength(0);
//												unit2->wallLevel_ = 0;
									}
									
									found = true;
									++count;
								}
								
								// check for attack formation
								int formation_depth = 1;
								for (int rr=r+1; rr<kFieldHeight; ++rr)
								{
									if (Unit* unit2 = field_.unit(c, rr))
									{
										if (unit2->unitClass() == UnitClass_Normal &&
											unit2->unitType() == unit->unitType() &&
											unit2->color() == unit->color() &&
											!unitIsWallOrAttack(unit2))
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
									// we have found attack formation
									AttackFormation* formation = new AttackFormation();
									formation->totalRoundsToCharge_ = formation->roundsToCharge_ = unit->unitType()->roundsToCharge();
									formation->totalStrength_ = 0;
									for (int rr=r; rr<(r + kMaxFormationDepth); ++rr)
									{
										Unit* unit2 = field_.unit(c, rr);
										formation->units_.push_back(unit2);
										formation->totalStrength_ += unit2->unitType()->offensive();
									}
									formation->subtractedStrength_ = 0;
									
									// check for combo was here, but we ignore it for AI
									
									newAttackFormations_.push_back(formation);
									
									// TODO
									score_ += (formation->totalStrength_ * (4 - depth_));

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

									AttackFormation* formation = new AttackFormation();
									formation->totalRoundsToCharge_ = formation->roundsToCharge_ = unit->unitType()->roundsToCharge();
									formation->totalStrength_ = unit->unitType()->offensive();
									formation->units_.push_back(unit);
									
									for (int rr=r+2; rr<(r+4); ++rr)
									{
										Unit* unit2 = field_.unit(c, rr);
										formation->units_.push_back(unit2); // absorbed
										formation->totalStrength_ += unit2->unitType()->offensive();
										field_.setUnit(c, rr, 0);
									}
									formation->subtractedStrength_ = 0;
									
									// check for combo was here, but we ignore it for AI

									newAttackFormations_.push_back(formation);
									
									// TODO
									score_ += (formation->totalStrength_ * (3 - depth_));
								
									found = true;
									++count;
								}
							}
							break;
							
							case UnitClass_Large4:
							{
								if ((r+2) < (kFieldHeight - 1) &&
									field_.unit(c, r+2) && !field_.unit(c, r+2)->unitType()->isLarge()
										&& field_.unit(c, r+2)->color() == unit->color() &&
									field_.unit(c, r+3) && !field_.unit(c, r+3)->unitType()->isLarge()
										&& field_.unit(c, r+3)->color() == unit->color() &&
									field_.unit(c+1, r+2) && !field_.unit(c+1, r+2)->unitType()->isLarge()
										&& field_.unit(c+1, r+2)->color() == unit->color() &&
									field_.unit(c+1, r+3) && !field_.unit(c+1, r+3)->unitType()->isLarge() &&
										field_.unit(c+1, r+3)->color() == unit->color())
								{
									AttackFormation* formation = new AttackFormation();
									formation->totalRoundsToCharge_ = formation->roundsToCharge_ = unit->unitType()->roundsToCharge();
									formation->totalStrength_ = unit->unitType()->offensive();
									formation->units_.push_back(unit);
									
									for (int cc=c; cc<=(c+1); ++cc)
									{
										for (int rr=r+2; rr<(r+4); ++rr)
										{
											Unit* unit2 = field_.unit(cc, rr);
											formation->units_.push_back(unit2); // absorbed
											formation->totalStrength_ += unit2->unitType()->offensive();
											field_.setUnit(cc, rr, 0);
										}
									}
									formation->subtractedStrength_ = 0;
									
									// check for combo was here, but we ignore it for AI
									
									newAttackFormations_.push_back(formation);
									
									// TODO
									score_ += (formation->totalStrength_ * (3 - depth_));

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
		
		if (found)
		{
			// arrange units
			arrangeTroops();
		}
	}
	while (found);
	
	return count;
}

bool AIContext::isCellEmpty(const CellCoordinate& c, const Unit* except) const
{
	return field_.isCellEmpty(c, except);
}

int AIContext::lastFreeRow(int col, const Unit* except) const
{
	for (int r=kFieldHeight-1; r>=0; --r)
	{
		if (!isCellEmpty(CellCoordinate(col, r), except))
			return (r < (kFieldHeight - 1)) ? (r+1) : -1;
	}
	
	return 0;
}

int AIContext::lastOccupiedRow(int col) const
{
	for (int r=kFieldHeight-1; r>=0; --r)
	{
		if (!isCellEmpty(CellCoordinate(col, r)))
			return r;
	}
	
	return -1;
}

int AIContext::rowToPlace(int col, UnitClass uc, const Unit* except) const
{
	assert(col >= 0 && col < kFieldWidth);
	
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

int AIContext::rowToPlaceAnywhere(int col, UnitClass uc, const Unit* except) const
{
	assert(col >= 0 && col < kFieldWidth);
	
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

void AIContext::arrangeTroops()
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
				if (old_field.unit(c, r) && unitIsWall(old_field.unit(c, r)) &&
					old_field.unit(c, r)->wallLevel() == 0)
				{
					if (!wall1)
						wall1 = old_field.unit(c, r);
					else
					{
//								wall1->absorbedUnits_.push_back(old_field.unit(c, r));
//								wall1->wallLevel_ = 1;
//								wall1->wallStrength_ += wall1->unitType()->avatar()->wallStrength(1) -
//									wall1->unitType()->avatar()->wallStrength(0);

						// TODO
						wall_merged = true;
						
						//old_field.unit(c, r)->absorberWall_ = wall1;
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
						if (t == 0 && unitIsWall(unit))
						{
							// walls
							if (!most_front_unit)
							{
								most_front_unit = unit;
							}
							else if (unit->wallLevel() < most_front_unit->wallLevel())
							{
								most_front_unit = unit;
							}
						}
						else if (t == 1 && unitIsInAttackFormation(unit))
						{
							// attack formations
							if (!most_front_unit)
							{
								most_front_unit = unit;
							}
							else if (unitAttackFormation(unit)->roundsToCharge() <
								unitAttackFormation(most_front_unit)->roundsToCharge())
							{
								most_front_unit = unit;
							}
						}
					}
				}
				
				if (most_front_unit)
				{
					CellCoordinate coord = old_field.coord(most_front_unit);
					old_field.setUnit(coord.col, coord.row, 0);
					int new_row = rowToPlace(c, most_front_unit->unitClass());
					assert(new_row != -1);
					coord.row = new_row;
					field_.setUnit(coord.col, coord.row, most_front_unit);
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
				
				assert(new_row != -1);
				
//						unit->coord_.row = new_row;
//						unit->coord_.col = new_col;
				field_.setUnit(new_col, new_row, unit);
			}
		}
	}
}

int AIContext::rowToMoveUnit(int col, const Unit* unit) const
{
	return rowToPlace(col, unit->unitClass(), unit);
}

void AIContext::removeUnit(const CellCoordinate& coord)
{
	field_.setUnit(coord, 0);
	field_.alignTroops();
	processFormations();
}

void AIContext::moveUnit(const CellCoordinate& coord, int col)
{
	Unit* unit = field_.unit(coord);
	assert(unit);
	
	int row = rowToMoveUnit(col, unit);
	field_.setUnit(coord, 0);
	field_.setUnit(col, row, unit);
	
	field_.alignTroops();
	processFormations();
}

int AIContext::score() const
{
	return score_;

//	int score = 0;
//	for (list<AttackFormation*>::const_iterator i=newAttackFormations_.begin(),
//		e=newAttackFormations_.end(); i!=e; ++i)
//	{
//		score += (*i)->totalStrength_;
//	}
//	
//	for (set<const Unit*>::const_iterator i=walls_.begin(), e=walls_.end(); i!=e; ++i)
//	{
//		score += (*i)->unitType()->avatar()->wallStrength(0);
//	}
//	
//	return score;
}


AI::AI()
{
	
}

void AI::traverseTurns(const AIContext& context)
{
	if (context.depth() > 2)
	{
		// calculate result and return
		int s = context.score();
		if (s && s >= bestScore_)
		{
//			for (list<PlayerAction>::const_iterator i=context.actions().begin(),
//				e=context.actions().end(); i!=e; ++i)
//			{
//				std::cout << "type: " << i->type() << "; from: (" << i->coord().col << ", " << i->coord().row << "); to: " << i->to() << endl;
//			}
			
			bestScore_ = s;
			bestAction_ = context.firstAction();
		}
		
		return;
	}
	
	// test moving units
	for (int c=0; c<kFieldWidth; ++c)
	{
		int r = context.lastOccupiedRow(c);
		if (r >= 0)
		{
			Unit* u = context.field().occupyingUnit(CellCoordinate(c, r));
			if (!context.unitIsWallOrAttack(u))
			{
				for (int c2=0; c2<kFieldHeight; ++c2)
				{
					if (c != c2)
					{
						int r2 = context.rowToMoveUnit(c2, u);
						if (r2 != -1)
						{
							AIContext new_context = context;
							new_context.moveUnit(context.field().coord(u), c2);
							
							new_context.pushAction(PlayerAction::actionMoveUnit(context.field().coord(u), c2));
							
							traverseTurns(new_context);
						}
					}
				}
			}
		}
	}
	
	// test removing units
	for (int c=0; c<kFieldWidth; ++c)
	{
		for (int r=0; r<kFieldHeight; ++r)
		{
			Unit* u = context.field().unit(c, r);
			if (u && !context.unitIsWallOrAttack(u))
			{
				AIContext new_context = context;
				new_context.removeUnit(CellCoordinate(c, r));
				
				new_context.pushAction(PlayerAction::actionRemoveUnit(CellCoordinate(c, r)));
				
				traverseTurns(new_context);
			}
		}
	}
}

PlayerAction AI::nextTurn(const Player& player)
{
	if (float(player.troopsInReservoir()) / float(kMaxTroops) > 0.8)
		return PlayerAction::actionCallTroops();
	else
	{
		// filling context
		bestAction_ = PlayerAction();
		bestScore_ = 0;
		
		traverseTurns(AIContext(player.field_));
		
		if (bestAction_.type() == PlayerActionType_None)
		{
			if (float(player.troopsInReservoir()) / float(kMaxTroops) > 0.2)
			{
				return PlayerAction::actionCallTroops();
			}
			else
			{
                if (rand() % 2)
                {
                    while (true)
                    {
                        const Unit* u = player.occupyingUnit(CellCoordinate(rand() % kFieldWidth,
                                                                        rand() % kFieldHeight));
                        if (u && !u->isWall() && !u->attackFormation())
                            return PlayerAction::actionRemoveUnit(u->coord());
                    }
                }
                else
                {
                    PlayerAction::actionEndRound();
                }
			}
		}
		else
		{
			return bestAction_;
		}
	}
	
	return PlayerAction::actionEndRound();
}
