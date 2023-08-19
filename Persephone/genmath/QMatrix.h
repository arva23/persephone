#ifndef QMATRIX_H_INCLUDED
#define QMATRIX_H_INCLUDED
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include "ObjectBase.h"
#include "LongDouble.h"
#include "Vector.h"
#include "Matrix.h"
#include <type_traits>
#include "arithmetically_operable.h"

// Invertable quadratic matrix class with template data type
namespace genmath {

	template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
	class QMatrix : public Matrix<T>, public ObjectBase {
	
	public:
		QMatrix();
		QMatrix(size_t dim);
		QMatrix(std::string data);
		QMatrix(std::vector<std::vector<T> >& data, bool force_invertable = false);
		QMatrix(std::vector<Vector<T> >& data, bool force_invertable = false);
		QMatrix(const QMatrix<T>& orig);
		virtual ~QMatrix();

		std::vector<Vector<T> > GetData() const override;

		QMatrix<T>& operator=(const QMatrix<T>& orig);
		virtual bool operator=(std::string param) override;

		virtual std::vector<Vector<T> > GetColumnVecs() const override;
		virtual std::vector<Vector<T> > GetRowVecs() const override;
		virtual size_t GetDim() const;
		virtual Vector<T>& operator[](size_t ind) override;
		virtual const Vector<T>& operator[](size_t ind) const override;
	
	
		// determinant is not precalculated due to utilization of class
		//  (continuously changing matrices in algorithms)
		T GetDet();
		// concentrated to square matrices
		Matrix<T> GaussElim(size_t& no_row_swaps, const Vector<T>& operand) const;
		Matrix<T> GaussElim(const Vector<T>& operand) const;
	
		//QMatrix<T> BackwSubst();// in Matrix<T> to avoid violation of square matrix properties
		Vector<T> SolveGauss(const Vector<T>& operand) const;
	
		bool operator==(const QMatrix<T>& operand) const;
		bool operator!=(const QMatrix<T>& operand) const;

		inline void operator+=(const QMatrix<T>& operand);
		inline void operator-=(const QMatrix<T>& operand);
		inline void operator*=(const QMatrix<T>& operand);
	
		QMatrix<T> operator+(const QMatrix<T>& operand) const;
		QMatrix<T> operator-(const QMatrix<T>& operand) const;
		QMatrix<T> operator*(const QMatrix<T>& operand) const;

		virtual Vector<T> operator*(const Vector<T>& operand) const;
		// generate vector as matrix column vectors
		//  linear comination generation on the current matrix
		QMatrix<T> GenLinComb(const Vector<T>& operand) const;

		inline Vector<T> BackwSubst() = delete;

		virtual inline operator std::string() const override;

		static char DelimChar;

	protected:
		size_t size_;
	};
}

template class genmath::QMatrix<genmath::LongDouble>;#endif// QMATRIX_H_INCLUDED