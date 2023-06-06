#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/include/libfixmath/fixmath.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/genmath/Vector.h"
#include "../../Persephone/genmath/Matrix.h"
#include "../../Persephone/genmath/TriDiagMatrix.cpp"
#include <initializer_list>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;

	TEST_CLASS(QMatrixTests) {
			
		TEST_METHOD(ObjectResourceManagement) {

			// genmath::QMatrix();
			genmath::QMatrix<T> test_object_0 = genmath::QMatrix<T>();
			

			// genmath::QMatrix(size_t dim);
			genmath::QMatrix<T> test_object_1(10);
			

			// genmath::QMatrix(std::vector<std::vector<T> >&data);
			std::vector<genmath::Vector<T> > data_0{
				genmath::Vector<T>(std::vector<T>{T("3.35"), T("5.8")}),
				genmath::Vector<T>(std::vector<T>{T("3.5"), T("5.18")})};


			genmath::QMatrix<T> test_object_2(data_0, true);

			try {
				data_0.pop_back();
				genmath::QMatrix<T> test_object_2(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string(
					"Dimension mismatch (probably not quadratic matrix) (QMatrix).")); }
			
			
			// genmath::QMatrix(std::vector<genmath::Vector<T> >&data);
			std::vector<genmath::Vector<T> > data_1{
				genmath::Vector<T>(std::vector<T>{T("5.8"), T("8.2")}),
				genmath::Vector<T>(std::vector<T>{T("834.5"), T("1.9")})};
			genmath::QMatrix<T> test_object_3(data_1);
			
			try {

				data_1.pop_back();
				genmath::QMatrix<T> test_object_3(data_1);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string(
					"Dimension mismatch (probably not quadratic matrix) (QMatrix).")); }

			try {
			
				std::vector<std::vector<T> > data_2{

					std::vector<T>{T(), T()},
					std::vector<T>{T(), T()}
				};
				genmath::QMatrix<T> test_object_4(data_2, true);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Determinant is zero (QMatrix)."));
			}


			// genmath::QMatrix(const genmath::QMatrix<T>&orig);
			genmath::QMatrix<T> test_object_5(test_object_0);
			
			
			// ~genmath::QMatrix();

		}

		TEST_METHOD(AssignmentOperators) {

			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("3.5"), T("5.8")},
				std::vector<T>{T("3.5"), T("5.8")}};

			genmath::QMatrix<T> test_object_0(data_0);
			genmath::QMatrix<T> test_object_1;
			// genmath::QMatrix<T>& operator=(const genmath::QMatrix<T>&orig);
			test_object_1 = test_object_0;
			Assert::IsTrue(test_object_1[0] == test_object_0[0]);

			
			// virtual bool operator=(std::string param) override;
			Assert::IsTrue(test_object_0 = "");
			Assert::IsFalse(test_object_0 = "1.0 2.1 3.2|4.3 5.4 6.5|3.54 5.6 6.7");

			std::vector<std::vector<T> > data_1{
				std::vector<T>{T("1.0"), T("2.1"), T("3.2")},
				std::vector<T>{T("4.3"), T("5.4"), T("6.5")},
				std::vector<T>{T("3.54"), T("5.6"), T("6.7")}};

			Assert::IsTrue(test_object_0 == genmath::QMatrix<T>(data_1));

			Assert::IsTrue(test_object_0 = "1.0 2.1 3.2!4.3 5.4 6.5!3.54 5.6");
			Assert::IsTrue(test_object_0 = "1.0 2.1 3.2|4.3 5.4 6.5|3.54 5.6");
		}

		TEST_METHOD(LogicalOperators) {

			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.9"), T("7.4"), T("5.8")},
				std::vector<T>{T("5.8"), T("5.8"), T("5.8")},
				std::vector<T>{T("1.43"), T("345.67"), T("3.562")}};

			std::vector<std::vector<T> > data_1{
				std::vector<T>{T("4.6"), T("5.8"), T("5.8")},
				std::vector<T>{T("5.8"), T("5.8"), T("78.4")},
				std::vector<T>{T("1.43"), T("345.67"), T("3.562")}};

			genmath::QMatrix<T> test_object_0(data_0);
			genmath::QMatrix<T> test_object_1(data_1);
			
			
			// bool operator==(const genmath::QMatrix<T>&operand) const;
			Assert::IsTrue(test_object_0 == test_object_0);
			Assert::IsFalse(test_object_0 == test_object_1);
			
			
			// bool operator!=(const genmath::QMatrix<T>&operand) const;
			Assert::IsFalse(test_object_0 != test_object_0);
			Assert::IsTrue(test_object_0 != test_object_1);
		}

		TEST_METHOD(CompoundArithmeticUnaryOperators) {


			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.789"), T("234.4")},
				std::vector<T>{T("1.789"), T("234.4")}};

			genmath::QMatrix<T> test_object_0(data_0);
			
			std::vector<std::vector<T> > data_1{
				std::vector<T>{T("34.57"), T("45.324")},
				std::vector<T>{T("34.57"), T("45.324")}};

			genmath::QMatrix<T> test_object_1(data_1);

			std::vector<std::vector<T> > data_2{
				std::vector<T>{ T("81650.5373"), T("10705.030236"), T("10705.030236")},
				std::vector<T>{	T("81650.5373"), T("10705.030236"), T("10705.030236")},
				std::vector<T>{	T("81650.5373"), T("10705.030236"), T("10705.030236")}};

			genmath::QMatrix<T> test_object_2(data_2);
			
			// inline void operator+=(const genmath::QMatrix<T>&operand);
			genmath::QMatrix<T> result(test_object_0);
			result += test_object_1;
			
			try { 
				result += test_object_2; 
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			
			try {
				result += test_object_2; 
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			
			try {
				genmath::TriDiagMatrix<T> test_object_2;
				genmath::QMatrix<T>& test_object_3(test_object_2);
				test_object_3 += test_object_1;
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Object type is tridiagonal (unary addition) (QMatrix)."));
			}
			
			// inline void operator-=(const genmath::QMatrix<T>&operand);
			result -= test_object_1;
			
			try {
				result -= test_object_2; 
				Assert::Fail(); 
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			
			try { 
				result -= test_object_2;
				Assert::Fail(); }
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); 
			}

			try {
				genmath::TriDiagMatrix<T> test_object_2;
				genmath::QMatrix<T>& test_object_3(test_object_2);
				test_object_3 -= test_object_1;
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Object type is tridiagonal (unary subtraction) (QMatrix)."));
			}

			
			// inline void operator*=(const genmath::QMatrix<T>&operand);
			result *= test_object_1;

			try {
				result *= test_object_2;
				Assert::Fail(); 
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}

			try {
				genmath::TriDiagMatrix<T> test_object_2;
				genmath::QMatrix<T>& test_object_3(test_object_2);
				test_object_3 *= test_object_1;
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Object type is tridiagonal (unary multiplication) (QMatrix)."));
			}
		}

		TEST_METHOD(ArithmeticBinaryOperators) {

			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.789"), T("234.4")},
				std::vector<T>{T("1.789"), T("234.4")}
			};
			
			genmath::QMatrix<T> test_object_0(data_0);
			
			std::vector<std::vector<T> >data_1{
				std::vector<T>{T("34.57"), T("45.324")},
				std::vector<T>{T("34.57"), T("45.324")}
			};
			
			genmath::QMatrix<T> test_object_1(data_1);
			
			std::vector<std::vector<T> > data_2{
				std::vector<T>{ T("81650.5373"), T("10705.030236"), T("10705.030236")},
				std::vector<T>{ T("81650.5373"), T("10705.030236"), T("10705.030236")},
				std::vector<T>{ T("81650.5373"), T("10705.030236"), T("10705.030236")}
			};
			genmath::QMatrix<T> test_object_2(data_2);

			
			// genmath::QMatrix<T> operator+(const genmath::QMatrix<T>&operand) const;
			test_object_0 + test_object_1;
			
			try {
				test_object_0 + test_object_2; 
				Assert::Fail(); 
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			
			try { 
				test_object_0 + test_object_2; 
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			
			
			// genmath::QMatrix<T> operator-(const genmath::QMatrix<T>&operand) const;
			test_object_0 - test_object_1;
			
			try {
				test_object_0 - test_object_2; 
				Assert::Fail(); 
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); 
			}
			
			try {
				test_object_0 - test_object_2;
				Assert::Fail();
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}

			
			// genmath::QMatrix<T> operator*(const genmath::QMatrix<T>&operand) const;
			test_object_0 * test_object_1;
			
			try { 
				test_object_0* test_object_2; 
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}

			
			// virtual genmath::Vector<T> operator*(const genmath::Vector<T>&operand) const;
			genmath::Vector<T> test_object_3(std::vector<T>{T("543.2"), T("4.5")});
			genmath::Vector<T> test_object_4(std::vector<T>{T("2.35"), T("4.259"), T("-0.4345")});
			
			test_object_0 * test_object_3;
			
			try {
				test_object_0 * test_object_4; 
				Assert::Fail();
			}
			catch (std::exception err) { 

				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
		}

		TEST_METHOD(AuxiliaryMethods) {

			std::vector<genmath::Vector<T> > data_0{
				genmath::Vector<T>(std::vector<T>{T("1.789"), T("234.4")}),
				genmath::Vector<T>(std::vector<T>{T("1.789"), T("234.4")}) };

			genmath::QMatrix<T> test_object_0(data_0);

			// std::vector<genmath::Vector<T> > GetData() const override;
			Assert::IsTrue(test_object_0.GetData() == data_0);


			// virtual std::vector<genmath::Vector<T> > GetColumnVecs() const override;
			Assert::IsTrue(test_object_0.GetColumnVecs() == std::vector<genmath::Vector<T> >{
				genmath::Vector<T>(std::vector<T>{T("1.789"), T("1.789")}),
					genmath::Vector<T>(std::vector<T>{T("234.4"), T("234.4")})});


			// virtual std::vector<genmath::Vector<T> > GetRowVecs() const override;
			Assert::IsTrue(test_object_0.GetRowVecs() == data_0);


			// virtual size_t GetDim() const;
			Assert::IsTrue(test_object_0.GetDim() == 2);


			// virtual genmath::Vector<T>& operator[](size_t ind) override;
			Assert::IsTrue(test_object_0[0][0] == T("1.789"));

			try { 
				test_object_0[3]; 
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (requested index is not in range) (QMatrix)."));
			}

			try {
				genmath::TriDiagMatrix<T> test_object_0;
				genmath::QMatrix<T>& test_object_1 = test_object_0;
				test_object_1[0];
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Object type is tridiagonal (operator[]) (QMatrix)."));
			}


			// virtual const genmath::Vector<T>& operator[](size_t ind) const override;
			const genmath::QMatrix<T> test_object_1(test_object_0);
			Assert::IsTrue(test_object_1[0][0] == T("1.789"));

			try { 
				test_object_1[3];
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (requested index is not in range) (QMatrix)."));
			}

			std::vector<T> tridiag_test_object_data{
				T("1.0"), T("1.0"), T("1.0"), T("1.0"), T("1.0"), T("1.0"), T("1.0") };
			const genmath::TriDiagMatrix<T> test_object_7(tridiag_test_object_data);
			const genmath::Matrix<T>& test_object_8(test_object_7);
			test_object_8[0];

			// T GetDet();
			std::vector<std::vector<T> > data_1{

				std::vector<T>{T("1.0"), T("0.0"), T("0.0")},
				std::vector<T>{T("0.0"), T("1.0"), T("0.0")},
				std::vector<T>{T("0.0"), T("0.0"), T("1.0")} };

			genmath::QMatrix<T> test_object_2(data_1);
			Assert::IsTrue(test_object_2.GetDet() == T("1.0"));

			std::vector<std::vector<T> > data_2{

				std::vector<T>{T("7.0"), T("8.0"), T("0.0")},
				std::vector<T>{T("6.0"), T("0.0"), T("4.0")},
				std::vector<T>{T("2.0"), T("6.0"), T("1.0")} };

			genmath::QMatrix<T> test_object_3(data_2);
			Assert::IsTrue(test_object_3.GetDet() == T("-152.0"));


			// genmath::QMatrix<T> GaussElim(size_t & no_row_swaps, const genmath::Vector<T>&operand) const;
			// https://matrixcalc.org/en/slu.html#solve-using-Gaussian-elimination(%7B%7B7,8,0,1%7D,%7B6,0,4,2%7D,%7B2,6,1,3%7D%7D)
			genmath::Vector<T> test_object_4(std::vector<T>{T("1.0"), T("2.0"), T("3.0")});
			genmath::Vector<T> test_object_5(std::vector<T>{T("1.0"), T("2.0")});

			std::vector<std::vector<T> > data_3{

				std::vector<T>{T("7.0"), T("8.0"), T("0.0"), T("1.0")},
				std::vector<T>{T("0.0"), T("-48.0") / T("7.0"), T("4.0"), T("8.0") / T("7.0")},
				std::vector<T>{T("0.0"), T("0.0"), T("19.0") / T("6.0"), T("10.0") / T("3.0")} };

			genmath::Matrix<T> result_0(data_3);
			size_t no_row_swaps_test = 0;

			Assert::IsTrue(result_0 == test_object_3.GaussElim(no_row_swaps_test, test_object_4));

			const genmath::QMatrix<T> test_object_6(test_object_3);
			Assert::IsTrue(result_0 == test_object_6.GaussElim(no_row_swaps_test, test_object_4));


			// genmath::QMatrix<T> GaussElim(const genmath::Vector<T>&operand) const;
			Assert::IsTrue(result_0 == test_object_3.GaussElim(test_object_4));
			Assert::IsTrue(result_0 == test_object_6.GaussElim(test_object_4));

			try { 
				test_object_3.GaussElim(test_object_5);
				Assert::Fail(); 
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (QMatrix)."));
			}

			try { 
				test_object_6.GaussElim(test_object_5); 
				Assert::Fail(); 
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (QMatrix)."));
			}


			// genmath::Vector<T> SolveGauss(const genmath::Vector<T>&operand) const;
			Assert::IsTrue(test_object_3.SolveGauss(test_object_4) == genmath::Vector<T>(std::vector<T>{
				T("-7.0") / T("19.0"), T("17.0") / T("38.0"), T("20.0") / T("19.0")}));
			Assert::IsTrue(test_object_6.SolveGauss(test_object_4) == genmath::Vector<T>(std::vector<T>{
				T("-7.0") / T("19.0"), T("17.0") / T("38.0"), T("20.0") / T("19.0")}));
			try { 
				test_object_3.SolveGauss(test_object_5);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (QMatrix).")); 
			}

			try {
				
				test_object_6.SolveGauss(test_object_5);
				Assert::Fail();
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (QMatrix).")); 
			}

			// genmath::QMatrix<T> GenLinComb(const genmath::Vector<T>&operand) const;
			test_object_3.GenLinComb(test_object_4);
			
			try {
				test_object_3.GenLinComb(test_object_5); 
				Assert::Fail();
			}
			catch (std::exception err) { 
			
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			

			// virtual inline operator std::string() const override;
			Assert::IsTrue(std::string(test_object_3) == std::string(T("7.0"))
				+ " " + std::string(T("8.0")) + " " + std::string(T("0.0"))
				+ "|" + std::string(T("6.0")) + " " + std::string(T("0.0"))
				+ " " + std::string(T("4.0")) + "|" + std::string(T("2.0"))
				+ " " + std::string(T("6.0")) + " " + std::string(T("1.0")));
		}
	};
}