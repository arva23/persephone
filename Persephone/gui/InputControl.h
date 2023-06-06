#pragma once
#include <stdint.h>
#include <vector>
#include "../include/SDL.h"
#include <utility>
#include <atomic>
#include <thread>
#include <chrono>
#include "ComplexInputEvent.h"
#include "GenericWindowRenderer.h"

// composition class of GenericRenderer (for each window)
class InputControl
{
public:
	InputControl();
	InputControl(std::vector<GenericComplexInputEvent*>& object_complex_event_subscriptions);
	InputControl(const InputControl& orig);
	virtual ~InputControl();

	void Run();

	InputControl& operator=(const InputControl& orig);

	// registration occurs via GenericWindowRenderer object
	void RegisterGenericObjectComplexEvent(GenericComplexInputEvent* complex_event);
	void UnregisterGenericObjectComplexEvent(GenericComplexInputEvent* complex_event);
	void MonitorInputEvent();// keyboard events along with mouse events
	
	// can be improved to multiple windows management version with an extra parameter
	void SuspendInputMonitoring();
	void ContinueInputMonitoring();

private:
	std::thread control_thread_;
	// can be improved to multiple windows management version
	std::atomic_bool suspend_input_monitoring_;
	
	size_t size_of_registered_complex_events_;
	// detaching ComplexInputEvent - event handler pairs from their GenericPanel owner
	//  in case of optimization of reacing, evaluating event during processing
	std::vector<GenericComplexInputEvent*> object_complex_event_subscriptions_;
	std::mutex complex_event_mutex_;
	bool control_thread_started_;
};
