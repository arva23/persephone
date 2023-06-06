#include "Vector.h"
template <class T>
char genmath::Vector<T>::DelimChar = ' ';// ObjectBase::GenerateDelimiterCharacter();

template <class T>
genmath::Vector<T>::Vector() : ObjectBase() {
	
	size_ = 0;  data_ = std::vector<T>(); 
}

template <class T>
genmath::Vector<T>::Vector(size_t dim) : ObjectBase() {

	size_ = dim;
	data_ = std::vector<T>(dim, T());
}

template <class T>
genmath::Vector<T>::Vector(std::string data) : ObjectBase() {

	this->operator=(data);
}

template <class T>
genmath::Vector<T>::Vector(size_t size, T gen_data) : ObjectBase() {

	data_ = std::vector<T>(size, gen_data);
}

template <class T>
genmath::Vector<T>::Vector(std::vector<T> data) : ObjectBase() {
	
	size_ = data.size(); 
	data_ = data; 
}

template <class T>
genmath::Vector<T>::Vector(size_t size, T* data) : ObjectBase() {

	size_ = size;

	for (size_t i = 0; i < size_; ++i)
		data_.push_back(data[i]);
}

template <class T>
genmath::Vector<T>::Vector(const Vector& orig) : ObjectBase(orig) {
	
	size_ = orig.size_; 
	data_ = orig.data_; 
}

template <class T>
genmath::Vector<T>::~Vector() {}

template <class T>
genmath::Vector<T>& genmath::Vector<T>::operator=(const Vector<T>& orig) {

	ObjectBase::operator=(orig);
	size_ = orig.size_;
	data_ = orig.data_;
	return *this;
}

template <class T>
bool genmath::Vector<T>::operator=(std::string data) {

	if (data.size() == 0)
		return true;

	data_.clear();

	size_t len = 0;
	std::stringstream ss(data);
	std::string raw_str;
	while (!ss.eof() && !ss.fail()) {
	
		std::getline(ss, raw_str, DelimChar);
		//ss.getline(raw_str, 34, DelimChar);
		data_.push_back(T());
		if ((data_.back()) = raw_str) return true;
		++len;
	}

	if (ss.fail()) return true;

	size_ = len;

	return len > 0 ? false : true;
}

template <class T>
bool genmath::Vector<T>::operator==(const Vector<T>& operand)const{

	if (operand.Size() != size_)
		return false;

	bool p = true;

	for (size_t i = 0; i < size_; ++i)
		p = p && (data_[i] == operand.data_[i]);

	return p;
}

template <class T>
bool genmath::Vector<T>::operator!=(const Vector<T>& operand)const{

	if (operand.Size() != size_)
		return false;
	
	bool p = true;

	for (size_t i = 0; i < size_; ++i)
		p = p && (data_[i] != operand.data_[i]);

	return p;
}

template <class T>
void genmath::Vector<T>::operator +=(const Vector<T>& operand) {
	
	if(operand.Size() != size_)
		throw std::exception("Vector dimension error (Vector).");

	for (size_t i = 0; i < size_; ++i)
		data_[i] += operand.data_[i];
}

template <class T>
void genmath::Vector<T>::operator -=(const Vector<T>& operand) {

	if (operand.Size() != size_)
		throw std::exception("Vector dimension error (Vector).");

	for (size_t i = 0; i < size_; ++i)
		data_[i] -= operand.data_[i];
}

template <class T>
genmath::Vector<T> genmath::Vector<T>::operator+(const Vector<T>& operand) const {

	if (size_ != operand.Size())
		throw std::exception("Vector dimension error (Vector).");

	Vector<T> ret(size_);

	for (size_t i = 0; i < size_; ++i)
		ret[i] = data_[i] + operand.data_[i];

	return ret;
}

template <class T>
genmath::Vector<T> genmath::Vector<T>::operator-(const Vector<T>& operand) const {
	
	if (size_ != operand.Size())
		throw std::exception("Vector dimension error (Vector).");

	Vector<T> ret(size_);

	for (size_t i = 0; i < size_; ++i)
		ret[i] = data_[i] - operand.data_[i];

	return ret;
}

template <class T>
T genmath::Vector<T>::operator*(const Vector<T>& operand) const {

	if (operand.Size() != size_)
		throw std::exception("Vector dimension error (Vector).");

	T ret;
	
	for (size_t i = 0; i < size_; ++i)
		ret += data_[i] * operand.data_[i];

	return ret;
}

template <class T>
genmath::Vector<T> genmath::Vector<T>::operator*(const T& scalar) const {

	Vector<T> ret(*this);

	for (size_t i = 0; i < size_; ++i)
		ret[i] *= scalar;

	return ret;
}

template <class T>
genmath::Vector<T>::operator std::string() const{

	if (size_ == 0) return std::string("");

	std::string ret;

	for (T c : data_)
		ret += std::string(c) + " ";

	ret.pop_back();
	return ret;
}

template <class T>
std::vector<T> genmath::Vector<T>::GetData() const { return data_; }

template <class T>
void genmath::Vector<T>::PushBack(const T& value) {

	data_.push_back(value);
}

template <class T>
T genmath::Vector<T>::PopBack() {

	if (data_.empty()) throw std::exception("Empty Vector object container (Vector).");

	T value = data_.back();
	data_.pop_back();
	return value;
}

template <class T>
genmath::Vector<T> genmath::Vector<T>::HadamardProd(const Vector<T>& operand) {

	size_t size_of_operand = operand.data_.size();

	if (size_of_operand != data_.size())
		throw std::exception("Vector dimension mismatch (Vector).");

	Vector<T> ret(size_of_operand);

	for (size_t i = 0; i < size_of_operand; ++i)
		ret[i] = data_[i] * operand[i];

	return ret;
}

template <class T>
T& genmath::Vector<T>::operator[](size_t ind) {

	if (ind >= data_.size())
		throw std::exception("Vector dimension mismatch (Vector).");

	return data_[ind];
}

template <class T>
const T& genmath::Vector<T>::operator[](size_t ind) const {

	if (ind >= data_.size())
		throw std::exception("Vector dimension mismatch (Vector).");

	return data_[ind];
}

template <class T>
size_t genmath::Vector<T>::Size() const {
	
	return data_.size(); 
}
