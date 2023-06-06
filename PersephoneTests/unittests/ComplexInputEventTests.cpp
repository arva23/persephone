#include "pch.h"
#include "CppUnitTest.h"
#include <initializer_list>
#include "../../Persephone/gui/ComplexInputEvent.h"
#include <exception>
#include <string>

class EventHandlerTestClass {
public:
	EventHandlerTestClass() {}
	EventHandlerTestClass(const EventHandlerTestClass& orig) {}
	~EventHandlerTestClass() {}

	static inline bool HandlerCalled = false;

	void EventHandler(const SDL_Event& sdl_event) {
			
		HandlerCalled = true;
	}
};

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace PrinterOptimizerTests
{

	TEST_CLASS(GenericComplexInputEventTests) {

		TEST_METHOD(ObjectResourceManagement) {

			//GenericComplexInputEvent();
			GenericComplexInputEvent test_object_0;
			
			
			//GenericComplexInputEvent(const GenericComplexInputEvent & orig);
			GenericComplexInputEvent test_object_1(test_object_0);
			
			
			//virtual ~GenericComplexInputEvent();
			// RAII
		
		};

		TEST_METHOD(EventComparison) {
		
			//virtual bool operator==(const uint32_t * recent_event_time_stamp);
			GenericComplexInputEvent test_object_0;
			int64_t* test_params = new int64_t[]{ 0, 1, 2, 3 };
			Assert::IsFalse(test_object_0 == test_params);
			
		}

		TEST_METHOD(EventHandlerCall) {
		
			//virtual void operator()(const SDL_Event&);
			GenericComplexInputEvent test_object_0;
			SDL_Event test_param;
			test_object_0(test_param);
		}

	};

	TEST_CLASS(ComplexInputEventTests) {

		TEST_METHOD(ObjectResourceManagement) {
		
			//ComplexInputEvent();
			ComplexInputEvent<EventHandlerTestClass> test_object_0;

			
			//ComplexInputEvent(HANDLER * object_pointer,
			//  void(HANDLER:: * handler_member_function)(const SDL_Event&),
			//  uint8_t number_of_events, SimpleEvent * event_type_and_order, uint32_t registration_interval,
			//  uint32_t call_cooldown);
			EventHandlerTestClass handler_object_1;
			ComplexInputEvent<EventHandlerTestClass> test_object_1 =
				ComplexInputEvent<EventHandlerTestClass>(
					&handler_object_1, &EventHandlerTestClass::EventHandler,
					1, new SimpleEvent[]{ CEV_KEYBOARDKEYDOWN }, 10, 10);

			try {
			
				ComplexInputEvent<EventHandlerTestClass> test_object_1 =
					ComplexInputEvent<EventHandlerTestClass>(
						nullptr, &EventHandlerTestClass::EventHandler,
						1, new SimpleEvent[]{ CEV_MOUSEBUTTONDOWN }, 10, 10);
				Assert::Fail();
			}
			catch (std::exception err) {
			
				Assert::IsTrue(err.what() == std::string("No referenced object (ComplexInputEvent)."));
			}
			
			try {

				ComplexInputEvent<EventHandlerTestClass> test_object_1 =
					ComplexInputEvent<EventHandlerTestClass>(
						&handler_object_1, nullptr, 1, new SimpleEvent[]{ CEV_MOUSEBUTTONUP }, 10, 10);
				Assert::Fail();
			}
			catch (std::exception err) {

				Assert::IsTrue(err.what() == std::string("No referenced handler member function (ComplexInputEvent)."));
			}
			
			try {

				ComplexInputEvent<EventHandlerTestClass> test_object_2 =
					ComplexInputEvent<EventHandlerTestClass>(
						&handler_object_1, &EventHandlerTestClass::EventHandler,
						0, new SimpleEvent[]{ CEV_KEYBOARDKEYDOWN }, 10, 10);
				Assert::Fail();
			}
			catch (std::exception err) {

				Assert::IsTrue(err.what() == std::string("Zero number of events (ComplexInputEvent)."));
			}


			//ComplexInputEvent(const ComplexInputEvent<HANDLER>&orig);
			ComplexInputEvent<EventHandlerTestClass> test_object_3(test_object_1);
			

			//ComplexInputEvent<HANDLER>& operator=(const ComplexInputEvent<HANDLER>&orig);
			ComplexInputEvent<EventHandlerTestClass> test_object_5 = test_object_1;
			
			//~ComplexInputEvent();
			// RAII
		}

		TEST_METHOD(EventComparison) {
		
			//bool operator==(const uint32_t * recent_event_timestamps) override;
			EventHandlerTestClass handler_object_0;
			ComplexInputEvent<EventHandlerTestClass> test_object_0 =
				ComplexInputEvent<EventHandlerTestClass>(
					&handler_object_0, &EventHandlerTestClass::EventHandler,
					2, new SimpleEvent[]{ CEV_KEYBOARDKEYUP, CEV_KEYBOARDKEYDOWN}, 100, 10);
			
			Assert::IsTrue(test_object_0 == new int64_t[]{ 20, 0, 0, 0, 0, 0, 0, 0 });
			Assert::IsFalse(test_object_0 == new int64_t[]{ 0, 0, 0, 0, 0, 0, 0, 0 });
			Assert::IsTrue(test_object_0 == new int64_t[]{ 100, 0, 0, 0, 0, 0, 0, 0 });
		}

		TEST_METHOD(EventHandlerCalls) {
		
			//void operator()(const SDL_Event & sdl_event) override;
			EventHandlerTestClass handler_object_0;
			ComplexInputEvent<EventHandlerTestClass> test_object_0 =
				ComplexInputEvent<EventHandlerTestClass>(
					&handler_object_0, &EventHandlerTestClass::EventHandler,
					4, new SimpleEvent[]{CEV_KEYBOARDKEYUP, CEV_KEYBOARDKEYDOWN, CEV_MOUSWHEEL},
					10, 10);

			test_object_0(SDL_Event());

			Assert::IsTrue(EventHandlerTestClass::HandlerCalled);
		}
	};
}