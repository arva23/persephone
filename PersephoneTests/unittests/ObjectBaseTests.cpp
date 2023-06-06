#include "../pch.h"
#include "CppUnitTest.h"
#include <initializer_list>
#include <string>
#include "../../Persephone/genmath/ObjectBase.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	TEST_CLASS(ObjectBaseTests) {

		TEST_METHOD(ObjectResourceManagement) {
	
			// ObjectBase();
			genmath::ObjectBase test_object_0;

			// ObjectBase(const ObjectBase & orig);
			genmath::ObjectBase test_object_1(test_object_0);

			// virtual ~ObjectBase();
			// RAII
		}
	};
}
