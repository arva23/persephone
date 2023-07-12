#include "HeatMapModelVisualizer.h"
//#define TEST_DYNAMIC_TIMING

gui::HeatMapModelVisualizer::HeatMapModelVisualizer() : GenericPanel() {

	position_half_step_ = 0.01;
	thermal_lower_bound_ = 20.0;
	thermal_upper_bound_ = 350.0;
	thermal_range_ = thermal_upper_bound_ - thermal_lower_bound_;
	env_coeff_ = 1.0;
	preservation_temp_ = 110.0;
	next_frame_delay_time_lower_limit_ = std::chrono::duration<float>(10.0);
	next_frame_delay_time_upper_limit_ = std::chrono::duration<float>(10000.0);
	next_frame_delay_time_ = std::chrono::duration<float>(1000.0 / 24.0);
	rel_index_offset_ = 1;
	current_status_frame_ind_ = 0;
	active_spd_mod_ = false;
	model_loaded_ = false;
	pause_model_play_ = true;
	stop_model_play_ = true;
	model_play_stopped_ = true;
}

gui::HeatMapModelVisualizer::HeatMapModelVisualizer(uint64_t area_width, uint64_t area_height, float position_half_step,
	float thermal_lower_bound, float thermal_upper_bound, float env_coeff,
	float preservation_temp) : GenericPanel(0, 0, area_width, area_height) {

	if (position_half_step < 0.00001f)
		throw std::exception("Dimension of unit particle at simulation is less than the "
			"lower bound (0.00001f) (HeatMapModelVisualizer).");

	if (0.5f < position_half_step)
		throw std::exception("Dimension of unit particle at simulation is greater than the "
			"upper bound (0.5f) (HeatMapModelVisualizer).");

	position_half_step_ = position_half_step;
	
	if (thermal_lower_bound < -273.15f)
		throw std::exception("Thermal lower bound is less than the absolute zero degrees "
			"(-273.15f C, 0K) (HeatMapModelVisualizer).");

	if (1000000.0f < thermal_lower_bound)
		throw std::exception("Thermal lower bound is greater than the upper bound "
			"value (1000000f C) (HeatMapModelVisualizer).");

	thermal_lower_bound_ = thermal_lower_bound;
	
	if (thermal_upper_bound < thermal_lower_bound)
		throw std::exception("Thermal upper bound is lower then the thermal lower bound (HeatMapModelVisualizer).");

	if (1000000.0f < thermal_upper_bound)
		throw std::exception("Thermal upper bound is greater than the upper bound (1000000f C) (HeatMapModelVisualizer).");

	if (thermal_lower_bound > preservation_temp)
		throw std::exception("Thermal preservation temperature is lower than the thermal lower bound (HeatMapModelVisualizer).");

	if (preservation_temp > thermal_upper_bound)
		throw std::exception("Thermal preservation temperature is greater than the thermal upper bound (HeatMapModelVisualizer).");

	if (env_coeff <= 0.0)
		throw std::exception("Environmental heat conduction coefficient is theoretical absolute thermal "
			"insulator or is negative (HeatMapModelVisualizer).");

	thermal_upper_bound_ = thermal_upper_bound;
	thermal_range_ = thermal_upper_bound_ - thermal_lower_bound_;
	env_coeff_ = env_coeff;
	preservation_temp_ = preservation_temp;

	next_frame_delay_time_lower_limit_ = std::chrono::milliseconds(10);
	next_frame_delay_time_upper_limit_ = std::chrono::milliseconds(10000);
	next_frame_delay_time_ = std::chrono::milliseconds(1000 / 24);
	rel_index_offset_ = 1;
	current_status_frame_ind_ = 1;
	active_spd_mod_ = false;

	// subscriptions for input event control
	
	// play or pause loaded model play event
	start_of_pause_loaded_model_play_event_ = ComplexInputEvent<HeatMapModelVisualizer>(
		this, &gui::HeatMapModelVisualizer::StartOrPauseLoadedModelPlay,
		2, new SimpleEvent[]{ CEV_KEYBOARDKEYUP, CEV_KEYBOARDKEYDOWN },
		std::numeric_limits<uint32_t>::max(), 10);
	GenericPanel::event_handlers_.push_back(&start_of_pause_loaded_model_play_event_);
	
	// increase playback speed (this cause the whole model to be reloaded
	//  due to optimizations, see LoadComputedModel member function)
	change_playback_speed_event_ = ComplexInputEvent<HeatMapModelVisualizer>(
		this, &gui::HeatMapModelVisualizer::ChangePlaybackSpeed,
		2, new SimpleEvent[]{ CEV_KEYBOARDKEYUP, CEV_KEYBOARDKEYDOWN },
		std::numeric_limits<uint32_t>::max(), 10);
	GenericPanel::event_handlers_.push_back(&change_playback_speed_event_);

	model_loaded_ = false;
	pause_model_play_ = true;
	stop_model_play_ = true;
	model_play_stopped_ = true;
}

