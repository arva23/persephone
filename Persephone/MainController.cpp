#include "MainController.h"

MainController::MainController() {
	
	successful_config_init_ = false;
	rotation_precision_ = 1.0;
	zoom_precision_ = 1.0;
	translation_precision_ = 1.0;
	position_half_step_ = 0.5;
}

MainController::MainController(std::string config_file) {
	
	program_console_.Println("Initialization..");
	successful_config_init_ = true;


	// CREATING CONSOLE MENU OPTIONS
	program_console_.Println("Registering command options to console user interface manager.");

	program_console_ = ConsoleUI("PRINTER OPERATION OPTIMIZER", false);

	try {

		save_curr_sim_stat_menu_command_ = MenuCommand<MainController>(
			1, this, &MainController::SaveCurrSimStat,
			"[file_path/filename]", "Save status of recently paused simulation.Running simulations "
			"can not be saved. Pause or stop them before save.");
		program_console_.RegisterCommand("save-sim-stat", &save_curr_sim_stat_menu_command_);

		load_saved_sim_stat_menu_command_ = MenuCommand<MainController>(
			1, this, &MainController::LoadSavedSimStat,
			"[file_path/filename]", "Load previously saved simulation status.");
		program_console_.RegisterCommand("load-sim-stat", &load_saved_sim_stat_menu_command_);
		
		// 'exit' command is identical to internal default ConsoleUI exit command
		//  It only calls the user defined function in addition
		exit_program_menu_command_ = MenuCommand<MainController>(
			0, this, &MainController::ExitProgram,
			"", "Close application.");
		program_console_.RegisterCommand("exit", &exit_program_menu_command_);

		set_unit_step_menu_command_ = MenuCommand<MainController>(
			1, this, &MainController::SetUnitStep,
			"[new_value]", "Setting the metric unit step, adjust the distance between two thermal grid points.");
		program_console_.RegisterCommand("set-unit-step", &set_unit_step_menu_command_);

		schedule_model_menu_command_ = MenuCommand<MainController>(
			1, this, &MainController::ScheduleModel,
			"[file_path/filename]", "Loading new model for optimization.");
		program_console_.RegisterCommand("load-model", &schedule_model_menu_command_);

		run_sim_menu_command_ = MenuCommand<MainController>(
			0, this, &MainController::RunSimulation,
			"", "Play optimized printing simulation.");
		program_console_.RegisterCommand("run-sim", &run_sim_menu_command_);

		save_model_menu_command_ = MenuCommand<MainController>(
			0, this, &MainController::SaveResultIntoFile,
			"", "Save optimization result into a new file.");
		program_console_.RegisterCommand("save-model", &save_model_menu_command_);

		save_model_spec_menu_command_ = MenuCommand<MainController>(
			1, this, &MainController::SaveResultIntoFile,
			"[file_path/filename]", "Save optimization result into a user defined file with user "
			"defined filepath optionally.");
		program_console_.RegisterCommand("save-model-spec", &save_model_spec_menu_command_);

		cancel_opt_menu_command_ = MenuCommand<MainController>(
			0, this, &MainController::CancelOptimization,
			"", "Cancel gcode command sequence optimization.");
		program_console_.RegisterCommand("cancel-opt", &cancel_opt_menu_command_);

		suspend_opt_menu_command_ = MenuCommand<MainController>(
			0, this, &MainController::SuspendOptimization,
			"", "Suspend but NOT cancel optimization. Unfinished simulation can be played by 'run-sim' "
			"command. Calling this command more than once after each other has no effect.");
		program_console_.RegisterCommand("suspend-opt", &suspend_opt_menu_command_);

		continue_opt_menu_command_ = MenuCommand<MainController>(
			0, this, &MainController::ContinueOptimization,
			"", "Continue suspended optimization. Unfinished simulation can be continued to play. Calling "
			"this command more than once after each other has no effect.");
		program_console_.RegisterCommand("continue-opt", &continue_opt_menu_command_);
	}
	catch (std::exception err) {
	
		program_console_.Println("An error has occurred during menu option registrations.");
		program_console_.Println(err.what());
		successful_config_init_ = false;
	}

	genmath::LongDouble window_width;
	genmath::LongDouble window_height;
	genmath::String window_width_name;
	genmath::String window_height_name;
	genmath::String rotation_precision_name;
	genmath::String zoom_precision_name;
	genmath::String translation_precision_name;
	genmath::String position_half_step_name;
	genmath::String thermal_lower_bound_name;
	genmath::String thermal_upper_bound_name;
	
	std::vector<genmath::ObjectBase*> param_loader_list;
	
	// LOADING CONFIGURATION OF CURRENT PRINTER HARDWARE AND SOFTWARE PARAMETERS
	program_console_.Println("Loading parameters from configuration file (" + config_file + ")");
		
	FileControl config_file_;
	try {
		
		config_file_.ChangeSource("config.cfg", FileControl::R, true);
	}
	catch (std::exception err) {
		
		program_console_.Println("Can not open configuration file: " + std::string(err.what()));
		successful_config_init_ = false;
	}
	
	if (successful_config_init_) {

		/// simulation visualizer window parameters (GenericWindowRenderer)
		param_loader_list.push_back(&window_width_name);
		param_loader_list.push_back(&window_width);
		param_loader_list.push_back(&window_height_name);
		param_loader_list.push_back(&window_height);
		param_loader_list.push_back(&rotation_precision_name);
		param_loader_list.push_back(&rotation_precision_);
		param_loader_list.push_back(&zoom_precision_name);
		param_loader_list.push_back(&zoom_precision_);
		param_loader_list.push_back(&translation_precision_name);
		param_loader_list.push_back(&translation_precision_);
	
		/// simulation visualizer parameters (ModelVisualizer)
		param_loader_list.push_back(&position_half_step_name);
		param_loader_list.push_back(&position_half_step_);
		param_loader_list.push_back(&thermal_lower_bound_name);
		param_loader_list.push_back(&thermal_lower_bound_);
		param_loader_list.push_back(&thermal_upper_bound_name);
		param_loader_list.push_back(&thermal_upper_bound_);
		
		/// current higher level manager parameters (MainController)
		std::vector<genmath::ObjectBase*> printer_sequence_optimizer_param_name_list =
			printer_sequence_optimizer_.GetParamNameList();
		std::vector<genmath::ObjectBase*> printer_sequence_optimizer_param_list =
			printer_sequence_optimizer_.GetParamList();
		
		size_t param_loader_list_size = printer_sequence_optimizer_param_name_list.size();
		for (size_t i = 0; i < param_loader_list_size; ++i) {
		
			param_loader_list.push_back(printer_sequence_optimizer_param_name_list[i]);
			param_loader_list.push_back(printer_sequence_optimizer_param_list[i]);
		}
		
		try {
			config_file_.ReadLinesDefined(param_loader_list);
		}
		catch (std::exception err) {

			program_console_.Println("An error has occurred during reading configuration parameters from file.");
			program_console_.Println(err.what());
			successful_config_init_ = false;
		}
	}
	
	if (successful_config_init_) {
		
		bool corrections_were_needed = false;

		program_console_.Println("Validating loaded parameters..");

		if (window_width < genmath::LongDouble("800.0")) {
	
			program_console_.Println("Playback window width is less than the allowed minimum value (800). "
				"Setting default value (800)");
			window_width = "800.0";
			corrections_were_needed = true;
		}

		if (window_height < genmath::LongDouble("600.0")) {
			
			program_console_.Println("Playback window height is less than the allowed minimum value (600). "
				"Setting default value (600).");
			window_height = "600.0";
			corrections_were_needed = true;
		}

		if (printer_sequence_optimizer_.ValidateAndInitConfigParams())
			program_console_.Println("Parameter correction has set. See log for further information.");

		program_console_.Println("Creating model visualizer subsystem..");
		
		try {
			playback_presenter_ = new HeatMapModelVisualizer(window_width, window_height,
				position_half_step_, thermal_lower_bound_, thermal_upper_bound_,
				printer_sequence_optimizer_.GetEnvMediumCoeff(),
				printer_sequence_optimizer_.GetConsolidationTempBound());
	
		}
		catch (std::exception err) {
	
			program_console_.Println("Could not initiate playback persenter module (MainController).");
			program_console_.Println(err.what());
			successful_config_init_ = false;
		}
	
		try {
			panel_params_.push_back(playback_presenter_);
			
			playback_window_ = new GenericWindowRenderer(program_console_, rotation_precision_, 
				zoom_precision_, translation_precision_, panel_params_, window_width, window_height);

			playback_window_->Init();
	
		}
		catch (std::exception err) {
	
			program_console_.Println("Could not initiate playback window module (MainController).");
			program_console_.Println(err.what());
			successful_config_init_ = false;
		}
	}
	if (successful_config_init_) {
		
		program_console_.Println("Program has successfully initiated.");
	}

	program_console_.Println("--------------------------------------------------");
	program_console_.Println("Loaded parameters are the followings:");
	
	size_t size_of_config_params = param_loader_list.size();

	for (size_t i = 0; i < size_of_config_params - 1; i += 2)
		program_console_.Println("[" + static_cast<std::string>(*param_loader_list[i])
		+ std::string(" = ") + static_cast<std::string>(*param_loader_list[i + 1]) + std::string("]"));

	
}

