//
//  JSONObject.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "JSONVariant.h"
#include <stdio.h>

using namespace w2f2;
using namespace std;


JSONVariant::JSONVariant() :
	nsObject_(nil)
{
}

JSONVariant::JSONVariant(const JSONVariant& v)
{
	nsObject_ = [(NSObject*)v.nsObject_ retain];
}

JSONVariant& JSONVariant::operator=(const JSONVariant& v)
{
	[(NSObject*)nsObject_ release];
	nsObject_ = [(NSObject*)v.nsObject_ retain];
	return *this;
}

JSONVariant::JSONVariant(void* nsObject)
{
	nsObject_ = [(NSObject*)nsObject retain];
}

JSONVariant::~JSONVariant()
{
	[(NSObject*)nsObject_ release];
}

JSONVariant JSONVariant::objectForKey(const std::string& key) const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSDictionary class]])
		return JSONVariant([(NSDictionary*)nsObject_
			objectForKey:[NSString stringWithUTF8String:key.c_str()]]);
	return JSONVariant();
}

JSONVariant JSONVariant::objectAtIndex(int index) const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSArray class]])
		return JSONVariant([(NSArray*)nsObject_ objectAtIndex:index]);
	return JSONVariant();
}

int JSONVariant::objectsCount() const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSDictionary class]])
		return [(NSDictionary*)nsObject_ count];
	else if ([(NSObject*)nsObject_ isKindOfClass:[NSArray class]])
		return [(NSArray*)nsObject_ count];
	return 0;
}

JSONVariant::Type JSONVariant::type() const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSDictionary class]])
		return Object;
	else if ([(NSObject*)nsObject_ isKindOfClass:[NSArray class]])
		return Array;
	else if ([(NSObject*)nsObject_ isKindOfClass:[NSString class]])
		return String;
	else if ([(NSObject*)nsObject_ isKindOfClass:[NSNumber class]])
		return Number;
		
	return Null;
}

std::string JSONVariant::asString() const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSString class]])
		return string([(NSString*)nsObject_ UTF8String]);
	else if ([(NSObject*)nsObject_ isKindOfClass:[NSNumber class]])
		return string([[(NSNumber*)nsObject_ stringValue] UTF8String]);
	return string();
}

int64_t JSONVariant::asInteger() const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSNumber class]])
		return [(NSNumber*)nsObject_ longLongValue];
	return 0;
}

double JSONVariant::asReal() const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSNumber class]])
		return [(NSNumber*)nsObject_ doubleValue];
	return 0;
}

bool JSONVariant::asBoolean() const
{
	if ([(NSObject*)nsObject_ isKindOfClass:[NSNumber class]])
		return [(NSNumber*)nsObject_ boolValue] ? true : false;
	return false;
}

bool JSONVariant::isNull() const
{
	return nsObject_ == 0 || [(NSObject*)nsObject_ isKindOfClass:[NSNull class]];
}