gui::HeatMapModelVisualizer::HeatMapModelVisualizer(const HeatMapModelVisualizer& orig) : GenericPanel(orig) {

	position_half_step_ = orig.position_half_step_;
	thermal_lower_bound_ = orig.thermal_lower_bound_;
	thermal_upper_bound_ = orig.thermal_upper_bound_;
	thermal_range_ = orig.thermal_range_;
	env_coeff_ = orig.env_coeff_;
	preservation_temp_ = orig.preservation_temp_;
	next_frame_delay_time_lower_limit_ = orig.next_frame_delay_time_lower_limit_;
	next_frame_delay_time_upper_limit_ = orig.next_frame_delay_time_upper_limit_;
	next_frame_delay_time_ = orig.next_frame_delay_time_;
	rel_index_offset_ = orig.rel_index_offset_;
	current_status_frame_ind_ = orig.current_status_frame_ind_;
	active_spd_mod_ = orig.active_spd_mod_ ? true : false;
	grid_points_ = orig.grid_points_;
	grid_point_indices_ = orig.grid_point_indices_;
	model_loaded_ = orig.model_loaded_;
	pause_model_play_ = true;
	stop_model_play_ = true;
	model_play_stopped_ = true;
}

gui::HeatMapModelVisualizer::~HeatMapModelVisualizer() {

	
}

gui::HeatMapModelVisualizer& gui::HeatMapModelVisualizer::operator=(const HeatMapModelVisualizer& orig) {

	GenericPanel::operator=(orig);

	position_half_step_ = orig.position_half_step_;
	thermal_lower_bound_ = orig.thermal_lower_bound_;
	thermal_upper_bound_ = orig.thermal_upper_bound_;
	thermal_range_ = orig.thermal_range_;
	env_coeff_ = orig.env_coeff_;
	preservation_temp_ = orig.preservation_temp_;
	next_frame_delay_time_lower_limit_ = orig.next_frame_delay_time_lower_limit_;
	next_frame_delay_time_upper_limit_ = orig.next_frame_delay_time_upper_limit_;
	next_frame_delay_time_ = orig.next_frame_delay_time_;
	rel_index_offset_ = orig.rel_index_offset_;
	current_status_frame_ind_ = orig.current_status_frame_ind_;
	active_spd_mod_ = orig.active_spd_mod_ ? true : false;
	grid_points_ = orig.grid_points_;
	grid_point_indices_ = orig.grid_point_indices_;
	model_loaded_ = orig.model_loaded_;
	pause_model_play_ = true;
	stop_model_play_ = true;
	model_play_stopped_ = true;;

	return *this;
}

void* gui::HeatMapModelVisualizer::GetObjectRef() {

	return this;
}

