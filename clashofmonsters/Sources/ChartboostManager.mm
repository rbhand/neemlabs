//
//  ChartboostManager.cpp
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 6/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "ChartboostManager.h"

#ifndef IS_PRO
#import <Chartboost/Chartboost.h>
#endif

using namespace monsters;

static ChartboostManager* g_sharedManager = 0x0;

ChartboostManager& ChartboostManager::sharedManager()
{
	if (!g_sharedManager)
	{
		g_sharedManager = new ChartboostManager();
		
		#ifndef IS_PRO
		[Chartboost startWithAppId:@"51cd6c4216ba47a50b00001f" appSignature:@"879b5dc8ca1d729bc1e876992b8c6b90fc8098bc" delegate:nil];
		[Chartboost cacheInterstitial:CBLocationGameOver];
		
//		Chartboost *cb = [Chartboost sharedChartboost];
//		cb.appId = @"51cd6c4216ba47a50b00001f";
//		cb.appSignature = @"879b5dc8ca1d729bc1e876992b8c6b90fc8098bc";
//		[cb startSession];
//		[cb cacheInterstitial];

		#endif
	}
	
	return *g_sharedManager;
}

void ChartboostManager::showInterstitial()
{
	#ifndef IS_PRO
	if ([Chartboost hasInterstitial:CBLocationGameOver])
		[Chartboost showInterstitial:CBLocationGameOver];
	else
		[Chartboost cacheInterstitial:CBLocationGameOver];
	#endif
}
