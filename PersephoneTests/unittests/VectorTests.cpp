#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/include/libfixmath/fixmath.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/genmath/Vector.h"
#include <initializer_list>
#include <math.h>
#include <stdio.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;

	TEST_CLASS(VectorTests) {

		TEST_METHOD(ObjectResourceManagement) {

			// genmath::Vector();
			genmath::Vector<T> test_object_0 = genmath::Vector<T>();
			Assert::IsTrue(test_object_0.GetData() == std::vector<T>());
			

			// genmath::Vector(size_t dim);
			genmath::Vector<T> test_object_1(3);
			Assert::IsTrue(test_object_1.GetData() == std::vector<T>(3));
			

			// genmath::Vector(size_t size, T gen_data);
			genmath::Vector<T> test_object_2(3, T("2.0"));
			Assert::IsTrue(test_object_2[0] == T("2.0"));
			Assert::IsTrue(test_object_2[1] == T("2.0"));
			Assert::IsTrue(test_object_2[2] == T("2.0"));


			// genmath::Vector(std::vector<T> data);
			genmath::Vector<T> test_object_3(std::vector<T>{T("0.12")});
			std::vector<T> data_0{ T("1.111"), T("2.222"), T("3.333") };
			genmath::Vector<T> test_object_4(data_0);
			Assert::IsTrue(test_object_4.GetData() == data_0);
			

			// genmath::Vector(size_t size, T * data);
			T data_1[3]{ T("1.111"), T("2.222"), T("3.333") };
			genmath::Vector<T> test_object_5(3, data_1);
			Assert::IsTrue(test_object_5 == genmath::Vector<T>(3, data_1));
			

			// genmath::Vector(const genmath::Vector& orig);
			genmath::Vector<T> test_object_6(test_object_5);
			Assert::IsTrue(test_object_5 == test_object_6);
			

			// virtual ~genmath::Vector();
		}

		TEST_METHOD(AssignmentOperators) {

			// genmath::Vector<T> operator=(const genmath::Vector<T>&orig);
			genmath::Vector<T> test_object_0(
				3, new T[3]{ T("1.111"), T("2.222"), T("3.333") });
			genmath::Vector<T> test_object_1;
			Assert::IsTrue((test_object_1 = test_object_0) == test_object_0);


			// bool operator=(std::string data) override;
			Assert::IsTrue(test_object_1 = "");
			Assert::IsFalse(test_object_1 = "1.1111 2.2222 3.3333 4.4444");
			Assert::IsTrue(test_object_1 == genmath::Vector<T>(
				std::vector<T>{T("1.1111"), T("2.2222"), T("3.3333"), T("4.4444")}));
		}

		TEST_METHOD(LogicalOperators) {

			genmath::Vector<T> test_object_0(2, new T[2]{ T("1.123"), T("4.567") });
			genmath::Vector<T> test_object_1(2, new T[2]{ T("1.123"), T("4.567") });
			
			// bool operator==(const genmath::Vector<T>&operand) const;
			Assert::IsTrue(test_object_0 == test_object_1);
			
			
			// bool operator!=(const genmath::Vector<T>&operand) const;
			Assert::IsFalse(test_object_0 != test_object_1);
		}

		TEST_METHOD(CompoundArithmeticUnaryOperators) {

			// void operator +=(const genmath::Vector<T>&operand);
			genmath::Vector<T> test_object_0(4,
				new T[4]{ T("1.23"), T("4.56"), T("7.89"), T("10.12") });
			genmath::Vector<T> test_object_1(4,
				new T[4]{ T("1.746") , T("2.346") , T("3.0") , T("4.9984201") });
			test_object_0 += test_object_1;
			Assert::IsTrue(test_object_0 == genmath::Vector<T>(4,
				new T[4]{ T("1.23") + T("1.746"),
					T("4.56") + T("2.346"),
					T("7.89") + T("3.0"),
					T("10.12") + T("4.9984201")}));
			Assert::IsFalse(test_object_0 == genmath::Vector<T>(4,
				new T[4]{ T("1.23") + T("1.746"),
					T("4.56") + T("2.346"),
					T("7.89") + T("3.0"),
					T("10.12") + T("4.9201") }));
			

			// void operator -=(const genmath::Vector<T>&operand);
			test_object_0 -= test_object_1;
			Assert::IsTrue(test_object_0 == genmath::Vector<T>(4,
				new T[4]{ T("1.23"), T("4.56"), T("7.89"), T("10.12") }));
			Assert::IsFalse(test_object_0 == genmath::Vector<T>(4,
				new T[4]{ T("1.23"), T("4.56"), T("7.89"), T("10.144") }));
		}

		TEST_METHOD(ArithmeticBinaryOperators) {

			genmath::Vector<T> test_object_0(4,
				new T[4]{ T("1.23"), T("4.56"), T("7.89"), T("10.12") });
			genmath::Vector<T> test_object_1(4,
				new T[4]{ T("1.746") , T("2.346") , T("3.0") , T("4.9984201") });
			
			// genmath::Vector<T> operator+(const genmath::Vector<T>&operand) const;
			Assert::IsTrue(test_object_0 + test_object_1 == genmath::Vector<T>(4,
				new T[4]{ T("1.23") + T("1.746"),
					T("4.56") + T("2.346"),
					T("7.89") + T("3.0"),
					T("10.12") + T("4.9984201") }));
			Assert::IsFalse(test_object_0 + test_object_1 == genmath::Vector<T>(4,
				new T[4]{ T((T("1.23") + T("1.746"))),
					T("8.56") + T("2.346"),
					T("7.89") + T("3.0"),
					T("10.12") + T("4.9984201") }));
			

			// genmath::Vector<T> operator-(const genmath::Vector<T>&operand) const;
			Assert::IsTrue(test_object_0 - test_object_1 == genmath::Vector<T>(4,
				new T[4]{ T("1.23") - T("1.746"),
					T("4.56") - T("2.346"),
					T("7.89") - T("3.0"),
					T("10.12") - T("4.9984201") }));
			Assert::IsFalse(test_object_0 - test_object_1 == genmath::Vector<T>(4,
				new T[4]{ T("1.23") - T("1.746"),
					T("4.56") - T("2.346"),
					T("7.89") - T("3.0"),
					T("1.12") - T("4.9984201") }));
			
			
			// T operator*(const genmath::Vector<T>&operand) const;
			T result = test_object_0 * test_object_1;
			Assert::IsTrue(result ==
				T("1.23") * T("1.746") + 
				T("4.56") * T("2.346") + 
				T("7.89") * T("3.0") + 
				T("10.12") * T("4.9984201"));
			Assert::IsFalse(result ==
				T("1.23") * ("1.746") + 
				T("4.56") * T("2.346") + 
				T("7.8") * T("3.0") + 
				T("1.12") * T("4.9984201"));
			

			// genmath::Vector<T> operator*(const T & scalar) const;
			result = test_object_0 * T("2.21");
			Assert::IsTrue(result =
				T("1.23") * T("2.21") + 
				T("4.56") * T("2.21") + 
				T("7.89") * T("2.21") + 
				T("10.12") * T("2.21"));
		}

		TEST_METHOD(Conversions) {

			// operator std::string() override;
			Assert::IsTrue(std::string(genmath::Vector<T>(4, new T[4]{
				T("1.23"), T("4.56"), T("7.89"), T("10.12") }))
				== "" + std::string(T("1.23000")) + " " + std::string(T("4.56000"))
					+ " " + std::string(T("7.89000")) + " " + std::string(T("10.12000")));
			

			// operator std::string() const;
			Assert::IsFalse(std::string(genmath::Vector<T>(4, new T[4]{
				T("1.23"), T("4.56"), T("7.89"), T("10.12") }))
				== std::string(T("123000")) + " " + std::string(T("456000"))
				+ " " + std::string(T("789000")) + " " + std::string(T("1012000")));
		}

		TEST_METHOD(AuxiliaryMethods) {

			// std::vector<T> GetData() const;
			genmath::Vector<T> test_object_0(2, new T[2]{ T("21.1"), T("1.0") });

			Assert::IsTrue(test_object_0.GetData() == std::vector<T>{
				T("21.1"), T("1.0")});
			Assert::IsFalse(test_object_0.GetData() == std::vector<T>{
				T("212.1"), T("1.0")});
			

			// void PushBack(const T & value);
			test_object_0.PushBack(T("2.12"));
			Assert::IsTrue(test_object_0[test_object_0.Size() - 1] == T("2.12"));
			
			
			// T PopBack();
			test_object_0.PopBack();
			Assert::IsTrue(test_object_0[test_object_0.Size() - 1] == T("1.0"));
			
			genmath::Vector<T> test_object_2;
			try { test_object_2.PopBack(); Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(
				err.what() == std::string("Empty Vector object container (Vector).")); }
			
			
			// genmath::Vector<T> HadamardProd(const genmath::Vector<T>&operand);
			Assert::IsTrue(test_object_0.HadamardProd(genmath::Vector<T>(2, new T[2]{
				T("2.0"), T("32.01") }))
				== genmath::Vector<T>(2, new T[2]{
				T("21.1") * T("2.0"),
				T("1.0") * T("32.01")}));
			Assert::IsFalse(test_object_0.HadamardProd(genmath::Vector<T>(2, new T[2]{
				T("2.0") * T("32.01") })) == genmath::Vector<T>(2, new T[2]{
				T("21.1") * T("2.0"),
				T("1.0") * T("32.0501")}));
			
			
			// T& operator[](size_t ind);
			Assert::IsTrue(test_object_0[0] == T("21.1"));
			
			
			// const T& operator[](size_t ind) const;
			const genmath::Vector<T> test_object_1(std::vector<T>{T("1.0")});
			Assert::IsTrue(test_object_1[0] == T("1.0"));
			

			// size_t Size() const;
			Assert::IsTrue(test_object_0.Size() == 2);
			Assert::IsFalse(test_object_0.Size() == 3);
		}
	};
}