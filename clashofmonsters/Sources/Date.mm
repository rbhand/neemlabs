//
//  Date.mm
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/16/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "Date.h"
#include "ISO8601DateFormatter.h"

using namespace w2f2;

Date w2f2::dateFromISO8601(const std::string& str)
{
	ISO8601DateFormatter* fmt = [[[ISO8601DateFormatter alloc] init] autorelease];
	return [[fmt dateFromString:[NSString stringWithUTF8String:str.c_str()]] timeIntervalSince1970];
}
