#include "GenericWindowRenderer.h"
#include "GLUtils.hpp"

gui::InputControl gui::GenericWindowRenderer::InputEventControl = InputControl();

std::atomic_uint16_t gui::GenericWindowRenderer::ActiveRendererObjectCount = 0;


void gui::GenericWindowRenderer::StaticInit() {
	/*
	atexit([] {
		program_console_.Println("Press a key to exit the application...");
		std::cin.get();
	});
	*/
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1) {

		std::cout << "[SDL initialization] Error during the SDL initialization: " << SDL_GetError() << std::endl;
		throw "sdl init.";
	}
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	DisplayWidth = display_mode.w;
	DisplayHeight = display_mode.h;

	InputEventControl = InputControl();
	WasStaticInit = true;
	InputEventControl.Run();
}

gui::GenericWindowRenderer::GenericWindowRenderer() {

	program_console_ = ConsoleUI();
	window_id_ = 0;
	window_width_ = 0;
	window_height_ = 0;
	rotation_precision_ = 0;
	zoom_precision_ = 0;
	translation_precision_ = 0;
	sdl_win_ = 0;
	sdl_gl_context_ = 0;
	m_vaoID_ = 0;
	m_vboID_ = 0;
	m_ibID_ = 0;
	m_programID_ = 0;
	m_loc_mvp_ = 0;
	recent_z_dist_ = 0.0;
	run_rendering_ = false;
	suspend_rendering_ = false;
	size_of_render_objects_ = 0;
	ren_init_ = false;
}

gui::GenericWindowRenderer::GenericWindowRenderer(ConsoleUI& program_console, float rotation_precision, 
	float zoom_precision, float translation_precision, std::vector<GenericPanel*> render_objects, 
	uint16_t window_width, uint16_t window_height) {

	program_console_ = program_console;

	if (!WasStaticInit) StaticInit();

	if (window_width > DisplayWidth)
		throw std::exception("Window width is greater than or equal with the width of the"
			" current display (GenericWindowRenderer).");

	window_width_ = window_width;

	if (window_height > DisplayHeight)
		throw std::exception("Window height is greater than or equal with the height of the current"
			" display (GenericWindowRenderer).");

	window_height_ = window_height;

	ren_init_ = false;

	if (rotation_precision <= 0.1)
		throw std::exception("Unit time based rotation precision in radians is less than or "
			"equal with the lower bound (0.1) (GenericWindowRenderer).");

	rotation_precision_ = rotation_precision;

	if (zoom_precision <= 0.1)
		throw std::exception("Unit time based zoom (translation) is less than or equal with the "
			"lower bound (0.1) (GenericWindowRenderer).");

	zoom_precision_ = zoom_precision;

	if (translation_precision <= 0.1)
		throw std::exception("Unit time based translation precision is less than or equal with the "
			"lower bound (0.1) (GenericWindowRenderer).");

	translation_precision_ = translation_precision;

	// registering panels to GenericWindowRenderer object (this)
	if (!render_objects.size())
		throw std::exception("Render object must contain at least one object to accomplish "
			"the non null pointer condition of memory allocation of rendering objects (vertices) "
			"(GenericWindowRenderer).");


	render_objects_ = render_objects;
	size_of_render_objects_ = render_objects_.size();
}

gui::GenericWindowRenderer::GenericWindowRenderer(const GenericWindowRenderer& orig) {

	program_console_ = orig.program_console_;
	ren_init_ = orig.ren_init_;
	window_id_ = orig.window_id_;
	rotate_scene_event_ = orig.rotate_scene_event_;
	zoom_scene_event_ = orig.zoom_scene_event_;
	window_width_ = orig.window_width_;
	window_height_ = orig.window_height_;
	rotation_precision_ = orig.rotation_precision_;
	zoom_precision_ = orig.zoom_precision_;
	translation_precision_ = orig.translation_precision_;
	sdl_win_ = orig.sdl_win_;
	sdl_gl_context_ = orig.sdl_gl_context_;
	m_vaoID_ = orig.m_vaoID_;
	m_vboID_ = orig.m_vboID_;
	m_ibID_ = orig.m_ibID_;
	m_programID_ = orig.m_programID_;
	m_loc_mvp_ = orig.m_loc_mvp_;
	m_matProj_ = orig.m_matProj_;
	m_matView_ = orig.m_matView_;
	m_matWorld_ = orig.m_matWorld_;
	recent_z_dist_ = orig.recent_z_dist_;
	recent_eye_ = orig.recent_eye_;
	recent_center_ = orig.recent_center_;
	run_rendering_ = orig.run_rendering_ ? true : false;
	size_of_render_objects_ = orig.size_of_render_objects_;
	render_objects_ = orig.render_objects_;
	render_vertices_ = orig.render_vertices_;
	render_indices_ = orig.render_indices_;
}

