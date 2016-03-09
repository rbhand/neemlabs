//
//  RandomGenerator.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/29/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include <stdint.h>

#include "protobuf/Message.h"

namespace monsters
{
	class RandomGenerator
	{
	public:
		RandomGenerator();
		RandomGenerator(uint32_t seed);
		
		void srand(uint32_t seed);
		
		uint32_t randInt();
		float randFloat();
		
		pb::Message* serialized() const;
		bool deserialize(const pb::Message& msg);
		
		bool operator==(const RandomGenerator& rnd) const
		{
			return z1_ == rnd.z1_ && z2_ == rnd.z2_ && z3_ == rnd.z3_ && z4_ == rnd.z4_;
		}
		
		bool operator!=(const RandomGenerator& rnd) const
		{
			return z1_ != rnd.z1_ || z2_ != rnd.z2_ || z3_ != rnd.z3_ || z4_ != rnd.z4_;
		}
		
	private:
		uint32_t z1_, z2_, z3_, z4_;
	};
}