MainController::MainController(const MainController& orig) {

	successful_config_init_ = orig.successful_config_init_;
	set_unit_step_menu_command_ = orig.set_unit_step_menu_command_;
	schedule_model_menu_command_ = orig.schedule_model_menu_command_;
	save_curr_sim_stat_menu_command_ = orig.save_curr_sim_stat_menu_command_;
	load_saved_sim_stat_menu_command_ = orig.load_saved_sim_stat_menu_command_;
	run_sim_menu_command_ = orig.run_sim_menu_command_;
	save_model_menu_command_ = orig.save_model_menu_command_;
	save_model_spec_menu_command_ = orig.save_model_spec_menu_command_;
	cancel_opt_menu_command_ = orig.cancel_opt_menu_command_;
	suspend_opt_menu_command_ = orig.suspend_opt_menu_command_;
	continue_opt_menu_command_ = orig.continue_opt_menu_command_;
	exit_program_menu_command_ = orig.exit_program_menu_command_;
	program_console_ = orig.program_console_;
	panel_params_ = orig.panel_params_;
	rotation_precision_ = orig.rotation_precision_;
	zoom_precision_ = orig.zoom_precision_;
	translation_precision_ = orig.translation_precision_;
	playback_window_ = orig.playback_window_;
	playback_presenter_ = orig.playback_presenter_;
	position_half_step_ = orig.position_half_step_;
	thermal_lower_bound_ = orig.thermal_lower_bound_;
	thermal_upper_bound_ = orig.thermal_upper_bound_;
	printer_sequence_optimizer_ = orig.printer_sequence_optimizer_;
	command_sequence_optimization_task_list_ = orig.command_sequence_optimization_task_list_;
}

