#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/include/libfixmath/fixmath.h"
#include "../../Persephone/genmath/FixedPoint.h"
#include <initializer_list>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	TEST_CLASS(FixedPointTest) {

		TEST_METHOD(ObjectResourceManagement) {

			// genmath::FixedPoint();
			genmath::FixedPoint test_object_0 = genmath::FixedPoint();
			

			// genmath::FixedPoint(fix16_t data, bool from_int = false);
			genmath::FixedPoint test_object_1(fix16_from_str("1.1111"));
			genmath::FixedPoint test_object_2(123456789, true);
			

			// genmath::FixedPoint(std::string data);// if data is empty, data_ will be 0.0
			genmath::FixedPoint test_object_3(std::string(""));
			genmath::FixedPoint test_object_4(std::string("2.2222"));
			

			// genmath::FixedPoint(const char* data);
			genmath::FixedPoint test_object_5("3.3333");
			

			// genmath::FixedPoint(const genmath::FixedPoint& orig);
			genmath::FixedPoint test_object_6(test_object_5);
			

			// virtual ~genmath::FixedPoint();
			// RAII
		}

		TEST_METHOD(AssignmentOperators) {

			// bool operator=(std::string operand) override;
			genmath::FixedPoint test_object_0(std::string("1.11111"));
			

			// genmath::FixedPoint& operator=(const genmath::FixedPoint & operand);
			genmath::FixedPoint test_object_1;
			test_object_1 = test_object_0;
			Assert::IsTrue(test_object_0 == test_object_1);
			

			// bool operator=(const char* operand);
			Assert::IsTrue(test_object_0 = std::string(""));
			Assert::IsTrue(test_object_0 = "");
			Assert::IsFalse(test_object_0 = "0.0");
			Assert::IsFalse(test_object_0 = "3.3333");
			Assert::IsTrue(test_object_0 = std::string("test"));
			Assert::IsFalse(test_object_0 = std::string("4.44"));
		}

		TEST_METHOD(LogicalOperators) {

			genmath::FixedPoint operand_a("1.111");
			genmath::FixedPoint operand_b("2.222");

			
			// bool operator==(const genmath::FixedPoint & operand) const;
			Assert::IsFalse(operand_a == operand_b);
			
			
			// bool operator<=(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_a <= operand_a);
			
			
			// bool operator>=(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_b >= operand_b);
			
			
			// bool operator<(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_a < operand_b);
			
			
			// bool operator>(const genmath::FixedPoint & operand) const;
			Assert::IsFalse(operand_a > operand_b);
			

			// bool operator!=(const genmath::FixedPoint & operand)const;
			Assert::IsTrue(operand_a != operand_b);
		}

		TEST_METHOD(CompoundArithmerticUnaryOperators) {

			genmath::FixedPoint operand_a("1.234");
			genmath::FixedPoint operand_b("5.678");
			genmath::FixedPoint operand_c("6.912");

			//void operator+=(const genmath::FixedPoint & operand);
			genmath::FixedPoint result = operand_a;
			result += operand_b;
			Assert::IsTrue(result == genmath::FixedPoint(fix16_add(fix16_from_str("1.234"), fix16_from_str("5.678"))));
			
			
			//void operator-=(const genmath::FixedPoint & operand);
			result = operand_c;
			result -= operand_b;
			Assert::IsTrue(result == genmath::FixedPoint(fix16_sub(fix16_from_str("6.912"), fix16_from_str("5.678"))));
			
			
			//void operator*=(const genmath::FixedPoint & operand);
			result = operand_a;
			result *= operand_b;
			Assert::IsTrue(result == genmath::FixedPoint(fix16_mul(fix16_from_str("1.234"), fix16_from_str("5.678"))));
			
			
			//void operator/=(const genmath::FixedPoint & operand);
			result = operand_a * operand_b;
			result /= operand_b;
			Assert::IsTrue(result == genmath::FixedPoint(
				fix16_div(fix16_mul(fix16_from_str("1.234"), fix16_from_str("5.678")), fix16_from_str("5.678"))));
			
			
			//void operator%=(const genmath::FixedPoint & operand);
			result = operand_a * operand_b;
			result %= operand_a;
			Assert::IsTrue(result == genmath::FixedPoint(
				fix16_mod(fix16_mul(fix16_from_str("1.234"), fix16_from_str("5.678")), fix16_from_str("1.234"))));
		}

		TEST_METHOD(ArithmeticBinaryOperators) {

			genmath::FixedPoint operand_a("1.234");
			genmath::FixedPoint operand_b("5.678");
			genmath::FixedPoint operand_c("6.912");
			genmath::FixedPoint operand_d("7.006652");
			
			// genmath::FixedPoint operator+(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_a + operand_b == genmath::FixedPoint(
				fix16_add(fix16_from_str("1.234"), fix16_from_str("5.678"))));
			
			
			// genmath::FixedPoint operator-(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_c - operand_b == genmath::FixedPoint(
				fix16_sub(fix16_from_str("6.912"), fix16_from_str("5.678"))));
			
			
			// genmath::FixedPoint operator*(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_a * operand_b == genmath::FixedPoint(
				fix16_mul(fix16_from_str("1.234"), fix16_from_str("5.678"))));
			
			
			// genmath::FixedPoint operator/(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_d / operand_b == genmath::FixedPoint(
				fix16_div(fix16_from_str("7.006652"), fix16_from_str("5.678"))));
			
			
			// genmath::FixedPoint operator%(const genmath::FixedPoint & operand) const;
			Assert::IsTrue(operand_d % operand_a == genmath::FixedPoint(
				fix16_mod(fix16_from_str("7.006652"), fix16_from_str("1.234"))));
		}

		TEST_METHOD(UnaryOperators) {

			genmath::FixedPoint operand("1.11");
			// genmath::FixedPoint operator++();
			genmath::FixedPoint test_object = operand++;
			Assert::IsTrue(test_object == genmath::FixedPoint("1.11"));
			Assert::IsTrue(operand == genmath::FixedPoint("2.11"));
			
			
			// genmath::FixedPoint operator++(int);
			test_object = ++operand;
			Assert::IsTrue(test_object == genmath::FixedPoint("3.11"));
			Assert::IsTrue(operand == genmath::FixedPoint("3.11"));
			
			
			// genmath::FixedPoint operator--();
			operand = "1.11";
			test_object = operand--;
			Assert::IsTrue(test_object == genmath::FixedPoint("1.11"));
			Assert::IsTrue(operand == genmath::FixedPoint("0.11"));
			
			
			// genmath::FixedPoint operator--(int);
			test_object = --operand;
			Assert::IsTrue(test_object == genmath::FixedPoint("-0.89"));
			Assert::IsTrue(operand == genmath::FixedPoint("-0.89"));
		}

		TEST_METHOD(Conversions) {

			// int64_t ToInt64() const;
			int64_t test_object_1 = genmath::FixedPoint("1.111").ToInt64();
			Assert::IsTrue(test_object_1 == 1);
			
			
			// operator fix16_t() const;
			fix16_t test_object_2 = genmath::FixedPoint("2.222");
			Assert::IsTrue(test_object_2 == fix16_from_str("2.222"));
			
			
			// operator std::string() override;
			std::string test_object_3 = std::string(genmath::FixedPoint("1.0"));
			Assert::IsTrue(test_object_3 == "1.00000");
		}

		TEST_METHOD(AuxiliaryMethods) {

			// static void SetPrecisionThreshold(genmath::FixedPoint & threshold);
			genmath::FixedPoint precision("0.0001");
			genmath::FixedPoint::SetPrecisionThreshold(precision);

			
			// static genmath::FixedPoint Sqrt(const genmath::FixedPoint & operand);
			genmath::FixedPoint operand_a = (genmath::FixedPoint::Sqrt(genmath::FixedPoint("12.345")));
			genmath::FixedPoint operand_b = genmath::FixedPoint(fix16_sqrt(fix16_from_str("12.345")));
			Assert::IsTrue(operand_a == operand_b);
			
			
			// static genmath::FixedPoint RawRound(genmath::FixedPoint value, genmath::FixedPoint step);
			// floor rounding
			operand_a = genmath::FixedPoint::RawRound(genmath::FixedPoint("200.984367"), genmath::FixedPoint("3.983"));
			operand_b =
				(fix16_mod(fix16_from_str("200.984367"), fix16_from_str("3.983"))
					< fix16_div(fix16_from_str("3.983"), fix16_from_str("2.0")))
				? fix16_sub(fix16_from_str("200.984367"),
					fix16_mod(fix16_from_str("200.984367"), fix16_from_str("3.983")))
				: fix16_add(fix16_sub(fix16_from_str("200.984367"),
					fix16_mod(fix16_from_str("200.984367"), fix16_from_str("3.983"))), fix16_from_str("3.983"));
			Assert::IsTrue(operand_a == operand_b);

			// ceil rounding
			operand_a = genmath::FixedPoint::RawRound(genmath::FixedPoint("201.994367"), genmath::FixedPoint("3.983"));
			operand_b =
				(fix16_mod(fix16_from_str("201.994367"), fix16_from_str("3.983"))
				< fix16_div(fix16_from_str("3.983"), fix16_from_str("2.0")))
				? fix16_sub(fix16_from_str("201.994367"),
					fix16_mod(fix16_from_str("201.994367"), fix16_from_str("3.983")))
				: fix16_add(fix16_sub(fix16_from_str("201.994367"),
					fix16_mod(fix16_from_str("201.994367"), fix16_from_str("3.983"))), fix16_from_str("3.983"));
			Assert::IsTrue(operand_a == operand_b);
			

			// static genmath::FixedPoint Abs(genmath::FixedPoint operand);
			Assert::IsTrue(genmath::FixedPoint(fix16_from_str("1.0")) == genmath::FixedPoint::Abs(genmath::FixedPoint("-1.0")));
		}
	};
}