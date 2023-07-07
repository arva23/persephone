#pragma once
#include <vector>
#include <algorithm>
#include <sstream>
#include <exception>
#include "LongDouble.h"
#include "Vector.h"
#include "QMatrix.h"
#include <type_traits>
#include "arithmetically_operable.h"

namespace genmath {

	template <class T> requires std::totally_ordered<T>&& arithmetically_operable<T>
	class TriDiagMatrix : public QMatrix<T>, public ObjectBase {

	public:
		TriDiagMatrix();
		TriDiagMatrix(size_t dim);
		TriDiagMatrix(std::string data);
		TriDiagMatrix(std::vector<T>& data_, bool force_invertable = false);
		TriDiagMatrix(const TriDiagMatrix<T>& orig);
		virtual ~TriDiagMatrix();

		bool operator=(std::string param) override;
		TriDiagMatrix<T>& operator=(const TriDiagMatrix<T>& orig);

		std::vector<Vector<T> > GetData() const;
		std::vector<Vector<T> > GetColumnVecs() const override;
		std::vector<Vector<T> > GetRowVecs() const override;
		const Vector<T>& operator[](size_t ind) const override;
	
		T GetDet() const;
	
		// deleted operator+= with QMatrix type
		// deleted operator-= with QMatrix type
		// deleted operator*= with QMatrix type
		// deleted operator+ with QMatrix type
		// deleted operator- with QMatrix type
		// deleted operator* with QMatrix type

		bool operator==(const TriDiagMatrix<T>& operand) const;
		bool operator!=(const TriDiagMatrix<T>& operand) const;

		void operator+=(const TriDiagMatrix<T>& operand);
		void operator-=(const TriDiagMatrix<T>& operand);
	
		TriDiagMatrix<T> operator+(const TriDiagMatrix<T>& operand) const;
		TriDiagMatrix<T> operator-(const TriDiagMatrix<T>& operand) const;
	
		Vector<T> operator*(const Vector<T>& operand) const override;

		void GenLinCombImplicit(const Vector<T>& operand);
	
		TriDiagMatrix<T> GenLinComb(const Vector<T>& operand) const;

		Vector<T> SolveGauss(const Vector<T>& operand) const;
	
		operator std::string() const override;

		static char DelimChar;
	};
}


template class genmath::TriDiagMatrix<genmath::LongDouble>;