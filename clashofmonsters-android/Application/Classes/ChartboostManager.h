//
//  ChartboostManager.h
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 6/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

namespace monsters
{
	class ChartboostManager
	{
	public:
		
		static ChartboostManager& sharedManager();
		void showInterstitial();
	
	};
}