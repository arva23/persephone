#include "UInt64T.h"
genmath::UInt64T::UInt64T() {

	data_ = 0;
}
genmath::UInt64T::UInt64T(unsigned long long data) {

	data_ = data;
}

genmath::UInt64T::UInt64T(std::string data) {

	if (data.empty()) {
	
		throw std::exception("Value is empty (UInt64T).");
	}
	else {
	
		bool valid = true;
		size_t size_of_data = data.size();
		valid = valid &&
			(data[0] == '0' || data[0] == '1' || data[0] == '2' || data[0] == '3' || data[0] == '4'
				|| data[0] == '5' || data[0] == '6' || data[0] == '7' || data[0] == '8' || data[0] == '9'
				|| data[0] == '-');

		for (size_t i = 1; i < size_of_data && valid; ++i)
			valid = valid &&
			(data[i] == '0' || data[i] == '1' || data[i] == '2' || data[i] == '3' || data[i] == '4'
				|| data[i] == '5' || data[i] == '6' || data[i] == '7' || data[i] == '8' || data[i] == '9'
				|| data[i] == '.');

		if (!valid) {

			throw std::exception("Invalid conversion from literal (UInt64T).");
		}
		else {
	
			if (data[0] == '0') {
	
				data_ = 0;
			}
			else {
	
				data_ = std::atoi(data.c_str());
		
				if (!data_)
					throw std::exception("Invalid conversion from literal (UInt64T).");
			}
		}
	}
}

genmath::UInt64T::UInt64T(const char* data) {

	std::string sdata(data);

	if (sdata.empty()) {

		throw std::exception("Value is empty (UInt64T).");
	}
	else {

		bool valid = true;
		size_t size_of_data = sdata.size();
		valid = valid &&
			(sdata[0] == '0' || sdata[0] == '1' || sdata[0] == '2' || sdata[0] == '3' || sdata[0] == '4'
				|| sdata[0] == '5' || sdata[0] == '6' || sdata[0] == '7' || sdata[0] == '8' || sdata[0] == '9'
				|| sdata[0] == '-');

		for (size_t i = 1; i < size_of_data && valid; ++i)
			valid = valid &&
			(sdata[i] == '0' || sdata[i] == '1' || sdata[i] == '2' || sdata[i] == '3' || sdata[i] == '4'
				|| sdata[i] == '5' || sdata[i] == '6' || sdata[i] == '7' || sdata[i] == '8' || sdata[i] == '9'
				|| sdata[i] == '.');

		if (!valid) {

			throw std::exception("Invalid conversion from literal (UInt64T).");
		}
		else {

			if (sdata[0] == '0') {

				data_ = 0;
			}
			else {

				data_ = std::atoi(sdata.c_str());

				if (!data_)
					throw std::exception("Invalid conversion from literal (UInt64T).");
			}
		}
	}
}

genmath::UInt64T::UInt64T(const UInt64T& orig) {

	data_ = orig.data_;
}

genmath::UInt64T::~UInt64T() {


}

bool genmath::UInt64T::operator=(std::string data) {

	if (data.empty()) {

		return true;
	}
	else {

		bool valid = true;
		size_t size_of_data = data.size();
		valid = valid &&
			(data[0] == '0' || data[0] == '1' || data[0] == '2' || data[0] == '3' || data[0] == '4'
				|| data[0] == '5' || data[0] == '6' || data[0] == '7' || data[0] == '8' || data[0] == '9'
				|| data[0] == '-');

		for (size_t i = 1; i < size_of_data && valid; ++i)
			valid = valid &&
			(data[i] == '0' || data[i] == '1' || data[i] == '2' || data[i] == '3' || data[i] == '4'
				|| data[i] == '5' || data[i] == '6' || data[i] == '7' || data[i] == '8' || data[i] == '9'
				|| data[i] == '.');

		if (!valid) {

			return true;
		}
		else {

			if (data[0] == '0') {

				data_ = 0;
			}
			else {

				data_ = std::atoi(data.c_str());

				if (!data_) return true;
			}
		}
	}

	return false;
}

