#include "../pch.h"
#include "CppUnitTest.h"
#include <cstdlib>
#include <string>
#include "../../Persephone/genmath/LongDouble.h"
#include <initializer_list>
#include <iomanip>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	TEST_CLASS(LongDoubleTest) {

		TEST_METHOD(ObjectResourceManagement) {

			// genmath::LongDouble();
			genmath::LongDouble test_object_0 = genmath::LongDouble();


			// genmath::LongDouble(fix16_t data, bool from_int = false);
			genmath::LongDouble test_object_1(std::strtold("1.1111", NULL));
			genmath::LongDouble test_object_2(123456789);


			// genmath::LongDouble(std::string data);
			genmath::LongDouble test_object_3("");
			genmath::LongDouble test_object_4("2.2222");


			// genmath::LongDouble(const char* data);
			genmath::LongDouble test_object_5("3.3333");

			genmath::LongDouble test_object_6("-");
			genmath::LongDouble test_object_7("23-5");
			genmath::LongDouble test_object_8(".0");
			genmath::LongDouble test_object_9("test");
			Assert::IsTrue(test_object_6 == test_object_7);
			Assert::IsTrue(test_object_7 == test_object_8);
			Assert::IsTrue(test_object_8 == test_object_9);


			// genmath::LongDouble(const genmath::LongDouble& orig);
			genmath::LongDouble test_object_10(test_object_5);


			// virtual ~genmath::LongDouble();
			// RAII
		}

		TEST_METHOD(AssignmentOperators) {

			// bool operator=(std::string operand) override;
			genmath::LongDouble test_object_0("1.11111");


			// genmath::LongDouble& operator=(const genmath::LongDouble & operand);
			genmath::LongDouble test_object_1;
			test_object_1 = test_object_0;
			Assert::IsTrue(test_object_0 == test_object_1);


			// bool operator=(const char* operand);
			Assert::IsTrue(test_object_0 = "");
			Assert::IsFalse(test_object_0 = "0.0");
			Assert::IsFalse(test_object_0 = "3.3333");
			Assert::IsTrue(test_object_0 = "-");
			Assert::IsTrue(test_object_0 = "23-5");
			Assert::IsTrue(test_object_0 = ".0");
			Assert::IsTrue(test_object_0 = "test");
			Assert::IsFalse(test_object_0 = "4.44");
		}

		TEST_METHOD(LogicalOperators) {

			genmath::LongDouble operand_a("1.111");
			genmath::LongDouble operand_b("2.222");


			// bool operator==(const genmath::LongDouble & operand) const;
			Assert::IsFalse(operand_a == operand_b);


			// bool operator<=(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_a <= operand_a);


			// bool operator>=(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_b >= operand_b);


			// bool operator<(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_a < operand_b);


			// bool operator>(const genmath::LongDouble & operand) const;
			Assert::IsFalse(operand_a > operand_b);


			// bool operator!=(const genmath::LongDouble & operand)const;
			Assert::IsTrue(operand_a != operand_b);
		}

		TEST_METHOD(CompoundArithmerticUnaryOperators) {

			genmath::LongDouble operand_a("1.234");
			genmath::LongDouble operand_b("5.678");
			genmath::LongDouble operand_c("6.912");

			//void operator+=(const genmath::LongDouble & operand);
			genmath::LongDouble result = operand_a;
			result += operand_b;
			Assert::IsTrue(result == genmath::LongDouble(std::strtold("1.234", NULL) + std::strtold("5.678", NULL)));
			

			//void operator-=(const genmath::LongDouble & operand);
			result = operand_c;
			result -= operand_b;
			Assert::IsTrue(result == genmath::LongDouble(std::strtold("6.912", NULL) - std::strtold("5.678", NULL)));
			

			//void operator*=(const genmath::LongDouble & operand);
			result = operand_a;
			result *= operand_b;
			Assert::IsTrue(result == genmath::LongDouble(std::strtold("1.234", NULL) * std::strtold("5.678", NULL)));
			

			//void operator/=(const genmath::LongDouble & operand);
			result = operand_a * operand_b;
			result /= operand_b;
			Assert::IsTrue(result == genmath::LongDouble(
			std::strtold("1.234", NULL) * std::strtold("5.678", NULL) / std::strtold("5.678", NULL)));
			

			//void operator%=(const genmath::LongDouble & operand);
			result = operand_a * operand_b;
			result %= operand_a;
			Assert::IsTrue(result == genmath::LongDouble(
				std::fmodl(std::strtold("1.234", NULL) * std::strtold("5.678", NULL), std::strtold("1.234", NULL))));
			
		}

		TEST_METHOD(ArithmeticBinaryOperators) {

			genmath::LongDouble operand_a("1.234");
			genmath::LongDouble operand_b("5.678");
			genmath::LongDouble operand_c("6.912");
			genmath::LongDouble operand_d("7.006652");

			// genmath::LongDouble operator+(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_a + operand_b == genmath::LongDouble(
				std::strtold("1.234", NULL) + std::strtold("5.678", NULL)));


			// genmath::LongDouble operator-(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_c - operand_b == genmath::LongDouble(
				std::strtold("6.912", NULL) - std::strtold("5.678", NULL)));


			// genmath::LongDouble operator*(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_a * operand_b == genmath::LongDouble(
				std::strtold("1.234", NULL) * std::strtold("5.678", NULL)));


			// genmath::LongDouble operator/(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_d / operand_b == genmath::LongDouble(
				std::strtold("7.006652", NULL) / std::strtold("5.678", NULL)));


			// genmath::LongDouble operator%(const genmath::LongDouble & operand) const;
			Assert::IsTrue(operand_d % operand_a == genmath::LongDouble(
				std::fmodl(std::strtold("7.006652", NULL), std::strtold("1.234", NULL))));
		}

		TEST_METHOD(UnaryOperators) {
			std::setprecision(60);
			
			genmath::LongDouble operand("1.11");
			// genmath::LongDouble operator++();
			genmath::LongDouble test_object = operand++;
			Assert::IsTrue(test_object == genmath::LongDouble("1.11"));
			Assert::IsTrue(operand == genmath::LongDouble("2.11"));


			// genmath::LongDouble operator++(int);
			test_object = ++operand;
			Assert::IsTrue(test_object == genmath::LongDouble("3.11"));
			Assert::IsTrue(operand == genmath::LongDouble("3.11"));


			// genmath::LongDouble operator--();
			operand = "1.11";
			test_object = operand--;
			Assert::IsTrue(test_object == genmath::LongDouble("1.11"));
			Assert::IsTrue(operand == genmath::LongDouble("0.11"));


			// genmath::LongDouble operator--(int);
			test_object = --operand;
			Assert::IsTrue(test_object == genmath::LongDouble("-0.89"));
			Assert::IsTrue(operand == genmath::LongDouble("-0.89"));
		}

		TEST_METHOD(Conversions) {

			// int64_t ToInt64() const;
			int64_t test_object_0 = genmath::LongDouble("1.111").ToInt64();
			Assert::IsTrue(test_object_0 == 1);

			// operator long double() const;
			long double test_object_2 = genmath::LongDouble("2.222");
			Assert::IsTrue(test_object_2 == std::strtold("2.222", NULL));


			// operator std::string() override;
			std::string test_object_3 = std::string(genmath::LongDouble("1.0"));
			Assert::IsTrue(test_object_3 == "1.000000");
		}

		TEST_METHOD(AuxiliaryMethods) {

			// static void SetPrecisionThreshold(genmath::LongDouble & threshold);
			genmath::LongDouble precision("0.0001");
			genmath::LongDouble::SetPrecisionThreshold(precision);


			// static genmath::LongDouble Sqrt(const genmath::LongDouble & operand);
			genmath::LongDouble operand_a = (genmath::LongDouble::Sqrt(genmath::LongDouble("12.345")));
			genmath::LongDouble operand_b = genmath::LongDouble(std::sqrtl(std::strtold("12.345", NULL)));
			Assert::IsTrue(operand_a == operand_b);


			// static genmath::LongDouble RawRound(genmath::LongDouble value, genmath::LongDouble step);
			// floor rounding
			operand_a = genmath::LongDouble::RawRound(genmath::LongDouble("200.984367"), genmath::LongDouble("3.983"));
			operand_b =
				(std::fmodl(std::strtold("200.984367", NULL), std::strtold("3.983", NULL))
					< (std::strtold("3.983", NULL) / std::strtold("2.0", NULL)))
				? (std::strtold("200.984367", NULL) -
					std::fmodl(std::strtold("200.984367", NULL), std::strtold("3.983", NULL)))
				: ((std::strtold("200.984367", NULL) -
					std::fmodl(std::strtold("200.984367", NULL), std::strtold("3.983", NULL)))
					+ std::strtold("3.983", NULL));
			Assert::IsTrue(operand_a == operand_b);

			// ceil rounding
			operand_a = genmath::LongDouble::RawRound(genmath::LongDouble("201.994367"), genmath::LongDouble("3.983"));
			operand_b =
				(std::fmodl(std::strtold("201.994367", NULL), std::strtold("3.983", NULL))
					< (std::strtold("3.983", NULL) / std::strtold("2.0", NULL)))
				? (std::strtold("201.994367", NULL) -
					std::fmodl(std::strtold("201.994367", NULL), std::strtold("3.983", NULL)))
				: ((std::strtold("201.994367", NULL) -
					std::fmodl(std::strtold("201.994367", NULL), std::strtold("3.983", NULL)))
					+ std::strtold("3.983", NULL));
			Assert::IsTrue(operand_a == operand_b);


			// static genmath::LongDouble Abs(genmath::LongDouble operand);
			Assert::IsTrue(genmath::LongDouble(std::strtold("1.0", NULL)) == genmath::LongDouble::Abs(genmath::LongDouble("-1.0")));
		}
	};
}
