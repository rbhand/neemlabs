//
//  TutorialScreen.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/15/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once
#include "cocos2d.h"

namespace monsters
{
	struct TutorialScreenPrivate;
	
	class TutorialScreen
	{
	public:
		TutorialScreen();
		void show();
		
		~TutorialScreen();
		
	private:
		TutorialScreenPrivate* d_;
	
	};
}
