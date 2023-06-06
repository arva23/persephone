#include "PrinterSequenceOptimizer.h"
PrinterSequenceOptimizer::PrinterSequenceOptimizer() {

	line_width_ = "1.0";
	rel_width_ind_limit_ = 0;
	rel_layer_ind_limit_ = 0;
	x_len_ = 0;
	y_len_ = 0;
	z_len_ = 0;
	cmd_i_ = 0;
	num_of_layers_ = 0;
	simulation_active_ = false;
	simulation_suspended_ = true;
	program_console_ = ConsoleUI();
}

PrinterSequenceOptimizer::PrinterSequenceOptimizer(ConsoleUI& program_console) {

	line_width_ = "1.0";
	rel_width_ind_limit_ = 0;
	rel_layer_ind_limit_ = 0;
	x_len_ = 0;
	y_len_ = 0;
	z_len_ = 0;
	cmd_i_ = 0;
	num_of_layers_ = 0;
	simulation_active_ = false;
	simulation_suspended_ = true;
	program_console_ = program_console;
}
PrinterSequenceOptimizer::PrinterSequenceOptimizer(const PrinterSequenceOptimizer& orig) {

	program_console_ = orig.program_console_;
	line_width_name_ = orig.line_width_name_;
	unit_space_step_name_ = orig.unit_space_step_name_;
	rel_nozzle_dia_name_ = orig.rel_nozzle_dia_name_;
	rel_nozzle_height_name_ = orig.rel_nozzle_height_name_;
	extruded_material_avg_temp_offset_name_ = orig.extruded_material_avg_temp_offset_name_;
	nozzle_surface_avg_temp_offset_name_ = orig.nozzle_surface_avg_temp_offset_name_;
	consolidation_temp_bound_offset_name_ = orig.consolidation_temp_bound_offset_name_;
	nozzle_mat_melting_temp_name_ = orig.nozzle_mat_melting_temp_name_;
	material_cond_coeff_name_ = orig.material_cond_coeff_name_;
	build_plate_temp_name_ = orig.build_plate_temp_name_;
	build_plate_coeff_name_ = orig.build_plate_coeff_name_;
	env_temp_name_ = orig.env_temp_name_;
	env_medium_coeff_name_ = orig.env_medium_coeff_name_;
	min_stepper_rev_spd_name_ = orig.min_stepper_rev_spd_name_;
	line_width_ = orig.line_width_;
	layer_height_ = orig.layer_height_;
	rel_width_ind_limit_ = orig.rel_width_ind_limit_;
	rel_layer_ind_limit_ = orig.rel_layer_ind_limit_;
	unit_space_step_ = orig.unit_space_step_;
	x_offset_ = orig.x_offset_;
	y_offset_ = orig.y_offset_;
	build_volume_width_ = orig.build_volume_width_;
	build_volume_height_ = orig.build_volume_height_;
	build_volume_length_ = orig.build_volume_length_;
	x_len_ = orig.x_len_;
	y_len_ = orig.y_len_;
	z_len_ = orig.z_len_;
	rel_nozzle_dia_ = orig.rel_nozzle_dia_;
	rel_nozzle_height_ = orig.rel_nozzle_height_;
	rel_nozzle_hor_ind_limit_ = orig.rel_nozzle_hor_ind_limit_;
	rel_nozzle_vert_ind_limit_ = orig.rel_nozzle_vert_ind_limit_;
	extruded_material_avg_temp_ = orig.extruded_material_avg_temp_;
	nozzle_surface_avg_temp_ = orig.nozzle_surface_avg_temp_;
	consolidation_temp_bound_ = orig.consolidation_temp_bound_;
	material_cond_coeff_ = orig.material_cond_coeff_;
	build_plate_temp_ = orig.build_plate_temp_;
	build_plate_coeff_ = orig.build_plate_coeff_;
	env_temp_ = orig.env_temp_;
	env_medium_coeff_ = orig.env_medium_coeff_;
	disc_x_lines_ = orig.disc_x_lines_;
	disc_y_lines_ = orig.disc_y_lines_;
	disc_z_lines_ = orig.disc_z_lines_;
	disc_x_lines_coeffs_ = orig.disc_x_lines_coeffs_;
	disc_y_lines_coeffs_ = orig.disc_y_lines_coeffs_;
	disc_z_lines_coeffs_ = orig.disc_z_lines_coeffs_;
	unit_time_step_ = orig.unit_time_step_;
	thermal_space_model_ = orig.thermal_space_model_;
	heat_map_time_frames_ = orig.heat_map_time_frames_;
	heat_map_time_frames_coeffs_ = orig.heat_map_time_frames_coeffs_;
	proj_sect_hist_ = orig.proj_sect_hist_;
	proj_sect_ = orig.proj_sect_;
	gcode_commands_ = orig.gcode_commands_;
	cmd_i_ = orig.cmd_i_;
	num_of_layers_ = orig.num_of_layers_;
	simulation_active_ = orig.simulation_active_ ? true : false;
	simulation_suspended_ = orig.simulation_suspended_ ? true : false;
}

