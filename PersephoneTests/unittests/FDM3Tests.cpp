#include "pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/genmath/Vector.h"
#include "../../Persephone/genmath/Matrix.cpp"
#include "../../Persephone/genmath/QMatrix.h"
#include "../../Persephone/genmath/TriDiagMatrix.h"
#include "../../Persephone/printerheatconduction/FDM3.h"
#include <vector>
#include <initializer_list>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;

	TEST_CLASS(FDM3Tests) {
		TEST_METHOD(ObjectResourceManagement) {
		
			// FDM3();
			printerheatconduction::FDM3<T> test_object_0;
			

			// FDM3(const size_t x_length,
			//   const size_t y_length,
			// 	 const size_t z_length,
			//	 std::vector<genmath::Vector<T> >&x_values,
			// 	 std::vector<genmath::Vector<T> >&y_values,
			// 	 std::vector<genmath::Vector<T> >&z_values,
			// 	 std::vector<genmath::Vector<T> >&x_coeffs,
			// 	 std::vector<genmath::Vector<T> >&y_coeffs,
			// 	 std::vector<genmath::Vector<T> >&z_coeffs,
			// 	 const T space_step,
			//   const bool force_non_parallel = false);
			size_t x_length = 10;
			size_t y_length = 10;
			size_t z_length = 8;
			std::vector<genmath::Vector<T> > disc_x_lines((z_length * y_length), genmath::Vector<T>(x_length));
			std::vector<genmath::Vector<T> > disc_y_lines((z_length * x_length), genmath::Vector<T>(y_length));
			std::vector<genmath::Vector<T> > disc_z_lines((x_length * y_length), genmath::Vector<T>(z_length));
			std::vector<genmath::Vector<T> > disc_x_coeffs((z_length * y_length), genmath::Vector<T>(x_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_y_coeffs((z_length * x_length), genmath::Vector<T>(y_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_z_coeffs((x_length * y_length), genmath::Vector<T>(z_length, T("1.0")));
			
			//T space_step("0.4");
			T space_step("1.0");
			
			printerheatconduction::FDM3<T> test_object_1(x_length, y_length, z_length, &disc_x_lines, &disc_y_lines, &disc_z_lines,
				&disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, true);

			printerheatconduction::FDM3<T> test_object_2(x_length, y_length, z_length, &disc_x_lines, &disc_y_lines, &disc_z_lines,
				&disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, false);
			
			
			// FDM3(const FDM3<T>&orig);
			printerheatconduction::FDM3<T> test_object_4(test_object_2);
			
			
			// ~FDM3();
			printerheatconduction::FDM3<T>* test_object_7 = new printerheatconduction::FDM3<T>(
				x_length, y_length, z_length, &disc_x_lines, &disc_y_lines, &disc_z_lines,
				&disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, false);
			delete test_object_7;
			// or RAII

		}

		TEST_METHOD(AssignmentOperators) {

			// FDM3<T>& operator=(const FDM3<T>&orig);
			size_t x_length = 10;
			size_t y_length = 10;
			size_t z_length = 8;
			std::vector<genmath::Vector<T> > disc_x_lines((z_length * y_length), genmath::Vector<T>(x_length));
			std::vector<genmath::Vector<T> > disc_y_lines((z_length * x_length), genmath::Vector<T>(y_length));
			std::vector<genmath::Vector<T> > disc_z_lines((x_length * y_length), genmath::Vector<T>(z_length));
			std::vector<genmath::Vector<T> > disc_x_coeffs((z_length * y_length), genmath::Vector<T>(x_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_y_coeffs((z_length * x_length), genmath::Vector<T>(y_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_z_coeffs((x_length * y_length), genmath::Vector<T>(z_length, T("1.0")));

			//T space_step("0.4");
			T space_step("1.0");
			
			printerheatconduction::FDM3<T> test_object_1(x_length, y_length, z_length, &disc_x_lines, &disc_y_lines, &disc_z_lines,
				&disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, true);

			printerheatconduction::FDM3<T> test_object_2(x_length, y_length, z_length, &disc_x_lines, &disc_y_lines, &disc_z_lines,
				&disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, false);

			printerheatconduction::FDM3<T> test_object_3 = test_object_1;
			printerheatconduction::FDM3<T> test_object_4 = test_object_2;
			printerheatconduction::FDM3<T> test_object_5 = test_object_1;
		}

		TEST_METHOD(ComputationPerformers) {
		
			// numerical instabilty orrcurs at infinitesimal values (probably subnormal value losses)
			// better number representation sould be used
			size_t x_length = 6;
			size_t y_length = 6;
			size_t z_length = 10;
			std::vector<genmath::Vector<T> > disc_x_lines((z_length * y_length), genmath::Vector<T>(x_length));
			std::vector<genmath::Vector<T> > disc_y_lines((z_length * x_length), genmath::Vector<T>(y_length));
			std::vector<genmath::Vector<T> > disc_z_lines((x_length * y_length), genmath::Vector<T>(z_length));
			std::vector<genmath::Vector<T> > disc_x_coeffs((z_length * y_length), genmath::Vector<T>(x_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_y_coeffs((z_length * x_length), genmath::Vector<T>(y_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_z_coeffs((x_length * y_length), genmath::Vector<T>(z_length, T("1.0")));

			// setting ambient temperatures
			for (size_t z = 0; z < z_length; ++z) {

				for (size_t y = 1; y < y_length - 1; ++y) {

					for (size_t x = 1; x < x_length - 1; ++x) {

						disc_z_lines[y * x_length + x][z] = T("20.0");
						disc_z_coeffs[y * x_length + x][z] = T("0.21");
					}
				}
			}

			// setting border temperatures higher
			for (size_t z = 0; z < z_length; ++z) {

				for (size_t x = 0; x < x_length; ++x) {

					disc_z_lines[x][z] = T("350.0");
					disc_z_coeffs[x][z] = T("0.21");

					disc_z_lines[(y_length - 1) * x_length + x][z] = T("350.0");
					disc_z_coeffs[(y_length - 1) * x_length + x][z] = T("0.21");
				}

				for (size_t y = 1; y < y_length - 1; ++y) {

					disc_z_lines[y * x_length + 0][z] = T("350.0");
					disc_z_coeffs[y * x_length + 0][z] = T("0.21");

					disc_z_lines[y * x_length + x_length - 1][z] = T("350.0");
					disc_z_coeffs[y * x_length + x_length - 1][z] = T("0.21");
				}
			}

			T space_step("1.0");

			printerheatconduction::FDM3<T> test_object_2(x_length, y_length, z_length, &disc_x_lines,
				&disc_y_lines, &disc_z_lines, &disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, true);

			printerheatconduction::FDM3<T> test_object_3(x_length, y_length, z_length, &disc_x_lines, 
				&disc_y_lines, &disc_z_lines, &disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step);
			
			// void ProcessComponents(const uint16_t thread_ind);
			// ignored, thread pool method


			// void operator++();
			++test_object_2;
			++test_object_3;


			// void operator+=(const T duration);
			// rounding to the closest number of iterations by congruence
			test_object_2 += T("10.0");
			test_object_3 += T("10.0");

			for (size_t i = 0; i < x_length * y_length; ++i) {
				
				Logger::WriteMessage("\n");
				Logger::WriteMessage(std::string(disc_z_lines[i]).c_str());
			}
			
			try {

				test_object_2 += (T("0.0"));
				Assert::Fail();
			}
			catch (std::exception err) {

				Assert::IsTrue(err.what() == std::string("Desired duration is negative "
					"or less than the time step (operator+=) (FDM3)."));
			}

			try {
			
				test_object_2 += (T("-2.89"));
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Desired duration is negative "
					"or less than the time step (operator+=) (FDM3)."));
			}
		}
		TEST_METHOD(AuxiliaryMethods) {
		
			size_t x_length = 4;
			size_t y_length = 4;
			size_t z_length = 4;
			std::vector<genmath::Vector<T> > disc_x_lines((z_length * y_length), genmath::Vector<T>(x_length));
			std::vector<genmath::Vector<T> > disc_y_lines((z_length * x_length), genmath::Vector<T>(y_length));
			std::vector<genmath::Vector<T> > disc_z_lines((x_length * y_length), genmath::Vector<T>(z_length));
			std::vector<genmath::Vector<T> > disc_x_coeffs((z_length * y_length), genmath::Vector<T>(x_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_y_coeffs((z_length * x_length), genmath::Vector<T>(y_length, T("1.0")));
			std::vector<genmath::Vector<T> > disc_z_coeffs((x_length * y_length), genmath::Vector<T>(z_length, T("1.0")));

			//T space_step("0.4");
			T space_step("1.0");

			printerheatconduction::FDM3<T> test_object_1(x_length, y_length, z_length, &disc_x_lines, 
				&disc_y_lines, &disc_z_lines, &disc_x_coeffs, &disc_y_coeffs, &disc_z_coeffs, space_step, true);
			
			
			// T GetTimeStep();
			Assert::IsTrue(test_object_1.GetTimeStep() == T("3.0"));


			// T GetTime();
			Assert::IsTrue(test_object_1.GetTime() == T("0.0"));


			// void SetTime(const T time);
			test_object_1.SetTime("2.0");
			Assert::IsTrue(test_object_1.GetTime() == T("2.0"));
			

			// simple coordinate conversions, tests are not needed
			// inline void UpdateXObjects();
			// inline void UpdateYObjects();			
			// inline void UpdateZObjects();
		}
	};
}