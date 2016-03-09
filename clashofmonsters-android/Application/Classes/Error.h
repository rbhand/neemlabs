//
//  Error.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 3/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include <string>

namespace monsters
{
	class Error
	{
	public:
		enum
		{
			Code_OK = 0,
			Code_Network = 1,
			
			Code_Undefined = -1
		};
	
	public:
		Error() : code_(Code_OK)
		{
		}
	
		Error(std::string description, int code=Code_Undefined) :
			code_(code), description_(description)
		{
		}
		
		int code() const { return code_; }
		const std::string& description() const { return description_; }
		
		bool isOk() const { return code_ == Code_OK; }
	
	private:
		int code_;
		std::string description_;
	};
}