void gui::HeatMapModelVisualizer::LoadComputedModel(size_t sim_x_ind_rng, size_t sim_y_ind_rng,	size_t sim_z_ind_rng,
	float sim_time_step, std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >& sim_time_frames,
	std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >& sim_time_frame_coeffs) {

	// terminating possible previous playback
	pause_model_play_ = true;
	stop_model_play_ = true;

	if (!sim_x_ind_rng)	throw std::exception("No x index range provided (HeatMapModelVisualizer).");

	if (!sim_y_ind_rng) throw std::exception("No y index range provided (HeatMapModelVisualizer).");

	if (!sim_z_ind_rng) throw std::exception("No z index range provided (HeatMapModelVisualizer).");

	if (!sim_time_frames.size()) throw std::exception("No simulation time frames provided (HeatMapModelVisualizer).");

	size_t size_of_sim_time_frames = sim_time_frames.size();
	
	// finding the possible widest range during the playback to make coordinate normalization properly
	float norm_denominator = sim_x_ind_rng;
	if (norm_denominator < sim_y_ind_rng) norm_denominator = sim_y_ind_rng;
	if (norm_denominator < sim_z_ind_rng) norm_denominator = sim_z_ind_rng;
	
	float position_step = 1.0 / (float)norm_denominator;
	position_half_step_ = position_step / 2.0;
	float center_off_x = norm_denominator / 2.0 * position_step - position_half_step_;
	float center_off_y = norm_denominator / 2.0 * position_step - position_half_step_;
	float center_off_z = norm_denominator / 2.0 * position_step - position_half_step_;

	next_frame_delay_time_ = std::chrono::duration<float>(sim_time_step);

	grid_points_.clear();
	grid_point_indices_.clear();

	genmath::LongDouble thermal_value = "0.0";
	genmath::LongDouble lower_bound_ = thermal_lower_bound_;

	for (size_t ti = 0; ti < size_of_sim_time_frames; ++ti) {
	
		size_t curr_unit_vol_ind = 0;
		grid_points_.push_back(std::vector<Vertex>());
		grid_point_indices_.push_back(std::vector<GLuint>());

		for (size_t z = 0; z < sim_z_ind_rng; ++z) {
		
			for (size_t y = 0; y < sim_y_ind_rng; ++y) {
			
				for (size_t x = 0; x < sim_x_ind_rng; ++x) {
					
					// discarding unit volume creation in case of environmental point
					if ((thermal_value = sim_time_frames[ti][x * sim_y_ind_rng + y][z]) < lower_bound_
						&& (float)sim_time_frame_coeffs[ti][x * sim_y_ind_rng + y][z] == env_coeff_) {
					
						continue;
					}

					thermal_value = sim_time_frames[ti][x * sim_y_ind_rng + y][z];

					glm::vec4 color;
					color[0] = 1.0;

					if ((float)thermal_value < preservation_temp_ && (float)sim_time_frame_coeffs[ti][x * sim_y_ind_rng + y][z] != env_coeff_) {
						
						color[1] = 1.0 - ((float)preservation_temp_ - thermal_lower_bound_) / thermal_range_;
					}
					else {
					
						color[1] = 1.0 - ((float)thermal_value - thermal_lower_bound_) / thermal_range_;
					}
					color[2] = 0.0;
					color[3] = 1.0 - std::sqrt(color[1]);
					glm::vec3 position;

					// vertex 0
					position[0] = (float)x * position_step - position_half_step_ - center_off_x;
					position[1] = (float)y * position_step - position_half_step_ - center_off_y;
					position[2] = (float)z * position_step + position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 1
					position[0] = (float)x * position_step + position_half_step_ - center_off_x;
					position[1] = (float)y * position_step - position_half_step_ - center_off_y;
					position[2] = (float)z * position_step + position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 2
					position[0] = (float)x * position_step + position_half_step_ - center_off_x;
					position[1] = (float)y * position_step + position_half_step_ - center_off_y;
					position[2] = (float)z * position_step + position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 3
					position[0] = (float)x * position_step - position_half_step_ - center_off_x;
					position[1] = (float)y * position_step + position_half_step_ - center_off_y;
					position[2] = (float)z * position_step + position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 4
					position[0] = (float)x * position_step + position_half_step_ - center_off_x;
					position[1] = (float)y * position_step - position_half_step_ - center_off_y;
					position[2] = (float)z * position_step - position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 5
					position[0] = (float)x * position_step - position_half_step_ - center_off_x;
					position[1] = (float)y * position_step - position_half_step_ - center_off_y;
					position[2] = (float)z * position_step - position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 6
					position[0] = (float)x * position_step - position_half_step_ - center_off_x;
					position[1] = (float)y * position_step + position_half_step_ - center_off_y;
					position[2] = (float)z * position_step - position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
					
					// vertex 7
					position[0] = (float)x * position_step + position_half_step_ - center_off_x;
					position[1] = (float)y * position_step + position_half_step_ - center_off_y;
					position[2] = (float)z * position_step - position_half_step_ - center_off_z;
					grid_points_[ti].push_back(Vertex(position, color));
										
					// front face
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 0);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 1);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 2);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 0);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 2);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 3);

					// right face
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 1);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 4);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 7);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 1);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 7);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 2);

					// rear face
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 4);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 5);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 6);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 4);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 6);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 7);

					// left face
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 5);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 0);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 3);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 5);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 3);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 6);

					// top face
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 3);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 2);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 7);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 3);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 7);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 6);

					// bottom face
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 5);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 4);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 1);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 5);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 1);
					grid_point_indices_[ti].push_back(curr_unit_vol_ind + 0);
					
					curr_unit_vol_ind += 8;
				}
			}
		}
	}

	current_status_frame_ind_ = 0;// resetting frame index

	std::scoped_lock<std::mutex> update_data(GenericPanel::data_lock_mutex_);
	GenericPanel::vertices_.clear();
	GenericPanel::indices_.clear();
	GenericPanel::vertices_ = grid_points_[0];
	GenericPanel::indices_ = grid_point_indices_[0];
	
	model_loaded_ = true;
}