PrinterSequenceOptimizer& PrinterSequenceOptimizer::operator=(const PrinterSequenceOptimizer& orig) {

	line_width_name_ = orig.line_width_name_;
	unit_space_step_name_ = orig.unit_space_step_name_;
	rel_nozzle_dia_name_ = orig.rel_nozzle_dia_name_;
	rel_nozzle_height_name_ = orig.rel_nozzle_height_name_;
	extruded_material_avg_temp_offset_name_ = orig.extruded_material_avg_temp_offset_name_;
	nozzle_surface_avg_temp_offset_name_ = orig.nozzle_surface_avg_temp_offset_name_;
	consolidation_temp_bound_offset_name_ = orig.consolidation_temp_bound_offset_name_;
	nozzle_mat_melting_temp_name_ = orig.nozzle_mat_melting_temp_name_;
	material_cond_coeff_name_ = orig.material_cond_coeff_name_;
	build_plate_temp_name_ = orig.build_plate_temp_name_;
	build_plate_coeff_name_ = orig.build_plate_coeff_name_;
	env_temp_name_ = orig.env_temp_name_;
	env_medium_coeff_name_ = orig.env_medium_coeff_name_;
	min_stepper_rev_spd_name_ = orig.min_stepper_rev_spd_name_;
	line_width_ = orig.line_width_;
	layer_height_ = orig.layer_height_;
	rel_width_ind_limit_ = orig.rel_width_ind_limit_;
	rel_layer_ind_limit_ = orig.rel_layer_ind_limit_;
	unit_space_step_ = orig.unit_space_step_;
	x_offset_ = orig.x_offset_;
	y_offset_ = orig.y_offset_;
	build_volume_width_ = orig.build_volume_width_;
	build_volume_height_ = orig.build_volume_height_;
	build_volume_length_ = orig.build_volume_length_;
	x_len_ = orig.x_len_;
	y_len_ = orig.y_len_;
	z_len_ = orig.z_len_;
	rel_nozzle_dia_ = orig.rel_nozzle_dia_;
	rel_nozzle_height_ = orig.rel_nozzle_height_;
	rel_nozzle_hor_ind_limit_ = orig.rel_nozzle_hor_ind_limit_;
	rel_nozzle_vert_ind_limit_ = orig.rel_nozzle_vert_ind_limit_;
	extruded_material_avg_temp_ = orig.extruded_material_avg_temp_;
	nozzle_surface_avg_temp_ = orig.nozzle_surface_avg_temp_;
	consolidation_temp_bound_ = orig.consolidation_temp_bound_;
	material_cond_coeff_ = orig.material_cond_coeff_;
	build_plate_temp_ = orig.build_plate_temp_;
	build_plate_coeff_ = orig.build_plate_coeff_;
	env_temp_ = orig.env_temp_;
	env_medium_coeff_ = orig.env_medium_coeff_;
	disc_x_lines_ = orig.disc_x_lines_;
	disc_y_lines_ = orig.disc_y_lines_;
	disc_z_lines_ = orig.disc_z_lines_;
	disc_x_lines_coeffs_ = orig.disc_x_lines_coeffs_;
	disc_y_lines_coeffs_ = orig.disc_y_lines_coeffs_;
	disc_z_lines_coeffs_ = orig.disc_z_lines_coeffs_;
	unit_time_step_ = orig.unit_time_step_;
	thermal_space_model_ = orig.thermal_space_model_;
	heat_map_time_frames_ = orig.heat_map_time_frames_;
	heat_map_time_frames_coeffs_ = orig.heat_map_time_frames_coeffs_;
	proj_sect_hist_ = orig.proj_sect_hist_;
	proj_sect_ = orig.proj_sect_;
	gcode_commands_ = orig.gcode_commands_;
	cmd_i_ = orig.cmd_i_;
	num_of_layers_ = orig.num_of_layers_;
	simulation_active_ = orig.simulation_active_ ? true : false;
	simulation_suspended_ = orig.simulation_suspended_ ? true : false;

	return *this;
}

PrinterSequenceOptimizer::~PrinterSequenceOptimizer() {

	if (proj_sect_hist_.empty()) {

		size_t size_of_container = proj_sect_hist_.size();

		for (size_t i = 0; i < size_of_container; ++i)
			delete proj_sect_hist_[i];
	}

	if (proj_sect_.empty()) {

		size_t size_of_container = proj_sect_.size();

		for (size_t i = 0; i < size_of_container; ++i)
			delete proj_sect_[i];
	}
}

std::vector<genmath::ObjectBase*> PrinterSequenceOptimizer::GetParamNameList() {

	std::vector<genmath::ObjectBase*> param_loader_name_list;

	param_loader_name_list.push_back(&line_width_name_);
	param_loader_name_list.push_back(&unit_space_step_name_);
	param_loader_name_list.push_back(&rel_nozzle_dia_name_);
	param_loader_name_list.push_back(&rel_nozzle_height_name_);
	param_loader_name_list.push_back(&extruded_material_avg_temp_offset_name_);
	param_loader_name_list.push_back(&nozzle_surface_avg_temp_offset_name_);
	param_loader_name_list.push_back(&consolidation_temp_bound_offset_name_);
	param_loader_name_list.push_back(&nozzle_mat_melting_temp_name_);
	param_loader_name_list.push_back(&material_cond_coeff_name_);
	param_loader_name_list.push_back(&build_plate_temp_name_);
	param_loader_name_list.push_back(&build_plate_coeff_name_);
	param_loader_name_list.push_back(&env_temp_name_);
	param_loader_name_list.push_back(&env_medium_coeff_name_);
	param_loader_name_list.push_back(&min_stepper_rev_spd_name_);

	return param_loader_name_list;
}

std::vector<genmath::ObjectBase*> PrinterSequenceOptimizer::GetParamList() {

	std::vector<genmath::ObjectBase*> param_loader_list;

	param_loader_list.push_back(&line_width_);
	param_loader_list.push_back(&unit_space_step_);
	param_loader_list.push_back(&rel_nozzle_dia_);
	param_loader_list.push_back(&rel_nozzle_height_);
	param_loader_list.push_back(&extruded_material_avg_temp_offset_);
	param_loader_list.push_back(&nozzle_surface_avg_temp_offset_);
	param_loader_list.push_back(&consolidation_temp_bound_offset_);
	param_loader_list.push_back(&nozzle_mat_melting_temp_);
	param_loader_list.push_back(&material_cond_coeff_);
	param_loader_list.push_back(&build_plate_temp_);
	param_loader_list.push_back(&build_plate_coeff_);
	param_loader_list.push_back(&env_temp_);
	param_loader_list.push_back(&env_medium_coeff_);
	param_loader_list.push_back(&Section::MinResSpd);

	return param_loader_list;
}