MainController::~MainController() {

	delete playback_window_;
	delete playback_presenter_;
}

int8_t MainController::Run() {

	if (!successful_config_init_) return -1;

	// running threads:
	//  program console thread
	//  renderer thread
	//  intermittent model optimizer thread
	//  input monitoring thread
	//  model visualizer thread
	//  intermittent FDM3 threads

	//program_console_.RunConsole();
	processing_thread_ = std::thread(&MainController::ModelProcessingThread, this);
	console_thread_ = std::thread(&ConsoleUI::RunConsole, &program_console_);
	playback_window_->Render();
	console_thread_.join();
	
	return 0;
}

void MainController::ModelProcessingThread() {

	while (!exit_program_) {
	
		// optimization, simulation is thread safe by simulation status semaphores
		while (true) {
		
			std::scoped_lock<std::mutex> scheduled_model_list_lock(scheduled_model_list_mutex_);
			if (!command_sequence_optimization_task_list_.empty()) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	
		while (!command_sequence_optimization_task_list_.empty()) {
	
			// processing one model

			if (printer_sequence_optimizer_.GetSimulationActive()) {

				program_console_.Println("Model has been scheduled for further processing."
					"A model optimization has already been running. "
					"Type 'cancel-opt' to abort the current operation. ");
				return;
			}

			if (command_sequence_optimization_task_list_.front().Empty()) {

				program_console_.Println("Provided source file or file path can not be used.");
				return;
			}

			start_time_ = std::chrono::system_clock::now();

			printer_sequence_optimizer_.GetSimulationActive() = true;
			printer_sequence_optimizer_.GetSimulationSuspended() = false;

			try {

				printer_sequence_optimizer_.LoadCommandSequence(
					command_sequence_optimization_task_list_.front());
				program_console_.Println("Source model has loaded.");
				// cancelling parallel computation threads
				printer_sequence_optimizer_.StopSimulation();
				// and creating new FDM model with new threads
				
				printer_sequence_optimizer_.SeekCmdI(0);
				playback_window_->SuspendRendering();
				printer_sequence_optimizer_.LoadModelConfig();
				program_console_.Println("Local source model configuration has loaded.");
				
				printer_sequence_optimizer_.ProcessSourceModel();
				playback_window_->ContinueRendering();
			}
			catch (std::exception err) {

				program_console_.Println("An error has occurred during source model loading.");
				program_console_.Println(err.what());
				printer_sequence_optimizer_.GetSimulationActive() = false;
				printer_sequence_optimizer_.GetSimulationSuspended() = true;
				playback_window_->ContinueRendering();
				continue;
			}

			std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();

			program_console_.Println("Optimization went successfully ("
				+ std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
					end_time - start_time_ + start_time_offset_).count())
				+ std::string(")."));

			printer_sequence_optimizer_.GetSimulationActive() = false;
			printer_sequence_optimizer_.GetSimulationSuspended() = true;

			// autosaving optimized command sequences
			if(printer_sequence_optimizer_.GetCmdI() + 1 == printer_sequence_optimizer_.GetGCodeCommands()->size())
				this->SaveResultIntoFile(std::vector<std::string>());
		
			command_sequence_optimization_task_list_.pop_back();
		}

		program_console_.Println("All scheduled models have optimized. Nothing to do.");
	}
}

