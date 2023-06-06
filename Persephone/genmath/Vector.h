#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include "ObjectBase.h"
#include "LongDouble.h"

namespace genmath {

	template <class T>
	class Vector : public ObjectBase {

	public:
	
		Vector();
		Vector(size_t dim);
		Vector(std::string data);
		Vector(size_t size, T gen_data);
		Vector(std::vector<T> data);
		Vector(size_t size, T* data);
		Vector(const Vector& orig);
		virtual ~Vector();

		Vector<T>& operator=(const Vector<T>& orig);
		bool operator=(std::string data) override;

		bool operator==(const Vector<T>& operand) const;
		bool operator!=(const Vector<T>& operand) const;

		void operator +=(const Vector<T>& operand);
		void operator -=(const Vector<T>& operand);
	
		Vector<T> operator+(const Vector<T>& operand) const;
		Vector<T> operator-(const Vector<T>& operand) const;
		T operator*(const Vector<T>& operand) const;
		Vector<T> operator*(const T& scalar) const;
	
		operator std::string() const override;

		std::vector<T> GetData() const;
		void PushBack(const T& value);
		T PopBack();
		Vector<T> HadamardProd(const Vector<T>& operand);
		T& operator[](size_t ind);
		const T& operator[](size_t ind) const;
		size_t Size() const;

		static char DelimChar;

	private:
		size_t size_;
		std::vector<T> data_;
	};
}

template class genmath::Vector<genmath::LongDouble>;

