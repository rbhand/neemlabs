//
//  Message.cpp
//

#include "Message.h"

#include <iostream>
#include "cocos2d.h"
#include <stdlib.h>
#include <string.h>

using namespace pb;
using namespace std;


Message::Message()
{
	
}

Message::Message(const bytes_t& bytes)
{
	deserialize(bytes);
}

Message::~Message()
{
	for (fields_map::iterator i=fields_.begin(), e=fields_.end(); i!=e; ++i)
	{
		for (fields_vector::iterator fi=i->second.begin(), fe=i->second.end(); fi!=fe; ++fi)
		{
			cleanUpField(*fi);
		}
	}
}

void Message::cleanUpField(const Field& field)
{
	if (field.dataType == Field::DataType_Message || field.dataType == Field::DataType_RawBytes)
	{
		if (field.own_bytes && field.value_bytes.bytes)
			free(field.value_bytes.bytes);
		
		if (field.own_message && field.value_message)
			delete field.value_message;
	}
}

void Message::setField(tag_t tag, const Field& field)
{
	fields_map::iterator it = fields_.find(tag);
	if (it != fields_.end())
	{
		for (fields_vector::const_iterator i=it->second.begin(), e=it->second.end();
			i!=e; ++i)
		{
			cleanUpField(*i);
		}
		
		it->second.clear();
		it->second.push_back(field);
	}
	else
	{
		fields_vector fields;
		fields.push_back(field);
		fields_.insert(pair<tag_t, fields_vector>(tag, fields));
	}
}

void Message::addField(tag_t tag, const Field& field)
{
	fields_map::iterator it = fields_.find(tag);
	if (it != fields_.end())
	{
		it->second.push_back(field);
	}
	else
	{
		fields_vector fields;
		fields.push_back(field);
		fields_.insert(pair<tag_t, fields_vector>(tag, fields));
	}
}


void Message::setFieldValueVarint(tag_t tag, uint64_t value)
{
	Field f(Field::DataType_UInt64, WireType_Varint);
	f.value_uint64 = value;
	setField(tag, f);
}

void Message::setFieldValueVarintBool(tag_t tag, bool value)
{
	Field f(Field::DataType_UInt64, WireType_Varint);
	f.value_uint64 = value ? 1 : 0;
	setField(tag, f);
}

void Message::setFieldValueVarintSigned(tag_t tag, int64_t value)
{
	Field f(Field::DataType_Int64, WireType_Varint);
	f.value_int64 = value;
	setField(tag, f);
}

void Message::addFieldValueVarint(tag_t tag, uint64_t value)
{
	Field f(Field::DataType_UInt64, WireType_Varint);
	f.value_uint64 = value;
	addField(tag, f);
}

void Message::addFieldValueVarintBool(tag_t tag, bool value)
{
	Field f(Field::DataType_UInt64, WireType_Varint);
	f.value_uint64 = value ? 1 : 0;
	addField(tag, f);
}

void Message::addFieldValueVarintSigned(tag_t tag, int64_t value)
{
	Field f(Field::DataType_Int64, WireType_Varint);
	f.value_int64 = value;
	addField(tag, f);
}




void Message::setFieldValueFixed32(tag_t tag, int32_t value)
{
	Field f(Field::DataType_Int32, WireType_Fixed32);
	f.value_int32 = value;
	setField(tag, f);
}

void Message::setFieldValueFixed32(tag_t tag, uint32_t value)
{
	Field f(Field::DataType_UInt32, WireType_Fixed32);
	f.value_uint32 = value;
	setField(tag, f);
}

void Message::setFieldValueFixed64(tag_t tag, int64_t value)
{
	Field f(Field::DataType_Int64, WireType_Fixed64);
	f.value_uint32 = value;
	setField(tag, f);
}

void Message::setFieldValueFixed64(tag_t tag, uint64_t value)
{
	Field f(Field::DataType_UInt64, WireType_Fixed64);
	f.value_uint64 = value;
	setField(tag, f);
}

void Message::setFieldValueFloat(tag_t tag, float value)
{
	Field f(Field::DataType_Float, WireType_Fixed32);
	f.value_float = value;
	setField(tag, f);
}