bool PrinterSequenceOptimizer::ValidateAndInitConfigParams() {

	bool corrections_were_needed = false;

	if (line_width_ <= genmath::LongDouble("0.0")) {

		program_console_.Println("Line width is equal to or less than zero. Setting default value (0.4 mm).");
		line_width_ = "0.4";
		corrections_were_needed = true;
	}

	rel_width_ind_limit_ = static_cast<unsigned long>(line_width_ / unit_space_step_);

	if (unit_space_step_ <= genmath::LongDouble("0.0")) {

		program_console_.Println("Spatial unit step (unit space step) is equal to or less than zero. "
			"Setting default value (half of the line width).");
		unit_space_step_ = line_width_ / genmath::LongDouble("2.0");
		corrections_were_needed = true;
	}

	if (unit_space_step_ > line_width_) {

		program_console_.Println("Spatial unit step (unit space step) is greater than line width. "
			"Setting default value (half of the line width).");
		unit_space_step_ = line_width_ / genmath::LongDouble("2.0");
		corrections_were_needed = true;
	}

	if (rel_nozzle_dia_ < line_width_) {

		program_console_.Println("Heated nozzle diameter is inappropriate. It is less than the diameter "
			"of line width. Setting default value (twice of line width).");
		rel_nozzle_dia_ = genmath::LongDouble("2.0") * line_width_;
		corrections_were_needed = true;
	}

	rel_nozzle_hor_ind_limit_ = static_cast<unsigned long>(rel_nozzle_dia_ / unit_space_step_);

	if (rel_nozzle_height_ < unit_space_step_) {

		program_console_.Println("Heated nozzle height is inappropriate. It is less than the unit space "
			"step. Setting default value (unit_space step).");
		rel_nozzle_height_ = unit_space_step_;
		corrections_were_needed = true;
	}

	rel_nozzle_vert_ind_limit_ = static_cast<unsigned long>(rel_nozzle_height_ / unit_space_step_);

	if (extruded_material_avg_temp_offset_ <= genmath::LongDouble("0.0")) {

		program_console_.Println("Extruded material average temperature offset is equal to or less than "
			"zero. Setting default value (20 C - in case of use of ABS plastic).");
		extruded_material_avg_temp_offset_ = "20.0";
		corrections_were_needed = true;
	}


	if (nozzle_surface_avg_temp_offset_ <= genmath::LongDouble("0.0")) {

		program_console_.Println("Average of surface temperature offset of nozzle is equal to or less than zero. "
			"Setting default value (env_temp + 1.0)");
		nozzle_surface_avg_temp_offset_ = env_temp_ - genmath::LongDouble("1.0");
		corrections_were_needed = true;
	}


	if (consolidation_temp_bound_offset_ <= genmath::LongDouble("0.0")) {

		program_console_.Println("Material consolidation boundary temperature offset is less than or equal to "
			"zero. Setting default value (extruded material offset temperature + 2 degrees).");
		consolidation_temp_bound_offset_ = extruded_material_avg_temp_offset_ + genmath::LongDouble("2.0");
		corrections_were_needed = true;
	}

	// nozzle material melting point investigation is not neccessary, it could be under zero or zero


	if (Section::MinResSpd < genmath::LongDouble("0.0001")) {

		program_console_.Println("Minimum stepper induced axial speed is less than the minimum value "
			"(0.0001 mm/s). Setting default value (0.0001).");
		Section::MinResSpd = "0.0001";
		corrections_were_needed = true;
	}

	extruded_material_avg_temp_ = nozzle_mat_melting_temp_ - extruded_material_avg_temp_offset_;
	nozzle_surface_avg_temp_ = nozzle_mat_melting_temp_ - nozzle_surface_avg_temp_offset_;
	consolidation_temp_bound_ = nozzle_mat_melting_temp_ - consolidation_temp_bound_offset_;

	cmd_i_ = 0;
	num_of_layers_ = 0;
	simulation_active_ = false;
	simulation_suspended_ = true;

	return false;
}

genmath::LongDouble& PrinterSequenceOptimizer::GetEnvMediumCoeff() {

	return env_medium_coeff_;
}

genmath::LongDouble& PrinterSequenceOptimizer::GetConsolidationTempBound() {

	return consolidation_temp_bound_;
}

genmath::LongDouble& PrinterSequenceOptimizer::GetLineWidth() {

	return line_width_;
}

genmath::LongDouble& PrinterSequenceOptimizer::GetUnitSpaceStep() {

	return unit_space_step_;
}

bool& PrinterSequenceOptimizer::GetSimulationActive() {

	return simulation_active_;
}

bool& PrinterSequenceOptimizer::GetSimulationSuspended() {

	return simulation_suspended_;
}

void PrinterSequenceOptimizer::StopSimulation() {

	thermal_space_model_.StopSimulation();
}

void PrinterSequenceOptimizer::SuspendSimulation() {

	thermal_space_model_.SuspendSimulation();
}

void PrinterSequenceOptimizer::ContinueSimulation() {

	thermal_space_model_.ContinueSimulation();
}

void PrinterSequenceOptimizer::SeekCmdI(size_t cmd_i) {

	if (cmd_i >= gcode_commands_.size())
		throw std::exception("Command index is out of range (PrinterSequenceOptimizer).");

	cmd_i_ = cmd_i;
}

size_t PrinterSequenceOptimizer::GetCmdI() {

	return cmd_i_;
}

size_t PrinterSequenceOptimizer::GetNumOfGCodeCommands() {

	return gcode_commands_.size();
}

std::vector<GCodeCommand<genmath::LongDouble> >* PrinterSequenceOptimizer::GetGCodeCommands() {

	return &gcode_commands_;
}

std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* PrinterSequenceOptimizer::GetHeatMapTimeFrames() {

	return &heat_map_time_frames_;
}

std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* PrinterSequenceOptimizer::GetHeatMapTimeFramesCoeffs() {

	return &heat_map_time_frames_coeffs_;
}

size_t PrinterSequenceOptimizer::GetXLen() {

	return x_len_;
}

size_t PrinterSequenceOptimizer::GetYLen() {

	return y_len_;
}

size_t PrinterSequenceOptimizer::GetZLen() {

	return z_len_;
}

void PrinterSequenceOptimizer::SetRecentLineImage() {

	disc_z_lines_ = heat_map_time_frames_.back();
}

void PrinterSequenceOptimizer::SetRecentCoeffLineImage() {

	disc_z_lines_coeffs_ = heat_map_time_frames_coeffs_.back();
}

genmath::LongDouble PrinterSequenceOptimizer::GetTimeStep() {

	return thermal_space_model_.GetTimeStep();
}

// MODEL PROCESSING METHODS

void PrinterSequenceOptimizer::LoadCommandSequence(std::string file_path) {

	try {

		input_file_.ChangeSource(file_path, FileControl::R, true);
	}
	catch (std::exception err) {

		program_console_.Println("Can not open file, probably file path is wrong or access rights are restricted.");
		program_console_.Println(err.what());
		return;
	}

	gcode_commands_.clear();
	gcode_commands_.push_back(GCodeCommand<genmath::LongDouble>());

	try {

		while (!input_file_.ReadLineUndefined(&gcode_commands_.back()))
			gcode_commands_.push_back(GCodeCommand<genmath::LongDouble>());
	}
	catch (std::exception err) {

		program_console_.Println("File operation error at reading command sequence file.");
		gcode_commands_.clear();
		throw err;
	}

	gcode_commands_.pop_back();
}

