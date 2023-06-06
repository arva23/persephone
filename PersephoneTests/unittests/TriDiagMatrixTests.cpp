#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/include/libfixmath/fixmath.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/genmath/Vector.h"
#include "../../Persephone/genmath/Matrix.cpp"
#include "../../Persephone/genmath/QMatrix.h"
#include "../../Persephone/genmath/TriDiagMatrix.h"
#include <initializer_list>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;

	TEST_CLASS(TriDiagMatrixTests) {

		TEST_METHOD(ObjectResourceManagement) {

			// genmath::TriDiagMatrix();
			genmath::TriDiagMatrix<T> test_object_0 = genmath::TriDiagMatrix<T>();
			
			// genmath::TriDiagMatrix(size_t dim);
			genmath::TriDiagMatrix<T> test_object_1(10);
			
			try { 
				genmath::TriDiagMatrix<T> test_object_1(0);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Insufficient dimension parameter(s) (Matrix)."));
			}
			
			try { 
				genmath::TriDiagMatrix<T> test_object_1(1);
				Assert::Fail(); 
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension parameter is out of range (TriDiagMatrix).")); 
			}
			
			try { 
				genmath::TriDiagMatrix<T> test_object_1(2);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Dimension parameter is out of range (TriDiagMatrix).")); 
			}

			// genmath::TriDiagMatrix(std::vector<T>&data_);
			std::vector<T> data_0{ T("1.0"), T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("6.0"), T("7.0")};
			genmath::TriDiagMatrix<T> test_object_2(data_0);
			
			try {
			
				std::vector<T> data_1{ T("2.0"), T("3.0"), T("4.0"), T("5.0") };
				genmath::TriDiagMatrix<T> test_object_2(data_1);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension parameter is out of range (TriDiagMatrix)."));
			}

			try {

				std::vector<T> data_1{ T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("2.0"), T("2.0"), T("2.0"), T("2.0") };
				genmath::TriDiagMatrix<T> test_object_2(data_1);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Inappropriate storage parameter length (TriDiagMatrix)."));
			}

			try {
			
				std::vector<T> data_1{T("0.0"), T("0.0"), T("0.0"), T("0.0"), T("0.0"), T("0.0") , T("0.0") };
				genmath::TriDiagMatrix<T> test_object_2(data_1, true);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Determinant is zero (TriDiagMatrix)."));
			}

			try {

				std::vector<T> data_1{ T("0.0"), T("1.0"), T("1.0"), T("0.0"), T("1.0"), T("1.0") , T("0.0") };
				genmath::TriDiagMatrix<T> test_object_2(data_1, true);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Determinant is zero (TriDiagMatrix).")); 
			}

			// genmath::TriDiagMatrix(const genmath::TriDiagMatrix<T>&orig);
			genmath::TriDiagMatrix<T> test_object_3(test_object_2);
			Assert::IsTrue(test_object_3 == test_object_2);

			// ~genmath::TriDiagMatrix();
		
		}

		TEST_METHOD(AssignmentOperators) {

			std::vector<T> data_0{T("1.0"), T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("6.0"), T("7.0")};
			std::vector<T> data_1{T("8.0"), T("9.0"), T("10.0"), T("11.0"), T("12.0"), T("13.0"), T("14.0"), T("15.0"), T("16.0"), T("17.0")};
			genmath::TriDiagMatrix<T> test_object_0(data_0);
			genmath::TriDiagMatrix<T> test_object_1(data_1);
			// bool operator=(std::string param) override;
			test_object_1 = test_object_0;
			Assert::IsTrue(test_object_0 == test_object_1);

			// genmath::TriDiagMatrix<T>& operator=(const genmath::TriDiagMatrix<T>&orig);
			Assert::IsTrue(test_object_0 = "1.0 2.0 0.0!3.0 4.0 5.0!0.0 6.0 7.0");
			test_object_0 = "1.0 2.0 0.0|3.0 4.0 5.0|0.0 6.0 7.0";
			Assert::IsTrue(test_object_0 == test_object_1);
		}

		TEST_METHOD(LogicalOperators) {

			std::vector<T> data_0{ T("1.0"), T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("6.0"), T("7.0") };
			std::vector<T> data_1{ T("8.0"), T("9.0"), T("10.0"), T("11.0"), T("12.0"), T("13.0"), T("14.0") };
			genmath::TriDiagMatrix<T> test_object_0(data_0);
			genmath::TriDiagMatrix<T> test_object_1(data_1);
			
			// bool operator==(const genmath::TriDiagMatrix<T>&operand) const;
			Assert::IsFalse(test_object_0 == test_object_1);
			Assert::IsTrue(test_object_0 == test_object_0);

			// bool operator!=(const genmath::TriDiagMatrix<T>&operand) const;
			Assert::IsTrue(test_object_0 != test_object_1);
			Assert::IsFalse(test_object_1 != test_object_1);
		}

		TEST_METHOD(CompoundArithmeticUnaryOperators) {

			std::vector<T> data_0{ T("1.0"), T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("6.0"), T("7.0") };
			std::vector<T> data_1{ T("8.0"), T("9.0"), T("10.0"), T("11.0"), T("12.0"), T("13.0"), T("14.0") };
			genmath::TriDiagMatrix<T> test_object_0(data_0);
			genmath::TriDiagMatrix<T> test_object_1(data_1);

			// void operator+=(const genmath::TriDiagMatrix<T>&operand);
			std::vector<T> data_2{ T("1.0") + T("8.0"), T("2.0") + T("9.0"), T("3.0") + T("10.0"),
				T("4.0") + T("11.0"), T("5.0") + T("12.0"), T("6.0") + T("13.0"), T("7.0") + T("14.0") };
			genmath::TriDiagMatrix<T> result(data_2);
			test_object_0 += test_object_1;
			Assert::IsTrue(result == test_object_0);

			// void operator-=(const genmath::TriDiagMatrix<T>&operand);
			test_object_0 -= test_object_1;
			result = genmath::TriDiagMatrix<T>(data_0);
			Assert::IsTrue(result == test_object_0);
		}

		TEST_METHOD(ArithmeticBinaryOperators) {

			std::vector<T> data_0{ T("1.0"), T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("6.0"), T("7.0") };
			std::vector<T> data_1{ T("8.0"), T("9.0"), T("10.0"), T("11.0"), T("12.0"), T("13.0"), T("14.0") };
			genmath::TriDiagMatrix<T> test_object_0(data_0);
			genmath::TriDiagMatrix<T> test_object_1(data_1);

			// genmath::TriDiagMatrix<T> operator+(const genmath::TriDiagMatrix<T>&operand) const;
			std::vector<T> data_2{ T("1.0") + T("8.0"), T("2.0") + T("9.0"), T("3.0") + T("10.0"),
				T("4.0") + T("11.0"), T("5.0") + T("12.0"), T("6.0") + T("13.0"), T("7.0") + T("14.0") };
			genmath::TriDiagMatrix<T> result(data_2);

			Assert::IsTrue(result == test_object_0 + test_object_1);

			// genmath::TriDiagMatrix<T> operator-(const genmath::TriDiagMatrix<T>&operand) const;
			data_2 = std::vector<T>{ T("1.0") - T("8.0"), T("2.0") - T("9.0"), T("3.0") - T("10.0"),
				T("4.0") - T("11.0"), T("5.0") - T("12.0"), T("6.0") - T("13.0"), T("7.0") - T("14.0") };
			result = genmath::TriDiagMatrix<T>(data_2);

			Assert::IsTrue(result == test_object_0 - test_object_1);

			// genmath::Vector<T> operator*(const genmath::Vector<T>&operand) const override;
			genmath::Vector<T> test_object_3(std::vector<T>{T("9.8"), T("7.6"), T("5.4")});
			data_2 = std::vector<T>{
				T("1.0") * T("9.8") + T("2.0") * T("7.6"),
				T("3.0") * T("9.8") + T("4.0") * T("7.6") + T("5.0") * T("5.4"),
				T("6.0") * T("7.6") + T("7.0") * T("5.4")
			};

			genmath::Vector<T> result_2(data_2);
			Assert::IsTrue(result_2 == test_object_0 * test_object_3);
		}

		TEST_METHOD(AuxiliaryMethods) {

			std::vector<T> data_0{ T("1.0"), T("2.0"), T("3.0"), T("4.0"), T("5.0"), T("6.0"), T("7.0") };
			std::vector<T> data_1{ T("8.0"), T("9.0"), T("10.0"), T("11.0"), T("12.0"), T("13.0"), T("14.0") };
			genmath::TriDiagMatrix<T> test_object_0(data_0);
			genmath::TriDiagMatrix<T> test_object_1(data_1);

			// std::vector<genmath::Vector<T> > GetData() const
			std::vector<genmath::Vector<T> > result_0{
				genmath::Vector(std::vector<T>{T("1.0"), T("2.0"), T("0.0")}),
				genmath::Vector(std::vector<T>{T("3.0"), T("4.0"), T("5.0")}),
				genmath::Vector(std::vector<T>{T("0.0"), T("6.0"), T("7.0")})};

			Assert::IsTrue(test_object_0.GetData() == result_0);
			
			
			//std::vector<genmath::Vector<T> > GetColumnVecs() const override;
			std::vector<genmath::Vector<T> > result_1{

				genmath::Vector(std::vector<T>{T("1.0"), T("3.0"), T("0.0")}),
				genmath::Vector(std::vector<T>{T("2.0"), T("4.0"), T("6.0")}),
				genmath::Vector(std::vector<T>{T("0.0"), T("5.0"), T("7.0")})};
			Assert::IsTrue(test_object_0.GetColumnVecs() == result_1);


			// std::vector<genmath::Vector<T> > GetRowVecs() const override;
			Assert::IsTrue(test_object_0.GetRowVecs() == result_0);

			// const genmath::Vector<T>& operator[](size_t ind) const;
			const genmath::TriDiagMatrix<T> test_object_2(test_object_0);
			Assert::IsTrue(test_object_2[0][0] == T("1.0"));

			try { 
				test_object_2[3];
				Assert::Fail();
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("Undefined value at index (TriDiagMatrix)."));  
			}
			
			// T GetDet() const;
			std::vector<T> data_2{T("1.0"), T("0.0"),
							      T("0.0"), T("1.0"), T("0.0"),
							      T("0.0"), T("1.0")};
			test_object_1 = genmath::TriDiagMatrix<T>(data_2);
			Assert::IsTrue(test_object_1.GetDet() == T("1.0"));
			
			data_2 = { 
				T("7.0"), T("4.0"),
				T("5.0"), T("5.0"), T("0.0"),
				T("3.0"), T("8.0"), T("2.0"),
				T("6.0"), T("5.0"), T("2.0"),
				T("1.0"), T("2.0"), T("9.0"),
				T("4.0"), T("1.0"), T("5.0"),
				T("6.0"), T("7.0") };
			test_object_1 = genmath::TriDiagMatrix<T>(data_2);
			
			Assert::IsTrue(test_object_1.GetDet() == T("-119640.000000"));

			
			// genmath::TriDiagMatrix<T> GenLinComb(const genmath::Vector<T>&operand) const;
			std::vector<T> data_3{ T("1.0") * T("9.8"), T("2.0") * T("7.6"), T("3.0") * T("9.8"),
				T("4.0") * T("7.6"), T("5.0") * T("5.4"), T("6.0") * T("7.6"), T("7.0") * T("5.4") };
			genmath::TriDiagMatrix<T> result_2(data_3);
			genmath::Vector<T> test_object_3(std::vector<T>{T("9.8"), T("7.6"), T("5.4")});
			genmath::TriDiagMatrix<T> test_object_5(test_object_0);
			test_object_5.GenLinCombImplicit(test_object_3);
			Assert::IsTrue(test_object_5 == result_2);
			Assert::IsTrue(test_object_0.GenLinComb(test_object_3) == result_2);
			
			
			// genmath::Vector<T> SolveGauss(const genmath::Vector<T>&operand) const;
			test_object_0 = genmath::TriDiagMatrix<T>(data_0);
			// https://matrixcalc.org/en/slu.html#solve-using-Gaussian-elimination(%7B%7B1,2,0,9%2e8%7D,%7B3,4,5,7%2e6%7D,%7B0,6,7,5%2e4%7D%7D)
			Assert::IsTrue(test_object_0.SolveGauss(test_object_3) == genmath::Vector<T>(
				std::vector<T>{T("18.0") / T("11.0"), T("449.0") / T("110.0"), T("-30.0") / T("11.0")}));

			genmath::Vector<T> test_object_4(std::vector<T>{T("9.8"), T("7.6")});
			try { test_object_0.SolveGauss(test_object_4); Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(
				err.what() == std::string("Dimension mismatch (QMatrix).")); }


			// genmath::Vector<T> SolveThomas(genmath::Vector<T> operand) const;
			

			// operator std::string() const override;
			test_object_0 = genmath::TriDiagMatrix<T>(data_0);
			Assert::IsTrue(std::string(test_object_0)
				== std::string(T("1.0")) + " " + std::string(T("2.0"))
				+ " " + std::string(T("0.0")) + "|" + std::string(T("3.0"))
				+ " " + std::string(T("4.0")) + " " + std::string(T("5.0"))
				+ "|" + std::string(T("0.0")) + " " + std::string(T("6.0"))
				+ " " + std::string(T("7.0")));
		}
	};
}