void Message::setFieldValueDouble(tag_t tag, double value)
{
	Field f(Field::DataType_Double, WireType_Fixed64);
	f.value_double = value;
	setField(tag, f);
}


void Message::addFieldValueFixed32(tag_t tag, int32_t value)
{
	Field f(Field::DataType_Int32, WireType_Fixed32);
	f.value_int32 = value;
	addField(tag, f);
}

void Message::addFieldValueFixed32(tag_t tag, uint32_t value)
{
	Field f(Field::DataType_UInt32, WireType_Fixed32);
	f.value_uint32 = value;
	addField(tag, f);
}

void Message::addFieldValueFixed64(tag_t tag, int64_t value)
{
	Field f(Field::DataType_Int64, WireType_Fixed64);
	f.value_int64 = value;
	addField(tag, f);
}

void Message::addFieldValueFixed64(tag_t tag, uint64_t value)
{
	Field f(Field::DataType_UInt64, WireType_Fixed64);
	f.value_uint64 = value;
	addField(tag, f);
}

void Message::addFieldValueFloat(tag_t tag, float value)
{
	Field f(Field::DataType_Float, WireType_Fixed32);
	f.value_float = value;
	addField(tag, f);
}

void Message::addFieldValueDouble(tag_t tag, double value)
{
	Field f(Field::DataType_Double, WireType_Fixed64);
	f.value_double = value;
	addField(tag, f);
}



void Message::setFieldValueBytes(tag_t tag, const uint8_t* bytes, int length, bool copy)
{
	Field f(Field::DataType_RawBytes, WireType_LengthDelimited);
	f.value_bytes.length = length;
	f.own_bytes = copy;
	if (copy)
	{
		f.value_bytes.bytes = (uint8_t*)malloc(length);
		memcpy(f.value_bytes.bytes, bytes, length);
	}
	else
	{
		f.value_bytes.bytes = const_cast<uint8_t*>(bytes);
	}
	
	setField(tag, f);
}

void Message::setFieldValueString(tag_t tag, const std::string& str)
{
	setFieldValueBytes(tag, (const uint8_t*)str.c_str(), str.size(), true);
}

void Message::setFieldValueMessage(tag_t tag, Message* msg)
{
	Field f(Field::DataType_Message, WireType_LengthDelimited);
	f.value_message = msg;
	f.own_message = true;
	setField(tag, f);
}


void Message::addFieldValueBytes(tag_t tag, const uint8_t* bytes, int length, bool copy)
{
	Field f(Field::DataType_RawBytes, WireType_LengthDelimited);
	f.value_bytes.length = length;
	f.own_bytes = copy;
	if (copy)
	{
		f.value_bytes.bytes = (uint8_t*)malloc(length);
		memcpy(f.value_bytes.bytes, bytes, length);
	}
	else
	{
		f.value_bytes.bytes = const_cast<uint8_t*>(bytes);
	}
	
	addField(tag, f);
}

void Message::addFieldValueBytesWithOwns(tag_t tag, const uint8_t* bytes, int length, bool owns)
{
	Field f(Field::DataType_RawBytes, WireType_LengthDelimited);
	f.value_bytes.length = length;
	f.own_bytes = owns;
	f.value_bytes.bytes = const_cast<uint8_t*>(bytes);
	addField(tag, f);
}

void Message::addFieldValueString(tag_t tag, const std::string& str)
{
	addFieldValueBytes(tag, (const uint8_t*)str.c_str(), str.size(), true);
}

void Message::addFieldValueMessage(tag_t tag, Message* msg)
{
	Field f(Field::DataType_Message, WireType_LengthDelimited);
	f.value_message = msg;
	f.own_message = true;
	addField(tag, f);
}



int Message::countValues(tag_t tag) const
{
	fields_map::const_iterator it = fields_.find(tag);
	if (it != fields_.end())
	{
		return it->second.size();
	}
	
	return 0;
}

bool Message::hasValue(tag_t tag) const
{
	fields_map::const_iterator it = fields_.find(tag);
	return (it != fields_.end() && it->second.size() > 0);
}

