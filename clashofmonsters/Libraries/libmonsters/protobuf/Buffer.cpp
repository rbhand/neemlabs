//
//  Buffer.cpp
//

#include "Buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <algorithm>
#include <iostream>

extern "C"
{
#include <b64/cdecode.h>
#include <b64/cencode.h>
}

using namespace pb;
using namespace std;

Buffer::Buffer() :
	data_(0), p_(0), length_(0), capacity_(0), owns_(true)
{
	
}

Buffer::Buffer(int capacity) :
	length_(0), capacity_(capacity), owns_(true)
{
	assert(capacity_ > 0);
	p_ = data_ = (uint8_t*)malloc(capacity_);
}

Buffer::Buffer(uint8_t* data, int length) :
	data_(data), p_(data), length_(length), capacity_(length), owns_(false)
{
	assert(data_);
}

Buffer::~Buffer()
{
	if (owns_ && data_)
		free(data_);
}

uint8_t Buffer::read(bool* ok)
{
	if (data_ && p_ < (data_ + length_))
	{
		if (ok)
			*ok = true;
		return *(p_++);
	}
	
	if (ok)
		*ok = false;
	return 0;
}

int Buffer::read(void* out_bytes, int length)
{
	assert(length >= 0);
	int read = std::min(length_ - tell(), length);
	memcpy(out_bytes, p_, length);
	p_ += read;
	return read;
}

void Buffer::prepareWrite(int length)
{
	if (!data_)
	{
		p_ = data_ = (uint8_t*)malloc(capacity_ = std::max(32, length));
		length_ = 0;
	}
	
	if ((tell() + length) > capacity_ && owns_)
	{
		data_ = (uint8_t*)realloc(data_, tell() + length);
		capacity_ = tell() + length;
	}
}

int Buffer::write(uint8_t b)
{
	return write(&b, 1);
}

int Buffer::write(const uint8_t* bytes, int length)
{
//	cout << "Buffer::write ";
//	for (int i=0; i<length; ++i)
//		cout << hex << (unsigned int)bytes[i] << " ";
//	cout << endl;

	prepareWrite(length);
	
	int written = std::min(length, capacity_ - tell());
	if (written)
		memcpy(p_, bytes, written);
		
	length_ = std::max(length_, tell() + written);
	p_ = p_ + written;
	
	return written;
}

int Buffer::writeBase64(const char* bytes, int length)
{
	if (length > 0)
	{
		int bytes_length = (length * 3) / 4 + ((length * 3) % 4 ? 1 : 0);
		const char* b = bytes + length;
		while (*b == '=' && bytes != b)
		{
			--bytes_length;
			--b;
		}
		
		prepareWrite(bytes_length);
	
		base64_decodestate decoder;
		base64_init_decodestate(&decoder);
		int	written = base64_decode_block(bytes, length, (char*)p_, &decoder);
		length_ = std::max(length_, tell() + written);
		p_ = p_ + written;
		
		return written;
	}
	
	return 0;
}

std::vector<char> Buffer::toBase64() const
{
	int base64_length = ((length_ + ( (length_ % 3) ? (3 - (length_ % 3)) : 0) ) / 3) * 4 + 1;
	std::vector<char> v(base64_length);
	
	base64_encodestate encoder;
	base64_init_encodestate(&encoder);
	int written = base64_encode_block((const char*)data_, length_, v.data(), &encoder);
	written += base64_encode_blockend(v.data() + written, &encoder);
	v[written] = '\0';
	
	return v;
}

void Buffer::seek(int pos)
{
	assert(pos >= 0 && pos < length_);
	p_ = data_ + pos;
}

int Buffer::skip(int pos)
{
	int skipped = std::min(length_ - tell(), pos);
	p_ += skipped;
	return skipped;
}

int Buffer::tell() const
{
	return p_ - data_;
}

uint8_t* Buffer::current() const
{
	return p_;
}

void Buffer::writeToFile(const char* fileName)
{
	FILE* f = fopen(fileName, "w");
	if (length_)
	{
		uint8_t* p = data_;
		uint8_t* e = data_ + length_;
		
		while (p != e)
		{
			size_t sz = std::min((int)(e - p), 1024);
			p += fwrite(p, 1, sz, f);
		}
	}
	
	fclose(f);
}

Buffer::Buffer(const char* fileName) :
	data_(0), p_(0), length_(0), capacity_(0), owns_(false)
{
	FILE* f = fopen(fileName, "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length_ = capacity_ = ftell(f);
		owns_ = true;
		p_ = data_ = (uint8_t*)malloc(capacity_);
		
		fseek(f, 0, SEEK_SET);
		uint8_t* p = data_;
		uint8_t* e = data_ + length_;
		
		while (p != e)
		{
			size_t sz = std::min((int)(e - p), 1024);
			p += fread(p, 1, sz, f);
		}
	}
	
	fclose(f);
}