gui::GenericWindowRenderer::~GenericWindowRenderer() {
		
	if (ren_init_) {
		
		size_t size_of_render_objects = render_objects_.size();
		
		--ActiveRendererObjectCount;
		
		InputEventControl.UnregisterGenericObjectComplexEvent(&rotate_scene_event_);
		InputEventControl.UnregisterGenericObjectComplexEvent(&zoom_scene_event_);
		InputEventControl.UnregisterGenericObjectComplexEvent(&translate_scene_event_);
		

		size_t size_of_event_handlers_of_objects = 0;

		for (size_t i = 0; i < size_of_render_objects_; ++i)
			if (size_of_event_handlers_of_objects = render_objects_[i]->GetEventHandlersSize())
				for (size_t j = 0; j < size_of_event_handlers_of_objects; ++j)
					if (render_objects_[i]->GetEventHandler(j)->GetNumberOfEvents())
						InputEventControl.UnregisterGenericObjectComplexEvent(render_objects_[i]->GetEventHandler(j));

		glDeleteBuffers(1, &m_vboID_);
		glDeleteBuffers(1, &m_ibID_);
		glDeleteVertexArrays(1, &m_vaoID_);

		glDeleteProgram(m_programID_);
		SDL_GL_DeleteContext(sdl_gl_context_);
		SDL_DestroyWindow(sdl_win_);
	}
}

void gui::GenericWindowRenderer::Init() {

	sdl_win_ = SDL_CreateWindow("Printer operation optimizer",
		(DisplayWidth - window_width_) / 2, (DisplayHeight - window_height_) / 2,
		window_width_, window_height_, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!sdl_win_) {
		program_console_.Println(std::string("[Window creation] SDL initalization error: ") 
			+ std::string(SDL_GetError()));
		throw "sdl window creation error";
	}

	sdl_gl_context_ = SDL_GL_CreateContext(sdl_win_);

	if (!sdl_gl_context_) {
		program_console_.Println(std::string("[Window creation] Error during the creation of an SDL window: ") 
			+ std::string(SDL_GetError()));
		throw "sdl gl context creation err";
	}

	SDL_GL_SetSwapInterval(1);

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		program_console_.Println("[GLEW] Error during the initialization of glew.");
		throw "glew init err";
	}

	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	program_console_.Println(std::string("Running OpenGL ") + std::to_string(glVersion[0]) + std::string(".") 
		+ std::to_string(glVersion[1]));

	if (glVersion[0] == -1 && glVersion[1] == -1) {
		SDL_GL_DeleteContext(sdl_gl_context_);
		SDL_DestroyWindow(sdl_win_);

		program_console_.Println(std::string("[OGL context creation] Error during the creation of the OGL context: ") 
			+ std::string(SDL_GetError()));

		throw "ogl context creation err";
	}

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	//glEnable(GL_CULL_FACE);// disabling for transparency
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);// disabling for transparency
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// for mesh test
	
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vaoID_);
	glBindVertexArray(m_vaoID_);

	glGenBuffers(1, &m_vboID_);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID_);
	// data will be updated frequently during rendering process
	//glBufferData(GL_ARRAY_BUFFER, render_vertices_.size() * sizeof(Vertex), &render_vertices_[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glGenBuffers(1, &m_ibID_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID_);
	// data will be updated frequently during rendering process
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_indices_.size() * sizeof(GLuint), &render_indices_[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "vertex_shader.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "fragment_shader.frag");

	m_programID_ = glCreateProgram();
	glAttachShader(m_programID_, vs_ID);
	glAttachShader(m_programID_, fs_ID);
	glBindAttribLocation(m_programID_, 0, "vs_in_pos");
	glBindAttribLocation(m_programID_, 1, "vs_in_col");
	glLinkProgram(m_programID_);

	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID_, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID_, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength > 1)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID_, infoLogLength, nullptr, VertexShaderErrorMessage.data());
		program_console_.Println(std::string("[glLinkProgram] Shader linking error:\n") 
			+ std::string(&VertexShaderErrorMessage[0]));
	}

	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	m_loc_mvp_ = glGetUniformLocation(m_programID_, "MVP");

	SDL_GetWindowSize(sdl_win_, &window_width_, &window_height_);
	glViewport(0, 0, window_width_, window_height_);
	m_matProj_ = glm::perspective(glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.01f, 1000.0f);
	recent_z_dist_ = 2.0;
	recent_eye_ = glm::vec3(0, recent_z_dist_, 0);
	recent_center_ = glm::vec3(0, 0, 0);
	m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
	m_matWorld_ = glm::mat4(1.0f);

	window_id_ = SDL_GetWindowID(sdl_win_);

	
	// registering internal, own events

	rotate_scene_event_ = ComplexInputEvent<GenericWindowRenderer>(
		this, &gui::GenericWindowRenderer::RotateSceneView,
		3, new SimpleEvent[]{ CEV_MOUSEBUTTONUP, CEV_MOUSEBUTTONDOWN, CEV_MOUSEMOTION },
		std::numeric_limits<uint32_t>::max(), 10);
	InputEventControl.RegisterGenericObjectComplexEvent(&rotate_scene_event_);

	zoom_scene_event_ = ComplexInputEvent<GenericWindowRenderer>(
		this, &gui::GenericWindowRenderer::ZoomSceneView,
		1, new SimpleEvent[]{ CEV_MOUSWHEEL },
		std::numeric_limits<uint32_t>::max(), 10);
	InputEventControl.RegisterGenericObjectComplexEvent(&zoom_scene_event_);

	translate_scene_event_ = ComplexInputEvent<GenericWindowRenderer>(
		this, &gui::GenericWindowRenderer::TranslateSceneEvent,
		2, new SimpleEvent[]{ CEV_KEYBOARDKEYUP, CEV_KEYBOARDKEYDOWN },
		std::numeric_limits<uint32_t>::max(), 10);
	InputEventControl.RegisterGenericObjectComplexEvent(&translate_scene_event_);
	

	// Registering complex event and their event handler pairs of objects to input event monitoring process
	size_t size_of_event_handlers_of_objects = 0;

	// registering external, parametrically attached object events
	for (size_t i = 0; i < size_of_render_objects_; ++i)
		if (size_of_event_handlers_of_objects = render_objects_[i]->GetEventHandlersSize())
			for (size_t j = 0; j < size_of_event_handlers_of_objects; ++j)
				if (render_objects_[i]->GetEventHandler(j)->GetNumberOfEvents())
					InputEventControl.RegisterGenericObjectComplexEvent(render_objects_[i]->GetEventHandler(j));

	RegisteredWindows.insert(RegisteredWindows.end(), { window_id_, this });
	run_rendering_ = true;
	suspend_rendering_ = false;
	++ActiveRendererObjectCount;

	ren_init_ = true;
}

