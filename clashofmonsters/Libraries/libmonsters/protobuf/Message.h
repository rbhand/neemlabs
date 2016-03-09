//
//  Message.h
//

#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#include "Buffer.h"

namespace pb
{
	typedef uint64_t tag_t;
	enum WireType
	{
		WireType_Varint = 0,
		WireType_Fixed64 = 1,
		WireType_LengthDelimited = 2,
		WireType_Fixed32 = 5,
		
		WireType_Undefined = -1
	};
	
	struct bytes_t
	{
		bytes_t() : bytes(0), length(0) { }
		bytes_t(Buffer* buffer) : bytes(buffer->data()), length(buffer->size()) { }
	
		uint8_t* bytes;
		int length;
	};
	
	class varint
	{
	public:
		static varint b(tag_t tag, bool value);
		static varint i(tag_t tag, int64_t value);
		static varint ui(tag_t tag, uint64_t value);
	
		tag_t tag;
		enum
		{
			varint_b = 0,
			varint_i = 1,
			varint_ui = 2
		}
		type;
		
		union
		{
			bool value_b;
			int64_t value_i;
			uint64_t value_ui;
		};
	
	private:
		varint() { }
	};
	
	class fixed
	{
	public:
		static fixed i64(tag_t tag, int64_t value);
		static fixed ui64(tag_t tag, uint64_t value);
		static fixed i32(tag_t tag, int32_t value);
		static fixed ui32(tag_t tag, uint32_t value);
		static fixed f(tag_t tag, float value);
		static fixed d(tag_t tag, double value);
		
		tag_t tag;
		enum
		{
			fixed_i64 = 0,
			fixed_ui64 = 1,
			fixed_i32 = 2,
			fixed_ui32 = 3,
			fixed_float = 4,
			fixed_double = 5
		}
		type;
		
		union
		{
			int64_t value_i64;
			uint64_t value_ui64;
			int32_t value_i32;
			uint32_t value_ui32;
			float value_float;
			double value_double;
		};
		
	private:
		fixed() { }
	};
	
	class Message;
	class msg
	{
	public:
		msg(tag_t aTag, Message* aMessage) :
			tag(aTag), message(aMessage) { }
		
		tag_t tag;
		Message* message;
	};
	
	class bytes
	{
	public:
		bytes(tag_t aTag, const uint8_t* b, int length, bool copy=true);
		bytes(tag_t aTag, const char* str, bool copy=true);
		bytes(tag_t aTag, const std::string& str);
		~bytes();
	
		tag_t tag; 
		mutable bool owns;
		const uint8_t* data;
		int length;
		
		bytes(const bytes&); // no copy
		
	private:
		bytes& operator=(const bytes&) { return *this; } // no assignment
	};
	
	class Message
	{
	public:
		Message();
		Message(const bytes_t& bytes);
		
		~Message();
	
		void setFieldValueVarint(tag_t tag, uint64_t value);
		void setFieldValueVarintBool(tag_t tag, bool value);
		void setFieldValueVarintSigned(tag_t tag, int64_t value);
		
		void setFieldValueFixed32(tag_t tag, int32_t value);
		void setFieldValueFixed32(tag_t tag, uint32_t value);
		void setFieldValueFixed64(tag_t tag, int64_t value);
		void setFieldValueFixed64(tag_t tag, uint64_t value);
		
		void setFieldValueFloat(tag_t tag, float value);
		void setFieldValueDouble(tag_t tag, double value);
		
		void setFieldValueBytes(tag_t tag, const uint8_t* bytes, int length, bool copy=true);
		void setFieldValueString(tag_t tag, const std::string& str);
		void setFieldValueMessage(tag_t tag, Message* msg);
		
		void addFieldValueVarint(tag_t tag, uint64_t value);
		void addFieldValueVarintBool(tag_t tag, bool value);
		void addFieldValueVarintSigned(tag_t tag, int64_t value);
		
		void addFieldValueFixed32(tag_t tag, int32_t value);
		void addFieldValueFixed32(tag_t tag, uint32_t value);
		void addFieldValueFixed64(tag_t tag, int64_t value);
		void addFieldValueFixed64(tag_t tag, uint64_t value);
		
		void addFieldValueFloat(tag_t tag, float value);
		void addFieldValueDouble(tag_t tag, double value);
		
		void addFieldValueBytes(tag_t tag, const uint8_t* bytes, int length, bool copy=true);
		void addFieldValueString(tag_t tag, const std::string& str);
		void addFieldValueMessage(tag_t tag, Message* msg);
		
		int countValues(tag_t tag) const;
		bool hasValue(tag_t tag) const;
		WireType typeOfField(tag_t tag, int index=0) const;
		