// MENU OPTIONS, COMMANDS
void MainController::SetUnitStep(const std::vector<std::string>& values) {

	if (values.size() != 1 || values[0].empty()) {
	
		program_console_.Println("Wrong command parameter list (no parameter, more parameters or empty list).");
		return;
	}

	genmath::LongDouble new_unit_space_step;
	
	if (new_unit_space_step = values[0]) {
	
		program_console_.Println("Failed to set new unit step value (probably wrong value format.)");
		return;
	}

	if (new_unit_space_step >= printer_sequence_optimizer_.GetLineWidth()) {
	
		program_console_.Println("Spatial unit step (unit space step) is greater than or equal to line width.");
		return;
	}

	printer_sequence_optimizer_.GetUnitSpaceStep() = new_unit_space_step;
	program_console_.Println("New unit step has set successfully. Reload previously loaded model (if it is) to "
		"take changes in effect.");
}

void MainController::ScheduleModel(const std::vector<std::string>& values) {

	if (values.size() < 1) {
		
		program_console_.Println("Wrong command parameter list (no parameter or empty list).");
		return;
	}

	std::scoped_lock<std::mutex> scheduled_model_list_lock(scheduled_model_list_mutex_);
	command_sequence_optimization_task_list_.insert(
		command_sequence_optimization_task_list_.begin(), values.begin(), values.end());
}


