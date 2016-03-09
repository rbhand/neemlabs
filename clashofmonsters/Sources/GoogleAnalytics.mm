//
//  GoogleAnalytics.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 7/3/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "GoogleAnalytics.h"
#include "GAI.h"

using namespace w2f2;

static GoogleAnalytics* g_GoogleAnalytics = 0;

namespace w2f2
{
	struct GoogleAnalyticsPrivate
	{
		id<GAITracker> tracker;
	};
}


GoogleAnalytics::GoogleAnalytics(const std::string& trackingId) :
	d_(new GoogleAnalyticsPrivate)
{
//	[GAI sharedInstance].trackUncaughtExceptions = YES;
//	[GAI sharedInstance].dispatchInterval = 20;
//  
//	d_->tracker = [[GAI sharedInstance] trackerWithTrackingId:[NSString
//		stringWithUTF8String:trackingId.c_str()]];
}

GoogleAnalytics::~GoogleAnalytics()
{
//	d_->tracker = nil;
//	delete d_;
}

void GoogleAnalytics::trackScreenEnter(const std::string& screenName)
{
//	[d_->tracker sendView:[NSString stringWithUTF8String:screenName.c_str()]];
}

void GoogleAnalytics::initialize(const std::string& trackingId)
{
//	assert(!g_GoogleAnalytics);
//	g_GoogleAnalytics = new GoogleAnalytics(trackingId);
}

GoogleAnalytics& GoogleAnalytics::instance()
{
//	assert(g_GoogleAnalytics);
//	return *g_GoogleAnalytics;
}