bool PrinterSequenceOptimizer::ValidateLocalEnvTempVals(size_t curr_x, size_t curr_y, size_t curr_z,
	size_t* err_x, size_t* err_y, size_t* err_z) {

	size_t abs_loc_x_end = rel_width_ind_limit_ / 2 + curr_x + 1;
	size_t abs_loc_y_end = rel_width_ind_limit_ / 2 + curr_y + 1;

	size_t abs_loc_z_end = curr_z - rel_layer_ind_limit_ - rel_layer_ind_limit_;



	for (signed long abs_loc_z = curr_z; abs_loc_z >= abs_loc_z_end; --abs_loc_z) {

		for (size_t abs_loc_y = curr_y - rel_width_ind_limit_ / 2; abs_loc_y <= abs_loc_y_end; ++abs_loc_y) {

			for (size_t abs_loc_x = curr_x - rel_width_ind_limit_ / 2; abs_loc_x <= abs_loc_x_end; ++abs_loc_x) {

				if (disc_z_lines_[(abs_loc_x)*y_len_ + abs_loc_y][abs_loc_z] > consolidation_temp_bound_) {

					*err_x = abs_loc_x;
					*err_y = abs_loc_y;
					*err_z = abs_loc_z;
					return true;
				}
			}
		}
	}

	return false;
}

void inline PrinterSequenceOptimizer::SetupLocalEnvironmentalPoints(size_t curr_x, size_t curr_y, size_t curr_z) {

	// Dimensional range examination is not necessary due to environmental
	//  layer additions and printer workspace reductions by the slicer software.
	// Line width is constant due to dynamic extrusion acceleration and speed adjustment
	//  respect to kinetic properties of printing head.

	// at even limit, all points will be shifted into certain direction, translation will be occurred

	// cube shaped printing line representation (initial version of geometric representation)

	size_t abs_loc_x_end = rel_width_ind_limit_ / 2 + curr_x + 1;
	size_t abs_loc_y_end = rel_width_ind_limit_ / 2 + curr_y + 1;

	for (size_t abs_loc_z = curr_z - rel_layer_ind_limit_; abs_loc_z <= curr_z; ++abs_loc_z) {

		for (size_t abs_loc_y = curr_y - rel_width_ind_limit_ / 2; abs_loc_y <= abs_loc_y_end; ++abs_loc_y) {

			for (size_t abs_loc_x = curr_x - rel_width_ind_limit_ / 2; abs_loc_x <= abs_loc_x_end; ++abs_loc_x) {

				disc_z_lines_[abs_loc_x * y_len_ + abs_loc_y][abs_loc_z] = extruded_material_avg_temp_;
				disc_z_lines_coeffs_[abs_loc_x * y_len_ + abs_loc_y][abs_loc_z] = material_cond_coeff_;
			}
		}
	}

	size_t abs_loc_x_nozzl_end = rel_nozzle_hor_ind_limit_ / 2 + curr_x;
	size_t abs_loc_y_nozzl_end = rel_nozzle_hor_ind_limit_ / 2 + curr_y;

	for (size_t abs_loc_z = curr_z + 1; abs_loc_z <= rel_nozzle_vert_ind_limit_; ++abs_loc_z) {

		for (size_t abs_loc_y = curr_y - rel_nozzle_hor_ind_limit_ / 2; abs_loc_y <= abs_loc_y_nozzl_end; ++abs_loc_y) {

			for (size_t abs_loc_x = curr_x - rel_nozzle_hor_ind_limit_ / 2; abs_loc_x <= abs_loc_x_nozzl_end; ++abs_loc_x) {

				disc_z_lines_[abs_loc_x * y_len_ + abs_loc_y][abs_loc_z] = nozzle_surface_avg_temp_;
				disc_z_lines_coeffs_[abs_loc_x * y_len_ + abs_loc_y][abs_loc_z] = env_medium_coeff_;
			}
		}
	}
}

void PrinterSequenceOptimizer::PointProcessing(size_t& curr_x_ind, size_t& curr_y_ind, size_t& curr_z_ind) {

	// ignoring the first layer, only build plate adhesion is there
	if (curr_z_ind > rel_layer_ind_limit_ + rel_layer_ind_limit_) [[likely]] {

		// error correction on printing speeds by equated duration compensation
		genmath::LongDouble err_corr_dur = "0.0";

		// Coordinate shadowing problem occurs if multiple sections have the same point in the same
		// layer. This happens when slicer generated command sequence is not optimal, indistinct
		// paths or the configure optimizer line width is too big. At this case the algorithm finds
		// the first occurrence of the coordinate in the line history list by the previous line sections.
		size_t err_z_ind, err_y_ind, err_x_ind;
		bool err_pnt_found = false;

		while (this->ValidateLocalEnvTempVals(curr_x_ind, curr_y_ind, curr_z_ind,
			&err_x_ind, &err_y_ind, &err_z_ind) && simulation_active_) {

			++thermal_space_model_;
			heat_map_time_frames_.push_back(disc_z_lines_);
			heat_map_time_frames_coeffs_.push_back(disc_z_lines_coeffs_);

			// first version of environmental heat dissipation method
			// consant default values after each evaluation of heat map evaluation

			//this->ResetBoundaryValues();// boundary value reset is turned off
			err_corr_dur += unit_time_step_;
		}

		if (!simulation_active_) return;

		size_t size_of_proj_sect_hist = proj_sect_hist_.size();

		if (err_corr_dur != genmath::LongDouble("0.0") && size_of_proj_sect_hist) {

			// suppose, that the first occurrence of the point is the last point of the previous layer
			size_t redundancy_ind = size_of_proj_sect_hist - 1;

			genmath::LongDouble err_dist_x, err_dist_y, err_dist_z, quadratic_dist;
			genmath::LongDouble curr_quadratic_ref_err_dist = layer_height_ * layer_height_ + line_width_ * line_width_;

			for (signed long i = size_of_proj_sect_hist - 1; i >= 0; --i) {

				err_dist_x = genmath::LongDouble(err_x_ind) * unit_space_step_ + x_offset_ - proj_sect_hist_[i]->end_x_;
				err_dist_y = genmath::LongDouble(err_y_ind) * unit_space_step_ + y_offset_ - proj_sect_hist_[i]->end_y_;
				quadratic_dist = err_dist_x * err_dist_x + err_dist_y * err_dist_y + err_dist_z * err_dist_z;

				if (quadratic_dist <= curr_quadratic_ref_err_dist) {

					redundancy_ind = i;
					break;
				}
			}

			// printing duration of history sections
			genmath::LongDouble hist_sect_dur = proj_sect_.back()->end_time_ - proj_sect_.front()->start_time_
				+ proj_sect_hist_[size_of_proj_sect_hist - 1]->end_time_ - proj_sect_hist_[redundancy_ind]->start_time_;

			// speed modifier (ratio)
			genmath::LongDouble spd_ratio_mod = hist_sect_dur / (hist_sect_dur + err_corr_dur);

			// slowing down the previous layer
			for (; redundancy_ind < size_of_proj_sect_hist; ++redundancy_ind) {

				// since the simulation computes the fastest average speed
				//  for section, speed incrementation is not available
				// slowing the printing speed down in each previous section
				try {

					proj_sect_hist_[redundancy_ind]->ConfigureSpeed(spd_ratio_mod);
				}
				catch (std::exception err) {

					throw std::exception((std::string("Invalid printing speed was adjusted on recent layer."
						"\nTerminating optimization (PrinterSequenceOptimizer).\n") + std::string(err.what())).c_str());
				}
			}

			// decresing speed in the current unfinished layer
			size_t size_of_proj_sect = proj_sect_.size();
			for (size_t i = 0; i < size_of_proj_sect; ++i) {

				try {

					proj_sect_[i]->ConfigureSpeed(spd_ratio_mod);
				}
				catch (std::exception err) {

					throw std::exception((std::string("Invalid printing speed was adjusted on recent layer. "
							"Terminating optimization (PrinterSequenceOptimizer).\n") + std::string(err.what())).c_str());
				}
			}
		}
	}
}