void MainController::SaveCurrSimStat(const std::vector<std::string>& values) {

	if (values.size() != 1 || values[0].empty()) {
	
		program_console_.Println("Wrong command parameter list (no parameter, more parameters or empty list).");
		return;
	}

	if (printer_sequence_optimizer_.GetSimulationActive()
		&& !printer_sequence_optimizer_.GetSimulationSuspended()) {
	
		program_console_.Println("Simulation is in progress (not stopped).");
		return;
	}

	// save simulation as a sequence 1D lines
	try {

		printer_sequence_optimizer_.GetSimulationActive() = true;
		printer_sequence_optimizer_.GetSimulationSuspended() = true;
	
		FileControl stat_saver(program_console_, values[0], FileControl::WT);
		
		// saving number of gcode commands value
		size_t size_of_gcode_commands = printer_sequence_optimizer_.GetNumOfGCodeCommands();
		genmath::LongDouble value = size_of_gcode_commands;
		stat_saver.WriteLineUndefined(&value);


		// saving partially modified gcode command sequence
		std::vector<GCodeCommand<genmath::LongDouble> >* gcode_commands = 
			printer_sequence_optimizer_.GetGCodeCommands();
		
		for (size_t i = 0; i < size_of_gcode_commands; ++i)
			stat_saver.WriteLineUndefined(&(*gcode_commands)[i]);
		
		
		// saving number of heat map time frames size
		std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* heat_map_time_frames = 
			printer_sequence_optimizer_.GetHeatMapTimeFrames();
		size_t size_of_time_frames = heat_map_time_frames->size();
		value = size_of_time_frames;
		stat_saver.WriteLineUndefined(&value);
		
		
		// preserving only z direction lines
		// saving thermal values of time frames
		size_t size_of_lines = printer_sequence_optimizer_.GetXLen() * printer_sequence_optimizer_.GetYLen();
		for(size_t t = 0; t < size_of_time_frames; ++t)
			for (size_t i = 0; i < size_of_lines; ++i)
				stat_saver.WriteLineUndefined(&(*heat_map_time_frames)[t][i]);

		// loading coeffs of heat map time frames
		// saving heat conduction coefficients of time frames
		std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* heat_map_time_frames_coeffs =
			printer_sequence_optimizer_.GetHeatMapTimeFramesCoeffs();

		for (size_t t = 0; t < size_of_time_frames; ++t)
			for (size_t i = 0; i < size_of_lines; ++i)
				stat_saver.WriteLineUndefined(&(*heat_map_time_frames_coeffs)[t][i]);


		// saving the next gcode command index after the recently processed one
		value = printer_sequence_optimizer_.GetCmdI();
		stat_saver.WriteLineUndefined(&value);


		// saving recent global spd parameter
		value = Section::MaxResSpd * genmath::LongDouble("60");
		stat_saver.WriteLineUndefined(&value);
	

		// saving recent elapsed time for optimization (individual, recently started and paused model processing)
		value = (std::chrono::system_clock::now() - start_time_ + start_time_offset_).count();
		stat_saver.WriteLineUndefined(&value);

		
		// saving scheduled models (for optimization)
		std::scoped_lock<std::mutex> scheduled_model_list_lock(scheduled_model_list_mutex_);
		std::list<genmath::String>::iterator scheduled_model = command_sequence_optimization_task_list_.begin();
		std::list<genmath::String>::iterator list_end = command_sequence_optimization_task_list_.end();
		for (; scheduled_model != list_end; ++scheduled_model)
			stat_saver.WriteLineUndefined(&(*scheduled_model));
	}
	catch (std::exception err) {
	
		program_console_.Println("An error has occurred during saving current simulation status.");
		program_console_.Println(err.what());
		printer_sequence_optimizer_.GetSimulationActive() = false;

		return;
	}

	program_console_.Println("Recent simulation status has been saved successfully.");
}

