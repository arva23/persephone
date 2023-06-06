#include "FixedPoint.h"
genmath::FixedPoint::FixedPoint() : ObjectBase() {data_ = fix16_from_str("0.0");}

genmath::FixedPoint::FixedPoint(fix16_t data, bool from_int) : ObjectBase() {
	if (from_int) {
		data_ = fix16_from_int(data);
		if (data_ == fix16_overflow)
			data_ = fix16_from_str("0.0");
	}
	else {

		data_ = data;
	}
}

genmath::FixedPoint::FixedPoint(std::string data) : ObjectBase() {
	
	if (data.empty()) data_ = fix16_from_str("0.0");

	bool valid = true;
	size_t size_of_data = data.size();
	for (size_t i = 0; i < size_of_data && valid; ++i)
		valid = valid &&
			(data[i] == '0' || data[i] == '1' || data[i] == '2' || data[i] == '3' || data[i] == '4'
			|| data[i] == '5' || data[i] == '6' || data[i] == '7' || data[i] == '8' || data[i] == '9'
			|| data[i] == '-' || data[i] == '.');
	
	if (!valid) data_ = fix16_from_str("0.0");

	data_ = fix16_from_str(data.c_str());
	
	if (data_ == fix16_overflow) data_ = fix16_from_str("0.0");
}

genmath::FixedPoint::FixedPoint(const char* data) : ObjectBase() {

	data_ = fix16_from_str(data); if (data_ == fix16_overflow) data_ = fix16_from_str("0.0");
}

genmath::FixedPoint::FixedPoint(const FixedPoint& orig) : ObjectBase(orig) {
	
	data_ = orig.data_;
}

genmath::FixedPoint::~FixedPoint() {}

//fix16_t FixedPoint::precision_threshold_ = fix16_from_str("0.0001525878");// arbitrary resolution value
fix16_t genmath::FixedPoint::precision_threshold_ = fix16_from_str("0.0009765625");// 2^-10 the Q16.16 resolution value

void genmath::FixedPoint::SetPrecisionThreshold(FixedPoint& threshold) {

	precision_threshold_ = threshold;
}

bool genmath::FixedPoint::operator=(std::string operand) {
	
	if(operand.empty()) return true;

	bool valid = true;
	size_t size_of_data = operand.size();
	for (size_t i = 0; i < size_of_data && valid; ++i)
		valid = valid &&
			(operand[i] == '0' || operand[i] == '1' || operand[i] == '2' || operand[i] == '3'
			|| operand[i] == '4' || operand[i] == '5' || operand[i] == '6' || operand[i] == '7'
			|| operand[i] == '8' || operand[i] == '9' || operand[i] == '-' || operand[i] == '.');
			
	if (!valid) return true;

	data_ = fix16_from_str(operand.c_str());
	
	if (data_ == fix16_overflow) data_ = fix16_from_str("0.0");
	
	return data_ == fix16_overflow ? true : false;
}

genmath::FixedPoint& genmath::FixedPoint::operator=(const FixedPoint& operand) {
	
	ObjectBase::operator=(operand);
	data_ = operand.data_;
	return *this;
}

bool genmath::FixedPoint::operator=(const char* operand) {
	
	if (strlen(operand) == 0) return true;

	bool valid = true;
	size_t size_of_data = sizeof(operand) - 1;
	for (size_t i = 0; i < size_of_data && valid; ++i)
		valid = valid &&
		(operand[i] == '0' || operand[i] == '1' || operand[i] == '2' || operand[i] == '3'
			|| operand[i] == '4' || operand[i] == '5' || operand[i] == '6' || operand[i] == '7'
			|| operand[i] == '8' || operand[i] == '9' || operand[i] == '-' || operand[i] == '.');

	if (!valid) return true;
	
	data_ = fix16_from_str(operand);
	if (data_ == fix16_overflow) data_ = fix16_from_str("0.0");

	return data_ == fix16_overflow ? true : false;
}

bool genmath::FixedPoint::operator==(const FixedPoint& operand) const {

	return fix16_abs(fix16_sub(data_, operand.data_)) <= precision_threshold_;
}

bool genmath::FixedPoint::operator<=(const FixedPoint& operand) const {

	return fix16_sub(operand.data_, data_) >= fix16_from_str("0.0"); 
}

bool genmath::FixedPoint::operator>=(const FixedPoint& operand) const {

	return fix16_sub(data_, operand.data_) >= fix16_from_str("0.0");
}

bool genmath::FixedPoint::operator<(const FixedPoint& operand) const {

	return fix16_sub(operand.data_, data_) > precision_threshold_;
}

bool genmath::FixedPoint::operator>(const FixedPoint& operand) const {

	return fix16_sub(data_, operand.data_) > precision_threshold_; 
}

bool genmath::FixedPoint::operator!=(const FixedPoint& operand) const {

	return fix16_abs(fix16_sub(data_, operand.data_)) > precision_threshold_; 
}