Message::Field Message::field(tag_t tag, int index) const
{
	CC_ASSERT(index >= 0);
	
	fields_map::const_iterator it = fields_.find(tag);
	if (it != fields_.end())
	{
		CC_ASSERT((size_t)index < it->second.size());
		return it->second.at(index);
	}
	
	return Field();
}

WireType Message::typeOfField(tag_t tag, int index) const
{
	return field(tag, index).wireType;
}

int32_t Message::getFieldValueInt32(tag_t tag, bool* ok) const
{
	return getFieldValueInt32AtIndex(tag, 0, ok);
}

int32_t Message::getFieldValueInt32AtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	int32_t ret = 0;
	bool ret_ok = false;
	if (f.dataType == Field::DataType_Int32)
	{
		ret = f.value_int32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt32)
	{
		if (f.wireType == WireType_Varint)
		{
			// for varints we need to apply zig zag decoding
			ret = zigZagDecode32(f.value_uint32);
		}
		else
		{
			ret = f.value_uint32;
		}
		
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Fixed32)
	{
		memcpy(&ret, &f.value_uint32, 4);
	}
	
	if (ok)
		*ok = ret_ok;
	return ret;
}

uint32_t Message::getFieldValueUInt32(tag_t tag, bool* ok) const
{
	return getFieldValueUInt32AtIndex(tag, 0, ok);
}

uint32_t Message::getFieldValueUInt32AtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	uint32_t ret = 0;
	bool ret_ok = false;
	if (f.dataType == Field::DataType_Int32)
	{
		ret = f.value_int32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt32 ||
		f.dataType == Field::DataType_Fixed32)
	{
		ret = f.value_uint32;
		ret_ok = true;
	}
	
	if (ok)
		*ok = ret_ok;
	return ret;
}

int64_t Message::getFieldValueInt64(tag_t tag, bool* ok) const
{
	return getFieldValueInt64AtIndex(tag, 0, ok);
}

int64_t Message::getFieldValueInt64AtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	int64_t ret = 0;
	bool ret_ok = false;
	if (f.dataType == Field::DataType_Int32)
	{
		ret = f.value_int32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Int64)
	{
		ret = f.value_int64;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt32)
	{
		if (f.wireType == WireType_Varint)
		{
			// for varints we need to apply zig zag decoding
			ret = zigZagDecode32(f.value_uint32);
		}
		else
		{
			ret = f.value_uint32;
		}
		
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt64)
	{
		if (f.wireType == WireType_Varint)
		{
			// for varints we need to apply zig zag decoding
			ret = zigZagDecode64(f.value_uint64);
		}
		else
		{
			ret = f.value_uint64;
		}
		
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Fixed64)
	{
		memcpy(&ret, &f.value_uint64, 8);
	}
	else if (f.dataType == Field::DataType_Fixed32)
	{
		int32_t v32;
		memcpy(&v32, &f.value_uint32, 4);
		ret = v32;
	}
	
	
	if (ok)
		*ok = ret_ok;
	return ret;
}

uint64_t Message::getFieldValueUInt64(tag_t tag, bool* ok) const
{
	return getFieldValueUInt64AtIndex(tag, 0, ok);
}

uint64_t Message::getFieldValueUInt64AtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	uint32_t ret = 0;
	bool ret_ok = false;
	if (f.dataType == Field::DataType_Int32)
	{
		ret = f.value_int32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt32)
	{
		ret = f.value_uint32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Int64)
	{
		ret = f.value_int64;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt64 ||
		f.dataType == Field::DataType_Fixed64)
	{
		ret = f.value_uint64;
		ret_ok = true;
	}
	
	if (ok)
		*ok = ret_ok;
	return ret;
}

float Message::getFieldValueFloat(tag_t tag, bool* ok) const
{
	return getFieldValueFloatAtIndex(tag, 0, ok);
}

