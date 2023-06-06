#pragma once
#include "../include/SDL.h"
#include <stdint.h>
#include <exception>
class GenericWindowRenderer;
class GenericPanel;
class ModelVisualizer;
class EventHandlerTestClass;

// complex event (CEV) timestamps
//  0 - keyboard keydown event
//  1 - keyboard keyup event
//  2 - mouse button down event
//  3 - mouse button up event
//  4 - mouse wheel event
//  5 - mousemotion event
//  6 - window event close
//  7 - window event resized
typedef enum {

	CEV_EMPTY = 99,
	CEV_KEYBOARDKEYDOWN = 0,
	CEV_KEYBOARDKEYUP = 1,
	CEV_MOUSEBUTTONDOWN = 2,
	CEV_MOUSEBUTTONUP = 3,
	CEV_MOUSWHEEL = 4,
	CEV_MOUSEMOTION = 5,
	CEV_WINDOWCLOSE = 6,
	CEV_WINDOWSIZECHANGED = 7
} SimpleEvent;

struct GenericComplexInputEvent {

	GenericComplexInputEvent();
	GenericComplexInputEvent(const GenericComplexInputEvent& orig);
	virtual ~GenericComplexInputEvent();

	virtual void operator()(const SDL_Event&);
	virtual bool operator==(const int64_t* recent_event_time_stamp);
	GenericComplexInputEvent& operator=(const GenericComplexInputEvent& orig);
	static uint32_t DefaultRegistrationInterval;
	virtual uint8_t GetNumberOfEvents();
};


// specifying panel (visual items) events by certain input event conditions
// only manages one keystroke at a time from keyboard events (improvement is required)
template <class HANDLER>
struct ComplexInputEvent : public GenericComplexInputEvent {
		
	ComplexInputEvent();
	
	/// <summary>
	/// Complex input management
	/// </summary>
	/// <param name="object_pointer">pointer to the called object, acting on it</param>
	/// <param name="handler_member_function">called member function to activate complex event</param>
	/// <param name="number_of_events">the manually defined number of events (array length, see next)</param>
	/// <param name="event_type_and_order">type of input events and their indexed enum names
	/// to be triggered</param>
	/// <param name="registration_interval">interval of simple events where they must be occurred
	///  to activate the complex event</param>
	/// <param name="call_cooldown">amount of time to elapse after last call, activation</param>
	ComplexInputEvent(HANDLER* object_pointer, void(HANDLER::*handler_member_function)(const SDL_Event&),
		uint8_t number_of_events, SimpleEvent* event_type_and_order, uint32_t registration_interval,
		uint32_t call_cooldown);

	ComplexInputEvent(const ComplexInputEvent<HANDLER>& orig);

	ComplexInputEvent<HANDLER>& operator=(const ComplexInputEvent<HANDLER>& orig);

	~ComplexInputEvent();
	/// <summary>
	/// member function call for activation
	/// </summary>
	/// <param name="sdl_event">the input event data</param>
	void operator()(const SDL_Event& sdl_event) override;

	/// <summary>
	/// comlex input event comparison with the recent general simple input event presences
	/// </summary>
	/// <param name="recent_event_timestamps">recent simple input event timestamps in fixed order</param>
	/// <returns></returns>
	bool operator==(const int64_t* recent_event_timestamps) override;

	uint8_t GetNumberOfEvents() override;

	HANDLER* object_pointer_;
	void(HANDLER::*handler_member_function_)(const SDL_Event&);
	
	uint8_t number_of_events_;
	SimpleEvent* event_type_and_order_;
	// Interval for the simple event composition, they must be in the time interval
	//  to activate the connected event handler
	uint32_t registration_interval_;
	uint32_t last_call_;// timestamp for the last call
	uint32_t call_cooldown_;// certain delay must be elapsed after the last handler call
};

template struct ComplexInputEvent<GenericPanel>;
template struct ComplexInputEvent<GenericWindowRenderer>;
template struct ComplexInputEvent<ModelVisualizer>;
template struct ComplexInputEvent<EventHandlerTestClass>;