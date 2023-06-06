#pragma once
#include "ObjectBase.h"
#include <string>
#include <math.h>

namespace genmath {

	class UInt64T : public ObjectBase {

	public:
		UInt64T();
		UInt64T(unsigned long long data);
		UInt64T(std::string data);
		UInt64T(const char* data);
		UInt64T(const UInt64T& orig);
		virtual ~UInt64T();

		bool operator=(std::string data) override;

		UInt64T& operator=(const UInt64T& operand);
		//UInt64T& operator=(unsigned long long operand);
		bool operator=(const char* data);

		bool operator==(const UInt64T& operand) const;
		bool operator<=(const UInt64T& operand) const;
		bool operator>=(const UInt64T& operand) const;
		bool operator<(const UInt64T& operand) const;
		bool operator>(const UInt64T& operand) const;
		bool operator!=(const UInt64T& operand) const;

		void operator+=(const UInt64T& operand);
		void operator-=(const UInt64T& operand);
		void operator*=(const UInt64T& operand);
		void operator/=(const UInt64T& operand);
		void operator%=(const UInt64T& operand);

		UInt64T operator+(const UInt64T& operand) const;
		UInt64T operator-(const UInt64T& operand) const;
		UInt64T operator*(const UInt64T& operand) const;
		UInt64T operator/(const UInt64T& operand) const;
		UInt64T operator%(const UInt64T& operand) const;

		UInt64T operator++();
		UInt64T operator++(int);
		UInt64T operator--();
		UInt64T operator--(int);

		operator unsigned long long() const;
		operator std::string() const override;
		static UInt64T Sqrt(const UInt64T& operand);

	private:
		unsigned long long data_;
	};

}

