#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/include/libfixmath/fixmath.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/genmath/Vector.h"
#include "../../Persephone/genmath/Matrix.cpp"
#include "../../Persephone/genmath/QMatrix.h"
#include "../../Persephone/genmath/TriDiagMatrix.h"
#include "../../Persephone/printerheatconduction/FDM1.h"
#include <initializer_list>
#include <exception>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;
	TEST_CLASS(FDM1Tests) {

		TEST_METHOD(ObjectResourceManagement) {

			// FDM1();
			FDM1<T> test_object_0;


			// FDM1(std::vector<T*>&values, std::vector<T*>&coeffs, T space_step, T start_time);
			genmath::Vector<T> values(std::vector<T>{ T("24.0")});
			genmath::Vector<T> coeffs(std::vector<T>{T("0.5"), T("0.5")});

			try {
				
				FDM1<T> test_object_0(&values, &coeffs, T("1.0"), T("0.0"));
				Assert::Fail(); 
			}
			catch (std::exception err) {
				Assert::IsTrue(err.what() == std::string("Sizes of containers are different"
					" (values and coefficients) (FDM1)."));
			}

			values.PushBack(T("24.0"));

			try {
			
				FDM1<T> test_object_0(&values, &coeffs, T("1.0"), T("0.0"));
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Size of values are less than 3 (FDM1)."));
			}

			values.PushBack(T("50.0"));
			coeffs.PushBack(T("0.5"));

			try {
			
				FDM1<T> test_object_0(&values, &coeffs, T("-1.0"), T("0.0"));
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Space step can not be zero or negative (FDM1)."));
			}

			try {
			
				FDM1<T> test_object_0(&values, &coeffs, T("1.0"), T("-1.0"));
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Start time can not be negative (FDM1)."));
			}

			FDM1<T> test_object_1(&values, &coeffs, T("1.0"), T("0.0"));

			FDM1<T> test_object_2(test_object_1);
			Assert::IsTrue(test_object_2 == test_object_1);

			// ~FDM1();
			// RAII

		}

		TEST_METHOD(AssignmentOperators) {

			// FDM1<T>& operator=(const FDM1<T>&orig);
			genmath::Vector<T> values(std::vector<T>{ T("24.0"), T("24.0"), T("50.0") });
			genmath::Vector<T> coeffs(std::vector<T>{ T("0.5"), T("0.5"), T("0.5") });
			FDM1<T> test_object_0(&values, &coeffs, T("1.0"), T("0.0"));
			FDM1<T> test_object_1 = test_object_0;
			Assert::IsTrue(test_object_1 == test_object_0);
		}

		TEST_METHOD(ComputationPerformers) {
		
			// genmath::Vector<T>& operator++();
			genmath::Vector<T> values_0(std::vector<T>{ T("80.0"), T("80.0"), T("80.0"), T("80.0"), T("80.0"),
				T("80.0"), T("80.0"), T("80.0"), T("80.0"), T("601.1")});
			genmath::Vector<T> values_1(values_0);
			genmath::Vector<T> values_2(values_0);
			// for too small numbers, numerical instability occurs
			genmath::Vector<T> coeffs_0(std::vector<T>{ T("0.21"), T("0.21"), T("0.21"), T("0.21"), T("0.21"),
				T("0.21"), T("0.21"), T("0.21"), T("0.21"), T("0.21") });
			
			T space_step = "0.2";
			T time_step = (space_step * space_step) / (genmath::LongDouble("2.0") * coeffs_0[0]);
			T null_elem("0.0");
			T unit_elem("1.0");
			T mtx_coeff = time_step / (T("2.0") * space_step * space_step);
			
			FDM1<T> test_object_0(&values_0, &coeffs_0, T("0.2"), T("0.0"));
			
			std::vector<T> lhs_data{

				(T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff), (T("-1.0") * mtx_coeff),
				(T("-1.0") * mtx_coeff), (T("2.0") * mtx_coeff)
			};
			genmath::TriDiagMatrix<T> lhs_mtx(lhs_data);

			std::vector<T> rhs_data{
			
				(T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff), (mtx_coeff),
				(mtx_coeff), (T("-2.0") * mtx_coeff)
			};
			genmath::TriDiagMatrix<T> rhs_mtx(rhs_data);

			std::vector<T> identity_data{

				T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0"), T("0.0"),
				T("0.0"), T("1.0")
			};
			
			genmath::TriDiagMatrix<T> identity_mtx(identity_data);
			genmath::TriDiagMatrix<T> mod_lhs = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			genmath::Vector<T> mod_rhs = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * values_0;
			genmath::Vector<T> result_0 = mod_lhs.SolveGauss(mod_rhs);
			
			genmath::Vector<T> result_00 = ++test_object_0;
			Assert::IsTrue(result_00 == result_0);
			

			// genmath::Vector<T> operator++(int);
			FDM1<T> test_object_1(&values_1, &coeffs_0, T("0.2"), T("0.0"));
			test_object_1++;
			genmath::Vector<T> result_01 = test_object_1.GetRecentValues();
			Assert::IsTrue(result_01 == result_0);


			// genmath::Vector<T>& operator+=(T target_time);
			// iteration 0.
			genmath::TriDiagMatrix<T> mod_lhs_1 = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			genmath::Vector<T> mod_rhs_1 = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * values_2;
			genmath::Vector<T> result_020 = mod_lhs_1.SolveGauss(mod_rhs_1);

			// iteration 1.
			mod_lhs_1 = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			mod_rhs_1 = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * result_020;
			genmath::Vector<T> result_021 = mod_lhs_1.SolveGauss(mod_rhs_1);

			// iteration 2.
			mod_lhs_1 = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			mod_rhs_1 = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * result_021;
			genmath::Vector<T> result_022 = mod_lhs_1.SolveGauss(mod_rhs_1);

			// iteration 3.
			mod_lhs_1 = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			mod_rhs_1 = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * result_022;
			genmath::Vector<T> result_023 = mod_lhs_1.SolveGauss(mod_rhs_1);

			// iteration 4.
			mod_lhs_1 = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			mod_rhs_1 = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * result_023;
			genmath::Vector<T> result_024 = mod_lhs_1.SolveGauss(mod_rhs_1);

			// iteration 5.
			mod_lhs_1 = identity_mtx + lhs_mtx.GenLinComb(coeffs_0);
			mod_rhs_1 = (identity_mtx + rhs_mtx.GenLinComb(coeffs_0)) * result_024;
			genmath::Vector<T> result_025 = mod_lhs_1.SolveGauss(mod_rhs_1);

			FDM1<T> test_object_2(&values_2, &coeffs_0, T("0.2"), T("0.0"));
			genmath::Vector<T> result_02 = test_object_2 += (T("0.095238095238095261") * T("5.0"));



			Assert::IsTrue(result_02 == result_025);

			result_02 = test_object_2 += (T("20.0"));
			
			try {
			
				test_object_2 += T("0.0");
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Requested duration is not valid "
					"or is less than the time step (operator+=) (FDM1)."));
			}

			try {
			
				test_object_2 += T("-10.49");
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Requested duration is not valid "
					"or is less than the time step (operator+=) (FDM1)."));
			}
		}
		
		TEST_METHOD(AuxiliaryMethods) {

			// genmath::Vector<T>& GetRecentValues();
			genmath::Vector<T> values_0(std::vector<T>{ T("0.0"), T("0.0"), T("0.0"), T("0.0"), T("0.0"),
				T("0.0"), T("0.0"), T("0.0"), T("0.0"), T("100.0")});
			genmath::Vector<T> coeffs_0(std::vector<T>{ T("0.5"), T("0.5"), T("0.5"), T("0.5"), T("0.5"),
				T("0.5"), T("0.5"), T("0.5"), T("0.5"), T("0.5") });

			FDM1<T> test_object_0(&values_0, &coeffs_0, T("1.0"), T("0.0"));
			Assert::IsTrue(test_object_0.GetRecentValues() == values_0);


			// genmath::Vector<T>& GetCoeffs()
			Assert::IsTrue(test_object_0.GetCoeffs() == coeffs_0);


			// void SetTime(T time);
			test_object_0.SetTime(T("2.112"));
			Assert::IsTrue(test_object_0.GetTime() == T("2.112"));
		}
	};
}
