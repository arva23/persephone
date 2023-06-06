#include "InputControl.h"

InputControl::InputControl() {

	suspend_input_monitoring_ = false;
	size_of_registered_complex_events_ = 0;
	control_thread_started_ = false;
}

InputControl::InputControl(std::vector<GenericComplexInputEvent*>& object_complex_event_subscriptions) {

	suspend_input_monitoring_ = false;
	size_of_registered_complex_events_ = object_complex_event_subscriptions.size();
	object_complex_event_subscriptions_ = object_complex_event_subscriptions;
	control_thread_started_ = false;
}

InputControl::InputControl(const InputControl& orig) {

	size_of_registered_complex_events_ = orig.size_of_registered_complex_events_;
	object_complex_event_subscriptions_ = orig.object_complex_event_subscriptions_;
	control_thread_started_ = orig.control_thread_started_;
}

InputControl::~InputControl() {
	
	if (control_thread_started_ && !GenericWindowRenderer::ActiveRendererObjectCount)
		control_thread_.join();
}

void InputControl::Run() {

	// launching global input control subsystem
	control_thread_ = std::thread(&InputControl::MonitorInputEvent, this);
	control_thread_started_ = true;
}

InputControl& InputControl::operator=(const InputControl& orig) {

	size_of_registered_complex_events_ = orig.size_of_registered_complex_events_;
	object_complex_event_subscriptions_ = orig.object_complex_event_subscriptions_;
	control_thread_started_ = orig.control_thread_started_;
	
	return *this;
}

// registration occurs via GenericWindowRenderer object
void InputControl::RegisterGenericObjectComplexEvent(GenericComplexInputEvent* complex_event) {

	std::scoped_lock<std::mutex> insert_lock(complex_event_mutex_);

	object_complex_event_subscriptions_.push_back(complex_event);
	++size_of_registered_complex_events_;
}

void InputControl::UnregisterGenericObjectComplexEvent(GenericComplexInputEvent* complex_event) {


	std::scoped_lock<std::mutex> erase_lock(complex_event_mutex_);
	
	for (size_t i = 0; i < size_of_registered_complex_events_; ++i) {
	
		if (object_complex_event_subscriptions_[i] == complex_event) {
		
			object_complex_event_subscriptions_.erase(object_complex_event_subscriptions_.begin() + i);
			--size_of_registered_complex_events_;
		}
	}
}

void InputControl::MonitorInputEvent() {

	// waiting for GenericWindowRenderer object instances
	while (!GenericWindowRenderer::ActiveRendererObjectCount) {
		std::this_thread::yield(); }

	SDL_Event recent_input_event;
	SDL_KeyboardEvent recent_keyboard_event;
	SDL_MouseButtonEvent recent_mouse_button_event;
	SDL_MouseWheelEvent recent_mouse_wheel_event;
	SDL_MouseMotionEvent recent_mouse_mostion_event;
	SDL_WindowEvent recent_window_event;
	// event timestamps
	//  0 - keyboard keydown event
	//  1 - keyboard keyup event
	//  2 - mouse button down event
	//  3 - mouse button up event
	//  4 - mouse wheel event
    //  5 - mousemotion event
    //  6 - window event close
	//  7 - window event resized
	int64_t recent_event_timestamps[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };// fixed order of event types
	size_t object_ind = 0;

	while (GenericWindowRenderer::ActiveRendererObjectCount) {
		
		while (suspend_input_monitoring_) std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		// if there is a new event, gathering different types of events in certain determined interval
		//  to build, establish complex input events
		while (SDL_PollEvent(&recent_input_event)){

			if (recent_input_event.type == SDL_KEYDOWN) {
				// keydowns and keyups should be differentiated in called function

				recent_keyboard_event = recent_input_event.key;
				recent_event_timestamps[0] = recent_keyboard_event.timestamp;
			}
			else if (recent_input_event.type == SDL_KEYUP) {
				// keydowns and keyups should be differentiated in called function

				recent_keyboard_event = recent_input_event.key;
				recent_event_timestamps[1] = recent_keyboard_event.timestamp;
			}
			else if (recent_input_event.type == SDL_MOUSEBUTTONDOWN) {
				// mouse button downs and mouse button ups should be differentiated in called function

				recent_mouse_button_event = recent_input_event.button;
				recent_event_timestamps[2] = recent_mouse_button_event.timestamp;
			}
			else if (recent_input_event.type == SDL_MOUSEBUTTONUP) {
				// mouse button downs and mouse button ups should be differentiated in called function

				recent_mouse_button_event = recent_input_event.button;
				recent_event_timestamps[3] = recent_mouse_button_event.timestamp;
			}
			else if (recent_input_event.type == SDL_MOUSEWHEEL) {
				// mouse wheel directions should be differentiated in called function

				recent_mouse_wheel_event = recent_input_event.wheel;
				recent_event_timestamps[4] = recent_mouse_wheel_event.timestamp;
			}
			else if (recent_input_event.type == SDL_MOUSEMOTION) {
				// mouse motion directions should be differentiated in called function

				recent_mouse_mostion_event = recent_input_event.motion;
				recent_event_timestamps[5] = recent_mouse_mostion_event.timestamp;
			}
			else if (recent_input_event.type == SDL_WINDOWEVENT) {

				if (recent_input_event.window.type == SDL_WINDOWEVENT_CLOSE) {
				
					recent_window_event = recent_input_event.window;
					recent_event_timestamps[6] = recent_window_event.timestamp;
				}
				else if (recent_input_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				
					recent_window_event = recent_input_event.window;
					recent_event_timestamps[7] = recent_window_event.timestamp;
				}
			}
		}

		std::unique_lock<std::mutex> handler_lock(complex_event_mutex_);

		// checking registered objects if their conditions are met with recent occurred event
		for (object_ind = 0; object_ind < size_of_registered_complex_events_; ++object_ind)
			if (*object_complex_event_subscriptions_[object_ind] == recent_event_timestamps)
				// Calling dynamically dispatched function
				// Elapsed time since the last call to activate the function again is defined
				//  by a duration that must be greater than the delay
				(*object_complex_event_subscriptions_[object_ind])(recent_input_event);		
	}
}

void InputControl::SuspendInputMonitoring() {

	suspend_input_monitoring_ = true;
}

void InputControl::ContinueInputMonitoring() {

	suspend_input_monitoring_ = false;
}
