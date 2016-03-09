//
//  JSONVariantIOS.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include <string>

namespace w2f2
{
	class JSONVariant
	{
	public:
		enum Type
		{
			Null = 0,
			Object = 1,
			Array = 2,
			String = 3,
			Number = 4
		};
		
	public:
		JSONVariant();
		JSONVariant(const JSONVariant& v);
		JSONVariant(void* nsObject);
		~JSONVariant();
		
		JSONVariant& operator=(const JSONVariant& v);
		
		Type type() const;
		
		JSONVariant objectForKey(const std::string& key) const;
		JSONVariant objectAtIndex(int index) const;
		int objectsCount() const;
		
		std::string asString() const;
		int64_t asInteger() const;
		double asReal() const;
		bool asBoolean() const;
		
		bool isNull() const;
		
	private:
		void* nsObject_;
	};
}
