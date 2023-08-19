#ifndef GENERICWINDOWRENDERER_H_INCLUDED
#define GENERICWINDOWRENDERER_H_INCLUDED

#include <stdint.h>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <exception>
#include "../genmath/Vector.h"
#include "../genmath/LongDouble.h"
#include "ComplexInputEvent.h"
#include "InputControl.h"
#include "GenericPanel.h"
#include <functional>
#include <map>

#include "../include/GL/glew.h"
#include "../include/SDL.h"
#include "../include/SDL_main.h"
#include "../include/SDL_opengl.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtx/transform2.hpp"
#include "../cli/ConsoleUI.h"

namespace gui {

	class InputControl;
	// encapsulated SDL-OGL renderer
	class GenericWindowRenderer
	{
	private:
		static void StaticInit();
		static InputControl InputEventControl;

	public:
		inline static std::atomic_bool WasStaticInit = false;
		static std::atomic_uint16_t ActiveRendererObjectCount;
	
		GenericWindowRenderer();
	
		GenericWindowRenderer(ConsoleUI& program_console, float rotation_precision, float zoom_precision,
			float translation_precision, std::vector<GenericPanel*> render_objects, uint16_t window_width = 800,
			uint16_t window_height = 600);
	
		GenericWindowRenderer(const GenericWindowRenderer& orig);
	
		void Init();
	
		GenericWindowRenderer& operator=(const GenericWindowRenderer& orig);

		virtual ~GenericWindowRenderer();

	
		// event driven functions
		void RotateSceneView(const SDL_Event& sdl_event);
		void ZoomSceneView(const SDL_Event& sdl_event);
		void TranslateSceneEvent(const SDL_Event& sdl_event);
		void CloseWindow();
	
		// rendering functions
		void UpdateData();
		void Render();
		void SuspendRendering();
		void ContinueRendering();
	
	private:

		ConsoleUI program_console_;

		uint8_t ren_init_;// avoidance of multiple destruction of renderer members

		// multiple GenericWindowRenderer object management especially for InputControl static object
		inline static std::mutex CloseWindowMutex;
		inline static std::condition_variable CloseSelectedWindowCond;
		inline static std::map<uint32_t, GenericWindowRenderer*> RegisteredWindows;
	
		static inline uint16_t DisplayWidth = 1920;
		static inline uint16_t DisplayHeight = 1080;

		uint32_t window_id_;
	
		ComplexInputEvent<GenericWindowRenderer> rotate_scene_event_;
		ComplexInputEvent<GenericWindowRenderer> zoom_scene_event_;
		ComplexInputEvent<GenericWindowRenderer> translate_scene_event_;
	
		// members for OpenGL rendering
		int window_width_;
		int window_height_;
		float rotation_precision_;
		float zoom_precision_;
		float translation_precision_;
		SDL_Window* sdl_win_;
		SDL_GLContext sdl_gl_context_;
		GLuint m_vaoID_;
		GLuint m_vboID_;
		GLuint m_ibID_;
		GLuint m_programID_;
		GLuint m_loc_mvp_;
		glm::mat4 m_matProj_;
		glm::mat4 m_matView_;
		glm::mat4 m_matWorld_;

		std::mutex view_transform_mutex_;

		float recent_z_dist_;
		glm::vec3 recent_eye_;
		glm::vec3 recent_center_;


		std::atomic_bool run_rendering_;
		std::atomic_bool suspend_rendering_;
	
		size_t size_of_render_objects_;
		std::vector<GenericPanel*> render_objects_;
		std::vector<Vertex> render_vertices_;
		std::vector<GLuint> render_indices_;
		// handling multiple windows (for further improvement)
	};
}

#endif// GENERICWINDOWRENDERER_H_INCLUDED