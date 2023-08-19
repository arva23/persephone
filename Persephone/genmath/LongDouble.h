#ifndef LONGDOUBLE_H_INCLUDED
#define LONGDOUBLE_H_INCLUDED
#include <stdint.h>
#include <string>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <numeric>
#include <cfenv>
#include "ObjectBase.h"

namespace genmath {

	class LongDouble : public ObjectBase {

	public:

		static LongDouble Zero;
		static LongDouble Two;

		LongDouble();
		LongDouble(long double data);
	private:
		size_t ProcessLiteral(std::string data);
	public:
		LongDouble(std::string data);
		LongDouble(const char* data);
		LongDouble(const LongDouble& orig);
		virtual ~LongDouble();

		static long double precision_threshold_;
		static void SetPrecisionThreshold(LongDouble& threshold);

		bool operator=(std::string data) override;

		LongDouble& operator=(const LongDouble& operand);
		bool operator=(const char* data);

		bool operator==(const LongDouble& operand) const;
		bool operator<=(const LongDouble& operand) const;
		bool operator>=(const LongDouble& operand) const;
		bool operator<(const LongDouble& operand) const;
		bool operator>(const LongDouble& operand) const;
		bool operator!=(const LongDouble& operand)const;

		void operator+=(const LongDouble& operand);
		void operator-=(const LongDouble& operand);
		void operator*=(const LongDouble& operand);
		void operator/=(const LongDouble& operand);
		void operator%=(const LongDouble& operand);

		LongDouble operator+(const LongDouble& operand) const;
		LongDouble operator-(const LongDouble& operand) const;
		LongDouble operator*(const LongDouble& operand) const;
		LongDouble operator/(const LongDouble& operand) const;
		LongDouble operator%(const LongDouble& operand) const;

		LongDouble operator++();
		LongDouble operator++(int);
		LongDouble operator--();
		LongDouble operator--(int);

		int64_t ToInt64() const;
		operator long double() const;
		operator std::string() const override;
		static LongDouble Sqrt(const LongDouble& operand);
		// assigning input value to the nearest value according to the discreetly generated sequence
		static LongDouble RawRound(const LongDouble value, const LongDouble step);
		static LongDouble Abs(const LongDouble operand);
		static LongDouble Sgn(const LongDouble operand);

	private:
		long double data_;
	};
}

#endif// LONGDOUBLE_H_INCLUDED