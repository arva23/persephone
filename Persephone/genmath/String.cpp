#include "String.h"
genmath::String::String() : ObjectBase() {
	
	data_ = ""; 
}

genmath::String::String(std::string data) {
	
	data_ = data;
}

genmath::String::String(const char* data) {

	data_ = std::string(data);
}

genmath::String::String(const String& orig) : ObjectBase(orig) {
	
	data_ = orig.data_;
}

genmath::String::~String(){

}

bool genmath::String::Empty() {

	return data_.empty();
}

bool genmath::String::operator=(std::string data) {
	
	data_ = data;
	return data_.empty();
}

void genmath::String::operator+=(String& data) {

	data_ += data.data_;
}

void genmath::String::operator+=(String data) {

	data_ += data.data_;
}

bool genmath::String::operator!=(String& data) {

	return data.data_ != data_;
}

bool genmath::String::operator<(const String operand) const {

	return data_.compare(operand.data_) <= 0;
}

genmath::String& genmath::String::operator=(const String& orig) {
	
	ObjectBase::operator=(orig); 
	data_ = orig.data_; 
	return *this; 
}

genmath::String::operator std::string() const {
	
	return data_; 
}

std::string& genmath::String::GetData() {

	return data_;
}

size_t genmath::String::Size() {

	return data_.size();
}