//
//  Fixed.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 3/27/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "Fixed.h"
#include <stdlib.h>
#include <memory.h>
#include "cocos2d.h"

using namespace w2f2;

fixed64_t::fixed64_t(const char* v) :
	v_(0)
{
	int64_t whole = 0, numerator = 0, denominator = 1;
	char buf[256];
	const char* p=v, *fp;
	int64_t sign = 1;
	
	if (*p == '-')
	{
		sign = -1;
		++p;
		++v;
	}
	
	// whole part
	for (; ; ++p)
	{
		if ((*p == '.' || *p == '\0') && p != v)
		{
			memcpy(buf, v, p-v);
			buf[p-v] = '\0';
			whole = atoi(buf);
			
			if (*p != 0)
				++p;
			break;
		}
		else if ((*p < '0' || *p > '9') && *p != '-')
		{
			return;
		}
	}
	
	fp = p;
	for (; *p; ++p)
	{
		if (*p < '0' || *p > '9')
		{
			break;
		}
		
		denominator *= 10;
	}
	
	if (!(*p) && p != v && fp != p)
	{
		memcpy(buf, fp, p-v);
		numerator = atoi(buf);
	}
	
	v_ = ((whole << 16) + (numerator * one / denominator)) * sign;
}

int64_t fixed64_t::_mul(int64_t v1, int64_t v2)
{
	int64_t product = (int64_t)v1 * v2;
	//int64_t whole = product >> 32;
	
	int64_t result = product >> 16;
	result += (product & 0x8000) >> 15;
	return result;
	//return (whole << 16);
}

#ifdef __GNUC__
// Count leading zeros, using processor-specific instruction if available.
#define clz(x) __builtin_clzll(x)
#else
static uint8_t clz(uint64_t x)
{
	uint8_t result = 0;
	if (x == 0) return 64;
	while (!(x & 0xF000000000000000)) { result += 4; x <<= 4; }
	while (!(x & 0x8000000000000000)) { result += 1; x <<= 1; }
	return result;
}
#endif

int64_t fixed64_t::_div(int64_t a, int64_t b)
{
	CC_ASSERT(b != 0);
	
//	// Both are effectively whole numbers
//	if ((a & 0xFFFF) == 0 && (b & 0xFFFF) == 0)
//	{
//		return ((a >> 16) / (b >> 16)) << 16;
//	}
  
	uint64_t remainder = (a >= 0) ? a : (-a);
	uint64_t divider = (b >= 0) ? b : (-b);
	uint64_t quotient = 0;
	int bit_pos = 17;
  
//	// Kick-start the division a bit.
//	// This improves speed in the worst-case scenarios where N and D are large
//	// It gets a lower estimate for the result by N/(D >> 17 + 1).
//	if (divider & 0xFFF00000)
//	{
//		uint32_t shifted_div = ((divider >> 17) + 1);
//		quotient = remainder / shifted_div;
//		remainder -= ((uint64_t)quotient * divider) >> 17;
//	}
  
	// If the divider is divisible by 2^n, take advantage of it.
	while (!(divider & 0xF) && bit_pos >= 4)
	{
		divider >>= 4;
		bit_pos -= 4;
	}
  
	while (remainder && bit_pos >= 0)
	{
		// Shift remainder as much as we can without overflowing
		int shift = clz(remainder);
		if (shift > bit_pos)
			shift = bit_pos;
		remainder <<= shift;
		bit_pos -= shift;

		uint64_t div = remainder / divider;
		remainder = remainder % divider;
		quotient += div << bit_pos;

		remainder <<= 1;
		bit_pos--;
	}

//  #ifndef FIXMATH_NO_ROUNDING
//  // Quotient is always positive so rounding is easy
	quotient++;
//  #endif
  
	int64_t result = quotient >> 1;
  
	// Figure out the sign of the result
	if ((a ^ b) & 0x80000000)
	{
		result = -result;
	}
  
	return result;
}