float Message::getFieldValueFloatAtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	float ret = 0;
	bool ret_ok = false;
	if (f.dataType == Field::DataType_Float)
	{
		ret = f.value_float;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Double)
	{
		ret = f.value_double;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Int32)
	{
		ret = f.value_int32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt32)
	{
		ret = f.value_uint32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Fixed32)
	{
		memcpy(&ret, &f.value_uint32, 4);
	}
	else if (f.dataType == Field::DataType_Fixed64)
	{
		double ret_double;
		memcpy(&ret_double, &f.value_uint64, 8);
		ret = ret_double;
	}
	
	if (ok)
		*ok = ret_ok;
	return ret;
}

double Message::getFieldValueDouble(tag_t tag, bool* ok) const
{
	return getFieldValueDoubleAtIndex(tag, 0, ok);
}

double Message::getFieldValueDoubleAtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	double ret = 0;
	bool ret_ok = false;
	if (f.dataType == Field::DataType_Float)
	{
		ret = f.value_float;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Double)
	{
		ret = f.value_double;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Int32)
	{
		ret = f.value_int32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_UInt32)
	{
		ret = f.value_uint32;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Fixed64)
	{
		memcpy(&ret, &f.value_uint64, 8);
	}
	else if (f.dataType == Field::DataType_Fixed32)
	{
		float ret_float;
		memcpy(&ret_float, &f.value_uint32, 4);
		ret = ret_float;
	}
	
	if (ok)
		*ok = ret_ok;
	return ret;
}

bytes_t Message::getFieldValueBytes(tag_t tag, bool* ok) const
{
	return getFieldValueBytesAtIndex(tag, 0, ok);
}

bytes_t Message::getFieldValueBytesAtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	bytes_t ret;
	bool ret_ok = false;
	
	if (f.dataType == Field::DataType_RawBytes)
	{
		ret = f.value_bytes;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Message)
	{
		if (f.value_bytes.bytes)
		{
			ret = f.value_bytes;
		}
		else
		{
			// TODO
		}
	
		ret_ok = true;
	}
	
	if (ok)
		*ok = ret_ok;
	
	return ret;
}

std::string Message::getFieldValueString(tag_t tag, bool* ok) const
{
	return getFieldValueStringAtIndex(tag, 0, ok);
}

std::string Message::getFieldValueStringAtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	std::string ret;
	bool ret_ok = false;
	
	if (f.dataType == Field::DataType_RawBytes)
	{
		ret = std::string((const char*)f.value_bytes.bytes, f.value_bytes.length);
		ret_ok = true;
	}
	
	if (ok)
		*ok = ret_ok;
	
	return ret;
}

Message* Message::getFieldValueMessage(tag_t tag, bool* ok) const
{
	return getFieldValueMessageAtIndex(tag, 0, ok);
}

Message* Message::getFieldValueMessageAtIndex(tag_t tag, int index, bool* ok) const
{
	Field f = field(tag, index);
	Message* ret = 0;
	bool ret_ok = false;
	
	if (f.dataType == Field::DataType_RawBytes && f.value_bytes.bytes)
	{
		if (!f.value_message)
		{
			f.own_message = true;
			f.value_message = new Message(f.value_bytes);
		}
		
		ret = f.value_message;
		ret_ok = true;
	}
	else if (f.dataType == Field::DataType_Message)
	{
		ret = f.value_message;
		ret_ok = true;
	}
	
	return ret;
}

void Message::writeVarint(Buffer& buf, uint64_t v)
{
	do
	{
		uint8_t b = (v & 0x7f);
		v = v >> 7;
		if (v)
			b |= 0x80;
		
		int sz = buf.write(b);
		CC_ASSERT(sz == 1);
		sz = 0; // quick way to supress warn
	}
	while (v);
}

uint64_t Message::readVarint(Buffer& buf, bool* ok)
{
	CC_ASSERT(ok);
	
	int pos = buf.tell();

	uint64_t v = 0;
	uint8_t b;
	int r = 0;
	do
	{
		b = buf.read();
		if (r == 63 && (b & 0xfe))
		{
			cerr << "Invalid varint ending at " << hex << pos << dec << endl;
			*ok = false;
			return v;
		}
		
		v |= ((uint64_t)(b & 0x7f) << r);
		r += 7;
	}
	while ((b & 0x80));
	
	*ok = true;
	return v;
}

