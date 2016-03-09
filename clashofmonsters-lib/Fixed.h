//
//  Fixed.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 3/25/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include <stdint.h>

namespace w2f2
{
	/**
	 * Fixed-point number 64-bit with 16 bit fraction part (leaving 48 bits for whole part).
	 */
	class fixed64_t
	{
	public:
		fixed64_t() : v_(0) { }
		fixed64_t(const char* v);
		fixed64_t(const fixed64_t& other) : v_(other.v_) { }
//		fixed64_t(const float v) : v_(_convertFromFloat(v)) { }
//		fixed64_t(const double v) : v_(_convertFromDouble(v)) { }
		fixed64_t(const int64_t v) : v_(_convertFromInt64(v)) { }
		fixed64_t(const int32_t v) : v_(_convertFromInt64(v)) { }
		
		fixed64_t & operator=(const fixed64_t &rhs) { v_ = rhs.v_; return *this; }
		fixed64_t & operator=(const int64_t rhs) { v_ = _convertFromInt64(rhs); return *this; }
		fixed64_t & operator=(const int32_t rhs) { v_ = _convertFromInt64(rhs); return *this; }

		fixed64_t & operator+=(const fixed64_t &rhs) { v_ += rhs.v_; return *this; }
		fixed64_t & operator+=(const int64_t rhs) { v_ += _convertFromInt64(rhs); return *this; }
		fixed64_t & operator+=(const int32_t rhs) { v_ += _convertFromInt64(rhs); return *this; }

		fixed64_t & operator-=(const fixed64_t &rhs)  { v_ -= rhs.v_; return *this; }
		fixed64_t & operator-=(const int64_t rhs) { v_ -= _convertFromInt64(rhs); return *this; }
		fixed64_t & operator-=(const int32_t rhs) { v_ -= _convertFromInt64(rhs); return *this; }

		fixed64_t & operator*=(const fixed64_t &rhs) { v_ = _mul(v_, rhs.v_); return *this; }
		fixed64_t & operator*=(const int64_t rhs) { v_ = _mul(v_, _convertFromInt64(rhs)); return *this; }
		fixed64_t & operator*=(const int32_t rhs) { v_ = _mul(v_, _convertFromInt64(rhs)); return *this; }

		fixed64_t & operator/=(const fixed64_t &rhs)  { v_ = _div(v_, rhs.v_); return *this; }
		fixed64_t & operator/=(const int64_t rhs) { v_ = _div(v_, _convertFromInt64(rhs)); return *this; }
		fixed64_t & operator/=(const int32_t rhs) { v_ = _div(v_, _convertFromInt64(rhs)); return *this; }

		const fixed64_t operator+(const fixed64_t &other) const { fixed64_t ret = *this; ret += other; return ret; }
		const fixed64_t operator+(const int64_t other) const { fixed64_t ret = *this; ret += other; return ret; }
		const fixed64_t operator+(const int32_t other) const { fixed64_t ret = *this; ret += other; return ret; }
		
		const fixed64_t operator-(const fixed64_t &other) const { fixed64_t ret = *this; ret -= other; return ret; }
		const fixed64_t operator-(const int64_t other) const { fixed64_t ret = *this; ret -= other; return ret; }
		const fixed64_t operator-(const int32_t other) const { fixed64_t ret = *this; ret -= other; return ret; }
		
		const fixed64_t operator*(const fixed64_t &other) const { fixed64_t ret = *this; ret *= other; return ret; }
		const fixed64_t operator*(const int64_t other) const { fixed64_t ret = *this; ret *= other; return ret; }
		const fixed64_t operator*(const int32_t other) const { fixed64_t ret = *this; ret *= other; return ret; }
		
		const fixed64_t operator/(const fixed64_t &other) const { fixed64_t ret = *this; ret /= other; return ret; }
		const fixed64_t operator/(const int64_t other) const { fixed64_t ret = *this; ret /= other; return ret; }
		const fixed64_t operator/(const int32_t other) const { fixed64_t ret = *this; ret /= other; return ret; }
		
		bool operator==(const fixed64_t &other) const { return v_ == other.v_; }
		bool operator==(const int64_t &other) const { return v_ == _convertFromInt64(other); }
		bool operator==(const int32_t &other) const { return v_ == _convertFromInt64(other); }
		
		bool operator!=(const fixed64_t &other) const { return v_ != other.v_; }
		bool operator!=(const int64_t &other) const { return v_ != _convertFromInt64(other); }
		bool operator!=(const int32_t &other) const { return v_ != _convertFromInt64(other); }
		
		bool operator<(const fixed64_t &other) const { return v_ < other.v_; }
		bool operator<(const int64_t &other) const { return v_ < _convertFromInt64(other); }
		bool operator<(const int32_t &other) const { return v_ < _convertFromInt64(other); }
		
		bool operator<=(const fixed64_t &other) const { return v_ <= other.v_; }
		bool operator<=(const int64_t &other) const { return v_ <= _convertFromInt64(other); }
		bool operator<=(const int32_t &other) const { return v_ <= _convertFromInt64(other); }
		
		bool operator>(const fixed64_t &other) const { return v_ > other.v_; }
		bool operator>(const int64_t &other) const { return v_ > _convertFromInt64(other); }
		bool operator>(const int32_t &other) const { return v_ > _convertFromInt64(other); }
		
		bool operator>=(const fixed64_t &other) const { return v_ >= other.v_; }
		bool operator>=(const int64_t &other) const { return v_ >= _convertFromInt64(other); }
		bool operator>=(const int32_t &other) const { return v_ >= _convertFromInt64(other); }
		
		#if __cplusplus >= 201103L
		explicit
		#endif
		operator bool() const { return v_ != 0; }
		
		float toFloat() const { return (float)(v_ >> 16) + (float)(v_ & 0xffff) / (float)one; }
		double toDouble() const { return (double)(v_ >> 16) + (double)(v_ & 0xffff) / (double)one; }
		
		int64_t roundInt() const { return (v_ >> 16) + ((((uint64_t)v_ & 0xFFFF) >= (one / 2)) ? 1 : 0); }
		int64_t floorInt() const { return (v_ >> 16); }
		int64_t ceilInt() const { return (v_ >> 16) + ((((uint64_t)v_ & 0xFFFF) > 0) ? 1 : 0); }
		
	private:
		static int64_t _convertFromFloat(float v) { return v * (float)one; }
		static int64_t _convertFromDouble(double v) { return v * (double)one; }
		static int64_t _convertFromInt64(int64_t v) { return v << 16; }
		
		static int64_t _mul(int64_t v1, int64_t v2);
		static int64_t _div(int64_t v1, int64_t v2);
	
	private:
		static const int64_t one = 0x010000;
		
		int64_t v_;
	};
	
	inline fixed64_t operator*(int32_t a, fixed64_t b) { return fixed64_t(a) * b; }
	inline fixed64_t operator*(int64_t a, fixed64_t b) { return fixed64_t(a) * b; }
	inline fixed64_t operator/(int32_t a, fixed64_t b) { return fixed64_t(a) / b; }
	inline fixed64_t operator/(int64_t a, fixed64_t b) { return fixed64_t(a) / b; }
	inline fixed64_t operator+(int32_t a, fixed64_t b) { return fixed64_t(a) + b; }
	inline fixed64_t operator+(int64_t a, fixed64_t b) { return fixed64_t(a) + b; }
	inline fixed64_t operator-(int32_t a, fixed64_t b) { return fixed64_t(a) - b; }
	inline fixed64_t operator-(int64_t a, fixed64_t b) { return fixed64_t(a) - b; }
}
