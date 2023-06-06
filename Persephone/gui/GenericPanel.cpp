#include "GenericPanel.h"

GenericPanel::GenericPanel() {

	interval_x_start_ = 0;
	interval_x_end_ = 0;
	interval_y_start_ = 0;
	interval_y_end_ = 0;
	recent_index_index_ = 0;
}

GenericPanel::GenericPanel(uint64_t interval_x_start, uint64_t interval_x_end, uint64_t interval_y_start,
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

GenericPanel::GenericPanel(const GenericPanel& orig) {

	event_handlers_ = orig.event_handlers_;
	interval_x_start_ = orig.interval_x_start_;
	interval_x_end_ = orig.interval_x_end_;
	interval_y_start_ = orig.interval_y_start_;
	interval_y_end_ = orig.interval_y_end_;
}

GenericPanel& GenericPanel::operator=(const GenericPanel& orig) {

	event_handlers_ = orig.event_handlers_;
	interval_x_start_ = orig.interval_x_start_;
	interval_x_end_ = orig.interval_x_end_;
	interval_y_start_ = orig.interval_y_start_;
	interval_y_end_ = orig.interval_y_end_;
}

GenericPanel::~GenericPanel() {


}

std::mutex& GenericPanel::GetDataLockMutex() {

	return data_lock_mutex_;
}

std::vector<Vertex>& GenericPanel::FetchVertexData() {

	return vertices_;
}

std::vector<GLuint>& GenericPanel::FetchIndexData() {

	return indices_;
}

size_t GenericPanel::GetVertexContainerSize() {

	return vertices_.size();// planar rectangular object
}

size_t GenericPanel::GetIndexContainerSize() {

	return indices_.size();
}

size_t GenericPanel::GetEventHandlersSize() {

	return event_handlers_.size();
}

GenericComplexInputEvent* GenericPanel::GetEventHandler(size_t ind) {

	return event_handlers_[ind];
}

void* GenericPanel::GetObjectRef() {

	return this;
}