// loop body of simulation of unrecorded static point (generated) procedure
void PrinterSequenceOptimizer::ProcessCurrUnitDurSect(genmath::LongDouble& start_virt_x, genmath::LongDouble& start_virt_y,
	genmath::LongDouble& end_virt_x, genmath::LongDouble& end_virt_y, size_t& curr_z_ind, Section* sect) {

	std::string msg;

	// CREATING DISCRETE SECTION POINTS FROM LINEARLY INTERPOLATED SECTION IN UNIT TIME STEP DURATION
	// discretized gridpoint assignment, discretization only in loop, minimizing the error
	size_t curr_x_ind, curr_y_ind;

	genmath::LongDouble curr_virt_c = "0.0";
	genmath::LongDouble end_virt_c = "0.0";
	genmath::LongDouble dir = sect->x_dir_;
	if (dir < genmath::LongDouble("0.0")) curr_virt_c = start_virt_x - end_virt_x;
	else end_virt_c = end_virt_x - start_virt_x;
	genmath::LongDouble dir_aux = sect->y_dir_;
	genmath::LongDouble ratio_x = sect->ratio_x_;
	genmath::LongDouble ratio_y = sect->ratio_y_;

	if (end_virt_x - start_virt_x == genmath::LongDouble("0.0")) [[unlikely]] {

		dir = sect->y_dir_;
		if (dir < genmath::LongDouble("0.0")) curr_virt_c = start_virt_y - end_virt_y;
		else end_virt_c = end_virt_y - start_virt_y;
		dir_aux = sect->x_dir_;
	}

	if (dir < genmath::LongDouble("0.0")) {

		for (; curr_virt_c > end_virt_c; curr_virt_c -= unit_space_step_) {

			curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
				curr_virt_c * ratio_x - x_offset_ + start_virt_x, unit_space_step_)
				/ unit_space_step_);

			curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
				curr_virt_c * dir_aux * ratio_y - y_offset_ + start_virt_y, unit_space_step_)
				/ unit_space_step_);

#ifdef TEST_MODULES
			program_console_.Println(std::string("(") + std::to_string(curr_x_ind) + std::string(", ")
				+ std::to_string(curr_y_ind) + std::string(", ") + std::to_string(curr_z_ind) + std::string(")"));
#endif
			this->PointProcessing(curr_x_ind, curr_y_ind, curr_z_ind);
			this->SetupLocalEnvironmentalPoints(curr_x_ind, curr_y_ind, curr_z_ind);
		}
	}
	else {

		for (; curr_virt_c < end_virt_c; curr_virt_c += unit_space_step_) {

			curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
				curr_virt_c * ratio_x - x_offset_ + start_virt_x, unit_space_step_)
				/ unit_space_step_);

			curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
				curr_virt_c * dir_aux * ratio_y - y_offset_ + start_virt_y, unit_space_step_)
				/ unit_space_step_);

#ifdef TEST_MODULES

			program_console_.Println(std::string("(") + std::to_string(curr_x_ind) + std::string(", ") 
				+ std::to_string(curr_y_ind) + std::string(", ") + std::to_string(curr_z_ind) + std::string(")"));
#endif
			this->PointProcessing(curr_x_ind, curr_y_ind, curr_z_ind);
			this->SetupLocalEnvironmentalPoints(curr_x_ind, curr_y_ind, curr_z_ind);
		}
	}
}

// used only at initialization due to thermal instabilities
void PrinterSequenceOptimizer::ResetBoundaryValues() {

	// Boundary value settings occur at each iteration during heat map conduction evaluation (simulation)
	// oscillation of environmental boundary values with condition of energy level decrementation
	for (size_t y = 0; y < y_len_; ++y) {

		for (size_t x = 0; x < x_len_; ++x) {

			if (disc_z_lines_[x * y_len_ + y][0] < build_plate_temp_)
				disc_z_lines_[x * y_len_ + y][0] = build_plate_temp_;
			if (disc_z_lines_[x * y_len_ + y][z_len_ - 1] < env_temp_)
				disc_z_lines_[x * y_len_ + y][z_len_ - 1] = env_temp_;
		}
	}

	for (size_t z = 1; z < z_len_; ++z) {

		for (size_t y = 0; y < y_len_; ++y) {

			if (disc_z_lines_[0 * y_len_ + y][z] < env_temp_)
				disc_z_lines_[0 * y_len_ + y][z] = env_temp_;
			if (disc_z_lines_[(x_len_ - 1) * y_len_ + y][z] < env_temp_)
				disc_z_lines_[(x_len_ - 1) * y_len_ + y][z] = env_temp_;
		}
	}

	for (size_t z = 1; z < z_len_; ++z) {

		for (size_t x = 0; x < x_len_; ++x) {

			if (disc_z_lines_[x * y_len_ + 0][z] < env_temp_)
				disc_z_lines_[x * y_len_ + 0][z] = env_temp_;
			if (disc_z_lines_[x * y_len_ + y_len_ - 1][z] < env_temp_)
				disc_z_lines_[x * y_len_ + y_len_ - 1][z] = env_temp_;
		}
	}
}

