//
//  Date.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/16/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include <string>
#include <stdint.h>

namespace w2f2
{
	typedef uint64_t Date;
	
	Date dateFromISO8601(const std::string& str);
}