void MainController::LoadSavedSimStat(const std::vector<std::string>& values) {

	if (values.size() != 1 || values[0].empty()) {
	
		program_console_.Println("Wrong command parameter list (no parameter, more parameters or empty list).");
		return;
	}

	if (printer_sequence_optimizer_.GetSimulationActive()
		&& !printer_sequence_optimizer_.GetSimulationSuspended()) {
	
		program_console_.Println("Simulation is in progress (not stopped).");
		return;
	}

	try{

		printer_sequence_optimizer_.GetSimulationActive() = true;
		printer_sequence_optimizer_.GetSimulationSuspended() = true;
		playback_presenter_->StopModelPlay();
		playback_window_->SuspendRendering();
	
		FileControl stat_loader(program_console_, values[0], FileControl::R);
		

		// loading number of gcode commands value
		genmath::LongDouble value;
		stat_loader.ReadLineUndefined(&value);
		size_t size_of_gcode_commands = value;


		// loading partially modified gcode command sequence
		std::vector<GCodeCommand<genmath::LongDouble> >* gcode_commands = 
			printer_sequence_optimizer_.GetGCodeCommands();
		gcode_commands->clear();
		gcode_commands->resize(size_of_gcode_commands);
		for (size_t i = 0; i < size_of_gcode_commands; ++i)
			stat_loader.ReadLineUndefined(&(*gcode_commands)[i]);
		

		// loading model configuration
		printer_sequence_optimizer_.SeekCmdI(0);
		printer_sequence_optimizer_.LoadModelConfig();
		

		// loading number of heat map time frames size
		stat_loader.ReadLineUndefined(&value);
		size_t size_of_time_frames = value;
		size_t size_of_lines = printer_sequence_optimizer_.GetXLen() * printer_sequence_optimizer_.GetYLen();

		
		// loading thermal values of time frames
		std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* heat_map_time_frames =
			printer_sequence_optimizer_.GetHeatMapTimeFrames();
		heat_map_time_frames->clear();
		*heat_map_time_frames = std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >(
			size_of_time_frames, std::vector<genmath::Vector<genmath::LongDouble> >(
			printer_sequence_optimizer_.GetXLen() * printer_sequence_optimizer_.GetYLen(),
			genmath::Vector<genmath::LongDouble>(printer_sequence_optimizer_.GetZLen())));

		for (size_t t = 0; t < size_of_time_frames; ++t)
			for (size_t i = 0; i < size_of_lines; ++i)
				stat_loader.ReadLineUndefined(&(*heat_map_time_frames)[t][i]);

		printer_sequence_optimizer_.SetRecentLineImage();


		// loading heat conduction coefficients from the last time frame
		std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* heat_map_time_frames_coeffs =
			printer_sequence_optimizer_.GetHeatMapTimeFramesCoeffs();
		heat_map_time_frames_coeffs->clear();
		*heat_map_time_frames_coeffs =  std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >(
			size_of_time_frames, std::vector<genmath::Vector<genmath::LongDouble> >(
			printer_sequence_optimizer_.GetXLen() * printer_sequence_optimizer_.GetYLen(),
			genmath::Vector<genmath::LongDouble>(printer_sequence_optimizer_.GetZLen())));
		
		for (size_t t = 0; t < size_of_time_frames; ++t)
			for (size_t i = 0; i < size_of_lines; ++i)
				stat_loader.ReadLineUndefined(&(*heat_map_time_frames_coeffs)[t][i]);

		printer_sequence_optimizer_.SetRecentCoeffLineImage();

		
		// loading the next gcode command index after the recently processed one
		stat_loader.ReadLineUndefined(&value);
		// jump to the next unprocessed command in case of sequence processing continuation
		printer_sequence_optimizer_.SeekCmdI(value);



		// loading recent global spd parameter
		stat_loader.ReadLineUndefined(&value);
		Section::UpdateGlobalResSpd(value / genmath::LongDouble("60"));


		// loading elapsed time of recent operation fragment
		stat_loader.ReadLineUndefined(&value);
		std::chrono::duration<long double> start_time_offset(value);


		// loading recent elapsed time for optimization (individual, recently started and paused model processing)
		stat_loader.ReadLineUndefined(&value);
		start_time_offset_ = std::chrono::duration<long double>(value);


		// loading scheduled models
		std::scoped_lock<std::mutex> scheduled_model_list_lock(scheduled_model_list_mutex_);
		command_sequence_optimization_task_list_.push_back(genmath::String());

		while (!stat_loader.ReadLineUndefined(&command_sequence_optimization_task_list_.back()))
			command_sequence_optimization_task_list_.push_back(genmath::String());

		command_sequence_optimization_task_list_.pop_back();
	}
	catch (std::exception err) {
	
		program_console_.Println("An error has occurred during loading simulation status.");
		program_console_.Println(err.what());
		printer_sequence_optimizer_.GetSimulationActive() = false;

		playback_window_->ContinueRendering();
		return;
	}

	playback_window_->ContinueRendering();
	program_console_.Println("Simulation status has been loaded successfully.");
}

void MainController::ExitProgram(const std::vector<std::string>& values) {

	playback_presenter_->StopModelPlay();
	if(playback_window_->ActiveRendererObjectCount)
		playback_window_->CloseWindow();
	
	if(printer_sequence_optimizer_.GetSimulationActive())
		printer_sequence_optimizer_.StopSimulation();
	
	printer_sequence_optimizer_.GetSimulationSuspended() = false;

	if (printer_sequence_optimizer_.GetSimulationActive()) {
	
		printer_sequence_optimizer_.GetSimulationActive() = false;
		processing_thread_.detach();
	}
}

