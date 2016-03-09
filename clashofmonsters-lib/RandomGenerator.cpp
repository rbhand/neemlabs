//
//  RandomGenerator.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "RandomGenerator.h"
#include <stdlib.h>
#include <algorithm>

using namespace monsters;

RandomGenerator::RandomGenerator()
{
	srand(time(0));
}

RandomGenerator::RandomGenerator(uint32_t seed)
{
	srand(seed);
}

pb::Message* RandomGenerator::serialized() const
{
	using namespace pb;
	
	Message* m = new Message();
	*m << varint::ui(1, z1_) << varint::ui(2, z2_)
		<< varint::ui(3, z3_) << varint::ui(4, z4_);
	return m;
}

bool RandomGenerator::deserialize(const pb::Message& msg)
{
	if (msg.hasValue(1) && msg.hasValue(2) && msg.hasValue(3) && msg.hasValue(4))
	{
		z1_ = msg.getFieldValueUInt32(1);
		z2_ = msg.getFieldValueUInt32(2);
		z3_ = msg.getFieldValueUInt32(3);
		z4_ = msg.getFieldValueUInt32(4);
		return true;
	}
	
	return false;
}

void RandomGenerator::srand(uint32_t seed)
{
	z1_ = z2_ = z3_ = z4_ = std::max(seed, 127U);
}

uint32_t RandomGenerator::randInt()
{
	uint32_t b;
	b  = ((z1_ << 6) ^ z1_) >> 13;
	z1_ = ((z1_ & 4294967294U) << 18) ^ b;
	b  = ((z2_ << 2) ^ z2_) >> 27;
	z2_ = ((z2_ & 4294967288U) << 2) ^ b;
	b  = ((z3_ << 13) ^ z3_) >> 21;
	z3_ = ((z3_ & 4294967280U) << 7) ^ b;
	b  = ((z4_ << 3) ^ z4_) >> 12;
	z4_ = ((z4_ & 4294967168U) << 13) ^ b;
	return (z1_ ^ z2_ ^ z3_ ^ z4_);
}

float RandomGenerator::randFloat()
{
	uint32_t b;
	b  = ((z1_ << 6) ^ z1_) >> 13;
	z1_ = ((z1_ & 4294967294U) << 18) ^ b;
	b  = ((z2_ << 2) ^ z2_) >> 27;
	z2_ = ((z2_ & 4294967288U) << 2) ^ b;
	b  = ((z3_ << 13) ^ z3_) >> 21;
	z3_ = ((z3_ & 4294967280U) << 7) ^ b;
	b  = ((z4_ << 3) ^ z4_) >> 12;
	z4_ = ((z4_ & 4294967168U) << 13) ^ b;
	return (z1_ ^ z2_ ^ z3_ ^ z4_) * 2.3283064365386963e-10;
}