int Message::varintSize(uint64_t v)
{
	int sz = 0;
	do
	{
		++sz;
		v = v >> 7;
	}
	while (v);
	
	return sz;
}

int Message::serializedLength() const
{
	int l = 0;
	for (fields_map::const_iterator i=fields_.begin(), e=fields_.end(); i!=e; ++i)
	{
		for (fields_vector::const_iterator fi=i->second.begin(), fe=i->second.end();
			fi!=fe; ++fi)
		{
			int fl = varintSize((i->first << 3) | fi->wireType);
			
			switch (fi->wireType)
			{
				case WireType_Varint:
				{
					if (fi->dataType == Field::DataType_Int32)
						fl += varintSize(zigZagEncode32(fi->value_int32));
					else if (fi->dataType == Field::DataType_Int64)
						fl += varintSize(zigZagEncode64(fi->value_int64));
					else if (fi->dataType == Field::DataType_UInt32)
						fl += varintSize(fi->value_uint32);
					else if (fi->dataType == Field::DataType_UInt64)
						fl += varintSize(fi->value_uint64);
					else
						CC_ASSERT(0);
				}
				break;
				
				case WireType_Fixed32:
				{
					fl += 4;
				}
				break;
				
				case WireType_Fixed64:
				{
					fl += 8;
				}
				break;
				
				case WireType_LengthDelimited:
				{
					if (fi->dataType == Field::DataType_Message)
					{
						int ml = fi->value_message->serializedLength();
						fl += ml + varintSize(ml);
					}
					else if (fi->dataType == Field::DataType_RawBytes)
					{
						fl += fi->value_bytes.length + varintSize(fi->value_bytes.length);
					}
					else
					{
						CC_ASSERT(0);
					}
				}
				break;
				
				default:
				{
					CC_ASSERT(0);
				}
				break;
			}
			
			l += fl;
		}
	}
	
	return l;
}

void Message::serialize(Buffer& buf) const
{
	for (fields_map::const_iterator i=fields_.begin(), e=fields_.end(); i!=e; ++i)
	{
		for (fields_vector::const_iterator fi=i->second.begin(), fe=i->second.end();
			fi!=fe; ++fi)
		{
			writeVarint(buf, (i->first << 3) | fi->wireType);
			
			switch (fi->wireType)
			{
				case WireType_Varint:
				{
					if (fi->dataType == Field::DataType_Int32)
						writeVarint(buf, zigZagEncode32(fi->value_int32));
					else if (fi->dataType == Field::DataType_Int64)
						writeVarint(buf, zigZagEncode64(fi->value_int64));
					else if (fi->dataType == Field::DataType_UInt32)
						writeVarint(buf, fi->value_uint32);
					else if (fi->dataType == Field::DataType_UInt64)
						writeVarint(buf, fi->value_uint64);
					else
						CC_ASSERT(0);
				}
				break;
				
				case WireType_Fixed32:
				{
					// TODO: big endian support
					if (fi->dataType == Field::DataType_Int32)
						buf.write((const uint8_t*)&fi->value_int32, 4);
					else if (fi->dataType == Field::DataType_UInt32 || fi->dataType == Field::DataType_Fixed32)
						buf.write((const uint8_t*)&fi->value_uint32, 4);
					else if (fi->dataType == Field::DataType_Float)
						buf.write((const uint8_t*)&fi->value_float, 4);
					else
						CC_ASSERT(0);
				}
				break;
				
				case WireType_Fixed64:
				{
					// TODO: big endian support
					if (fi->dataType == Field::DataType_Int64)
						buf.write((const uint8_t*)&fi->value_int64, 8);
					else if (fi->dataType == Field::DataType_UInt64 || fi->dataType == Field::DataType_Fixed64)
						buf.write((const uint8_t*)&fi->value_uint64, 8);
					else if (fi->dataType == Field::DataType_Double)
						buf.write((const uint8_t*)&fi->value_double, 8);
					else
						CC_ASSERT(0);
				}
				break;
				
				case WireType_LengthDelimited:
				{
					if (fi->dataType == Field::DataType_Message)
					{
						CC_ASSERT(fi->value_message);
						writeVarint(buf, fi->value_message->serializedLength());
						fi->value_message->serialize(buf);
					}
					else if (fi->dataType == Field::DataType_RawBytes)
					{
						writeVarint(buf, fi->value_bytes.length);
						buf.write(fi->value_bytes.bytes, fi->value_bytes.length);
					}
					else
					{
						CC_ASSERT(0);
					}
				}
				break;
				
				default:
				{
					CC_ASSERT(0);
				}
				break;
			}
		}
	}
}

