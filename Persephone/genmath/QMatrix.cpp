#include "QMatrix.h"
template <class T>
char genmath::QMatrix<T>::DelimChar = '|';

template <class T>
genmath::QMatrix<T>::QMatrix() : Matrix<T>(), ObjectBase(){

	size_ = 0;
	Matrix<T>::data_type_ = 1;
}

template <class T>
genmath::QMatrix<T>::QMatrix(size_t dim) : Matrix<T>(dim, dim), ObjectBase() {

	size_ = dim;

	Matrix<T>::data_type_ = 1;
}

template <class T>
genmath::QMatrix<T>::QMatrix(std::string data) : ObjectBase(){

	operator=(data);
	Matrix<T>::data_type_ = 1;
}

template <class T>
genmath::QMatrix<T>::QMatrix(std::vector<std::vector<T> >& data, bool force_invertable) : Matrix<T>(data), ObjectBase() {

	if (data.size() != data[0].size())
		throw std::exception("Dimension mismatch (probably not quadratic matrix) (QMatrix).");

	size_ = Matrix<T>::no_rows_;
	Matrix<T>::data_type_ = 1;

	// T data type can not represent too small numbers
	if (force_invertable && this->GetDet() == T("0.0"))
		throw std::exception("Determinant is zero (QMatrix).");
}

template <class T>
genmath::QMatrix<T>::QMatrix(std::vector<Vector<T> >& data, bool force_invertable) : Matrix<T>(data), ObjectBase(){

	if (data.size() != data[0].Size())
		throw std::exception("Dimension mismatch (probably not quadratic matrix) (QMatrix).");
	
	size_ = Matrix<T>::no_rows_;
	Matrix<T>::data_type_ = 1;

	// T data type can not represent too small numbers
	if (force_invertable && this->GetDet() == T("0.0"))
		throw std::exception("Determinant is zero (QMatrix).");
}

template <class T>
genmath::QMatrix<T>::QMatrix(const QMatrix<T>& orig) : Matrix<T>(orig), ObjectBase(orig) {

	size_ = orig.size_;
}

template <class T>
genmath::QMatrix<T>::~QMatrix() {}

template <class T>
genmath::QMatrix<T>& genmath::QMatrix<T>::operator=(const QMatrix<T>& orig) {

	ObjectBase::operator=(orig);
	Matrix<T>::operator=(orig);

	size_ = orig.size_;

	return *this;
}

template <class T>
std::vector<genmath::Vector<T> > genmath::QMatrix<T>::GetData() const {

	return Matrix<T>::GetData();
}

template <class T>
bool genmath::QMatrix<T>::operator=(std::string param) {

	if (param.size() == 0)
		return true;

	size_t len = 0;
	std::stringstream ss(param);
	char raw_str[34];
	std::vector<std::string> raw_data;

	while (!ss.eof() && !ss.fail()) {
	
		ss.getline(raw_str, 34, DelimChar);
		raw_data.push_back(raw_str);
	}

	if (ss.fail()) return true;

	size_t raw_data_size = raw_data.size();
	if (raw_data_size == 0) {
	
		return true;
	}
	else {
	
		Matrix<T>::no_rows_ = Matrix<T>::no_cols_ = size_ = raw_data_size;
		Matrix<T>::data_.resize(size_);
		
		for (size_t i = 0; i < size_; ++i) {
		
			for (size_t j = 0; j < size_; ++j) {
				
				Matrix<T>::data_[i] = Vector<T>(raw_data[i]);
				if (Matrix<T>::data_[i].Size() != size_) return true;
			}
		}
	
		return false;
	}
}

template <class T>
std::vector<genmath::Vector<T> > genmath::QMatrix<T>::GetColumnVecs() const {

	return Matrix<T>::GetColumnVecs();
}

template <class T>
std::vector<genmath::Vector<T> > genmath::QMatrix<T>::GetRowVecs() const {
	
	return Matrix<T>::data_; 
}

template <class T>
size_t genmath::QMatrix<T>::GetDim() const {
	
	return size_; 
}

template <class T>
genmath::Vector<T>& genmath::QMatrix<T>::operator[](size_t ind) {

	if (Matrix<T>::data_type_ == 2) throw std::exception("Object type is tridiagonal (operator[]) (QMatrix).");

	if (ind >= size_)
		throw std::exception("Dimension mismatch (requested index is not in range) (QMatrix).");

	return Matrix<T>::data_[ind];
}

template <class T>
const genmath::Vector<T>& genmath::QMatrix<T>::operator[](size_t ind) const {

	if (ind >= size_)
		throw std::exception("Dimension mismatch (requested index is not in range) (QMatrix).");

	return Matrix<T>::data_[ind];
}

template <class T>
T genmath::QMatrix<T>::GetDet() {

	if (size_ == 0) {
		
		return T("0.0");
	}
	else if (size_ == 1) {
	
		return Matrix<T>::data_[0][0];
	}
	else if (size_ == 2) {
	
		return Matrix<T>::data_[0][0] * Matrix<T>::data_[1][1]
			- Matrix<T>::data_[0][1] * Matrix<T>::data_[1][0];
	}
	else [[likely]] {
	
		size_t no_row_swaps = 0;
		QMatrix mtx(*this);
		Vector<T> vec(size_);
		Matrix<T> mtx_res = mtx.GaussElim(no_row_swaps, vec);
		T det_res("1.0");

		for (size_t i = 0; i < size_; ++i)
			det_res *= mtx_res[i][i];
		
		return (no_row_swaps % 2) ? det_res * T("-1.0") : det_res;
	}

}

