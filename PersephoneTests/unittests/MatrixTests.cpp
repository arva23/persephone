#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/genmath/Vector.h"
#include "../../Persephone/genmath/Matrix.cpp"
#include <initializer_list>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;
	TEST_CLASS(MatrixTests) {

		TEST_METHOD(ObjectResourceManagement) {
		
			// genmath::Matrix();
			genmath::Matrix<T> test_object_0 = genmath::Matrix<T>();


			// genmath::Matrix(size_t no_rows, size_t no_cols);
			genmath::Matrix<T> test_object_1(3, 2);
			try {
				genmath::Matrix<T> test_object_0(0, 3);
				genmath::Matrix<T> test_object_1(2, 0);
				genmath::Matrix<T> test_object_2(0, 0);
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Insufficient dimension parameter(s) (Matrix)."));
			}


			// genmath::Matrix(std::vector<std::vector<T> >&data);
			std::vector<std::vector<T> > data_0{
				std::vector<T>{std::vector<T>{T("1.54"), T("4.236")}},
				std::vector<T>{std::vector<T>{T("3.68"), T("1.0")}},
				std::vector<T>{std::vector<T>{T("5.8"), T("0.093")}}};
			genmath::Matrix<T> test_object_2(data_0);

			try {
				std::vector<std::vector<T> > data_0;
				genmath::Matrix<T> test_object_0(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Insufficient row dimension parameter (Matrix)."));
			}
			
			try {
				std::vector<std::vector<T> > data_0{
					std::vector<T>{},
					std::vector<T>{T("3.68"), T("1.0")},
					std::vector<T>{T("5.8"), T("0.093")}
				};
				genmath::Matrix<T> test_object_0(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Insufficient column dimension parameter (Matrix)."));
			}
				
			try{
				std::vector<std::vector<T> > data_0{
					std::vector<T>{T("1.54"), T("4.236")},
					std::vector<T>{T("3.68")},
					std::vector<T>{T("5.8"), T("0.093")}
				};
				genmath::Matrix<T> test_object_0(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Row vector dimension mismatch (Matrix)."));
			}

			// genmath::Matrix(std::vector<genmath::Vector<T> >&data);
			std::vector<genmath::Vector<T> > data_1{
				genmath::Vector<T>(std::vector<T>{T("1.54"), T("4.236")}),
				genmath::Vector<T>(std::vector<T>{T("3.68"), T("1.0")}),
				genmath::Vector<T>(std::vector<T>{T("5.8"), T("0.093")}) };
			genmath::Matrix<T> test_object_3(data_1);

			try {
				std::vector<genmath::Vector<T> > data_0;
				genmath::Matrix<T> test_object_0(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Insufficient row dimension parameter (Matrix)."));
			}

			try {
				std::vector<genmath::Vector<T> > data_0{
					genmath::Vector(std::vector<T>{}),
					genmath::Vector(std::vector<T>{T("3.68"), T("1.0")}),
					genmath::Vector(std::vector<T>{T("5.8"), T("0.093")})};
				genmath::Matrix<T> test_object_0(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Insufficient column dimension parameter (Matrix)."));
			}

			try {
				std::vector<genmath::Vector<T> > data_0{
					genmath::Vector(std::vector<T>{T("1.54"), T("4.236")}),
					genmath::Vector(std::vector<T>{T("3.68")}),
					genmath::Vector(std::vector<T>{T("5.8"), T("0.093")})};
				genmath::Matrix<T> test_object_0(data_0);
				Assert::Fail();
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Row vector dimension mismatch (Matrix)."));
			}
			

			// genmath::Matrix(const genmath::Matrix<T>&orig);
			std::vector<std::vector<T> > data_2{
				std::vector<T>{T("1.0"), T("2.0")}};

			genmath::Matrix<T> test_object_4(data_2);
			genmath::Matrix<T> test_object_5(test_object_4);
			Assert::IsTrue(test_object_5[0] == test_object_4[0]);
			

			// ~genmath::Matrix();
		}

		TEST_METHOD(AssignmentOperators) {
		
			// genmath::Matrix<T>& operator=(const genmath::Matrix<T>&orig);
			std::vector<std::vector<T> > data_0{std::vector<T>{T("1.0"), T("2.0")}};
			
			genmath::Matrix<T> test_object_0(data_0);
			
			genmath::Matrix<T> test_object_1 = test_object_0;
			Assert::IsTrue(test_object_1[0] == test_object_0[0]);

			
			// virtual bool operator=(std::string param) override;
			Assert::IsTrue(test_object_0 = "1.0 2.1 3.2!4.3 5.4 6.5");
			test_object_0 = std::string(T("1.0")) + " " + std::string(T("2.1"))
				+ " " + std::string(T("3.2")) + "|" + std::string(T("4.3"))
				+ " " + std::string(T("5.4")) + " " + std::string(T("6.5"));
			
			Assert::IsFalse(std::string(test_object_0) == std::string(T("1.0"))
				+ " " + std::string(T("2.10001")) + " " + std::string(T("3.20000"))
				+ "|" + std::string(T("4.3")) + " " + std::string(T("5.39999"))
				+ " " + std::string(T("6.5")));
		}

		TEST_METHOD(LogicalOperators) {
		
			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.0"), T("2.0"), T("3.0")},
				std::vector<T>{T("4.0"), T("5.0"), T("6.0")}};
			
			genmath::Matrix<T> test_object_0(data_0);

			std::vector<std::vector<T> > data_1{
				std::vector<T>{T("21.0"), T("32.0"), T("34.0")},
				std::vector<T>{T("54.0"), T("656.0"), T("76.0")}};

			genmath::Matrix<T> test_object_1(data_1);
			
			// inline bool operator==(const genmath::Matrix<T>&operand) const;
			Assert::IsFalse(test_object_0 == test_object_1);


			// inline bool operator!=(const genmath::Matrix<T>&operand) const;
			Assert::IsTrue(test_object_0 != test_object_1);
			
		}

		TEST_METHOD(CompoundArithmeticUnaryOperators) {
		
			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.0"), T("2.0"), T("3.0")},
				std::vector<T>{T("4.0"), T("5.0"), T("6.0")}};

			genmath::Matrix<T> test_object_0(data_0);

			std::vector<std::vector<T> > data_1{
				std::vector<T>{T("21.0"), T("32.0"), T("34.0")},
				std::vector<T>{T("54.0"), T("656.0"), T("76.0")}};

			genmath::Matrix<T> test_object_1(data_1);

			std::vector<std::vector<T> > data_2{std::vector<T>{T("1.4"), T("4.99"), T("4.764")}};

			genmath::Matrix<T> test_object_2(data_2);

			std::vector<std::vector<T> > data_3{
				std::vector<T>{T("1.0"), T("9.0")},
				std::vector<T>{T("6.54"), T("1.0")},
				std::vector<T>{T("4.6"), T("59.33")}};

			genmath::Matrix<T> test_object_3(data_3);

			
			// inline void operator+=(const genmath::Matrix<T>& opearnd);
			std::vector<std::vector<T> > data_4{
				std::vector<T>{
					test_object_0[0][0] + test_object_1[0][0],
					test_object_0[0][1] + test_object_1[0][1],
					test_object_0[0][2] + test_object_1[0][2]
				},
				std::vector<T>{
					test_object_0[1][0] + test_object_1[1][0],
					test_object_0[1][1] + test_object_1[1][1],
					test_object_0[1][2] + test_object_1[1][2]
				}
			};
			
			genmath::Matrix<T> result_0(data_4);

			test_object_0 += test_object_1;
			Assert::IsTrue(result_0 == test_object_0);
			
			try { genmath::Matrix<T> test_object_4(test_object_0); test_object_4 += test_object_2; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
			
			try { genmath::Matrix<T> test_object_4(test_object_0); test_object_4 += test_object_3; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }

						
			// inline void operator-=(const genmath::Matrix<T>& operand);
			test_object_0 -= test_object_1;

			std::vector<std::vector<T> > data_5{
				std::vector<T>{T("1.0"), T("2.0"), T("3.0")},
				std::vector<T>{T("4.0"), T("5.0"), T("6.0")}};

			Assert::IsTrue(test_object_0 == genmath::Matrix<T>(data_5));

			try { genmath::Matrix<T> test_object_4(test_object_0); test_object_4 -= test_object_2; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
			
			try { genmath::Matrix<T> test_object_4(test_object_0); test_object_4 -= test_object_3; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
			
			
			// inline void operator*=(const genmath::Matrix<T>& operand);
			std::vector<std::vector<T> > data_6{
				std::vector<T>{
					test_object_0[0][0] * test_object_3[0][0] +
					test_object_0[0][1] * test_object_3[1][0] +
					test_object_0[0][2] * test_object_3[2][0],
					test_object_0[0][0] * test_object_3[0][1] +
					test_object_0[0][1] * test_object_3[1][1] +
					test_object_0[0][2] * test_object_3[2][1]},
				std::vector<T>{
					test_object_0[1][0] * test_object_3[0][0] +
					test_object_0[1][1] * test_object_3[1][0] +
					test_object_0[1][2] * test_object_3[2][0],
					test_object_0[1][0] * test_object_3[0][1] +
					test_object_0[1][1] * test_object_3[1][1] +
					test_object_0[1][2] * test_object_3[2][1]
				}
			};

			test_object_0 *= test_object_3;
			
			result_0 = genmath::Matrix<T>(data_6);
			Assert::IsTrue(result_0 == test_object_0);

			try { test_object_1 *= test_object_1; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
		}

		TEST_METHOD(ArithmeticBinaryOperators) {
			
			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.0"), T("2.0"), T("3.0")},
				std::vector<T>{T("4.0"), T("5.0"), T("6.0")}};

			genmath::Matrix<T> test_object_0(data_0);

			std::vector<std::vector<T> > data_1{
				std::vector<T>{T("21.0"), T("32.0"), T("34.0")},
				std::vector<T>{T("54.0"), T("656.0"), T("76.0")}};

			genmath::Matrix<T> test_object_1(data_1);

			std::vector<std::vector<T> > data_2{
				std::vector<T>{T("1.4"), T("4.99"), T("4.764")}};

			genmath::Matrix<T> test_object_2(data_2);

			std::vector<std::vector<T> > data_3{
				std::vector<T>{T("1.0"), T("9.0")},
				std::vector<T>{T("6.54"), T("1.0")},
				std::vector<T>{T("4.6"), T("59.33")}};

			genmath::Matrix<T> test_object_3(data_3);

			
			// inline genmath::Matrix<T> operator+(const genmath::Matrix<T>&operand) const;
			std::vector<std::vector<T> > data_4{

				std::vector<T>{
					test_object_0[0][0] + test_object_1[0][0],
					test_object_0[0][1] + test_object_1[0][1],
					test_object_0[0][2] + test_object_1[0][2]
				},
				std::vector<T>{
					test_object_0[1][0] + test_object_1[1][0],
					test_object_0[1][1] + test_object_1[1][1],
					test_object_0[1][2] + test_object_1[1][2]
				}
			};

			genmath::Matrix<T> result_0(data_4);

			Assert::IsTrue(result_0 == test_object_0 + test_object_1);

			try { test_object_0 + test_object_2; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
			try { test_object_0 + test_object_3; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }

			
			// inline genmath::Matrix<T> operator-(const genmath::Matrix<T>&operand) const;
			std::vector<std::vector<T> > data_5{

				std::vector<T>{
					test_object_0[0][0] - test_object_1[0][0],
					test_object_0[0][1] - test_object_1[0][1],
					test_object_0[0][2] - test_object_1[0][2]
				},
				std::vector<T>{
					test_object_0[1][0] - test_object_1[1][0],
					test_object_0[1][1] - test_object_1[1][1],
					test_object_0[1][2] - test_object_1[1][2]
				}
			};

			result_0 = genmath::Matrix<T>(data_5);

			Assert::IsTrue(result_0 == test_object_0 - test_object_1);

			try { test_object_0 - test_object_2; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
			try { test_object_0 - test_object_3; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }

			
			// inline genmath::Matrix<T> operator*(const genmath::Matrix<T>&operand) const;
			std::vector<std::vector<T> > data_6{

				std::vector<T>{
					test_object_0[0][0] * test_object_3[0][0] +
					test_object_0[0][1] * test_object_3[1][0] +
					test_object_0[0][2] * test_object_3[2][0],
					test_object_0[0][0] * test_object_3[0][1] +
					test_object_0[0][1] * test_object_3[1][1] +
					test_object_0[0][2] * test_object_3[2][1]
				},
				std::vector<T>{
					test_object_0[1][0] * test_object_3[0][0] +
					test_object_0[1][1] * test_object_3[1][0] +
					test_object_0[1][2] * test_object_3[2][0],
					test_object_0[1][0] * test_object_3[0][1] +
					test_object_0[1][1] * test_object_3[1][1] +
					test_object_0[1][2] * test_object_3[2][1]
				}
			};

			result_0 = genmath::Matrix<T>(data_6);

			Assert::IsTrue(result_0 == test_object_0 * test_object_3);

			try { test_object_1 * test_object_1; Assert::Fail(); }
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }

			
			// virtual inline genmath::Vector<T> operator*(const genmath::Vector<T>& operand) const;
			genmath::Vector<T> test_object_4(std::vector<T>{T("2.35"), T("4.259"), T("-0.4345")});
			genmath::Vector<T> test_object_5(std::vector<T>{T("543.2"), T("4.5")});
			genmath::Vector<T> result_1(std::vector<T>{
			
				test_object_0[0][0] * test_object_4[0] +
				test_object_0[0][1] * test_object_4[1] + 
				test_object_0[0][2] * test_object_4[2],
				test_object_0[1][0] * test_object_4[0] + 
				test_object_0[1][1] * test_object_4[1] +
				test_object_0[1][2] * test_object_4[2]
			});

			Assert::IsTrue(result_1 == test_object_0 * test_object_4);

			try { test_object_0 * test_object_5; Assert::Fail(); }
			catch(std::exception err){ Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix).")); }
		}

		TEST_METHOD(AuxiliaryMethods) {
			
			std::vector<std::vector<T> > data_0{
				std::vector<T>{T("1.0"), T("2.0"), T("3.0")},
				std::vector<T>{T("4.0"), T("5.0"), T("6.0")}};

			genmath::Matrix<T> test_object_0(data_0);

			
			// virtual std::vector<genmath::Vector<T> > GetData() const;
			Assert::IsTrue(test_object_0.GetData() == std::vector<genmath::Vector<T> >{
				genmath::Vector<T>(std::vector<T>{T("1.0"), T("2.0"), T("3.0")}),
				genmath::Vector<T>(std::vector<T>{T("4.0"), T("5.0"), T("6.0")})
			});

			
			// virtual inline std::vector<genmath::Vector<T> > GetColumnVecs() const;
			Assert::IsTrue(test_object_0.GetColumnVecs() == std::vector<genmath::Vector<T> >{
				genmath::Vector<T>(std::vector<T>{T("1.0"), T("4.0")}),
				genmath::Vector<T>(std::vector<T>{T("2.0"), T("5.0")}),
				genmath::Vector<T>(std::vector<T>{T("3.0"), T("6.0")})
			});
			
			
			// virtual inline std::vector<genmath::Vector<T> > GetRowVecs() const;
			Assert::IsTrue(test_object_0.GetData() == std::vector<genmath::Vector<T> >{
				genmath::Vector<T>(std::vector<T>{T("1.0"), T("2.0"), T("3.0")}),
				genmath::Vector<T>(std::vector<T>{T("4.0"), T("5.0"), T("6.0")})
			});

			
			// virtual inline std::pair<size_t, size_t> GetDims() const;
			Assert::IsTrue(test_object_0.GetDims() == std::pair<size_t, size_t>(2, 3));

			
			// virtual inline genmath::Vector<T>& operator[](size_t ind);
			Assert::IsTrue(test_object_0[0][0] == T("1.0"));

			try { test_object_0[3]; Assert::Fail();}
			catch (std::exception err) { 
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (requested index is "
					"not in range) (Matrix).")); }			
			
			// virtual inline const genmath::Vector<T>& operator[](size_t ind) const;
			const genmath::Matrix<T> test_object_1(test_object_0);
			Assert::IsTrue(test_object_1[0][0] == T("1.0"));
			
			try { test_object_1[3]; Assert::Fail(); }
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (requested index is "
					"not in range) (Matrix)."));}
			

			// inline genmath::Vector<T> BackwSubst();
			// augmented upper triangular matrix object
			//  (one column vector added for solution finding)

			std::vector<std::vector<T> > data_1{

				std::vector<T>{T("2.45"), T("3.49"), T("53.4"), T("23.45"), T("3.9")},
				std::vector<T>{T("0.0"), T("3.5"), T("4.09"), T("423.9"), T("3.439")},
				std::vector<T>{T("0.0"), T("0.0"), T("5.692"), T("23.4"), T("234.2")},
				std::vector<T>{T("0.0"), T("0.0"), T("0.0"), T("5.93"), T("34.59")}};

			genmath::Matrix<T> aug_upper_tri_test_object(data_1);

			// https://matrixcalc.org/en/slu.html#solve-using-Gauss-Jordan-elimination(%7B%7B2%2e45,3%2e49,53%2e4,23%2e45,3%2e9%7D,%7B0,3%2e5,4%2e09,423%2e9,3%2e439%7D,%7B0,0,5%2e692,23%2e4,234%2e2%7D,%7B0,0,0,5%2e93,34%2e59%7D%7D)
			Assert::IsTrue(aug_upper_tri_test_object.BackwSubst() == genmath::Vector<T>(
				std::vector<T>{
					T("605.149298785"),
					T("-725.542614503"),
					T("17.1655967548"),
					T("5.83305227656")}
			));
			
			try {

				std::vector<std::vector<T> > data_0{
					std::vector<T>{T("4.5"), T("5.9")},
					std::vector<T>{T("0.934"), T("4.90")}};

				genmath::Matrix<T> test_object_0(data_0);

				test_object_0.BackwSubst();
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("No appropriate form has provided for "
					"backward substitution (Matrix)."));
			}


			// inline void GenLinCombImplicit(const genmath::Vector<T>&operand);
			genmath::Matrix<T> test_object_3(test_object_0);
			genmath::Vector<T> test_object_2(std::vector<T>{T("9.9"), T("8.8"), T("3.7")});
			
			std::vector<std::vector<T> > data_2{
				
				std::vector<T>{
					test_object_0[0][0] * test_object_2[0],
					test_object_0[0][1] * test_object_2[1],
					test_object_0[0][2] * test_object_2[2]
				},
				std::vector<T>{
					test_object_0[1][0] * test_object_2[0],
					test_object_0[1][1] * test_object_2[1],
					test_object_0[1][2] * test_object_2[2]
				}
			};

			genmath::Matrix<T> result(data_2);
			test_object_0.GenLinCombImplicit(test_object_2);
			Assert::IsTrue(test_object_0 == result);

			try {
				genmath::Vector<T> test_object_2(std::vector<T>{T("9.9"), T("8.8")});
				test_object_0.GenLinCombImplicit(test_object_2);
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Dimension mismatch (Matrix)."));
			}
			

			// inline genmath::Matrix<T> GenLinComb(const genmath::Vector<T>&operand) const;
			Assert::IsTrue(test_object_3.GenLinComb(test_object_2) == result);
			

			// virtual inline operator std::string() const override;
			Assert::IsTrue(std::string(test_object_3) == std::string(T("1.0"))
				+ " " + std::string(T("2.0")) + " " + std::string(T("3.0"))
				+ "|" + std::string(T("4.0")) + " " + std::string(T("5.0"))
				+ " " + std::string(T("6.0")));
		}
	};
}