Buffer* Message::serialized() const
{
	if (int l = serializedLength())
	{
		Buffer* buffer = new Buffer(l);
		serialize(*buffer);
		return buffer;
	}
	
	return new Buffer();
}

bool Message::deserialize(const Buffer& buffer)
{
	return deserialize(buffer.data(), buffer.size());
}

bool Message::deserialize(Buffer& buffer, bool doNotCopy)
{
	clear();
	
	bool ok = true;
	uint64_t key;
	
	// TODO: big endian support
	while (ok)
	{
		if (buffer.tell() == buffer.size())
		{
			break;
		}
		
		key = readVarint(buffer, &ok);
		if (!ok)
		{
			break;
		}
		
		int pos = buffer.tell();
		WireType wt = (WireType)(key & 0x07);
		tag_t tag = key >> 3;
		Field f;
		f.wireType = wt;
		
		switch (wt)
		{
			case WireType_Varint:
			{
				uint64_t v  = readVarint(buffer, &ok);
				if (v > 0xffffffff)
				{
					f.dataType = Field::DataType_UInt64;
					f.value_uint64 = v;
				}
				else
				{
					f.dataType = Field::DataType_UInt32;
					f.value_uint32 = v;
				}
				
				if (!ok)
					break;
			}
			break;
			
			case WireType_Fixed32:
			{
				f.dataType = Field::DataType_Fixed32;
				if (buffer.read(&f.value_uint32, 4) != 4)
				{
					cerr << "Not enough bytes for fixed32 at " << hex << pos << dec << endl;
					
					ok = false;
					break;
				}
			}
			break;
			
			case WireType_Fixed64:
			{
				f.dataType = Field::DataType_Fixed64;
				if (buffer.read(&f.value_uint64, 8) != 8)
				{
					cerr << "Not enough bytes for fixed64 at " << hex << pos << dec << endl;
					
					ok = false;
					break;
				}
			}
			break;
			
			case WireType_LengthDelimited:
			{
				f.dataType = Field::DataType_RawBytes;
				f.value_message = 0;
				uint64_t length = readVarint(buffer, &ok);
				if (!ok)
					break;
				
				if (doNotCopy)
				{
					f.own_bytes = false;
					f.value_bytes.bytes = buffer.current();
					f.value_bytes.length = length;
					if ((uint64_t)buffer.skip(length) != length)
					{
						cerr << "Not enough bytes for length delimited at " << hex << pos << dec << endl;
						ok = false;
						break;
					}
				}
				else
				{
					f.own_bytes = true;
					f.value_bytes.bytes = (uint8_t*)malloc(length);
					f.value_bytes.length = length;
					if ((uint64_t)buffer.read(f.value_bytes.bytes, length) != length)
					{
						cerr << "Not enough bytes for length delimited at " << hex << pos << dec << endl;
						ok = false;
						break;
					}
				}
			}
			break;
			
			default:
			{
				cerr << "Invalid wire type " << wt << " before " << hex << pos << dec << endl;
			}
			break;
		}
		
		addField(tag, f);
	}
	
	
	if (!ok)
		clear();
	
	return ok;
}

bool Message::deserialize(bytes_t bytes, bool doNotCopy)
{
	Buffer buf(bytes.bytes, bytes.length);
	return deserialize(buf, doNotCopy);
}

bool Message::deserialize(const uint8_t* bytes, int length, bool doNotCopy)
{
	Buffer buf(const_cast<uint8_t*>(bytes), length);
	return deserialize(buf, doNotCopy);
}


varint varint::b(tag_t tag, bool value)
{
	varint v;
	v.tag = tag;
	v.value_b = value;
	v.type = varint_b;
	return v;
}

