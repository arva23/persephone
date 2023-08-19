#ifndef FIXEDPOINT_H_INCLUDED
#define FIXEDPOINT_H_INCLUDED
#include "../include/libfixmath/fixmath.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <exception>
#include "ObjectBase.h"
namespace genmath {

	class FixedPoint : public ObjectBase {

	public:
		FixedPoint();
		FixedPoint(fix16_t data, bool from_int = false);
		FixedPoint(std::string data);
		FixedPoint(const char* data);
		FixedPoint(const FixedPoint& orig);
		virtual ~FixedPoint();

		static fix16_t precision_threshold_;
		static void SetPrecisionThreshold(FixedPoint& threshold);

		bool operator=(std::string operand) override;
		FixedPoint& operator=(const FixedPoint& operand);
		bool operator=(const char* operand);
	
		bool operator==(const FixedPoint& operand) const;
		bool operator<=(const FixedPoint& operand) const;
		bool operator>=(const FixedPoint& operand) const;
		bool operator<(const FixedPoint& operand) const;
		bool operator>(const FixedPoint& operand) const;
		bool operator!=(const FixedPoint& operand)const;

		void operator+=(const FixedPoint& operand);
		void operator-=(const FixedPoint& operand);
		void operator*=(const FixedPoint& operand);
		void operator/=(const FixedPoint& operand);
		void operator%=(const FixedPoint& operand);
	
		FixedPoint operator+(const FixedPoint& operand) const;
		FixedPoint operator-(const FixedPoint& operand) const;
		FixedPoint operator*(const FixedPoint& operand) const ;
		FixedPoint operator/(const FixedPoint& operand) const;
		FixedPoint operator%(const FixedPoint& operand) const;

		FixedPoint operator++();
		FixedPoint operator++(int);
		FixedPoint operator--();
		FixedPoint operator--(int);

		int64_t ToInt64() const;
		operator fix16_t() const;
		operator std::string() const override;
		static FixedPoint Sqrt(const FixedPoint& operand);
		// assigning input value to the nearest value according to the discreetly generated sequence
		static FixedPoint RawRound(FixedPoint value, FixedPoint step);
		static FixedPoint Abs(FixedPoint operand);

	private:
		fix16_t data_;
	};
}
#endif// FIXEDPOINT_H_INCLUDED