void genmath::FixedPoint::operator+=(const FixedPoint& operand) {

	if((data_ = fix16_add(data_, operand.data_)) == fix16_overflow)
		throw std::exception("Number overflow (unary addition) (FixedPoint).");
}

void genmath::FixedPoint::operator-=(const FixedPoint& operand) {

	if((data_ = fix16_sub(data_, operand.data_)) == fix16_overflow)
		throw std::exception("Number overflow (unary subtraction) (FixedPoint)."); 
}

void genmath::FixedPoint::operator*=(const FixedPoint& operand) {

	if((data_ = fix16_mul(data_, operand.data_)) == fix16_overflow)
		throw std::exception("Number overflow (unary multiplication) (FixedPoint)."); 
}

void genmath::FixedPoint::operator/=(const FixedPoint& operand) {

	if((data_ = fix16_div(data_, operand.data_)) == fix16_overflow)
		throw std::exception("Number overflow (unary division) (FixedPoint).");
}

void genmath::FixedPoint::operator%=(const FixedPoint& operand) {

	if((data_ = fix16_mod(data_, operand.data_)) == fix16_overflow)
		throw std::exception("Number overflow (unary congruence) (FixedPoint).");
}

genmath::FixedPoint genmath::FixedPoint::operator+(const FixedPoint& operand) const {

	fix16_t ret_val = fix16_add(data_, operand.data_);
	if (ret_val == fix16_overflow) throw std::exception("Number overflow (binary addition) (FixedPoint).");
	else [[likely]] return FixedPoint(ret_val);
}

genmath::FixedPoint genmath::FixedPoint::operator-(const FixedPoint& operand) const {

	fix16_t ret_val = fix16_sub(data_, operand.data_);
	if (ret_val == fix16_overflow) throw std::exception("Number overflow (binary subtraction) (FixedPoint).");
	else [[likely]] return FixedPoint(ret_val);
}

genmath::FixedPoint genmath::FixedPoint::operator*(const FixedPoint& operand) const {

	fix16_t ret_val = fix16_mul(data_, operand.data_);
	if (ret_val == fix16_overflow) throw std::exception("Number overflow (binary multiplication) (FixedPoint).");
	else [[likely]] return FixedPoint(ret_val);
}

genmath::FixedPoint genmath::FixedPoint::operator/(const FixedPoint& operand) const {

	fix16_t ret_val = fix16_div(data_, operand.data_);
	if (ret_val == fix16_overflow) throw std::exception("Number overflow (binary division) (FixedPoint).");
	else [[likely]] return FixedPoint(ret_val);
}

genmath::FixedPoint genmath::FixedPoint::operator%(const FixedPoint& operand) const {

	fix16_t ret_val = fix16_mod(data_, operand.data_);
	if (ret_val == fix16_overflow) throw std::exception("Number overflow (binary congruence) (FixedPoint)");
	else [[likely]] return FixedPoint(ret_val);
}

genmath::FixedPoint genmath::FixedPoint::operator++() {

	data_ = fix16_add(data_, fix16_from_int(1));
	if (data_ == fix16_overflow) throw std::exception("Prefix++ overflow (FixedPoint)."); else [[likely]] return *this; 
}

genmath::FixedPoint genmath::FixedPoint::operator++(int) {

	FixedPoint ret(*this); data_ = fix16_add(data_, fix16_from_int(1));
	if (data_ == fix16_overflow) throw std::exception("Postfix++ overflow (FixedPoint)."); else [[likely]] return ret;
}

genmath::FixedPoint genmath::FixedPoint::operator--() {

	data_ = fix16_sub(data_, fix16_from_int(1));
	if (data_ == fix16_overflow) throw std::exception("Prefix-- overflow (FixedPoint)."); else [[likely]] return *this;
}

genmath::FixedPoint genmath::FixedPoint::operator--(int) {

	FixedPoint ret(*this); data_ = fix16_sub(data_, fix16_from_int(1));
	if (data_ == fix16_overflow) throw std::exception("Postfix-- overflow (FixedPoint)."); else [[likely]] return ret;
}

int64_t genmath::FixedPoint::ToInt64() const {
	
	return fix16_to_int(data_); }

genmath::FixedPoint::operator fix16_t() const {
	
	return data_; }

genmath::FixedPoint::operator std::string() const {
	
	char ret[34]; fix16_to_str(data_, ret, 13); return std::string(ret); }

genmath::FixedPoint genmath::FixedPoint::Sqrt(const FixedPoint& operand) {
	
	return FixedPoint(fix16_sqrt(operand.data_));}

genmath::FixedPoint genmath::FixedPoint::RawRound(FixedPoint value, FixedPoint step) {

	return (value % step < step / FixedPoint("2.0")) ?
		FixedPoint(value - (value % step)) : FixedPoint(value - (value % step) + step);
}

genmath::FixedPoint genmath::FixedPoint::Abs(FixedPoint operand) {
	return fix16_abs(operand.data_); }