void PrinterSequenceOptimizer::InitiateThermalSimulation() {

	// INITIALIZING SPATIAL THERMAL VALUES AT t0 time
	// reduced dimensional parameters must be lower or equal than the printer workspace volume parameters
	//  this is fulfilled by the slicer program

	// Since at least three points have to be in 1D for heat simulation.
	//  2 points are not a case, because the created geometry would suffer precision loss by rounding
	//  coordinate values. In this case, heat simulation must be satisfied with at least 3 points.
	// Adding more two gridpoints at each dimension for environmental boundaries
	x_len_ = 3 + static_cast<unsigned long>(build_volume_width_ / unit_space_step_);
	y_len_ = 3 + static_cast<unsigned long>(build_volume_length_ / unit_space_step_);
	z_len_ = 3 + static_cast<unsigned long>(build_volume_height_ / unit_space_step_);

	program_console_.Println(std::string("X range: ") + std::to_string(x_len_));
	program_console_.Println(std::string("Y range: ") + std::to_string(y_len_));
	program_console_.Println(std::string("Z range: ") + std::to_string(z_len_));

	// cuboid space simulation
	// (optimization would be the dynamic, object surface boundary-based space formation)
	disc_x_lines_ = std::vector<genmath::Vector<genmath::LongDouble> >(
		z_len_ * y_len_, genmath::Vector<genmath::LongDouble>(x_len_));

	disc_y_lines_ = std::vector<genmath::Vector<genmath::LongDouble> >(
		z_len_ * x_len_, genmath::Vector<genmath::LongDouble>(y_len_));

	// simulation starts with this dimension specific container
	disc_z_lines_ = std::vector<genmath::Vector<genmath::LongDouble> >(
		x_len_ * y_len_, genmath::Vector<genmath::LongDouble>(z_len_, env_temp_));

	disc_x_lines_coeffs_ = std::vector<genmath::Vector<genmath::LongDouble> >(
		z_len_ * y_len_, genmath::Vector<genmath::LongDouble>(x_len_, env_medium_coeff_));

	disc_y_lines_coeffs_ = std::vector<genmath::Vector<genmath::LongDouble> >(
		z_len_ * x_len_, genmath::Vector<genmath::LongDouble>(y_len_, env_medium_coeff_));

	disc_z_lines_coeffs_ = std::vector<genmath::Vector<genmath::LongDouble> >(
		x_len_ * y_len_, genmath::Vector<genmath::LongDouble>(z_len_, env_medium_coeff_));

	// constant build plate heat conduction coefficient initialization
	for (size_t y = 0; y < y_len_; ++y) {

		for (size_t x = 0; x < x_len_; ++x) {

			disc_x_lines_coeffs_[y][x] = build_plate_coeff_;
			disc_y_lines_coeffs_[x][y] = build_plate_coeff_;
			disc_z_lines_coeffs_[x * y_len_ + y][0] = build_plate_coeff_;
		}
	}

	// setting default values for medium boundary values and build plate
	this->ResetBoundaryValues();

	// deallocation of previous session (model processing)
	size_t size_of_container = proj_sect_hist_.size();
	for (size_t i = 0; i < size_of_container; ++i)
		delete proj_sect_hist_[i];

	proj_sect_hist_.clear();

	size_of_container = proj_sect_.size();

	for (size_t i = 0; i < size_of_container; ++i)
		delete proj_sect_[i];

	proj_sect_.clear();
	heat_map_time_frames_.clear();
	heat_map_time_frames_.push_back(disc_z_lines_);// inserting first heat space status
	heat_map_time_frames_coeffs_.push_back(disc_z_lines_coeffs_);// inserting first heat space status

	// creating new spatial simulation model
	thermal_space_model_ = FDM3<genmath::LongDouble>(x_len_, y_len_, z_len_, &disc_x_lines_, &disc_y_lines_,
		&disc_z_lines_, &disc_x_lines_coeffs_, &disc_y_lines_coeffs_, &disc_z_lines_coeffs_, unit_space_step_);

	program_console_.Println("Heat map and FDM3 init. was successful.");
}

