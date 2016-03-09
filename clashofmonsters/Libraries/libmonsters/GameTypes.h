//
//  GameTypes.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

namespace monsters
{
	typedef float TimeInterval;
	
	struct CellCoordinate
	{
		CellCoordinate() : col(-1), row(-1) { }
		CellCoordinate(int aCol, int aRow) :
			col(aCol), row(aRow)
		{
		}
		
		bool operator==(const CellCoordinate& c) const
		{
			return col == c.col && row == c.row;
		}
		
		int col, row;
	};
	
	struct ProcessFormationsHint
	{
		enum Type
		{
			Combo,
			Sync,
			Cascade
		};
	
		ProcessFormationsHint(Type aType, const CellCoordinate& c, int aPlayer) : type(aType),
			coord(c), player(aPlayer) { }
		
		Type type;
		CellCoordinate coord;
		int player;
	};
}

