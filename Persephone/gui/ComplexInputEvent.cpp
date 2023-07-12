#include "ComplexInputEvent.h"

uint32_t gui::GenericComplexInputEvent::DefaultRegistrationInterval = 100;

gui::GenericComplexInputEvent::GenericComplexInputEvent() {


}

gui::GenericComplexInputEvent::GenericComplexInputEvent(const GenericComplexInputEvent& orig) {

}

gui::GenericComplexInputEvent::~GenericComplexInputEvent() {


}

void gui::GenericComplexInputEvent::operator()(const SDL_Event& sdl_event) {


}

bool gui::GenericComplexInputEvent::operator==(const int64_t* recent_event_time_stamp) {

	return false;
}

gui::GenericComplexInputEvent& gui::GenericComplexInputEvent::operator=(const GenericComplexInputEvent& orig) {

	return *this;
}

uint8_t gui::GenericComplexInputEvent::GetNumberOfEvents() {

	return 0;
}



template <class HANDLER>
gui::ComplexInputEvent<HANDLER>::ComplexInputEvent() : GenericComplexInputEvent() {

	object_pointer_ = nullptr;
	handler_member_function_ = nullptr;
	number_of_events_ = 0;
	event_type_and_order_ = new SimpleEvent[1]{ CEV_EMPTY };
	// default registration event
	registration_interval_ = gui::GenericComplexInputEvent::DefaultRegistrationInterval;
	last_call_ = 0;
	call_cooldown_ = 0;
}

template <class HANDLER>
gui::ComplexInputEvent<HANDLER>::ComplexInputEvent(HANDLER* object_pointer,
	void(HANDLER::*handler_member_function)(const SDL_Event&), uint8_t number_of_events,
	SimpleEvent* event_type_and_order, uint32_t registration_interval, uint32_t call_cooldown) : GenericComplexInputEvent() {

	if (object_pointer == nullptr)
		throw std::exception("No referenced object (ComplexInputEvent).");

	object_pointer_ = object_pointer;

	if (handler_member_function == nullptr)
		throw std::exception("No referenced handler member function (ComplexInputEvent).");

	handler_member_function_ = handler_member_function;

	if (!number_of_events)
		throw std::exception("Zero number of events (ComplexInputEvent).");

	number_of_events_ = number_of_events;

	event_type_and_order_ = event_type_and_order;
	registration_interval_ = registration_interval;
	last_call_ = 0;
	call_cooldown_ = call_cooldown;
}

template <class HANDLER>
gui::ComplexInputEvent<HANDLER>::ComplexInputEvent(const ComplexInputEvent<HANDLER>& orig) : GenericComplexInputEvent(orig) {

	object_pointer_ = orig.object_pointer_;
	handler_member_function_ = orig.handler_member_function_;
	number_of_events_ = orig.number_of_events_;
	event_type_and_order_ = orig.event_type_and_order_;
	registration_interval_ = orig.registration_interval_;
	last_call_ = orig.last_call_;
	call_cooldown_ = orig.call_cooldown_;
}

template <class HANDLER>
gui::ComplexInputEvent<HANDLER>& gui::ComplexInputEvent<HANDLER>::operator=(const ComplexInputEvent<HANDLER>& orig) {

	gui::GenericComplexInputEvent::operator=(*this);

	object_pointer_ = orig.object_pointer_;
	handler_member_function_ = orig.handler_member_function_;
	number_of_events_ = orig.number_of_events_;
	event_type_and_order_ = orig.event_type_and_order_;
	registration_interval_ = orig.registration_interval_;
	last_call_ = orig.last_call_;
	call_cooldown_ = orig.call_cooldown_;
	
	return *this;
}

template <class HANDLER>
gui::ComplexInputEvent<HANDLER>::~ComplexInputEvent() {

	//delete object_pointer_;// RAII declared in upper system level
}

template <class HANDLER>
void gui::ComplexInputEvent<HANDLER>::operator()(const SDL_Event& sdl_event) {
	
	(object_pointer_->*handler_member_function_)(sdl_event);
}

// event timestamps
//  0 - keyboard keydown event - should be improved to active key symbol filtering
//  1 - keyboard keyup event - should be improved to active key symbol filtering
//  2 - mouse button down event
//  3 - mouse button up event
//  4 - mouse wheel event
//  5 - mousemotion event
//  6 - window event close
//  7 - window event resized
template <class HANDLER>
bool gui::ComplexInputEvent<HANDLER>::operator==(const int64_t* rec_ev_tstamps) {

	// examining the order of input events (chronological consistency)
	int64_t curr_call_tstamp = 0;
	curr_call_tstamp = rec_ev_tstamps[event_type_and_order_[number_of_events_ - 1]];
	
	bool chrono_consist = curr_call_tstamp - last_call_ >= call_cooldown_;

	if (!chrono_consist) [[unlikely]] return false;

	for (size_t i = 0; i < number_of_events_ - 1 && chrono_consist; ++i) {
		
		chrono_consist = chrono_consist
			&& rec_ev_tstamps[event_type_and_order_[i]] < rec_ev_tstamps[event_type_and_order_[i + 1]];
	}

	if (chrono_consist) {
	
		last_call_ = curr_call_tstamp;
		return true;
	}
	
	return false;
}

template <class HANDLER>
uint8_t gui::ComplexInputEvent<HANDLER>::GetNumberOfEvents() {

	return number_of_events_;
}
