#include "LongDouble.h"
genmath::LongDouble genmath::LongDouble::Zero = "0.0";
genmath::LongDouble genmath::LongDouble::Two = "2.0";

genmath::LongDouble::LongDouble() : ObjectBase() {
	
	data_ = 0.0;
}

genmath::LongDouble::LongDouble(long double data) : ObjectBase() {

	if (std::fetestexcept(FE_INVALID)) {

		std::feclearexcept(FE_INVALID);
		
		throw std::exception("Invalid conversion from long double (LongDobule).");
	}
	else {
	
		data_ = data;
	}
}

size_t genmath::LongDouble::ProcessLiteral(std::string data) {

	if (data.empty()) {

		data_ = 0.0;
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

			throw std::exception("Invalid conversion from literal (LongDouble).");
		}
		else {

			data_ = std::strtold(data.c_str(), NULL);

			if (std::fetestexcept(FE_INVALID)) {

				std::feclearexcept(FE_INVALID);

				throw std::exception("Invalid conversion from literal (LongDouble).");
			}
		}
	}

	return data.size();
}

genmath::LongDouble::LongDouble(std::string data) : ObjectBase() {

	ProcessLiteral(data);
}

genmath::LongDouble::LongDouble(const char* data) : ObjectBase() {
	
	ProcessLiteral(std::string(data));
}

genmath::LongDouble::LongDouble(const LongDouble& orig) : ObjectBase(orig) { data_ = orig.data_; }

genmath::LongDouble::~LongDouble() {}

long double genmath::LongDouble::precision_threshold_ = 0.01;// std::strtold("0.015625", NULL);//2^-6

void genmath::LongDouble::SetPrecisionThreshold(LongDouble& threshold) {

	precision_threshold_ = threshold;
}

bool genmath::LongDouble::operator=(std::string data) {
	
	return ProcessLiteral(data);
}

genmath::LongDouble& genmath::LongDouble::operator=(const LongDouble& operand) {

	ObjectBase::operator=(operand);
	data_ = operand.data_;
	return *this;
}

bool genmath::LongDouble::operator=(const char* data) {

	return ProcessLiteral(std::string(data));
}

bool genmath::LongDouble::operator==(const LongDouble& operand) const {

	return std::fabsl(data_ - operand.data_) <= precision_threshold_;
}

bool genmath::LongDouble::operator<=(const LongDouble& operand) const {

	return operand.data_ - data_ >= 0.0;
}

bool genmath::LongDouble::operator>=(const LongDouble& operand) const {

	return data_ - operand.data_ >= 0.0;
}

bool genmath::LongDouble::operator<(const LongDouble& operand) const {

	return operand.data_ - data_ > precision_threshold_;
}

bool genmath::LongDouble::operator>(const LongDouble& operand) const {

	return data_ - operand.data_ > precision_threshold_;
}

bool genmath::LongDouble::operator!=(const LongDouble& operand) const {

	return std::fabsl(data_ - operand.data_) > precision_threshold_;
}

void genmath::LongDouble::operator+=(const LongDouble& operand) {

	data_ += operand.data_;

	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {
		
		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow (unary addition) (LongDouble).");
	}
}

void genmath::LongDouble::operator-=(const LongDouble& operand) {
	
	data_ -= operand.data_;

	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {
	
		throw std::exception("Number overflow (unary subtraction) (LongDouble).");
		std::feclearexcept(FE_ALL_EXCEPT);
	}
}

void genmath::LongDouble::operator*=(const LongDouble& operand) {
	
	data_ *= operand.data_;

	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {
		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow (unary multiplication) (LongDouble).");
	}
}

void genmath::LongDouble::operator/=(const LongDouble& operand) {
	
	data_ /= operand.data_;

	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW) || std::fetestexcept(FE_DIVBYZERO)){
		
		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow or division by zero (unary division) (LongDouble).");
	}
}