genmath::UInt64T& genmath::UInt64T::operator=(const UInt64T& operand) {

	data_ = operand.data_;
	
	return *this;
}

/*
genmath::UInt64T& genmath::UInt64T::operator=(unsigned long long operand) {

	data_ = operand;

	return *this;
}
*/

bool genmath::UInt64T::operator=(const char* data) {

	std::string sdata(data);

	if (sdata.empty()) {

		return true;
	}
	else {

		bool valid = true;
		size_t size_of_data = sdata.size();
		valid = valid &&
			(sdata[0] == '0' || sdata[0] == '1' || sdata[0] == '2' || sdata[0] == '3' || sdata[0] == '4'
				|| sdata[0] == '5' || sdata[0] == '6' || sdata[0] == '7' || sdata[0] == '8' || sdata[0] == '9'
				|| sdata[0] == '-');

		for (size_t i = 1; i < size_of_data && valid; ++i)
			valid = valid &&
			(sdata[i] == '0' || sdata[i] == '1' || sdata[i] == '2' || sdata[i] == '3' || sdata[i] == '4'
				|| sdata[i] == '5' || sdata[i] == '6' || sdata[i] == '7' || sdata[i] == '8' || sdata[i] == '9'
				|| sdata[i] == '.');

		if (!valid) {

			return true;
		}
		else {

			if (sdata[0] == '0') {

				data_ = 0;
			}
			else {

				data_ = std::atoi(sdata.c_str());

				if (!data_) return true;
			}
		}
	}
}

bool genmath::UInt64T::operator==(const UInt64T& operand) const {

	return data_ == operand.data_;
}

bool genmath::UInt64T::operator<=(const UInt64T& operand) const {

	return data_ <= operand.data_;
}

bool genmath::UInt64T::operator>=(const UInt64T& operand) const {

	return data_ >= operand.data_;
}

bool genmath::UInt64T::operator<(const UInt64T& operand) const {

	return data_ < operand.data_;
}

bool genmath::UInt64T::operator>(const UInt64T& operand) const {

	return data_ > operand.data_;
}

bool genmath::UInt64T::operator!=(const UInt64T& operand) const {

	return data_ != operand.data_;
}

void genmath::UInt64T::operator+=(const UInt64T& operand) {

	data_ += operand.data_;
}

void genmath::UInt64T::operator-=(const UInt64T& operand) {

	data_ -= operand.data_;
}

void genmath::UInt64T::operator*=(const UInt64T& operand) {

	data_ *= operand.data_;
}

void genmath::UInt64T::operator/=(const UInt64T& operand) {

	data_ /= operand.data_;
}

void genmath::UInt64T::operator%=(const UInt64T& operand) {

	data_ %= operand.data_;
}

genmath::UInt64T genmath::UInt64T::operator+(const UInt64T& operand) const {

	return UInt64T(data_ + operand.data_);
}

genmath::UInt64T genmath::UInt64T::operator-(const UInt64T& operand) const {

	return UInt64T(data_ - operand.data_);
}

genmath::UInt64T genmath::UInt64T::operator*(const UInt64T& operand) const {

	return UInt64T(data_ * operand.data_);
}

genmath::UInt64T genmath::UInt64T::operator/(const UInt64T& operand) const {

	return UInt64T(data_ / operand.data_);
}

genmath::UInt64T genmath::UInt64T::operator%(const UInt64T& operand) const {

	return UInt64T(data_ % operand.data_);
}

genmath::UInt64T genmath::UInt64T::operator++() {

	++data_;
	return *this;
}

genmath::UInt64T genmath::UInt64T::operator++(int) {

	UInt64T ret(*this);
	++data_;
	return ret;
}

genmath::UInt64T genmath::UInt64T::operator--() {

	--data_;
	return *this;
}

genmath::UInt64T genmath::UInt64T::operator--(int) {

	UInt64T ret(*this);
	--data_;
	return ret;}

genmath::UInt64T::operator unsigned long long() const {

	return data_;
}

genmath::UInt64T::operator std::string() const {

	return std::to_string(data_);
}

genmath::UInt64T genmath::UInt64T::Sqrt(const UInt64T& operand) {

	return sqrt(operand.data_);
}