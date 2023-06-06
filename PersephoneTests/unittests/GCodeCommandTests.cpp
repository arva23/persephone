#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/printerheatconduction/GCodeCommand.h"
#include <initializer_list>
#include <string>
#include <exception>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	using T = genmath::LongDouble;

	//CmdModels CommandModels = CmdModels();
	
	TEST_CLASS(GCodeCommandsTests) {

		TEST_METHOD(DataConsistency) {

			//GCodeCommand();
			GCodeCommand<T> test_object_0;

			
			//GCodeCommand(std::string name, std::map<std::string, T> params);
			GCodeCommand<T> test_object_1("G0", std::map<std::string, T>{ { "X", T("1.0") }});
		
			Assert::IsTrue(test_object_1.GetName() == "G0");
			Assert::IsTrue(test_object_1.Get("X") == T("1.0"));
		
			try {

				GCodeCommand<T> test_object_0 = GCodeCommand<T>(
					"XYZ0", std::map<std::string, T>{ { "X", T("1.0") }});
				Assert::Fail();
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("No such command (GCodeCommand).")); 
			}
				

			try {
				GCodeCommand<T> test_object_0("G1", std::map<std::string, T>{ { "K", T("2.0") }});
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Parameter name is not valid (GCodeCommand)."));
			}

			try {
			
				GCodeCommand<T> test_object_0("M84", std::map<std::string, T>{ {"X", T("1.0") }});
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Wrong value format. Command has valueless "
					"parameters (GCodeCommand)."));
			}

			try {
				GCodeCommand<T> test_object_0("G0", std::map<std::string, T>{});
				Assert::Fail();
			}
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string(
			
				"Insufficient number of parameters (GCodeCommand)."));
			}

			
			//GCodeCommand(std::string raw_data);
			// uses bool operator=(std::string);

			//GCodeCommand(const GCodeCommand<T>& orig);
			GCodeCommand<T> test_object_2(test_object_1);
			Assert::IsTrue(test_object_1.GetName() == test_object_2.GetName());
			Assert::IsTrue(test_object_1.Get("X") == test_object_2.Get("X"));
		
			
			//~GCodeCommand();
			// RAII
		
			
			//GCodeCommand<T>& operator=(const GCodeCommand<T>& orig);
			GCodeCommand<T> test_object_3;
			test_object_3 = test_object_1;
			Assert::IsTrue(test_object_3.GetName() == "G0");
			Assert::IsTrue(test_object_3.Get("X") == T("1.0"));
		
			
			//bool operator=(std::string raw_data) override;
			GCodeCommand<T> test_object_4;
			test_object_4 = "G0 X1.0";
			Assert::IsTrue(test_object_4.GetName() == "G0");
			Assert::IsTrue(test_object_4.Get("X") == T("1.0"));
			
			test_object_4 = "G1 X1.0 Y2.2345 E8.435";
			Assert::IsTrue(test_object_4.GetName() == "G1");
			Assert::IsTrue(test_object_4.Get("X") == T("1.0"));
			Assert::IsTrue(test_object_4.Get("Y") == T("2.2345"));
			Assert::IsTrue(test_object_4.Get("E") == T("8.435"));

			test_object_4 = "G0 X1.0 Y2.2345 ;TEST COMMENT";
			Assert::IsTrue(test_object_4.GetName() == "G0");
			Assert::IsTrue(test_object_4.Get("X") == T("1.0"));
			Assert::IsTrue(test_object_4.Get("Y") == T("2.2345"));
			
			Assert::IsTrue(test_object_0 = "XYZ0 X1.0");
			Assert::IsTrue(test_object_0 = "G1 K2.0");
			Assert::IsTrue(test_object_0 = "G0");
			test_object_4 = ";TEST COMMENT";
		}
	
		TEST_METHOD(DataAccess) {
		
			//operator std::string() const override;
			GCodeCommand<T> test_object_0("G1", std::map<std::string, T>{ { "Y", T("9.0") }});
			Assert::IsTrue(std::string(test_object_0) == "G1 Y" + std::string(T("9.0")));
		
			
			//std::string& GetName();
			Assert::IsTrue(test_object_0.GetName() == "G1");
		
			
			//bool IsSet(std::string key);
			Assert::IsTrue(test_object_0.IsSet("Y"));
			Assert::IsFalse(test_object_0.IsSet("Z"));
		
			
			//T& Get(std::string key);
			Assert::IsTrue(test_object_0.Get("Y") == T("9.0"));
			
			try {
				test_object_0.Get("X");
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Requested parameter is not set or "
					"inaccessable (GCodeCommand)."));
			}
		
			GCodeCommand<T> test_object_1("M84 X Y");

			try {
			
				test_object_1.Get("X");
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Command contains no parameters. "
					"It is valueless (GCodeCommand)."));
			}
			
			//bool Set(std::string key, T value);// true if key not found, then new inserted
			test_object_0.Set("Y", T("3.0"));

			try {
				test_object_0.Set("U", T("1.0"));
				Assert::Fail();
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Requested parameter can not be set or "
					"inaccessable (GCodeCommand)."));
			}
		
			try {
			
				test_object_1.Set("X", T("0.0"));
				Assert::Fail();

			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Command contains valueless parameters (GCodeCommand)."));
			}
		}
	};
}
