//
//  Buffer.h
//

#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace pb
{
	class Buffer
	{
	public:
		Buffer();
		Buffer(int capacity);
		Buffer(const char* fileName);
		Buffer(uint8_t* data, int length);
		~Buffer();
		
		uint8_t read(bool* ok=0);
		int read(void* out_bytes, int length);
		
		int write(const uint8_t* bytes, int length);
		int write(uint8_t b);
		int writeBase64(const char* bytes, int length);
		void seek(int pos);
		void rewind() { p_ = data_; }
		int skip(int pos);
		int tell() const;
		uint8_t* current() const;
		
		uint8_t* data() const { return data_; }
		
		int size() const { return length_; }
		int capacity() const { return capacity_; }
		
		void writeToFile(const char* fileName);
		std::vector<char> toBase64() const;
		
	private:
		Buffer(const Buffer&b)
		{
			#ifndef PB_NO_EXCEPTIONS
			throw;
			#endif
		} // no copy
		Buffer& operator=(const Buffer&b)
		{
			#ifndef PB_NO_EXCEPTIONS
			throw;
			#endif
			return *this;
		} // no assignment
		
		void prepareWrite(int length);
	
	private:
		uint8_t* data_;
		uint8_t* p_;
		int length_;
		int capacity_;
		bool owns_;
	};
}