template <class T>
genmath::Matrix<T> genmath::QMatrix<T>::GaussElim(size_t& no_row_swaps, const Vector<T>& operand) const {

	if (operand.Size() != size_)
		throw std::exception("Dimension mismatch (QMatrix).");

	std::vector<Vector<T> > cpy_data(this->GetData());
	for (size_t i = 0; i < size_; ++i)
		cpy_data[i].PushBack(operand[i]);
	
	Matrix<T> copy_mtx(cpy_data);

	// Gauss elmination algorithm
	size_t h = 0;
	size_t k = 0;

	while (h < size_ && k < size_) {

		size_t i_max = h;

		// finding k-th pivot
		for (size_t i = h; i < size_; ++i)
			if (T::Abs(copy_mtx[i][k]) > T::Abs(copy_mtx[i_max][k])) {
			
				i_max = i;
				++no_row_swaps;
			}

		if (copy_mtx[i_max][k] == T("0.0")) {

			++k;
		}
		else {
			
			Vector<T> row_tmp;
			row_tmp = copy_mtx[i_max];
			copy_mtx[i_max] = copy_mtx[h];
			copy_mtx[h] = row_tmp;

			for (size_t i = h + 1; i < size_/*Matrix<T>::no_cols_*/; ++i) {

				T f = copy_mtx[i][k] / copy_mtx[h][k];
				copy_mtx[i][k] = "0.0";

				for (size_t j = k + 1; j < size_ + 1; ++j)
					copy_mtx[i][j] = copy_mtx[i][j] - copy_mtx[h][j] * f;
			}

			++h;
			++k;
		}
	}

	return copy_mtx;
}

template <class T>
genmath::Matrix<T> genmath::QMatrix<T>::GaussElim(const Vector<T>& operand) const {

	size_t no_row_swaps = 0;
	return this->GaussElim(no_row_swaps, operand);
}

template <class T>
genmath::Vector<T> genmath::QMatrix<T>::SolveGauss(const Vector<T>& operand) const {

	return this->GaussElim(operand).BackwSubst();
}

template <class T>
bool genmath::QMatrix<T>::operator==(const QMatrix<T>& operand) const { return Matrix<T>::operator==(operand); }

template <class T>
bool genmath::QMatrix<T>::operator!=(const QMatrix<T>& operand) const { return Matrix<T>::operator!=(operand); }


template <class T>
void genmath::QMatrix<T>::operator+=(const QMatrix<T>& operand) {

	if (Matrix<T>::data_type_ == 2) throw std::exception("Object type is tridiagonal (unary addition) (QMatrix).");

	Matrix<T>::operator+=(operand);
}

template <class T>
void genmath::QMatrix<T>::operator-=(const QMatrix<T>& operand) {
	
	if (Matrix<T>::data_type_ == 2) throw std::exception("Object type is tridiagonal (unary subtraction) (QMatrix).");

	Matrix<T>::operator-=(operand);
}

template <class T>
void genmath::QMatrix<T>::operator*=(const QMatrix<T>& operand) {

	if (Matrix<T>::data_type_ == 2) throw std::exception("Object type is tridiagonal (unary multiplication) (QMatrix).");
	Matrix<T>::operator*=(operand);
}

template <class T>
genmath::QMatrix<T> genmath::QMatrix<T>::operator+(const QMatrix<T>& operand) const {

	QMatrix<T> mtx_ret(*this);
	mtx_ret += operand;
	return mtx_ret;
}

template <class T>
genmath::QMatrix<T> genmath::QMatrix<T>::operator-(const QMatrix<T>& operand) const {

	QMatrix<T> mtx_ret(*this);
	mtx_ret -= operand;
	return mtx_ret;
}

template <class T>
genmath::QMatrix<T> genmath::QMatrix<T>::operator*(const QMatrix<T>& operand) const {
	
	QMatrix<T> mtx_ret(*this);
	mtx_ret *= operand;
	return mtx_ret;
}

template <class T>
genmath::Vector<T> genmath::QMatrix<T>::operator*(const Vector<T>& operand) const {

	return Matrix<T>::operator*(operand);
}

template <class T>
genmath::QMatrix<T> genmath::QMatrix<T>::GenLinComb(const Vector<T>& operand) const {

	QMatrix<T>* mtx_op = new QMatrix<T>(*this);
	static_cast<Matrix<T>*>(mtx_op);
	mtx_op->GenLinCombImplicit(operand);
	static_cast<QMatrix<T>*>(mtx_op);
	QMatrix<T> mtx_ret(*mtx_op);
	delete mtx_op;
	return mtx_ret;
}

template <class T>
genmath::QMatrix<T>::operator std::string() const {

	return Matrix<T>::operator std::string();
}