void PrinterSequenceOptimizer::LoadModelConfig() {

	// LOADING WORK OBJECT SPECIFIC PARAMETERS

	// operating parameters
	//  MINX - lower bound of x coordinates
	//  MINY - lower bound of y coordinates
	//  MINZ - lower bound of z coordinates
	//  MAXX - upper bound of x coordinates
	//  MAXY - upper bound of y coordinates
	//  MAXZ - upper bound of z coordinates
	//  M140 - setting build plate temperature
	//  M104 - setting noozle print head temperature
	//  M201 - setting up acceleration
	//  M205 - setting up jerk
	//  the rest of the commands are negligible except G0 and G1
	uint8_t operating_params_configured = 0;// if all parameters are loaded, it is 11

	size_t size_of_gcode_commands = gcode_commands_.size();
	proj_sect_hist_.reserve(size_of_gcode_commands);
	proj_sect_.reserve(size_of_gcode_commands);

	GCodeCommand<genmath::LongDouble>* curr_cmd;

	// declared as member to assure availability of save option and load option of simulation status
	//size_t cmd_i = 0;

	// setup runs in case of unfinished, continued processing as well
	for (; cmd_i_ < size_of_gcode_commands && operating_params_configured < 11; ++cmd_i_) {

		curr_cmd = &gcode_commands_[cmd_i_];
		if (curr_cmd->GetName().size() >= 6 && curr_cmd->GetName().substr(0, 6) == ";MINX:") {

			x_offset_ = curr_cmd->GetName().substr(6, curr_cmd->GetName().size() - 6);
			x_offset_ -= rel_nozzle_dia_ / genmath::LongDouble("2.0");
			++operating_params_configured;
			program_console_.Println(std::string("MINX: ") + static_cast<std::string>(x_offset_));
		}
		else if (curr_cmd->GetName().size() >= 6 && curr_cmd->GetName().substr(0, 6) == ";MINY:") {

			y_offset_ = curr_cmd->GetName().substr(6, curr_cmd->GetName().size() - 6);
			y_offset_ -= rel_nozzle_dia_ / genmath::LongDouble("2.0");
			++operating_params_configured;
			program_console_.Println(std::string("MINY: ") + static_cast<std::string>(y_offset_));
		}
		else if (curr_cmd->GetName().size() >= 6 && curr_cmd->GetName().substr(0, 6) == ";MINZ:") {

			layer_height_ = curr_cmd->GetName().substr(6, curr_cmd->GetName().size() - 6);
			rel_layer_ind_limit_ = static_cast<unsigned long>(layer_height_ / unit_space_step_);
			++operating_params_configured;
			program_console_.Println(std::string("layer height: ") + static_cast<std::string>(layer_height_));
		}
		else if (curr_cmd->GetName().size() >= 6 && curr_cmd->GetName().substr(0, 6) == ";MAXX:") {

			build_volume_width_ = curr_cmd->GetName().substr(6, curr_cmd->GetName().size() - 6);
			build_volume_width_ += rel_nozzle_dia_ / genmath::LongDouble("2.0");
			build_volume_width_ -= x_offset_;
			++operating_params_configured;
			program_console_.Println(std::string("build volume width: ") 
				+ static_cast<std::string>(build_volume_width_));
		}
		else if (curr_cmd->GetName().size() >= 6 && curr_cmd->GetName().substr(0, 6) == ";MAXY:") {

			build_volume_length_ = curr_cmd->GetName().substr(6, curr_cmd->GetName().size() - 6);
			build_volume_length_ += rel_nozzle_dia_ / genmath::LongDouble("2.0");
			build_volume_length_ -= y_offset_;
			++operating_params_configured;
			program_console_.Println(std::string("build volume length: ") 
				+ static_cast<std::string>(build_volume_length_));
		}
		else if (curr_cmd->GetName().size() >= 6 && curr_cmd->GetName().substr(0, 6) == ";MAXZ:") {

			build_volume_height_ = curr_cmd->GetName().substr(6, curr_cmd->GetName().size() - 6);
			++operating_params_configured;
			program_console_.Println(std::string("build volume height: ") 
				+ static_cast<std::string>(build_volume_height_));

		}
		else if (curr_cmd->GetName().size() >= 4 && curr_cmd->GetName() == "M140") {

			// setting build plate heating temperature
			build_plate_temp_ = curr_cmd->Get("S");
			++operating_params_configured;
			program_console_.Println(std::string("build plate temperature: ") 
				+ static_cast<std::string>(build_plate_temp_));
		}
		else if (curr_cmd->GetName().size() >= 4 && curr_cmd->GetName() == "M104") {

			// setting extruder head heating temperature
			nozzle_mat_melting_temp_ = curr_cmd->Get("S");
			extruded_material_avg_temp_ = nozzle_mat_melting_temp_ - extruded_material_avg_temp_offset_;
			nozzle_surface_avg_temp_ = nozzle_mat_melting_temp_ - nozzle_surface_avg_temp_offset_;
			consolidation_temp_bound_ = nozzle_mat_melting_temp_ - consolidation_temp_bound_offset_;

			++operating_params_configured;
			program_console_.Println(std::string("nozzle material melting temperature:: ")
				+ static_cast<std::string>(nozzle_mat_melting_temp_));
			program_console_.Println(std::string("extruded material average temperature: ")
				+ static_cast<std::string>(extruded_material_avg_temp_));
			program_console_.Println(std::string("material consolidation temperature bound: ")
				+ static_cast<std::string>(consolidation_temp_bound_));
		}
		else if (curr_cmd->GetName().size() >= 4 && curr_cmd->GetName() == "M201") {

			Section::XAcc = curr_cmd->Get("X");
			Section::YAcc = curr_cmd->Get("Y");

			program_console_.Println(std::string("Linear section global x acceleration: ")
				+ static_cast<std::string>(Section::XAcc));

			program_console_.Println(std::string("Linear section global y acceleration: ")
				+ static_cast<std::string>(Section::YAcc));

			++operating_params_configured;
		}
		else if (curr_cmd->GetName().size() >= 4 && curr_cmd->GetName() == "M205") {

			// setup jerk
			Section::XJerk = curr_cmd->Get("X");
			Section::YJerk = curr_cmd->Get("Y");

			program_console_.Println(std::string("Linear section global x jerk for initial speed: ")
				+ static_cast<std::string>(Section::XJerk));

			program_console_.Println(std::string("Linear section global y jerk for initial speed: ")
				+ static_cast<std::string>(Section::YJerk));

			++operating_params_configured;
		}
		else if (curr_cmd->GetName().size() >= 13 && curr_cmd->GetName().substr(0, 13) == ";LAYER_COUNT:") {

			num_of_layers_ = std::atoi(curr_cmd->GetName().substr(13).c_str());
			program_console_.Println(std::string("Number of layers: ") + std::to_string(num_of_layers_));

			++operating_params_configured;
		}
		else if (curr_cmd->GetName().size() >= 8
			&& curr_cmd->GetName().substr(0, 8) == ";LAYER:0") {

			// ignoring command until first layer
			break;
		}
	}

	if (operating_params_configured == 11) {

		Section::Init();// initiating kinematic limits

		program_console_.Println("Local printing parameters loaded.");
		this->InitiateThermalSimulation();
		program_console_.Println("Initial spatial heat simulation model has initiated.");
	}
	else {

		throw std::exception("Not all local printing parameters are loaded. Parameter configuration "
			"failure (PrinterSequenceOptimizer).");
		return;
	}
}

