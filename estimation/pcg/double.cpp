#include "double.h"
#include <stdint.h>
#include <iostream>
#include <stdlib.h>

#define ENABLE_ASSERT 0

/**
* \brief construction function of Double struct
*/
Double::Double()
{
	Double(0);
}

/**
* \brief construct a Double struct from a double value
* \param const double &: double precision(64 bits) floating point value input
*/
Double::Double(const double &value)
{
	data = value * ((long long)1 << FRACTION_NBIT);
}

/**
* \brief adds a Double value
* \param const Double &: right operand
* \return Double: sum of two operands
*/
Double Double::operator + (const Double &value) const
{
	_ASSERT(!check_overflow(double(*this) + double(value)));
	long long sum = data + value.data;

	Double res;
	res.data = sum;

	_ASSERT(check_result(double(*this) + double(value), res));
	return res;
}

/**
* \brief minus a Double value
* \param const Double &: right operand
* \return Double: difference of two operands
*/
Double Double::operator - (const Double &value) const
{
	_ASSERT(!check_overflow(double(*this) - double(value)));
	long long diff = data - value.data;
	Double res;
	res.data = diff;

	_ASSERT(check_result(double(*this) - double(value), res));
	return res;
}

/**
* \brief multiplies by a Double value
* \param const Double &: right operand
* \return Double: product of two operands
*/
Double Double::operator * (const Double &value) const
{
	_ASSERT(!check_overflow(double(*this) * double(value)));
	union Data left, right;
	left.d = *this;
	right.d = value;

	long long mul = 0;
	unsigned long long frac = 0;

	int ex_bit = (FRACTION_NBIT > 32 ? FRACTION_NBIT - 32 : 0);
	frac = (left.f.fraction >> ex_bit)*(right.f.fraction >> ex_bit);

	mul += left.f.integer*right.f.integer*((long long)1 << FRACTION_NBIT);
	mul += left.f.integer*right.f.fraction;
	mul += left.f.fraction*right.f.integer;
	mul += frac >> (FRACTION_NBIT - ex_bit * 2);

	Double res;
	res.data = mul;

	_ASSERT(check_result(double(*this) * double(value), res));
	return res;
}

/**
* \brief check how many bits atmost member 'data' can be shifted left without bit overflow
* \param const Double &: right operand
* \return Double: max bit count that member 'data' can be shifted left
*/
int Double::check_shift() const
{
	bool sign_bit = (data >> 63) & 1;
	int sl = 0;

	for (int i = 62; i > 0; i--)
	{
		if ((data >> i & 1) == sign_bit)
		{
			sl++;
		}
		else
		{
			break;
		}
	}

	return sl;
}

/**
* \brief divides by a Double value
* \param const Double &: right operand
* \return Double: quotient
*/
Double Double::operator / (const Double &value) const
{
	_ASSERT(!check_overflow(double(*this)/double(value)));
	union Data left, right;
	left.d = *this;
	right.d = value;

	int sl = 0;
	int sr = 0;

	sl = check_shift();
	sl = (sl > FRACTION_NBIT) ? FRACTION_NBIT : sl;
	sr = FRACTION_NBIT - sl;

	left.d.data <<= sl;
	right.d.data >>= sr;

	long long div = ((unsigned long long)1 << 63) - 1;
	if (right.d.data != 0)
	{
		div = left.d.data / right.d.data;
	}

	Double res;
	res.data = div;

	_ASSERT(check_result(double(*this) / double(value), res));

	return res;
}

/**
* \brief compares with another Double value
* \param const Double &: right operand
* \return Double: true if left operand is greater, false otherwise
*/
bool Double::operator > (const Double &value)const
{
	return data > value.data;
}

/**
* \brief compares with another Double value
* \param const Double &: right operand
* \return Double: true if left operand is greater, false otherwise
*/
bool Double::operator < (const Double &value)const
{
	return data < value.data;
}

/**
* \brief compares with another Double value
* \param const Double &: right operand
* \return Double: true if two operands are equal, false otherwise
*/
bool Double::operator == (const Double &value)const
{
	return data == value.data;
}

/**
* \brief adds a Double value, and assigns the result to itself simutanously
* \param const Double &: right operand
* \return Double: itself
*/
Double& Double::operator += (const Double &value)
{
	_ASSERT(!check_overflow(double(*this) + double(value)));
	_ASSERT(check_result(double(*this) + double(value), (*this) + value));
	*this = *this + value;

	return *this;
}

/**
* \brief minus a Double value, and assigns the result to itself simutanously
* \param const Double &: right operand
* \return Double: itself
*/
Double& Double::operator -= (const Double &value)
{
	_ASSERT(!check_overflow(double(*this) - double(value)));
	_ASSERT(check_result(double(*this) - double(value), (*this) - value));
	*this = *this - value;

	return *this;
}

/**
* \brief multiplies by a Double value, and assigns the result to itself simutanously
* \param const Double &: right operand
* \return Double: itself
*/
Double& Double::operator *= (const Double &value)
{
	_ASSERT(!check_overflow(double(*this) * double(value)));
	_ASSERT(check_result(double(*this) * double(value), (*this) * value));
	*this = *this * value;

	return *this;
}

/**
* \brief divides by a Double value, and assigns the result to itself simutanously
* \param const Double &: right operand
* \return Double: itself
*/
Double& Double::operator /= (const Double &value)
{
	_ASSERT(!check_overflow(double(*this) / double(value)));
	_ASSERT(check_result(double(*this) / double(value), (*this) / value));
	*this = *this / value;

	return *this;
}

/**
* \brief converts to a double value 
* \param const void: none
* \return double: double value copy
*/
Double::operator double(void) const
{
	double d = (double)data / (double)((long long)1 << FRACTION_NBIT);

	return d;
}

/**
* \brief check if a double value can be expressed by a Double type approximately, without MSBs overflow
* \param const double: double precisioin floating point value
* \return double: true if value can be expressed by a Double type
*/
FOR_DEBUG bool Double::check_overflow(double value) const
{
	long long l = ((unsigned long long)1<<63) - 1;

	Double d;
	d.data = l;

	if (value > d || value < -d)
	{
		return ENABLE_ASSERT;
	}
	else
	{
		return false;
	}
}

/**
* \brief check if the absolute difference ratio of two values is less than a tolerance specified globally
* \param const double value1: the 1st value
* \param const double value2: the 2nd value
* \return bool: true if abs(value1-value2) < TOLERANCE false otherwise
*/
FOR_DEBUG bool Double::check_result(double value1, double value2) const
{
	
	if ((value1 != 0) && (fabs(1 - value2 / value1) < TOLERANCE) || fabs(value1 - value2)  < TOLERANCE)
	{
		return true;
	}
	else
	{
		return false;
	}
}