varint varint::i(tag_t tag, int64_t value)
{
	varint v;
	v.tag = tag;
	v.value_i = value;
	v.type = varint_i;
	return v;
}

varint varint::ui(tag_t tag, uint64_t value)
{
	varint v;
	v.tag = tag;
	v.value_ui = value;
	v.type = varint_ui;
	return v;
}

pb::fixed fixed::i64(tag_t tag, int64_t value)
{
	fixed f;
	f.tag = tag;
	f.value_i64 = value;
	f.type = fixed_i64;
	return f;
}

pb::fixed fixed::ui64(tag_t tag, uint64_t value)
{
	fixed f;
	f.tag = tag;
	f.value_ui64 = value;
	f.type = fixed_ui64;
	return f;
}

pb::fixed fixed::i32(tag_t tag, int32_t value)
{
	fixed f;
	f.tag = tag;
	f.value_i32 = value;
	f.type = fixed_i32;
	return f;
}

pb::fixed fixed::ui32(tag_t tag, uint32_t value)
{
	fixed f;
	f.tag = tag;
	f.value_ui32 = value;
	f.type = fixed_ui32;
	return f;
}

pb::fixed fixed::f(tag_t tag, float value)
{
	fixed f;
	f.tag = tag;
	f.value_float = value;
	f.type = fixed_float;
	return f;
}

pb::fixed fixed::d(tag_t tag, double value)
{
	fixed f;
	f.tag = tag;
	f.value_double = value;
	f.type = fixed_double;
	return f;
}


bytes::bytes(tag_t aTag, const uint8_t* b, int l, bool copy) :
	tag(aTag), owns(copy), length(l)
{
	if (copy)
	{
		uint8_t* cb = (uint8_t*)malloc(length);
		memcpy(cb, b, length);
		data = cb;
	}
	else
	{
		data = b;
	}
}

bytes::bytes(tag_t aTag, const char* str, bool copy) :
	tag(aTag), owns(copy), length(strlen(str))
{
	if (copy)
	{
		uint8_t* cb = (uint8_t*)malloc(length);
		memcpy(cb, str, length);
		data = cb;
	}
	else
	{
		data = (const uint8_t*)str;
	}
}

bytes::bytes(const bytes&)
{
	CC_ASSERT(0); // no copy
}

bytes::bytes(tag_t aTag, const std::string& str) :
	tag(aTag), owns(true), length(str.size())
{
	uint8_t* cb = (uint8_t*)malloc(length);
	memcpy(cb, str.c_str(), length);
	data = cb;
}

bytes::~bytes()
{
	if (owns)
	{
		free((void*)data);
	}
}


Message& Message::operator<< (const varint& value)
{
	switch (value.type)
	{
		case varint::varint_b:
			addFieldValueVarintBool(value.tag, value.value_b);
		break;
		
		case varint::varint_i:
			addFieldValueVarintSigned(value.tag, value.value_i);
		break;
		
		case varint::varint_ui:
			addFieldValueVarint(value.tag, value.value_ui);
		break;
	}
	
	return *this;
}

Message& Message::operator<< (const fixed& value)
{
	switch (value.type)
	{
		case fixed::fixed_i32:
			addFieldValueFixed32(value.tag, value.value_i32);
		break;
		
		case fixed::fixed_ui32:
			addFieldValueFixed32(value.tag, value.value_ui32);
		break;
		
		case fixed::fixed_i64:
			addFieldValueFixed64(value.tag, value.value_i64);
		break;
		
		case fixed::fixed_ui64:
			addFieldValueFixed64(value.tag, value.value_ui64);
		break;
		
		case fixed::fixed_float:
			addFieldValueFloat(value.tag, value.value_float);
		break;
		
		case fixed::fixed_double:
			addFieldValueDouble(value.tag, value.value_double);
		break;
	}
	
	return *this;
}

Message& Message::operator<< (const msg& msg)
{
	addFieldValueMessage(msg.tag, msg.message);
	return *this;
}

Message& Message::operator<< (const bytes& b)
{
	addFieldValueBytesWithOwns(b.tag, b.data, b.length, b.owns);
	b.owns = false;
	return *this;
}