void genmath::LongDouble::operator%=(const LongDouble& operand) {

	data_ = std::fmodl(data_, operand.data_);

	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW) || std::fetestexcept(FE_DIVBYZERO)){
	
		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow or division by zero (unary congruence) (LongDouble).");
	}
}

genmath::LongDouble genmath::LongDouble::operator+(const LongDouble& operand) const {
	
	long double ret_val = data_ + operand.data_;

	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow (binary addition) (LongDouble).");
	}
	else [[likely]] {
	
		return LongDouble(ret_val);
	}
}

genmath::LongDouble genmath::LongDouble::operator-(const LongDouble& operand) const {
	
	long double ret_val = data_ - operand.data_;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow (binary subtraction) (LongDouble).");
	}
	else [[likely]] {
		
		return LongDouble(ret_val);
	}
}

genmath::LongDouble genmath::LongDouble::operator*(const LongDouble& operand) const {
	
	long double ret_val = data_ * operand.data_;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow (binary multiplication) (LongDouble).");
	}
	else [[likely]] {
	
		return LongDouble(ret_val);
	}
}

genmath::LongDouble genmath::LongDouble::operator/(const LongDouble& operand) const {
	
	long double ret_val = data_ / operand.data_;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW) || std::fetestexcept(FE_DIVBYZERO)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow or division by zero (binary division) (LongDouble).");
	}
	else [[likely]] {
	
		return LongDouble(ret_val);
	}
}

genmath::LongDouble genmath::LongDouble::operator%(const LongDouble& operand) const {
	
	long double ret_val = std::fmodl(data_, operand.data_);
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW) || std::fetestexcept(FE_DIVBYZERO)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Number overflow or division by zero (binary congruence) (LongDouble).");
	}
	else [[likely]] {
	
		return LongDouble(ret_val);
	}
}

genmath::LongDouble genmath::LongDouble::operator++() {
	
	data_ += 1.0;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Prefix++ overflow (LongDouble).");
	}
	else [[likely]] {
	
		return *this;
	}
}

genmath::LongDouble genmath::LongDouble::operator++(int) {
	
	LongDouble ret(*this); data_ += 1.0;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Postfix++ overflow (LongDouble).");
	}
	else [[likely]] {
	
		return ret;
	}
}

genmath::LongDouble genmath::LongDouble::operator--() {
	
	data_ -= 1.0;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Prefix-- overflow (LongDouble).");
	}
	else [[likely]] {
	
		return *this;
	}
}

genmath::LongDouble genmath::LongDouble::operator--(int) {
	
	LongDouble ret(*this); data_ -= 1.0;
	
	if (std::fetestexcept(FE_UNDERFLOW) || std::fetestexcept(FE_OVERFLOW)) {

		std::feclearexcept(FE_ALL_EXCEPT);
		throw std::exception("Postfix-- overflow (LongDouble).");
	}
	else [[likely]] {
	
		return ret;
	}
}

int64_t genmath::LongDouble::ToInt64() const {
	
	return (int64_t)std::round(data_); 
}

genmath::LongDouble::operator long double() const {
	
	return data_;
}

genmath::LongDouble::operator std::string() const {
	
	return std::string(std::to_string(data_));
}

genmath::LongDouble genmath::LongDouble::Sqrt(const LongDouble& operand) {
	
	return LongDouble(std::sqrt(operand.data_)); 
}

genmath::LongDouble genmath::LongDouble::RawRound(const LongDouble value, const LongDouble step) {

	return ((value % step) < (step / LongDouble::Two)) ?
		(value - (value % step)) : (value - (value % step) + step);
}

genmath::LongDouble genmath::LongDouble::Abs(const LongDouble operand) {
	
	return std::fabsl(operand.data_);
}

genmath::LongDouble genmath::LongDouble::Sgn(const LongDouble operand) {

	if (operand < LongDouble::Zero) return LongDouble(-1.0);
	else return LongDouble(1.0);
}