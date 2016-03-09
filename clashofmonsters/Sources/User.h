//
//  User.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 3/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include <string>

namespace pb { class Message; }
namespace pb { class Buffer; }
//namespace w2f2 { class JSONVariant; }

namespace monsters
{
	class User
	{
	public:
		User() { }
		//User(const w2f2::JSONVariant& json);
		User(pb::Message* msg);
		
		const std::string& identifier() const { return identifier_; }
		const std::string& name() const { return name_; }
		const std::string& avatarUrl() const { return avatarUrl_; }
		const std::string& facebookId() const { return facebookId_; }
		
		void setIdentifier(const std::string& identifier) { identifier_ = identifier; }
		void setName(const std::string& name) { name_ = name; }
		void setAvatarUrl(const std::string& avatarUrl) { avatarUrl_ = avatarUrl; }
		void setFacebookId(const std::string& facebookId) { facebookId_ = facebookId; }
		
		bool isValid() const { return identifier_.size() > 0; }
		bool isLocal() const;
		
		bool operator==(const User& user) const { return identifier_ == user.identifier_; }
		bool operator!=(const User& user) const { return identifier_ != user.identifier_; }
		
		pb::Message* serialized() const;
		
	private:
		std::string identifier_;
		std::string name_;
		std::string facebookId_;
		std::string avatarUrl_;
	};
}
