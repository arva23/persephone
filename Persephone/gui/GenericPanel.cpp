#include "GenericPanel.h"

gui::GenericPanel::GenericPanel() {

	interval_x_start_ = 0;
	interval_x_end_ = 0;
	interval_y_start_ = 0;
	interval_y_end_ = 0;
	recent_index_index_ = 0;
}

gui::GenericPanel::GenericPanel(uint64_t interval_x_start, uint64_t interval_x_end, uint64_t interval_y_start,
	uint64_t interval_y_end) {
		
	interval_x_start_ = interval_x_start;
	interval_x_end_ = interval_x_end;
	interval_y_start_ = interval_y_start;
	interval_y_end_ = interval_y_end;

	// default vertex creations
	vertices_.push_back(Vertex(glm::vec3(-1.0, 0.0, -1.0), glm::vec4(0.125, 0.25, 0.5, 1.0)));
	vertices_.push_back(Vertex(glm::vec3(1.0, 0.0, -1.0), glm::vec4(0.125, 0.25, 0.5, 1.0)));
	vertices_.push_back(Vertex(glm::vec3(-1.0, 0.0, 1.0), glm::vec4(0.125, 0.25, 0.5, 1.0)));
	vertices_.push_back(Vertex(glm::vec3(1.0, 0.0, 1.0), glm::vec4(0.125, 0.25, 0.5, 1.0)));
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(3);
}

gui::GenericPanel::GenericPanel(const GenericPanel& orig) {

	event_handlers_ = orig.event_handlers_;
	interval_x_start_ = orig.interval_x_start_;
	interval_x_end_ = orig.interval_x_end_;
	interval_y_start_ = orig.interval_y_start_;
	interval_y_end_ = orig.interval_y_end_;
}

gui::GenericPanel& gui::GenericPanel::operator=(const GenericPanel& orig) {

	event_handlers_ = orig.event_handlers_;
	interval_x_start_ = orig.interval_x_start_;
	interval_x_end_ = orig.interval_x_end_;
	interval_y_start_ = orig.interval_y_start_;
	interval_y_end_ = orig.interval_y_end_;
}

gui::GenericPanel::~GenericPanel() {


}

std::mutex& gui::GenericPanel::GetDataLockMutex() {

	return data_lock_mutex_;
}

std::vector<gui::Vertex>& gui::GenericPanel::FetchVertexData() {

	return vertices_;
}

std::vector<GLuint>& gui::GenericPanel::FetchIndexData() {

	return indices_;
}

size_t gui::GenericPanel::GetVertexContainerSize() {

	return vertices_.size();// planar rectangular object
}

size_t gui::GenericPanel::GetIndexContainerSize() {

	return indices_.size();
}

size_t gui::GenericPanel::GetEventHandlersSize() {

	return event_handlers_.size();
}

gui::GenericComplexInputEvent* gui::GenericPanel::GetEventHandler(size_t ind) {

	return event_handlers_[ind];
}

void* gui::GenericPanel::GetObjectRef() {

	return this;
}