void MainController::RunSimulation(const std::vector<std::string>& values) {
	
	if (printer_sequence_optimizer_.GetSimulationActive() && !printer_sequence_optimizer_.GetSimulationSuspended()) {
	
		program_console_.Println("Simulation process is running. Pause or stop it before playback.");
		return;
	}

	if (printer_sequence_optimizer_.GetHeatMapTimeFrames()->empty()) {
	
		program_console_.Println("Nothing to play.");
		return;
	}
	
	std::chrono::time_point start_time = std::chrono::system_clock::now();
	program_console_.Println("Preparing for simulation playback..");
	
	try {
		
		playback_window_->SuspendRendering();
		playback_presenter_->LoadComputedModel(
			printer_sequence_optimizer_.GetXLen(), 
			printer_sequence_optimizer_.GetYLen(), 
			printer_sequence_optimizer_.GetZLen(),
			printer_sequence_optimizer_.GetTimeStep(),
			*printer_sequence_optimizer_.GetHeatMapTimeFrames(),
			*printer_sequence_optimizer_.GetHeatMapTimeFramesCoeffs());
		playback_window_->ContinueRendering();
	}
	catch (std::exception err) {
	
		program_console_.Println("An error has occurred during preparation for simulation playback.");
		program_console_.Println(err.what());
		return;
	}

	std::chrono::time_point end_time = std::chrono::system_clock::now();

	program_console_.Println("Preparation was successful ("
		+ std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count())
		+ std::string("). Ready to play."));
	// starting playback thread is triggered by keyboard event through ComplexInputEvent event handler
}

void MainController::SaveResultIntoFile(const std::vector<std::string>& values) {

	std::vector<GCodeCommand<genmath::LongDouble> >* gcode_commands =
		printer_sequence_optimizer_.GetGCodeCommands();

	if (values.size() != 1 || values[0].empty()) {

		program_console_.Println("File path and file name is empty. Setting default.");

		try {
			
			FileControl output_file(program_console_, "optimized_print_object.gcode", FileControl::WT);
			
			for (GCodeCommand command : (*gcode_commands))
				output_file.WriteLineUndefined(&command);
		}
		catch (std::exception err) {
		
			program_console_.Println("Can not save optimization result into the default result file.");
			program_console_.Println(err.what());
			return;
		}
	}
	else {

		try {
		
			FileControl output_file(program_console_, values[0] + "_optimized.gcode", FileControl::WT);

			for (GCodeCommand command : (*gcode_commands))
				output_file.WriteLineUndefined(&command);
		}
		catch (std::exception err) {
		
			program_console_.Println("Can not save optimization result into the user specified result file.");
			program_console_.Println(err.what());
			return;
		}
	}
}

void MainController::CancelOptimization(const std::vector<std::string>& values) {

	
	if (!printer_sequence_optimizer_.GetSimulationActive()) {
		
		program_console_.Println("No running simulation.");
		return;
	}
	
	printer_sequence_optimizer_.StopSimulation();
	playback_window_->ContinueRendering();
	printer_sequence_optimizer_.GetSimulationActive() = false;
	printer_sequence_optimizer_.GetSimulationSuspended() = false;
	processing_thread_.detach();

	program_console_.Println("Simulation has been terminating.");

	command_sequence_optimization_task_list_.pop_back();

}

void MainController::SuspendOptimization(const std::vector<std::string>& values) {

	if (!printer_sequence_optimizer_.GetSimulationActive()) {

		program_console_.Println("No model has been loaded for optimization.");
		return;
	}

	if (printer_sequence_optimizer_.GetSimulationSuspended()) {
	
		program_console_.Println("Simulation has already been suspended.");
		return;
	}

	printer_sequence_optimizer_.SuspendSimulation();
	playback_window_->ContinueRendering();
	printer_sequence_optimizer_.GetSimulationSuspended() = true;
	program_console_.Println("Optimization has suspended (simulation).");
}

void MainController::ContinueOptimization(const std::vector<std::string>& values) {

	if (!printer_sequence_optimizer_.GetSimulationActive()) {
	
		program_console_.Println("No model has been loaded for optimization.");
		return;
	}

	if (!printer_sequence_optimizer_.GetSimulationSuspended()) {
	
		program_console_.Println("Simulation has already been continued.");
		return;
	}

	printer_sequence_optimizer_.ContinueSimulation();
	playback_window_->SuspendRendering();
	printer_sequence_optimizer_.GetSimulationSuspended() = false;
	program_console_.Println("Optimization has continued (simulation).");
}
