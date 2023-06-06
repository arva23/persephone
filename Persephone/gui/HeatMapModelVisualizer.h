#pragma once
#include "GenericPanel.h"
#include "../genmath/LongDouble.h"
#include <vector>
#include <stdint.h>
#include <chrono>
#include <atomic>
#include <thread>
#include "../genmath/Vector.h"
#include "GenericPanel.h"
#include "../include/GL/glew.h"
#include <cmath>

class HeatMapModelVisualizer : public GenericPanel {

public:
	HeatMapModelVisualizer();
	/// <summary>
	/// Discrete thermal space visualizer, which plays the simulation back with adjustable speed.
	/// </summary>
	/// <param name="area_width">width of visualization area</param>
	/// <param name="area_height">height of visualization area</param>
	/// <param name="position_half_step">half of the unit edge of a unit volume</param>
	/// <param name="thermal_lower_bound">possible smallest thermal value</param>
	/// <param name="thermal_upper_bound">possible largest thermal value</param>
	/// <param name="env_coeff">coefficient for environmental point filtering</param>
	/// <param name="preservation_temp">thermal upper boundary of permanent preservation of unit visualization</param>
	HeatMapModelVisualizer(uint64_t area_width, uint64_t area_height, float position_half_step,
		float thermal_lower_bound, float thermal_upper_bound, float env_coeff, float preservation_temp);

	HeatMapModelVisualizer(const HeatMapModelVisualizer& orig);
	virtual ~HeatMapModelVisualizer();

	HeatMapModelVisualizer& operator=(const HeatMapModelVisualizer& orig);
	
	void* GetObjectRef() override;

	/// <summary>
	/// Loads the given (computed) discrete thermal space model. It uses rectangular simulation
	/// space form.
	/// </summary>
	/// <param name="sim_x_ind_rng">meshgrid length in x axis direction</param>
	/// <param name="sim_y_ind_rng">meshgrid length in y axis direction</param>
	/// <param name="sim_z_ind_rng">meshgrid length in z axis direction</param>
	/// <param name="sim_time_step">time resolution</param>
	/// <param name="sim_time_frames">the actual simulated, computed thermal space data in time
	/// frames</param>
	/// <param name="sim_time_frame_coeffs">coefficients of simulation time frames for time dependent
	/// material unit filtering</param>
	void LoadComputedModel(size_t sim_x_ind_rng, size_t sim_y_ind_rng, size_t sim_z_ind_rng,
		float sim_time_step, std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >& sim_time_frames,
		std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >& sim_time_frame_coeffs);

	void ModelPlayerThread();
	
	bool model_loaded_;
	ComplexInputEvent<HeatMapModelVisualizer> start_of_pause_loaded_model_play_event_;
	void StartOrPauseLoadedModelPlay(const SDL_Event& sdl_event);

	void StopModelPlay();

	std::mutex next_frame_delay_time_update_mutex_;
	ComplexInputEvent<HeatMapModelVisualizer> change_playback_speed_event_;
	void ChangePlaybackSpeed(const SDL_Event& sdl_event);

private:
	std::atomic_bool pause_model_play_;
	std::atomic_bool stop_model_play_;
	std::atomic_bool model_play_stopped_;

	float position_half_step_;
	float thermal_lower_bound_;
	float thermal_upper_bound_;
	float thermal_range_;
	float env_coeff_;
	float preservation_temp_;
	std::chrono::duration<double> next_frame_delay_time_lower_limit_;
	std::chrono::duration<double> next_frame_delay_time_upper_limit_;
	std::chrono::duration<double> next_frame_delay_time_;
	size_t rel_index_offset_;// for frame rate reduction
	size_t current_status_frame_ind_;
	std::atomic_bool active_spd_mod_;// avoidance of multiple playback speed modification at unit time step
	std::vector<std::vector<Vertex> > grid_points_;
	std::vector<std::vector<GLuint> > grid_point_indices_;
	std::thread model_player_thread_;
};
