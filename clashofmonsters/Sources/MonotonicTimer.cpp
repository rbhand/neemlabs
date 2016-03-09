//
//  MonotonicTimer.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "MonotonicTimer.h"

#include <sys/time.h>

using namespace f17y::utils;

namespace f17y
{
namespace utils
{
	time_interval_t get_absolute_monotonic_time()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
}}


MonotonicTimer::MonotonicTimer() :
	started_(0), stopped_(0), working_(false)
{
}

void MonotonicTimer::start()
{
	started_ = get_absolute_monotonic_time();
	working_ = true;
}

void MonotonicTimer::reset()
{
	started_ = stopped_ = 0;
	working_ = false;
}

time_interval_t MonotonicTimer::totalTime() const
{
	return working_ ? (get_absolute_monotonic_time() - started_) :
		(stopped_ - started_);
}

void MonotonicTimer::stop()
{
	working_ = false;
	stopped_ = get_absolute_monotonic_time();
}
