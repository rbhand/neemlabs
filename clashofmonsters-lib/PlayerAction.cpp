//
//  PlayerAction.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/12/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "PlayerAction.h"
#include <memory>

using namespace monsters;

pb::Message* PlayerAction::serialized() const
{
	using namespace pb;
	Message* m = new Message();
	
	*m
		<< varint::i(1, playerIndex_)
		<< varint::ui(2, type_);
	
	if (type_ == PlayerActionType_MoveUnit ||
		type_ == PlayerActionType_RemoveUnit)
	{
		*m
			<< varint::i(3, coord_.row)
			<< varint::i(4, coord_.col);
	}
		
	if (type_ == PlayerActionType_MoveUnit)
	{
		*m
			<< varint::i(5, to_);
	}
	
	
	return m;
}

bool PlayerAction::deserialize(const pb::Message& msg)
{
	if (msg.hasValue(1))
		playerIndex_ = msg.getFieldValueInt32(1);
	else
		playerIndex_ = -1;
	
	type_ = (PlayerActionType)msg.getFieldValueUInt32(2);
	
	if (type_ == PlayerActionType_MoveUnit ||
		type_ == PlayerActionType_RemoveUnit)
	{
		coord_.row = msg.getFieldValueInt32(3);
		coord_.col = msg.getFieldValueInt32(4);
	}
	
	if (type_ == PlayerActionType_MoveUnit)
	{
		to_ = msg.getFieldValueInt32(5);
	}
	
	return true;
}

PlayerAction PlayerAction::deserialized(const std::string& base64)
{
	pb::Message msg;
	pb::Buffer buf;
	buf.writeBase64(base64.c_str(), base64.length());
	buf.rewind();
	msg.deserialize(buf);
	
	PlayerAction a;
	a.deserialize(msg);
	return a;
}

std::string PlayerAction::serializedBase64() const
{
	std::auto_ptr<pb::Message> msg(serialized());
	std::auto_ptr<pb::Buffer> buf(msg->serialized());
	std::vector<char> base64 = buf->toBase64();
	return std::string(base64.begin(), base64.end());
}
