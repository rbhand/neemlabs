//
//  MonotonicTimer.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include <inttypes.h>

namespace f17y
{
namespace utils
{
	/// Time interval in msec
	typedef uint64_t time_interval_t;
	
	class MonotonicTimer
	{
	public:
		MonotonicTimer();
		MonotonicTimer(bool working, time_interval_t started, time_interval_t stopped) :
			working_(working), started_(started), stopped_(stopped) { }
		
		void start();
		void reset();
		void stop();
		time_interval_t totalTime() const;
		
		bool isWorking() const { return working_; }
		time_interval_t started() const { return started_; }
		time_interval_t stopped() const { return stopped_; }
		
	private:
		bool working_;
		time_interval_t started_;
		time_interval_t stopped_;
	};
	
	time_interval_t get_absolute_monotonic_time();
}}
