#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/genmath/String.h"
#include <initializer_list>
#include <string>
#include <stdio.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	TEST_CLASS(StringTests) {

		TEST_METHOD(ObjectResourceManagement) {
		
			// genmath::String();
			genmath::String test_object_0;

			// genmath::String(std::string data);
			genmath::String test_object_1("");
			genmath::String test_object_2("test");

			// genmath::String(const genmath::String & orig);
			genmath::String test_object_3(test_object_2);
			Assert::IsTrue(std::string(test_object_2) == std::string(test_object_3));

			// ~genmath::String();
			// RAII


			// bool Empty();
			Assert::IsTrue(test_object_1.Empty());
			Assert::IsFalse(test_object_2.Empty());
		}
	
		TEST_METHOD(AssignmentOperators) {
		
			// bool operator=(std::string data)override;
			genmath::String test_object_0;
			Assert::IsFalse(test_object_0 = std::string("test string"));
			Assert::IsTrue(std::string(test_object_0) == "test string");

			// genmath::String& operator=(genmath::String & orig);
			genmath::String test_object_1;
			test_object_1 = test_object_0;
			Assert::IsTrue(std::string(test_object_0) == std::string(test_object_1));
		}

		TEST_METHOD(Conversions) {
		
			// operator std::string() const override;
			genmath::String test_object_0;
			test_object_0 = std::string("test string");
			Assert::IsTrue(std::string(test_object_0) == "test string");
		}
	};
}
