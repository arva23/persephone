#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/printerheatconduction/Section.h"
#include "../../Persephone/printerheatconduction/GCodeCommand.h"
#include <initializer_list>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{

	TEST_CLASS(SectionTests) {

		using T = genmath::LongDouble;
		TEST_METHOD(ObjectResourceManagement) {

			//Section();
			Section test_object_0;

			//Section(genmath::GCodeCommand<genmath::LongDouble>* cmd_param);
			GCodeCommand<T> start_point;
			start_point = "G1 X1.0 Y2.0 Z3.0";
			genmath::LongDouble unit_time_step = "0.05";
			Section test_object_1(&start_point);

			
			//Section(const Section & orig);
			Section test_object_2(test_object_1);
			Assert::IsTrue(test_object_1.start_point_ == test_object_2.start_point_);
			Assert::IsTrue(test_object_1.end_point_ == test_object_2.end_point_);
			Assert::IsTrue(test_object_1.start_x_ == test_object_2.start_x_);
			Assert::IsTrue(test_object_1.start_y_ == test_object_2.start_y_);
			Assert::IsTrue(test_object_1.end_x_ == test_object_2.end_x_);
			Assert::IsTrue(test_object_1.end_y_ = test_object_2.end_y_);
			Assert::IsTrue(test_object_1.ratio_x_ = test_object_2.ratio_x_);
			Assert::IsTrue(test_object_1.ratio_y_ = test_object_2.ratio_y_);
			Assert::IsTrue(test_object_1.start_time_ == test_object_2.start_time_);
			Assert::IsTrue(test_object_1.end_time_ == test_object_2.end_time_);
			// differs from 0 and created section length
			//Assert::IsTrue(test_object_1.len_ = test_object_2.len_);
			//Assert::IsTrue(test_object_1.fst_len_ = test_object_2.fst_len_);
			//Assert::IsTrue(test_object_1.snd_len_ = test_object_2.snd_len_);
			//Assert::IsTrue(test_object_1.avg_spd_ == test_object_2.avg_spd_);
			//Assert::IsTrue(test_object_1.fst_avg_spd_ = test_object_2.fst_avg_spd_);
			//Assert::IsTrue(test_object_1.snd_avg_spd_ = test_object_2.snd_avg_spd_);
			//Assert::IsTrue(test_object_1.prev_sect_ = test_object_2.prev_sect_);


			//virtual ~Section();
			// RAII
			// genmath::GCodeCommand<T> objects are passed via pointers from a declared source with identifier


			//Section& operator=(const Section & orig);
			Section test_object_3 = test_object_1;
			Assert::IsTrue(test_object_1.start_point_ == test_object_3.start_point_);
			Assert::IsTrue(test_object_1.end_point_ == test_object_3.end_point_);
			Assert::IsTrue(test_object_1.start_x_ == test_object_3.start_x_);
			Assert::IsTrue(test_object_1.start_y_ == test_object_3.start_y_);
			Assert::IsTrue(test_object_1.end_x_ == test_object_3.end_x_);
			Assert::IsTrue(test_object_1.end_y_ = test_object_3.end_y_);
			Assert::IsTrue(test_object_1.ratio_x_ = test_object_3.ratio_x_);
			Assert::IsTrue(test_object_1.ratio_y_ = test_object_3.ratio_y_);
			Assert::IsTrue(test_object_1.start_time_ == test_object_3.start_time_);
			Assert::IsTrue(test_object_1.end_time_ == test_object_3.end_time_);
			//Assert::IsTrue(test_object_1.len_ = test_object_3.len_);
			//Assert::IsTrue(test_object_1.fst_len_ = test_object_3.fst_len_);
			//Assert::IsTrue(test_object_1.snd_len_ = test_object_3.snd_len_);
			//Assert::IsTrue(test_object_1.avg_spd_ == test_object_3.avg_spd_);
			//Assert::IsTrue(test_object_1.fst_avg_spd_ = test_object_3.fst_avg_spd_);
			//Assert::IsTrue(test_object_1.snd_avg_spd_ = test_object_3.snd_avg_spd_);
			//Assert::IsTrue(test_object_1.prev_sect_ = test_object_3.prev_sect_);
		}

		TEST_METHOD(TimeFunctionTest) {
		
			genmath::LongDouble unit_time_step_test("0.1");

			// setting up global parameters
			Section::XJerk = "8.0";// mm/s/s/s
			Section::YJerk = "8.0";// mm/s/s/s
			Section::XAcc = "500.0";// mm/s/s
			Section::YAcc = "500.0";// mm/s/s
			//Section::MaxResSpd = "11.7804";// 8mm/s(x), 8mm/s(y) // mm/s
			Section::Init();
			Section::UpdateGlobalResSpd(genmath::LongDouble("11.7804"));

			GCodeCommand<T> start_point;
			start_point = "G0 X0 Y0 Z0";
			GCodeCommand<T> end_point;
			end_point = "G0 X150 Y55 Z0";
			genmath::LongDouble unit_time_step = "0.05";
			Section test_object_0(&start_point);

			genmath::LongDouble test_time_point("0.0");
			try {
			
				test_object_0(test_time_point);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("No start point defined, unfinished "
					"section (Section)."));
			}

			Section test_object_2(&start_point);
			test_object_2.UpdateSection(&end_point, unit_time_step_test);
			genmath::LongDouble wrong_test_time_point("-10.0");

			try {

				test_object_2(wrong_test_time_point);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Requested time is out of range "
					"[start_time, end_time] (Section)."));
			}

			wrong_test_time_point = "2000";
			try {
			
				test_object_2(wrong_test_time_point);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Requested time is out of range [start_time, "
					"end_time] (Section)."));
			}

			Logger::WriteMessage("Average speed of section: ");
			Logger::WriteMessage(std::string(test_object_2.avg_spd_).c_str());

			Logger::WriteMessage("\nLength of section: ");
			genmath::LongDouble x_length = test_object_2.end_x_ - test_object_2.start_x_;
			genmath::LongDouble y_length = test_object_2.end_y_ - test_object_2.start_y_;
			genmath::LongDouble sect_length = genmath::LongDouble::Sqrt((x_length * x_length) + (y_length * y_length));

			Logger::WriteMessage(std::string(sect_length).c_str());
			Logger::WriteMessage("\nAbsolute time interval of section: [");
			Logger::WriteMessage(std::string(test_object_2.start_time_).c_str());
			Logger::WriteMessage(", ");
			Logger::WriteMessage(std::string(test_object_2.end_time_).c_str());
			Logger::WriteMessage("]\n");
			
			std::pair<genmath::LongDouble, genmath::LongDouble> result;
			test_time_point = "10.0";
			result = test_object_2(test_time_point);
			Logger::WriteMessage("Function call result: (");
			Logger::WriteMessage(std::string(result.first).c_str());
			Logger::WriteMessage(", ");
			Logger::WriteMessage(std::string(result.second).c_str());
			Logger::WriteMessage(")\n");

			test_time_point = "3.345";
			result = test_object_2(test_time_point);
			Logger::WriteMessage("Function call result: (");
			Logger::WriteMessage(std::string(result.first).c_str());
			Logger::WriteMessage(", ");
			Logger::WriteMessage(std::string(result.second).c_str());
			Logger::WriteMessage(")\n");
		}

		TEST_METHOD(SectionUpdateTest) {
		
			Section::XJerk = "0.0";
			Section::YJerk = "0.0";
			Section::XAcc = "0.0";
			Section::YAcc = "0.0";
			
			// setting up global parameters
			genmath::LongDouble unit_time_step_test("0.1");
			try {
			
				Section::Init();
				//Section::UpdateGlobalResSpd(genmath::LongDouble("0.0"));
				Assert::Fail();
			}
			catch (std::exception err) {

				Assert::IsTrue(err.what() == std::string("At least one global section parameter is not defined (Section)."));
			}

			Section::XJerk = "8.0";// mm/s/s/s
			Section::YJerk = "8.0";// mm/s/s/s
			Section::XAcc = "500.0";// mm/s/s
			Section::YAcc = "500.0";// mm/s/s
			//Section::MaxResSpd = "11.7804";// 8mm/s(x), 8mm/s(y) // mm/s
			Section::Init();
			Section::UpdateGlobalResSpd(genmath::LongDouble("11.7804"));

			Logger::WriteMessage("Example resultant maximum speed: ");
			Logger::WriteMessage(std::string(Section::MaxResSpd).c_str());
			Logger::WriteMessage("mm/s.\nDuration to reach example resultant maximum speed: ");
			Logger::WriteMessage(std::string(Section::MaxResSpdTime).c_str());
			Logger::WriteMessage("s. \nLength has taken during reach of maximum resultant speed: ");
			Logger::WriteMessage(std::string(Section::MaxResSpdLen).c_str());
			Logger::WriteMessage("mm\n");

			GCodeCommand<T> start_point;
			start_point = "G0 X0 Y0 Z0";
			GCodeCommand<T> end_point;
			//end_point = "G1 X5.666714 Y0 Z0.0";
			end_point = "G1 X100 Y0 Z0.0";

			Section test_object_0;

			try {
			
				test_object_0.UpdateSection(&end_point, unit_time_step_test);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(
					err.what() == std::string("No start point defined. Ill initiated section (Section)."));
			}

			Section test_object_1(&start_point);

			try {
			
				test_object_1.UpdateSection(nullptr, unit_time_step_test);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(
					err.what() == std::string("Null end point parameter. Ill initiated section (Section)."));
			}

			test_object_1.UpdateSection(&end_point, unit_time_step_test);
			Logger::WriteMessage("\nComputed average speed of section using linearized average");
			Logger::WriteMessage("\nspeed with number of average speed sections: 1.\n");
			Logger::WriteMessage(std::string(test_object_1.avg_spd_).c_str());
			Logger::WriteMessage("mm/s.\nComputed execution duration of section: ");
			Logger::WriteMessage(std::string(test_object_1.end_time_).c_str());
			Logger::WriteMessage(std::string("s.\n").c_str());
		}

		TEST_METHOD(SetPrevSectTest) {
		
			genmath::LongDouble unit_time_step = "0.1";

			GCodeCommand<T> start_point;
			start_point = "G0 X0 Y0 Z0";
			GCodeCommand<T> end_point;
			end_point = "G0 X100 Y0 Z0";
			GCodeCommand<T> next_point;
			next_point = "G1 X200 Y10 Z0";

			Section first_sect(&start_point);
			first_sect.UpdateSection(&end_point, unit_time_step);
			Section second_sect(first_sect);
			second_sect.UpdateSection(&next_point, unit_time_step);
			second_sect.SetPrevSect(&first_sect);

			try {
			
				second_sect.SetPrevSect(nullptr);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Previous section parameter is null (Section)."));
			}
		}

		TEST_METHOD(SectionExecutionTimeModificationTest) {
		
			genmath::LongDouble unit_time_step_test("0.1");

			// setting up global parameters
			Section::XJerk = "8.0";// mm/s/s/s
			Section::YJerk = "8.0";// mm/s/s/s
			Section::XAcc = "500.0";// mm/s/s
			Section::YAcc = "500.0";// mm/s/s
			Section::MinResSpd = "0.1";
			//Section::MaxResSpd = "11.7804";// 8mm/s(x), 8mm/s(y) // mm/s
			Section::Init();
			Section::UpdateGlobalResSpd(genmath::LongDouble("11.7804"));

			GCodeCommand<T> start_point;
			start_point = "G0 X0 Y0 Z0";
			GCodeCommand<T> end_point;
			end_point = "G0 X100 Y0 Z0";
			Section test_object_0(&start_point);
			test_object_0.UpdateSection(&end_point, unit_time_step_test);
			Section test_object_1(test_object_0);
			
			genmath::LongDouble speed_scaling_factor = "0.5";
			test_object_0.avg_spd_ = "0.1";

			try {
			
				test_object_0.ConfigureSpeed(speed_scaling_factor);
				Assert::Fail();
			}
			catch (std::exception err){
				
				Assert::IsTrue(err.what() == std::string("Reached minimum execution speed (Section)."));
			}

			Logger::WriteMessage("Average speed before speed reconfiguration: ");
			Logger::WriteMessage(std::string(test_object_1.avg_spd_).c_str());
			speed_scaling_factor = "0.945";// ratio of current and recent speed value
			test_object_1.ConfigureSpeed(speed_scaling_factor);
			Logger::WriteMessage("mm/s\nAverage speed AFTER speed reconfiguration: ");
			Logger::WriteMessage(std::string(test_object_1.avg_spd_).c_str());
			Logger::WriteMessage("mm/s");
		}
	};
}
