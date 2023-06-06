#include "TriDiagMatrix.h"

template <class T>
char genmath::TriDiagMatrix<T>::DelimChar = '|';

template <class T>
genmath::TriDiagMatrix<T>::TriDiagMatrix() : QMatrix<T>(), ObjectBase() {
	
	QMatrix<T>::size_ = 0;

	Matrix<T>::data_type_ = 2;
}

template <class T>
genmath::TriDiagMatrix<T>::TriDiagMatrix(size_t dim) : QMatrix<T>(dim), ObjectBase() {
	
	if (dim <= 2) throw std::exception("Dimension parameter is out of range (TriDiagMatrix).");

	Matrix<T>::data_type_ = 2;
}

template <class T>
genmath::TriDiagMatrix<T>::TriDiagMatrix(std::string data) : ObjectBase() {

	operator=(data);
	Matrix<T>::data_type_ = 2;
}

template <class T>
genmath::TriDiagMatrix<T>::TriDiagMatrix(std::vector<T>& data, bool force_invertable)
	: QMatrix<T>((data.size() - 1) / 3 + 1), ObjectBase() {
	
	if (data.size() < 7) {
		
		throw std::exception("Dimension parameter is out of range (TriDiagMatrix).");
	}
	else {
		size_t cond_size = data.size();
		--cond_size;
		
		if (data.size() < 7 || cond_size % 3 != 0)
			throw std::exception("Inappropriate storage parameter length (TriDiagMatrix).");

		Matrix<T>::no_rows_ = Matrix<T>::no_cols_ = QMatrix<T>::size_;;
		
		Matrix<T>::data_[0][0] = data[0];
		Matrix<T>::data_[0][1] = data[1];

		for (size_t i = 1; i < QMatrix<T>::size_ - 1; ++i) {

			Matrix<T>::data_[i][i - 1] = data[i * 3 - 1];
			Matrix<T>::data_[i][i] = data[i * 3];
			Matrix<T>::data_[i][i + 1] = data[i * 3 + 1];
		}

		Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2] = data[data.size() - 2];
		Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1] = data.back();

		// T data type can not represent too small numbers
		if (force_invertable && QMatrix<T>::GetDet() == T("0.0"))
			throw std::exception("Determinant is zero (TriDiagMatrix).");
	}

	Matrix<T>::data_type_ = 2;
}

template <class T>
genmath::TriDiagMatrix<T>::TriDiagMatrix(const TriDiagMatrix<T>& orig) : QMatrix<T>(orig), ObjectBase(orig) {

}

template <class T>
genmath::TriDiagMatrix<T>::~TriDiagMatrix() {
	
}

template <class T>
genmath::TriDiagMatrix<T>& genmath::TriDiagMatrix<T>::operator=(const TriDiagMatrix<T>& orig) {
	
	ObjectBase::operator=(orig);
	QMatrix<T>::operator=(orig);
	return *this;
}

template <class T>
bool genmath::TriDiagMatrix<T>::operator=(std::string param) {

	return QMatrix<T>::operator=(param);
}

template <class T>
std::vector<genmath::Vector<T> > genmath::TriDiagMatrix<T>::GetData() const {

	return QMatrix<T>::GetData();
}
template <class T>
std::vector<genmath::Vector<T> > genmath::TriDiagMatrix<T>::GetColumnVecs() const {

	return QMatrix<T>::GetColumnVecs();
}

template <class T>
std::vector<genmath::Vector<T> > genmath::TriDiagMatrix<T>::GetRowVecs() const {
	
	return Matrix<T>::data_;
}

template <class T>
const genmath::Vector<T>& genmath::TriDiagMatrix<T>::operator[](size_t ind) const{

	if (ind >= QMatrix<T>::size_)
		throw std::exception("Undefined value at index (TriDiagMatrix).");
	
	return Matrix<T>::data_[ind];
}

template <class T>
T genmath::TriDiagMatrix<T>::GetDet() const {

	if (QMatrix<T>::size_ == 0) return T("0.0");
	std::vector<Vector<T> > data = this->GetData();
	QMatrix<T> det_aux_mtx(data);

	return det_aux_mtx.GetDet();
}

template <class T>
bool genmath::TriDiagMatrix<T>::operator==(const TriDiagMatrix<T>& operand) const {

	if (QMatrix<T>::size_ != operand.size_)
		throw std::exception("Dimension mismatch (TriDiagMatrix).");

	bool equals = true;
	
	equals = equals && Matrix<T>::data_[0][0] == operand[0][0];
	equals = equals && Matrix<T>::data_[0][1] == operand[0][1];

	for (size_t i = 1; i < QMatrix<T>::size_ - 1; ++i) {

		equals = equals && Matrix<T>::data_[i][i - 1] == operand[i][i - 1];
		equals = equals && Matrix<T>::data_[i][i] == operand[i][i];
		equals = equals && Matrix<T>::data_[i][i + 1] == operand[i][i + 1];
	}

	equals = equals && Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2]
		== operand[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2];
	equals = equals && Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1]
		== operand[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1];

	return equals;
}

template <class T>
bool genmath::TriDiagMatrix<T>::operator!=(const TriDiagMatrix<T>& operand) const {

	return !operator==(operand);
}