gui::GenericWindowRenderer& gui::GenericWindowRenderer::operator=(const GenericWindowRenderer& orig) {

	program_console_ = orig.program_console_;
	ren_init_ = orig.ren_init_;
	window_id_ = orig.window_id_;
	rotate_scene_event_ = orig.rotate_scene_event_;
	zoom_scene_event_ = orig.zoom_scene_event_;
	window_width_ = orig.window_width_;
	window_height_ = orig.window_height_;
	rotation_precision_ = orig.rotation_precision_;
	zoom_precision_ = orig.zoom_precision_;
	translation_precision_ = orig.translation_precision_;
	sdl_win_ = orig.sdl_win_;
	sdl_gl_context_ = orig.sdl_gl_context_;
	m_vaoID_ = orig.m_vaoID_;
	m_vboID_ = orig.m_vboID_;
	m_ibID_ = orig.m_ibID_;
	m_programID_ = orig.m_programID_;
	m_loc_mvp_ = orig.m_loc_mvp_;
	m_matProj_ = orig.m_matProj_;
	m_matView_ = orig.m_matView_;
	m_matWorld_ = orig.m_matWorld_;
	recent_z_dist_ = orig.recent_z_dist_;
	recent_eye_ = orig.recent_eye_;
	recent_center_ = orig.recent_center_;
	run_rendering_ = orig.run_rendering_ ? true : false;
	//suspend_rendering_lock_ = orig.suspend_rendering_lock_;
	size_of_render_objects_ = orig.size_of_render_objects_;
	render_objects_ = orig.render_objects_;
	render_vertices_ = orig.render_vertices_;
	render_indices_ = orig.render_indices_;
	
	return *this;
}

void gui::GenericWindowRenderer::RotateSceneView(const SDL_Event& sdl_event) {

	// should use Tait-Bryan intrinsic rotation system
	std::scoped_lock<std::mutex> transform_lock(view_transform_mutex_);
	glm::vec4 rot_eye = glm::vec4(recent_eye_, 1);

	rot_eye = glm::rotate(glm::radians((float)(-1.0) * sdl_event.motion.xrel * rotation_precision_),
		glm::vec3(0, 0, 1)) * rot_eye;
	recent_eye_ = glm::vec3(rot_eye);
	recent_z_dist_ = recent_eye_[1];
	m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
	
	recent_eye_ = glm::vec3(rot_eye);
	rot_eye = glm::rotate(glm::radians((float)sdl_event.motion.yrel * rotation_precision_),
		glm::vec3(1, 0, 0)) * rot_eye;
	recent_eye_ = glm::vec3(rot_eye);
	recent_z_dist_ = recent_eye_[1];
	m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
}

