#pragma once
#include <vector>
#include <utility>
#include <stdint.h>
#include "../include/SDL.h"
#include <exception>
#include <chrono>
#include "../include/glm/glm.hpp"
#include "ComplexInputEvent.h"
#include "../include/SDL.h"
#include "../include/GL/glew.h"
#include <mutex>

struct Vertex {

	glm::vec3 vertex_;
	glm::vec4 color_;
};

class GenericPanel {

public:
	GenericPanel();

	GenericPanel(uint64_t interval_x_start, uint64_t interval_x_end, uint64_t interval_y_start,
		uint64_t interval_y_end);
	
	GenericPanel(const GenericPanel& orig);
	
	GenericPanel& operator=(const GenericPanel& orig);

	virtual ~GenericPanel();
	
	std::mutex& GetDataLockMutex();
	virtual std::vector<Vertex>& FetchVertexData();
	virtual std::vector<GLuint>& FetchIndexData();
	size_t GetVertexContainerSize();
	size_t GetIndexContainerSize();
	size_t GetEventHandlersSize();
	GenericComplexInputEvent* GetEventHandler(const size_t ind);
	virtual void* GetObjectRef();
	
protected:
	
	// IoC based handlers (dependency injection), dynamic dispatch, RAII
	std::vector<GenericComplexInputEvent*> event_handlers_;
	uint64_t interval_x_start_;
	uint64_t interval_x_end_;
	uint64_t interval_y_start_;
	uint64_t interval_y_end_;

	std::mutex data_lock_mutex_;
	GLuint recent_index_index_;
	std::vector<GLuint> indices_;
	std::vector<Vertex> vertices_;// normalized positions and normalized colors
};