void gui::HeatMapModelVisualizer::ModelPlayerThread() {

	// waiting for previous model playback to finish
	while (!model_play_stopped_) { std::this_thread::yield(); }
	model_play_stopped_ = false;

	size_t size_of_sim_time_frames = grid_points_.size();
	//size_t current_status_frame_ind_ = 0;// defined as a class member for later continuation of play
	size_t size_of_current_status_frame = 0;
	std::chrono::duration<float> update_duration(0);

	while (!stop_model_play_ && current_status_frame_ind_ < size_of_sim_time_frames) {

		while (pause_model_play_) { std::this_thread::yield(); }

		std::chrono::time_point start_time = std::chrono::system_clock::now();
		size_of_current_status_frame = grid_points_[current_status_frame_ind_].size();

		{
			std::scoped_lock<std::mutex> update_data(GenericPanel::data_lock_mutex_);
			GenericPanel::vertices_.clear();
			GenericPanel::indices_.clear();
			GenericPanel::vertices_ = grid_points_[current_status_frame_ind_];
			GenericPanel::indices_ = grid_point_indices_[current_status_frame_ind_];
		}
		
		current_status_frame_ind_ += rel_index_offset_;
		
		std::scoped_lock<std::mutex> next_frame_delay_time_update_lock(next_frame_delay_time_update_mutex_);
				
		update_duration = std::chrono::system_clock::now() - start_time;
		
		// restricting fps rate to keep the played real fps rate close to target rate
		if (update_duration > next_frame_delay_time_) {
		
			rel_index_offset_ += (std::chrono::system_clock::now() - start_time) / next_frame_delay_time_;
		}
		else if (update_duration < next_frame_delay_time_ && rel_index_offset_ > 1) {
		
			rel_index_offset_ -= (next_frame_delay_time_ - (std::chrono::system_clock::now() - start_time)) / next_frame_delay_time_;
		}
		std::this_thread::sleep_for(next_frame_delay_time_);
		
#ifdef TEST_DYNAMIC_TIMING
		std::cout << "OFFSET_CORRECTION: " << rel_index_offset_ << std::endl;
		std::cout << "OF TIME_SHIFT(target time | relative shift): " << next_frame_delay_time_
			<< " | " << (std::chrono::system_clock::now() - start_time)  - next_frame_delay_time_ << std::endl;
		std::cout << "----------" << std::endl;
#endif// TEST_DYNAMIC_TIMING
	}

	current_status_frame_ind_ = 0;
	model_play_stopped_ = true;
	return;
}

void gui::HeatMapModelVisualizer::StartOrPauseLoadedModelPlay(const SDL_Event& sdl_event) {

	if (model_loaded_ && !current_status_frame_ind_ && sdl_event.key.keysym.sym == SDLK_SPACE) {
	
		pause_model_play_ = false;
		stop_model_play_ = false;
		std::thread model_player_thread(&gui::HeatMapModelVisualizer::ModelPlayerThread, this);
		model_player_thread.detach();
	}
	else if (model_loaded_ && sdl_event.key.keysym.sym == SDLK_SPACE && !pause_model_play_) {
	
		pause_model_play_ = true;
	}
	else if (model_loaded_ && sdl_event.key.keysym.sym == SDLK_SPACE && pause_model_play_) {
	
		pause_model_play_ = false;
	}
}

// call without SDL event, at object destruction
void gui::HeatMapModelVisualizer::StopModelPlay() {

	pause_model_play_ = false;
	stop_model_play_ = true;

	while (!model_play_stopped_) std::this_thread::yield();
}

// an improvement would a queue-based speed factor scheduler
void gui::HeatMapModelVisualizer::ChangePlaybackSpeed(const SDL_Event& sdl_event) {
		
	// Setting frame rate to avoid overwhelmingly and unnecessarily continual data play (high fps rate)
	//  which should cause slower playback. This must be done due to the high status/fps rate of
	//  computation algorithm in general.
	if (sdl_event.key.keysym.sym == SDLK_KP_PLUS
		&& next_frame_delay_time_ > next_frame_delay_time_lower_limit_ && !active_spd_mod_) {
	
		active_spd_mod_ = true;
		std::scoped_lock<std::mutex> next_frame_delay_time_update_lock(
			next_frame_delay_time_update_mutex_);
		next_frame_delay_time_ /= 2.0;
		std::cout << (1 / next_frame_delay_time_.count()) << " Hz playback frequency has set." << std::endl;
		active_spd_mod_ = false;
	}
	else if (sdl_event.key.keysym.sym == SDLK_KP_MINUS
		&& next_frame_delay_time_ < next_frame_delay_time_upper_limit_ && !active_spd_mod_) {
	
		active_spd_mod_ = true;
		std::scoped_lock<std::mutex> next_frame_delay_time_update_lock(
			next_frame_delay_time_update_mutex_);
		next_frame_delay_time_ *= 2.0;
		std::cout << (1 / next_frame_delay_time_.count()) << " Hz playback frequency has set." << std::endl;
		active_spd_mod_ = false;
	}
}
