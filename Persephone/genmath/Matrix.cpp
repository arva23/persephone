#include "Matrix.h"

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
char genmath::Matrix<T>::DelimChar = '|';//ObjectBase::GenerateDelimiterCharacter();// sfinae

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::Matrix() : ObjectBase() {

	no_rows_ = 0;
	no_cols_ = 0;
	data_ = std::vector<Vector<T> >();
	data_type_ = 0;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::Matrix(size_t no_rows, size_t no_cols) : ObjectBase() {

	no_rows_ = no_rows;
	no_cols_ = no_cols;
	if (no_rows_ == 0 || no_cols_ == 0)
		throw std::exception("Insufficient dimension parameter(s) (Matrix).");

	data_ = std::vector<Vector<T> >(no_rows_, Vector<T>(no_cols_));
	
	data_type_ = 0;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::Matrix<T>(std::string data) : ObjectBase() {

	operator=(data);
	
	data_type_ = 0;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::Matrix(std::vector<std::vector<T> >& data) : ObjectBase() {

	no_rows_ = data.size();
	
	if (no_rows_ == 0)
		throw std::exception("Insufficient row dimension parameter (Matrix).");

	no_cols_ = data[0].size();

	if (no_cols_ == 0)
		throw std::exception("Insufficient column dimension parameter (Matrix).");
	
	for (size_t i = 0; i < no_rows_; ++i) {
	
		if (data[i].size() != no_cols_)
			throw std::exception("Row vector dimension mismatch (Matrix).");

		data_.push_back(Vector(data[i]));
	}
	
	data_type_ = 0;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::Matrix(std::vector<Vector<T> >& data) : ObjectBase() {

	no_rows_ = data.size();

	if (no_rows_ == 0)
		throw std::exception("Insufficient row dimension parameter (Matrix).");

	no_cols_ = data[0].Size();

	if (no_cols_ == 0)
		throw std::exception("Insufficient column dimension parameter (Matrix).");

	for (size_t i = 0; i < no_rows_; ++i) {

		if (data[i].Size() != no_cols_)
			throw std::exception("Row vector dimension mismatch (Matrix).");

		data_.push_back(data[i]);
	}
	
	data_type_ = 0;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::Matrix<T>(const Matrix<T>& orig) : ObjectBase(orig) {

	no_rows_ = orig.no_rows_;
	no_cols_ = orig.no_cols_;
	data_ = orig.data_;
	data_type_ = orig.data_type_;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::~Matrix() {

}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>& genmath::Matrix<T>::operator=(const Matrix<T>& orig) {

	ObjectBase::operator=(orig);

	no_rows_ = orig.no_rows_;
	no_cols_ = orig.no_cols_;
	data_ = orig.data_;
	data_type_ = orig.data_type_;

	return *this;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
std::vector<genmath::Vector<T> > genmath::Matrix<T>::GetData() const {

	return data_;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
bool genmath::Matrix<T>::operator=(std::string param) {

	if (param.size() == 0)
		return false;

	size_t len = 0;
	std::istringstream ss(param);
	std::string raw_str;// 1 + 16 + 1 + 16 = 34
	std::vector<std::string> raw_data;

	while (!ss.eof() && !ss.fail()) {

		std::getline(ss, raw_str, DelimChar);
		//ss.getline(raw_str, 34, DelimChar);
		raw_data.push_back(raw_str);
	}

	if (ss.fail()) return true;

	size_t raw_data_size = raw_data.size();
	if (raw_data_size == 0) {

		return true;
	}
	else {

		no_rows_ = raw_data_size;
		data_.resize(no_rows_);

		for (size_t i = 0; i < no_rows_; ++i)
			for (size_t j = 0; j < no_cols_; ++j)
				if(data_[i] = raw_data[i]) return true;

		// looking of dimension mismatch, missing elements in row vectors
		for (size_t i = 0; i < no_rows_ - 1; ++i)
			if (data_[i].Size() != data_[i + 1].Size())
				return true;

		return false;
	}
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
std::vector<genmath::Vector<T> > genmath::Matrix<T>::GetColumnVecs() const {

	std::vector<Vector<T> > ret(no_cols_, Vector<T>(no_rows_));

	for (size_t i = 0; i < no_rows_; ++i)
		for (size_t j = 0; j < no_cols_; ++j)
			ret[j][i] = data_[i][j];

	return ret;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
std::vector<genmath::Vector<T> > genmath::Matrix<T>::GetRowVecs() const { return data_; }

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
std::pair<size_t, size_t> genmath::Matrix<T>::GetDims() const { return std::pair<size_t, size_t>(no_rows_, no_cols_); }

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Vector<T>& genmath::Matrix<T>::operator[](size_t ind){

	if (ind >= no_rows_)
		throw std::exception("Dimension mismatch (requested index is not in range) (Matrix).");

	return data_[ind];
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
const genmath::Vector<T>& genmath::Matrix<T>::operator[](size_t ind) const {

	if (ind >= no_rows_)
		throw std::exception("Dimension mismatch (requested index is not in range) (Matrix).");

	return data_[ind];
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
bool genmath::Matrix<T>::operator==(const Matrix<T>& operand) const {

	if (no_rows_ != operand.no_rows_ || no_cols_ != operand.no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	bool equals = true;

	for (size_t i = 0; i < no_rows_; ++i)
		equals = equals && data_[i] == operand.data_[i];
	
	return equals;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
bool genmath::Matrix<T>::operator!=(const Matrix<T>& operand) const { return data_ != operand.data_; }


template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
void genmath::Matrix<T>::operator+=(const Matrix<T>& operand) {

	if (operand.no_rows_ != no_rows_ || operand.no_cols_ != no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	for (size_t i = 0; i < no_rows_; ++i)
		data_[i] += operand.data_[i];
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
void genmath::Matrix<T>::operator-=(const Matrix<T>& operand) {

	if (operand.no_rows_ != no_rows_ || operand.no_cols_ != no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	for (size_t i = 0; i < no_rows_; ++i)
		data_[i] -= operand.data_[i];
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
void genmath::Matrix<T>::operator*=(const Matrix<T>& operand) {
	
	if (no_cols_ != operand.no_rows_)
		throw std::exception("Dimension mismatch (Matrix).");

	Matrix<T> res(no_rows_, operand.no_cols_);

	for (size_t i = 0; i < no_rows_; ++i)
		for (size_t j = 0; j < operand.no_cols_; ++j)
			for (size_t k = 0; k < no_cols_; ++k)
				res[i][j] += data_[i][k] * operand.data_[k][j];

	no_rows_ = res.no_rows_;
	no_cols_ = res.no_cols_;
	data_ = res.data_;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T> genmath::Matrix<T>::operator+(const Matrix<T>& operand) const {

	if (operand.no_rows_ != no_rows_ || operand.no_cols_ != no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	Matrix<T> res(*this);

	for (size_t i = 0; i < no_rows_; ++i)
		res[i] += operand.data_[i];

	return res;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T> genmath::Matrix<T>::operator-(const Matrix<T>& operand) const {

	if (operand.no_rows_ != no_rows_ || operand.no_cols_ != no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	Matrix<T> res(*this);

	for (size_t i = 0; i < no_rows_; ++i)
		res[i] -= operand.data_[i];

	return res;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T> genmath::Matrix<T>::operator*(const Matrix<T>& operand) const {

	if (no_cols_ != operand.no_rows_)
		throw std::exception("Dimension mismatch (Matrix).");

	Matrix<T> ret_mtx(no_rows_, operand.no_cols_);

	for (size_t i = 0; i < no_rows_; ++i)
		for (size_t j = 0; j < operand.no_cols_; ++j)
			for (size_t k = 0; k < no_cols_; ++k)
				ret_mtx[i][j] += data_[i][k] * operand.data_[k][j];

	return ret_mtx;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Vector<T> genmath::Matrix<T>::operator*(const Vector<T>& operand) const {

	if (operand.Size() != no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	Vector<T> res(no_rows_);

	for (size_t i = 0; i < no_rows_; ++i)
		for (size_t j = 0; j < no_cols_; ++j)
			res[i] += data_[i][j] * operand[j];

	return res;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Vector<T> genmath::Matrix<T>::BackwSubst() {

	// https://algowiki-project.org/en/Backward_substitution
	
	if (no_rows_ != no_cols_ - 1)
		throw std::exception("No appropriate form has provided for backward substitution (Matrix).");

	Vector<T> vec_res(no_rows_);

	vec_res[no_rows_ - 1] = data_[no_rows_ - 1][no_cols_ - 1] / data_[no_rows_ - 1][no_cols_ - 2];
	
	for (int64_t i = no_rows_ - 2; i >= 0; --i) {

		vec_res[i] = data_[i][no_cols_ - 1];

		for (int64_t j = no_rows_ - 1; j > i; --j)
			vec_res[i] -= data_[i][j] * vec_res[j];
		
		vec_res[i] /= data_[i][i];
	}

	return vec_res;
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
void genmath::Matrix<T>::GenLinCombImplicit(const Vector<T>& operand) {

	if (operand.Size() != no_cols_)
		throw std::exception("Dimension mismatch (Matrix).");

	for (size_t i = 0; i < no_rows_; ++i)
		for (size_t j = 0; j < no_cols_; ++j)
			data_[i][j] = data_[i][j] * operand[j];
}

template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T> genmath::Matrix<T>::GenLinComb(const Vector<T>& operand) const {

	Matrix<T> mtx_ret(*this);
	mtx_ret.GenLinCombImplicit(operand);
	return mtx_ret;
}


template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
genmath::Matrix<T>::operator std::string() const {

	if (no_rows_ == 0) return std::string("");

	std::string str_res = "";
	for (size_t i = 0; i < no_rows_ - 1; ++i)
		str_res += std::string(data_[i]) + DelimChar;

	str_res += data_.back();
	return str_res;
}