template <class T>
void genmath::TriDiagMatrix<T>::operator+=(const TriDiagMatrix<T>& operand) {

	if (QMatrix<T>::size_ != operand.size_)
		throw std::exception("Dimension mismatch (TriDiagMatrix).");
	
	std::vector<Vector<T> > operand_data(operand.GetData());

	Matrix<T>::data_[0][0] += operand_data[0][0];
	Matrix<T>::data_[0][1] += operand_data[0][1];

	for (size_t i = 1; i < QMatrix<T>::size_ - 1; ++i) {
		Matrix<T>::data_[i][i - 1] += operand_data[i][i - 1];
		Matrix<T>::data_[i][i] += operand_data[i][i];
		Matrix<T>::data_[i][i + 1] += operand_data[i][i + 1];
	}
	Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2] +=
		operand_data[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2];
	Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1] +=
		operand_data[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1];
}

template <class T>
void genmath::TriDiagMatrix<T>::operator-=(const TriDiagMatrix<T>& operand) {

	if (QMatrix<T>::size_ != operand.size_)
		throw std::exception("Dimension mismatch (TriDiagMatrix).");

	std::vector<Vector<T> > operand_data(operand.GetData());

	Matrix<T>::data_[0][0] -= operand_data[0][0];
	Matrix<T>::data_[0][1] -= operand_data[0][1];

	for (size_t i = 1; i < QMatrix<T>::size_ - 1; ++i) {
		Matrix<T>::data_[i][i - 1] -= operand_data[i][i - 1];
		Matrix<T>::data_[i][i] -= operand_data[i][i];
		Matrix<T>::data_[i][i + 1] -= operand_data[i][i + 1];
	}
	Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2] -=
		operand_data[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2];
	Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1] -=
		operand_data[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1];
}

template <class T>
genmath::TriDiagMatrix<T> genmath::TriDiagMatrix<T>::operator+(const TriDiagMatrix<T>& operand) const {
	
	if (QMatrix<T>::size_ != operand.size_)
		throw std::exception("Dimension mismatch (TriDiagMatrix).");

	TriDiagMatrix<T> ret_mtx(*this);
	ret_mtx += operand;
	return ret_mtx;
}

template <class T>
genmath::TriDiagMatrix<T> genmath::TriDiagMatrix<T>::operator-(const TriDiagMatrix<T>& operand) const {

	if (QMatrix<T>::size_ != operand.size_)
		throw std::exception("Dimension mismatch (TriDiagMatrix).");

	TriDiagMatrix<T> ret_mtx(*this);
	ret_mtx -= operand;
	return ret_mtx;
}

template <class T>
genmath::Vector<T> genmath::TriDiagMatrix<T>::operator*(const Vector<T>& operand) const {

	if (QMatrix<T>::size_ != operand.Size())
		throw std::exception("Dimension mismatch (TriDiagMatrix).");

	Vector<T> ret_vec(QMatrix<T>::size_);// init. with null elem.
	
	ret_vec[0] = Matrix<T>::data_[0][0] * operand[0] + Matrix<T>::data_[0][1] * operand[1];
	
	for (size_t i = 1; i < QMatrix<T>::size_ - 1; ++i) {
	
		ret_vec[i] += Matrix<T>::data_[i][i - 1] * operand[i - 1]
			+ Matrix<T>::data_[i][i] * operand[i] + Matrix<T>::data_[i][i + 1] * operand[i + 1];
	}

	ret_vec[QMatrix<T>::size_ - 1] =
		Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2] * operand[QMatrix<T>::size_ - 2]
		+ Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1] * operand[QMatrix<T>::size_ - 1];

	return ret_vec;
}

template <class T>
void genmath::TriDiagMatrix<T>::GenLinCombImplicit(const Vector<T>& operand) {

	if (QMatrix<T>::size_ != operand.Size())
		throw std::exception("Dimension mismatch (TriDiagMatrix).");

	Matrix<T>::data_[0][0] *= operand[0];
	Matrix<T>::data_[0][1] *= operand[1];

	for (size_t i = 1; i < QMatrix<T>::size_ - 1; ++i) {

		Matrix<T>::data_[i][i - 1] *= operand[i - 1];
		Matrix<T>::data_[i][i] *= operand[i];
		Matrix<T>::data_[i][i + 1] *= operand[i + 1];
	}

	Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 2] *= operand[QMatrix<T>::size_ - 2];
	Matrix<T>::data_[QMatrix<T>::size_ - 1][QMatrix<T>::size_ - 1] *= operand[QMatrix<T>::size_ - 1];
}

template <class T>
genmath::TriDiagMatrix<T> genmath::TriDiagMatrix<T>::GenLinComb(const Vector<T>& operand) const {

	TriDiagMatrix<T> ret_mtx(*this);
	ret_mtx.GenLinCombImplicit(operand);
	return ret_mtx;
}

template <class T>
genmath::Vector<T> genmath::TriDiagMatrix<T>::SolveGauss(const Vector<T>& operand) const {

	return QMatrix<T>::SolveGauss(operand);
}

template <class T>
genmath::TriDiagMatrix<T>::operator std::string() const {

	return QMatrix<T>::operator std::string();
}