void PrinterSequenceOptimizer::ProcessSourceModel() {

	size_t size_of_gcode_commands = gcode_commands_.size();

	// OPTIMIZATION OF COMMAND SEQUENCE (OPERATES WITH LINEAR INTERPOLATION OF COMMAND SEQUENCE)
	genmath::LongDouble print_exec_time = "0.0";
	genmath::LongDouble prev_z_pos = "0.0";// previous layer z position
	genmath::LongDouble curr_z_pos = "0.0";// current layer z position
	bool first_layer_point = true;
	size_t curr_x_ind = 1;// current gridpoint based discretized x coordinate index
	size_t curr_y_ind = 1;// current gridpoint based discretized y coordinate index
	size_t curr_z_ind = 1;// current gridpoint based discretized z coordinate index
	Section* curr_sect = nullptr;// the currently processed linear section, init.
	unit_time_step_ = thermal_space_model_.GetTimeStep();

	if (!size_of_gcode_commands)
		throw std::exception("Command sequence does not contain at least two commands. "
			"Termination optimization (PrinterSequenceOptimizer).");

	size_t layer_i = 0;
	GCodeCommand<genmath::LongDouble>* curr_cmd = nullptr;

	for (; cmd_i_ < size_of_gcode_commands && simulation_active_; ++cmd_i_) {

		// suspension of simulation is non-deterministic, IoC is not used due to unpredictability
		while (simulation_suspended_) std::this_thread::yield();

		if (!simulation_active_) return;

		curr_cmd = &gcode_commands_[cmd_i_];

		if (curr_cmd->GetName().size() >= 2 && 
			(curr_cmd->GetName() == "G0" || curr_cmd->GetName() == "G1") && curr_cmd->IsSet("Z")) [[unlikely]] {

			//refresh layer height and absolute z position
			prev_z_pos = curr_z_pos;
			curr_z_pos = curr_cmd->Get("Z");
			layer_height_ = curr_z_pos - prev_z_pos;

			program_console_.Println(std::string("Layer ") + std::to_string(layer_i));
			++layer_i;
			first_layer_point = true;
		}

		if (curr_cmd->GetName().size() >= 2 && curr_cmd->GetName() == "G0" 
			&& !curr_cmd->IsSet("E") && curr_cmd->IsSet("X") && curr_cmd->IsSet("Y")) {

			// special G0 command, quick positioning to the next coordinate
			if (first_layer_point) {

				curr_sect = new Section(curr_cmd);

				// deallocating unsed objects (no further use, layer history depth is only 1)
				size_t size_of_container = proj_sect_hist_.size();
				for (size_t i = 0; i < size_of_container; ++i)
					delete proj_sect_hist_[i];

				proj_sect_hist_ = proj_sect_;
				proj_sect_.clear();
				first_layer_point = false;
			}
			else {

				curr_sect->UpdateSection(curr_cmd, unit_time_step_);
				print_exec_time += curr_sect->end_time_;
				proj_sect_.push_back(curr_sect);
				curr_sect = new Section(*curr_sect);
				curr_sect->SetPrevSect(proj_sect_.back());
			}
		}
		else if (curr_cmd->GetName().size() >= 2 && (curr_cmd->GetName() == "G0" || curr_cmd->GetName() == "G1")) [[likely]] {

			if (curr_cmd->IsSet("F")) [[unlikely]] {

				Section::UpdateGlobalResSpd(genmath::LongDouble(curr_cmd->Get("F")) / genmath::LongDouble("60"));
			}

			// LAYER CONSOLIDATION CONTROL
			if (curr_cmd->IsSet("X") && curr_cmd->IsSet("Y")) [[likely]] {

				// Indices are exception safe by the slicer dimensional guard conditions. Coordinates
				// must be in the printing zone

				// Increasing the indices with +1 according to constant medium point representation
				curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
					genmath::LongDouble(curr_cmd->Get("X")) - x_offset_, unit_space_step_) / unit_space_step_);

				curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
					genmath::LongDouble(curr_cmd->Get("Y")) - y_offset_, unit_space_step_) / unit_space_step_);

				curr_z_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
					curr_z_pos, unit_space_step_) / unit_space_step_);

#ifdef TEST_MODULES
				program_console_.Println(std::string("X") + std::to_string(curr_x_ind) 
					+ std::string(" Y") + std::to_string(curr_y_ind) 
					+ std::string(" Z") + std::to_string(curr_z_ind));
#endif
				if (first_layer_point) {

					first_layer_point = false;
					// breaking physical contact with previous layer by resetting the current section default
					curr_sect = new Section(curr_cmd);

					// clear and swap the recent layer section history with the currently finished
					size_t size_of_container = proj_sect_hist_.size();
					for (size_t i = 0; i < size_of_container; ++i)
						delete proj_sect_hist_[i];

					proj_sect_hist_ = proj_sect_;
					proj_sect_.clear();
				}
				else {

					curr_sect->UpdateSection(curr_cmd, unit_time_step_);
					print_exec_time += curr_sect->end_time_;
					proj_sect_.push_back(curr_sect);
					curr_sect = new Section(*curr_sect);
					curr_sect->SetPrevSect(proj_sect_.back());

					// PRINTING PATH SIMULATION BY UNIT TIME STEPS
					// (simulating points that are not in the original source gcode command sequence)
					// time interval ending do not cause time redundancy due to symmetric rounding
					genmath::LongDouble sect_end_time = genmath::LongDouble::RawRound(curr_sect->end_time_, unit_time_step_);
					genmath::LongDouble curr_time = genmath::LongDouble::RawRound(curr_sect->start_time_, unit_time_step_);
					genmath::LongDouble start_virt_x, start_virt_y, end_virt_x, end_virt_y;// virtualized points
					// generated virtual point between two gcode command based coordinate in projective form
					std::pair<genmath::LongDouble, genmath::LongDouble> virt_point;
					bool first_virt_point = true;

					for (; curr_time < sect_end_time; curr_time += unit_time_step_) {

						virt_point = (*curr_sect)(curr_time);// generating next linear section based point

						if (first_virt_point) {

							start_virt_x = virt_point.first;
							start_virt_y = virt_point.second;
							first_virt_point = false;

							this->PointProcessing(curr_x_ind, curr_y_ind, curr_z_ind);
							this->SetupLocalEnvironmentalPoints(curr_x_ind, curr_y_ind, curr_z_ind);
						}
						else {

							end_virt_x = virt_point.first;
							end_virt_y = virt_point.second;

							this->ProcessCurrUnitDurSect(start_virt_x, start_virt_y, end_virt_x,
								end_virt_y, curr_z_ind, curr_sect);

							start_virt_x = end_virt_x;
							start_virt_y = end_virt_y;
						}
					}
				}
			}
		}
		else if (layer_i == num_of_layers_ - 1 && curr_cmd->GetName().size() >= 13
			&& curr_cmd->GetName().substr(0, 13) == ";TIME_ELAPSED") {

			// terminating processing
			program_console_.Println("Simulated print execution time: " 
				+ std::to_string(print_exec_time) + std::string(" s"));
			// skipping auxiliary commands
			break;
		}
		else if (layer_i >= num_of_layers_) {

			program_console_.Println("Simulation could not process all the commands due to failure of processing "
				"enough predetermined number of layers.");
			throw std::exception("Number of layers differs from exact number of layers (PrinterSequenceOptimizer).");
		}
	}

	if (simulation_active_) {

		program_console_.Println("Presliced model based gcode command sequence optimization was successful.");
		program_console_.Println("Choose 'run-sim' to see the result or 'save-model' to save the optimized sequence "
			"into a new file.");
	}
}
