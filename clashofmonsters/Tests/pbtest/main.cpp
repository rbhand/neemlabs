//
//  main.cpp
//  pbtest
//
//

#include <iostream>

#include "Message.h"

using namespace pb;
using namespace std;

namespace pb_test
{
	struct Tests
	{
		Tests() : taken(0), passed(0), failed(0) {} 
		
		int taken;
		int passed;
		int failed;
	};
}

using namespace pb_test;

#define test_assert(condition) \
	{ \
		tests.taken++; \
		if (!(condition)) \
		{ \
			std::cout << "TEST FAILED: " << #condition << " at line " << __LINE__ << endl; \
			tests.failed++; \
		} \
		else \
		{ \
			tests.passed++; \
		} \
	}

int main(int argc, char** argv)
{
	Tests tests;
	
	cout << "Protocol buffer unit tests" << endl;
	
	cout << "Zig zag encoding..." << endl;
	
	test_assert(zigZagEncode32(0) == 0);
	test_assert(zigZagEncode32(-1) == 1);
	test_assert(zigZagEncode32(1) == 2);
	test_assert(zigZagEncode32(-2) == 3);
	test_assert(zigZagEncode32(2147483647) == 4294967294);
	test_assert(zigZagEncode32(-2147483648) == 4294967295);
	
	test_assert(zigZagDecode32(0) == 0);
	test_assert(zigZagDecode32(1) == -1);
	test_assert(zigZagDecode32(2) == 1);
	test_assert(zigZagDecode32(3) == -2);
	test_assert(zigZagDecode32(4294967294) == 2147483647);
	test_assert(zigZagDecode32(4294967295) == -2147483648);
	
	test_assert(zigZagEncode64(0) == 0);
	test_assert(zigZagEncode64(-1) == 1);
	test_assert(zigZagEncode64(1) == 2);
	test_assert(zigZagEncode64(-2) == 3);
	test_assert(zigZagEncode64(2147483647) == 4294967294);
	test_assert(zigZagEncode64(-2147483648) == 4294967295);
	
	test_assert(zigZagDecode64(0) == 0);
	test_assert(zigZagDecode64(1) == -1);
	test_assert(zigZagDecode64(2) == 1);
	test_assert(zigZagDecode64(3) == -2);
	test_assert(zigZagDecode64(4294967294) == 2147483647);
	test_assert(zigZagDecode64(4294967295) == -2147483648);
	
	
	cout << "Message..." << endl;
	cout << "  length..." << endl;
	
	cout << "    primitives..." << endl;
	
	{
		Message msg;
		msg << varint::ui(1, 100) // 1+1
			<< varint::i(2, -1); // 1+1
		test_assert(msg.serializedLength() == 4);
	}
	
	{
		Message msg;
		msg << varint::ui(1, 0x7ff) // 1+2
			<< varint::i(2, -1); // 1+1
		test_assert(msg.serializedLength() == 5);
	}
	
	{
		Message msg;
		msg << varint::ui(1, 100) // 2
			<< varint::i(2, -1) // 2
			<< fixed::i32(3, -453534); // 1+4
		test_assert(msg.serializedLength() == 9);
	}
	
	{
		Message msg;
		msg << varint::ui(1, 100) // 2
			<< varint::i(2, -1) // 2
			<< fixed::i32(3, -453534) // 5
			<< fixed::ui64(4, 0xff5677667); // 9
		test_assert(msg.serializedLength() == 18);
	}
	
	{
		Message msg;
		msg << varint::ui(1, 100) // 2
			<< varint::i(2, -1) // 2
			<< fixed::i32(3, -453534) // 5
			<< fixed::f(4, 0.50f) // 5
			<< fixed::d(5, 0.50); // 9
		test_assert(msg.serializedLength() == 23);
	}
	
	cout << "    repeated..." << endl;
	
	{
		Message msg;
		msg << varint::ui(1, 100) // 2
			<< varint::i(2, -1) // 2
			<< varint::i(2, -1) // 2
			<< varint::i(2, -255) // 3
			<< fixed::i32(3, -453534) // 5
			<< fixed::f(4, 0.50f) // 5
			<< fixed::d(5, 0.50); // 9
		test_assert(msg.serializedLength() == 28);
	}
	
	cout << "    length delimited..." << endl;
	
	{
		Message msg;
		msg << bytes(1, "hello"); // 1 + 1 + 5
		test_assert(msg.serializedLength() == 7);
	}
	
	{
		Message m;
		Message* m2 = new Message;
		
		m << varint::ui(1, 2) // 1 + 1
			<< msg(2, m2); // 1 + 1
		*m2 << varint::ui(1, 2); // 1 + 1
		
		test_assert(m.serializedLength() == 6);
	}
	
	
	
	cout << "  serializing/deserializing..." << endl;
	
	cout << "    primitives..." << endl;
	
	{
		Message msg;
		msg << varint::ui(1, 100)
			<< varint::i(2, -100);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueUInt32(1) == 100);
		test_assert(r_msg.getFieldValueInt32(2) == -100);
	}
	
	{
		Message msg;
		msg << fixed::i32(3, 20);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueInt32(3) == 20);
	}
	
	{
		Message msg;
		msg << fixed::i64(3, -1500);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueInt64(3) == -1500);
	}
	
	{
		Message msg;
		msg << fixed::i32(3, -32500);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueInt32(3) == -32500);
	}
	
	{
		Message msg;
		msg << fixed::ui32(1, 0xfffffff2);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueUInt32(1) == 0xfffffff2);
	}
	
	{
		Message msg;
		msg << fixed::f(3, 500.32f);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueFloat(3) == 500.32f);
	}
	
	{
		Message msg;
		msg << fixed::d(3, -100.356);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueDouble(3) == -100.356);
	}
	
	
	cout << "    repeated..." << endl;
	
	{
		Message msg;
		msg << varint::ui(1, 100)
			<< varint::i(2, -1)
			<< varint::i(2, 2)
			<< varint::i(2, -255)
			<< fixed::i32(3, -453534);
			
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.countValues(2) == 3);
		test_assert(r_msg.getFieldValueInt32AtIndex(2, 0) == -1);
		test_assert(r_msg.getFieldValueInt32AtIndex(2, 1) == 2);
		test_assert(r_msg.getFieldValueInt32AtIndex(2, 2) == -255);
		test_assert(r_msg.getFieldValueInt32AtIndex(3, 0) == -453534);
	}
	
	{
		Message msg;
		msg << varint::i(2, -1)
			<< varint::i(2, 2)
			<< varint::ui(1, 100)
			<< varint::i(2, -255)
			<< fixed::i32(3, -453534);
			
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.countValues(2) == 3);
		test_assert(r_msg.getFieldValueInt32AtIndex(2, 0) == -1);
		test_assert(r_msg.getFieldValueInt32AtIndex(2, 1) == 2);
		test_assert(r_msg.getFieldValueInt32AtIndex(2, 2) == -255);
		test_assert(r_msg.getFieldValueInt32AtIndex(3, 0) == -453534);
	}
	
	
	
	cout << "  conversions..." << endl;
	
	{
		Message msg;
		msg << fixed::i32(3, -32500);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueInt64(3) == -32500);
	}
	
	{
		Message msg;
		msg << fixed::d(3, -100.356);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueFloat(3) == -100.356f);
	}
	
	{
		Message msg;
		msg << fixed::f(3, -100.356f);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueDouble(3) == -100.356f);
	}
	
	{
		Message msg;
		msg << fixed::i32(3, 500);
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueUInt32(3) == 500);
	}

	
	cout << "    length delimited..." << endl;
	
	{
		Message msg;
		msg << bytes(1, "hello");
		
		auto_ptr<Buffer> buf(msg.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueString(1) == "hello");
	}
	
	{
		Message m;
		Message* m2 = new Message;
		
		m << varint::ui(1, 2)
			<< msg(2, m2);
		*m2 << varint::ui(1, 4);
		
		auto_ptr<Buffer> buf(m.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueUInt32(1) == 2);
		test_assert(r_msg.getFieldValueMessage(2));
		test_assert(r_msg.getFieldValueMessage(2)->getFieldValueUInt32(1) == 4);
	}
	
	{
		Message m;
		Message* m2 = new Message;
		
		m << varint::ui(1, 2)
			<< msg(2, m2);
		*m2 << varint::ui(1, 4)
			<< fixed::i32(2, 100);
		
		auto_ptr<Buffer> buf(m.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueUInt32(1) == 2);
		test_assert(r_msg.getFieldValueMessage(2));
		test_assert(r_msg.getFieldValueMessage(2)->getFieldValueUInt32(1) == 4);
		test_assert(r_msg.getFieldValueMessage(2)->getFieldValueUInt32(2) == 100);
	}
	
	{
		Message m;
		Message* m2 = new Message;
		Message* m3 = new Message;
		
		m << varint::ui(1, 2)
			<< msg(2, m2);
		*m3 << varint::ui(1, 4)
			<< fixed::i32(2, 100);
		*m2 << varint::ui(1, 4)
			<< msg(2, m3);
		
		auto_ptr<Buffer> buf(m.serialized());
		
		Message r_msg;
		test_assert(r_msg.deserialize(buf.get()));
		test_assert(r_msg.getFieldValueUInt32(1) == 2);
		test_assert(r_msg.getFieldValueMessage(2));
		test_assert(r_msg.getFieldValueMessage(2)->getFieldValueUInt32(1) == 4);
		test_assert(r_msg.getFieldValueMessage(2)->getFieldValueMessage(2)->getFieldValueUInt32(2) == 100);
	}
	
	
	
	
	cout << endl;
	cout << "----------------" << endl;
	cout << "Total:  " << tests.taken << endl;
	cout << "Passed: " << tests.passed << endl;
	cout << "Failed: " << tests.failed << endl;
	
	return tests.failed > 0 ? 1 : 0;
}
