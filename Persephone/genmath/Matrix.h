#pragma once
#include <vector>
#include <utility>
#include <exception>
#include "ObjectBase.h"
#include "LongDouble.h"
#include "Vector.h"
#include "arithmetically_operable.h"

// maybe should use mixin class compositions as aggregates instead of inheritance
// inheritance creates the problems of super call, ellipse-circle problems

namespace genmath {

	template <class T> requires std::totally_ordered<T> && arithmetically_operable<T>
	class Matrix : public ObjectBase {

	public:
		Matrix();
		Matrix(size_t no_rows, size_t no_cols);
		Matrix(std::string data);
		Matrix(std::vector<std::vector<T> >& data);
		Matrix(std::vector<Vector<T> >& data);
		Matrix(const Matrix<T>& orig);
		virtual ~Matrix();

		Matrix<T>& operator=(const Matrix<T>& orig);
		virtual std::vector<Vector<T> > GetData() const;
		virtual bool operator=(std::string param) override;

		virtual inline std::vector<Vector<T> > GetColumnVecs() const;
		virtual inline std::vector<Vector<T> > GetRowVecs() const;
		virtual inline std::pair<size_t, size_t> GetDims() const;
		virtual inline Vector<T>& operator[](size_t ind);
		virtual inline const Vector<T>& operator[](size_t ind) const;
	
		// base class substitutions for the following listed methods
		inline bool operator==(const Matrix<T>& operand) const;
		inline bool operator!=(const Matrix<T>& operand) const;

		inline void operator+=(const Matrix<T>& opearnd);
		inline void operator-=(const Matrix<T>& operand);
		inline void operator*=(const Matrix<T>& operand);

		inline Matrix<T> operator+(const Matrix<T>& operand) const;
		inline Matrix<T> operator-(const Matrix<T>& operand) const;
		inline Matrix<T> operator*(const Matrix<T>& operand) const;
	
		virtual inline Vector<T> operator*(const Vector<T>& operand) const;

		inline Vector<T> BackwSubst();
		inline void GenLinCombImplicit(const Vector<T>& operand);
		inline Matrix<T> GenLinComb(const Vector<T>& operand) const;
	
		virtual inline operator std::string() const override;
		static char DelimChar;
		uint8_t data_type_;// 0 - arbitrary  matrix, 1 - quadratic matrix, 2 - tridiagonal matrix

	protected:
		size_t no_rows_;
		size_t no_cols_;
		// vectors are stored as row vectors of matrix
		std::vector<Vector<T> > data_;
	};
}

template class genmath::Matrix<genmath::LongDouble>;
