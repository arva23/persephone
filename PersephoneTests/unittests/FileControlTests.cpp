#include "../pch.h"
#include "CppUnitTest.h"
#include "../../Persephone/genmath/LongDouble.h"
#include "../../Persephone/locresctl/FileControl.h"
#include <initializer_list>
#include <string>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{
	// NOT ONLY UNIT TESTS, BUT FUNCTIONAL TESTS (SEMANTICAL)
	TEST_CLASS(FileControlTests)
	{
		TEST_METHOD(FileAccess) {
			
			ConsoleUI program_console;

			// FileControl();
			FileControl test_object_0;


			// FileControl(std::string file_source, uint8_t mode = 0, bool skip_empty_lines = true);
			FileControl test_object_1(program_console, "FileControlTestFileEmpty.txt", FileControl::RW, true);
			test_object_1.ChangeSource("FileControlTestFileEmpty.txt", FileControl::RW, true);
			test_object_1.ChangeSource("FileControlTestFileEmpty.txt", FileControl::RWT, true);
			test_object_1.ChangeSource("FileControlTestFileEmpty.txt", FileControl::R, true);
			test_object_1.ChangeSource("FileControlTestFileEmpty.txt", FileControl::W, true);
			test_object_1.ChangeSource("FileControlTestFileEmpty.txt", FileControl::WT, true);


			// creates new file or uses an existing file in case of RWA,
			//  except if does not have the right to write
			try {
				
				FileControl test_object_0(program_console, "FileControlTestFileEmptyUnauth.txt", 
					FileControl::RW, true);
				
				Assert::Fail();
			}
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string(
				"Error at file open (r-w-app) (FileControl).")); }
			
			// creates new file or uses an existing file in case of RWT,
			//  except if it does not have the right to write
			try {
				
				FileControl test_object_0(program_console, "FileControlTestFileEmptyUNAUTHACC.txt",
					FileControl::RWT, true);
				
				Assert::Fail();
			}
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string(
				"Error at file open (r-w-trunc) (FileControl).")); }

			// reads from existing file
			//  except if it does not have the right to read or file does not exist
			try {
				
				FileControl test_object_0(program_console, "FileControlTestFileREADNONEXISTENCE.txt",
					FileControl::R, true);
				
				Assert::Fail();
			}
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string(
				"Error at file open (r) (FileControl).")); }

			// creates a new file or uses an existing file in case of WA,
			//  except if it does not have the right to write
			try {
				
				FileControl test_object_0(program_console, "FileControlTestFileEmptyUNAUTH.txt",
					FileControl::W, true);
				
				Assert::Fail();
			}
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string(
				"Error at file open (w-app) (FileControl).")); }

			// creates a new file or uses an existing file in case of WT,
			// except if it does not have the right to write
			try {
				
				FileControl test_object_0(program_console, "FileControlTestFileEmptyUnauth.txt", 
					FileControl::WT, true);
				
				Assert::Fail();
			}
			catch (std::exception err) { Assert::IsTrue(err.what() == std::string(
				"Error at file open (w-trunc) (FileControl).")); }

			// unidentified error at file access
			try { 
				
				FileControl test_object_0(program_console, "FileControlTestFileEmpty.txt", 5, true);
				Assert::Fail(); 
			}
			catch (std::exception err) {
				
				Assert::IsTrue(err.what() == std::string("Error at file open (unidentified opening mode) (FileControl).")); 
			}
			
			// ~FileControl();
			// RAII

		}

		TEST_METHOD(FileOperations) {

			ConsoleUI program_console;
			genmath::LongDouble object_0;
			genmath::LongDouble object_1;

			FileControl test_object_0(program_console, "FileControlTestFileMultipleLines.txt", FileControl::R, true);
			// bool ReadLineUndefined(genmath::ObjectBase& object);
			test_object_0.ReadLineUndefined(&object_0);
			test_object_0.ReadLineUndefined(&object_1);
			Assert::IsTrue(object_0 == genmath::LongDouble("1.0"));
			Assert::IsTrue(object_1 == genmath::LongDouble("123.456789"));
			Assert::IsTrue(test_object_0.ReadLineUndefined(&object_0));
			test_object_0.ChangeSource("FileControlTestFileSingleLineConversionError.txt", FileControl::R, true);
			try { 
				
				test_object_0.ReadLineUndefined(&object_0); Assert::Fail();
			}
			catch (std::exception err) {

				Assert::IsTrue(err.what() == std::string("Conversion error at data assignment to object (FileControl)."));
			}

			
			// void ReadLinesDefined(std::vector<genmath::ObjectBase&> objects);
			genmath::LongDouble object_2;
			genmath::LongDouble object_3;
			std::vector<genmath::ObjectBase*> objects;
			objects.push_back(&object_0);
			objects.push_back(&object_1);
			objects.push_back(&object_2);
			objects.push_back(&object_3);
			test_object_0.ChangeSource("FileControlTestFileMultipleLines1.txt", FileControl::RW, true);
			test_object_0.ReadLinesDefined(objects);
			Assert::IsTrue(object_0 == genmath::LongDouble("1.2"));
			Assert::IsTrue(object_1 == genmath::LongDouble("3.4"));
			Assert::IsTrue(object_2 == genmath::LongDouble("5.6"));
			Assert::IsTrue(object_3 == genmath::LongDouble("7.8"));

			test_object_0.ChangeSource("FileControlTestFileMultipleLinesConversionError.txt", FileControl::RW, true);
			try { 
				
				test_object_0.ReadLinesDefined(objects);
				Assert::Fail();
			}
			catch (std::exception err) {

				Assert::IsTrue(err.what() == std::string("Conversion error at data assignment to object (FileControl)."));
			}

			test_object_0.ChangeSource("FileControlTestFileSingleLine.txt", FileControl::RW, true);
			try { 
				
				test_object_0.ReadLinesDefined(objects); 
				Assert::Fail(); 
			}
			catch (std::exception err) {

				Assert::IsTrue("Reached end of file (eof() in definite multiple read).");
			}

			
			// void WriteLineUndefined(genmath::ObjectBase& object);
			test_object_0.ChangeSource("FileControlTestFileMultipleLines.txt", FileControl::R, true);
			try { 
				
				test_object_0.WriteLineUndefined(&object_0);
				Assert::Fail();
			}
			catch (std::exception err) { 
				
				Assert::IsTrue(err.what() == std::string("Unauthorized file operation (FileControl).")); 
			}

			test_object_0.ChangeSource("FileControlTestFileMultipleLinesWrite.txt", FileControl::RW, true);
			test_object_0.WriteLineUndefined(&object_0);
			genmath::LongDouble object_4;
			// file seek to the beginning of the file
			test_object_0.SeekLine(0);
			test_object_0.ReadLineUndefined(&object_4);
			Assert::IsTrue(object_0 == object_4);
			
			// void WriteLinesDefined(std::vector<genmath::ObjectBase&> objects);
			test_object_0.WriteLinesDefined(objects);
			// inaccessable resource
			test_object_0.ChangeSource("FileControlTestFileMultipleLines.txt", FileControl::R, true);
			try { 
				
				test_object_0.WriteLinesDefined(objects);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("Unauthorized file operation (FileControl).")); 
			}
		}
	};
}