		int32_t getFieldValueInt32(tag_t tag, bool* ok=0) const;
		int32_t getFieldValueInt32AtIndex(tag_t tag, int index, bool* ok=0) const;
		uint32_t getFieldValueUInt32(tag_t tag, bool* ok=0) const;
		uint32_t getFieldValueUInt32AtIndex(tag_t tag, int index, bool* ok=0) const;
		
		int64_t getFieldValueInt64(tag_t tag, bool* ok=0) const;
		int64_t getFieldValueInt64AtIndex(tag_t tag, int index, bool* ok=0) const;
		uint64_t getFieldValueUInt64(tag_t tag, bool* ok=0) const;
		uint64_t getFieldValueUInt64AtIndex(tag_t tag, int index, bool* ok=0) const;
		
		float getFieldValueFloat(tag_t tag, bool* ok=0) const;
		float getFieldValueFloatAtIndex(tag_t tag, int index, bool* ok=0) const;
		double getFieldValueDouble(tag_t tag, bool* ok=0) const;
		double getFieldValueDoubleAtIndex(tag_t tag, int index, bool* ok=0) const;
		
		bytes_t getFieldValueBytes(tag_t tag, bool* ok=0) const;
		bytes_t getFieldValueBytesAtIndex(tag_t tag, int index, bool* ok=0) const;
		std::string getFieldValueString(tag_t tag, bool* ok=0) const;
		std::string getFieldValueStringAtIndex(tag_t tag, int index, bool* ok=0) const;
		Message* getFieldValueMessage(tag_t tag, bool* ok=0) const;
		Message* getFieldValueMessageAtIndex(tag_t tag, int index, bool* ok=0) const;
		
		int serializedLength() const;
		Buffer* serialized() const;
		
		void clear() { fields_.clear(); }
		
		/**
		 * Deserializes message from buffer.
		 * @param doNotCopy  If true, length delimited chunks of data will not be copied
		 *                   while parsing data. Caller must ensure the whole buffer memory
		 *                   is not freed before accessing this chunks.
		 */
		bool deserialize(const Buffer& buffer);
		bool deserialize(Buffer& buffer, bool doNotCopy=false);
		bool deserialize(bytes_t bytes, bool doNotCopy=false);
		bool deserialize(const uint8_t* bytes, int length, bool doNotCopy=false);
		
		Message& operator<< (const varint& value);
		Message& operator<< (const fixed& value);
		Message& operator<< (const msg& msg);
		Message& operator<< (const bytes& b);
		
	private:
		struct Field
		{
			enum DataType
			{
				DataType_Null = 0,
				DataType_Int32 = 1,
				DataType_UInt32 = 2,
				DataType_Int64 = 3,
				DataType_UInt64 = 4,
				DataType_Float = 5,
				DataType_Double = 6,
				DataType_Fixed32 = 7, // bitwise convertible to any 32 type, stored in value_uint32
				DataType_Fixed64 = 8, // bitwise convertible to any 64 type, stored in value_uint64
				DataType_RawBytes = 9,
				DataType_Message = 10
			};
			
			Field() : dataType(DataType_Null), wireType(WireType_Undefined) { }
			Field(DataType dt, WireType wt) : dataType(dt), wireType(wt),
				own_bytes(false), own_message(false) { }
			
			bool isNull() const { return dataType == DataType_Null; }
			
			DataType dataType;
			WireType wireType;
			
			union
			{
				int32_t value_int32;
				uint32_t value_uint32;
				int64_t value_int64;
				uint64_t value_uint64;
				float value_float;
				double value_double;
			};
			
			bytes_t value_bytes;
			bool own_bytes;
			mutable Message* value_message;
			bool own_message;
		};
		
	private:
		void setField(tag_t tag, const Field& field);
		void addField(tag_t tag, const Field& field);
		void cleanUpField(const Field& field);
		Field field(tag_t tag, int index) const;
		
		void serialize(Buffer& buf) const;
		
		static uint64_t readVarint(Buffer& buf, bool* ok);
		static void writeVarint(Buffer& buf, uint64_t v);
		static int varintSize(uint64_t v);
		
		void addFieldValueBytesWithOwns(tag_t tag, const uint8_t* bytes, int length, bool owns);
		
	private:
		typedef std::vector<Field> fields_vector;
		typedef std::map<tag_t, fields_vector> fields_map;
		fields_map fields_;
	};
	
	
	inline uint32_t zigZagEncode32(int32_t x)
	{
		return (((x) << 1) ^ ((x) >> 31));
	}

	inline int32_t zigZagDecode32(uint32_t x)
	{
		return (((x) >> 1) ^ (-((x) & 1)));
	}

	inline uint64_t zigZagEncode64(int64_t x)
	{
		return (((x) << 1) ^ ((x) >> 63));
	}

	inline int64_t zigZagDecode64(uint64_t x)
	{
		return (((x) >> 1) ^ (-((x) & 1)));
	}

}