void gui::GenericWindowRenderer::ZoomSceneView(const SDL_Event& sdl_event) {

	std::scoped_lock<std::mutex> transform_lock(view_transform_mutex_);
	if ((-1.0) * sdl_event.wheel.y < 3 
		&& recent_z_dist_ + (-1.0) * sdl_event.wheel.y * zoom_precision_ > -10.0) {
	
		recent_z_dist_ += (-1.0) * sdl_event.wheel.y * zoom_precision_;
	}

	recent_eye_[1] = recent_z_dist_;
	m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
}

void gui::GenericWindowRenderer::TranslateSceneEvent(const SDL_Event& sdl_event) {

	if (sdl_event.key.keysym.sym == SDLK_RIGHT) {
	
		std::scoped_lock<std::mutex> transform_lock(view_transform_mutex_);
		recent_center_[0] += translation_precision_;
		recent_eye_[0] += translation_precision_;
		m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
	}
	else if (sdl_event.key.keysym.sym == SDLK_DOWN) {
	
		std::scoped_lock<std::mutex> transform_lock(view_transform_mutex_);
		recent_center_[2] += translation_precision_;
		recent_eye_[2] += translation_precision_;
		m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
	}
	else if (sdl_event.key.keysym.sym == SDLK_LEFT) {
	
		std::scoped_lock<std::mutex> transform_lock(view_transform_mutex_);
		recent_center_[0] += (-1.0) * translation_precision_;
		recent_eye_[0] += (-1.0) * translation_precision_;
		m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
	}
	else if (sdl_event.key.keysym.sym == SDLK_UP) {
	
		std::scoped_lock<std::mutex> transform_lock(view_transform_mutex_);
		recent_center_[2] += (-1.0) * translation_precision_;
		recent_eye_[2] += (-1.0) * translation_precision_;
		m_matView_ = glm::lookAt(recent_eye_, recent_center_, glm::vec3(0, 0, 1));
	}
}

void gui::GenericWindowRenderer::CloseWindow() {

	run_rendering_ = false;
}

void gui::GenericWindowRenderer::UpdateData() {
	

	render_vertices_.clear();
	render_indices_.clear();
	

	std::vector<Vertex> render_object_vertices;
	std::vector<GLuint> render_object_indices;

	for (size_t i = 0; i < size_of_render_objects_; ++i) {
	
		std::scoped_lock<std::mutex> data_update_lock(render_objects_[i]->GetDataLockMutex());
		
		render_object_vertices = render_objects_[i]->FetchVertexData();

		render_vertices_.insert(render_vertices_.end(),
			render_object_vertices.begin(), render_object_vertices.end());

		render_object_indices = render_objects_[i]->FetchIndexData();

		render_indices_.insert(render_indices_.end(),
			render_object_indices.begin(), render_object_indices.end());
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID_);
	glBufferData(GL_ARRAY_BUFFER, render_vertices_.size() * sizeof(Vertex), &render_vertices_[0],
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_indices_.size() * sizeof(GLuint), &render_indices_[0], 
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void gui::GenericWindowRenderer::Render() {

	if (!ren_init_)
		throw std::exception("No video initialization has done (GenericWindowRenderer).");

	while (run_rendering_) {
	
		while (suspend_rendering_) std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_programID_);
		this->UpdateData();

		std::unique_lock<std::mutex> transform_lock(view_transform_mutex_);
		glm::mat4 mvp = m_matProj_ * m_matView_ * m_matWorld_;
		transform_lock.unlock();

		glUniformMatrix4fv(m_loc_mvp_, 1, GL_FALSE,	&(mvp[0][0]));

		glBindVertexArray(m_vaoID_);

		glDrawElements(GL_TRIANGLES, render_indices_.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glUseProgram(0);

		SDL_GL_SwapWindow(sdl_win_);
		SDL_PumpEvents();
	}
}

void gui::GenericWindowRenderer::SuspendRendering() {

	InputEventControl.SuspendInputMonitoring();
	suspend_rendering_ = true;
}

void gui::GenericWindowRenderer::ContinueRendering() {

	InputEventControl.ContinueInputMonitoring();
	suspend_rendering_ = false;
}
