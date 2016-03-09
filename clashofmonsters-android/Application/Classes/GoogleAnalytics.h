//
//  GoogleAnalytics.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 7/3/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include <string>

namespace w2f2
{
	struct GoogleAnalyticsPrivate;
	class GoogleAnalytics
	{
	public:
		static void initialize(const std::string& trackingId);
		static GoogleAnalytics& instance();
	
		void trackScreenEnter(const std::string& screenName);
		
		~GoogleAnalytics();
		
	private:
		GoogleAnalytics(const std::string& trackingId);
	
	private:
		GoogleAnalyticsPrivate* d_;
	};
}