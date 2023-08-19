#ifndef MAINCONTROLLER_H_INCLUDED
#define MAINCONTROLLER_H_INCLUDED
#define TEST_MODULES
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <initializer_list>
#include "genmath/ObjectBase.h"
#include "genmath/String.h"
#include "genmath/LongDouble.h"
#include "locresctl/FileControl.h"
#include "cli/ConsoleUI.h"
#include "gui/GenericPanel.h"
#include "gui/HeatMapModelVisualizer.h"
#include "gui/GenericWindowRenderer.h"
#include "PrinterSequenceOptimizer.h"
#include <forward_list>
// measurement unit declarations
//  metric distance unit is millimeters
//  angle unit is in degrees
//  time unit is seconds
//  thermal unit is in celsius degrees

class MainController {

public:
	MainController();

	MainController(std::string config_file);
	MainController(const MainController& orig);
	virtual ~MainController();

	int8_t Run();
#ifndef TEST_MODULES
private:
#endif
	// an instance of the program can only handle a single, constructed computation network at a time
	// for more computation network, create more instances of the program with different
	//  control hosts that do not funciton as a computation node

	void SetUnitStep(const std::vector<std::string>& values);
	// LoadModel function not only loads the file and converts into processable form for the
	//  optimization algorithm(s) but does the optimization as well
	void ScheduleModel(const std::vector<std::string>& values);
	//void LoadModel(const std::vector<std::string>& values);
	void ModelProcessingThread();
	
	std::mutex scheduled_model_list_mutex_;
	std::thread processing_thread_;
	std::atomic_bool exit_program_;
	std::chrono::system_clock::time_point start_time_;
	std::chrono::duration<long double> start_time_offset_;

	void SaveCurrSimStat(const std::vector<std::string>& values);
	void LoadSavedSimStat(const std::vector<std::string>& values);
	void RunSimulation(const std::vector<std::string>& values);
	// save optimization result into file
	void SaveResultIntoFile(const std::vector<std::string>& values);
	void CancelOptimization(const std::vector<std::string>& values);
	void SuspendOptimization(const std::vector<std::string>& values);
	void ContinueOptimization(const std::vector<std::string>& value);
	void ExitProgram(const std::vector<std::string>& values);

	bool successful_config_init_;

	/// Console user interface
	MenuCommand<MainController> set_unit_step_menu_command_;
	MenuCommand<MainController> schedule_model_menu_command_;
	MenuCommand<MainController> save_curr_sim_stat_menu_command_;
	MenuCommand<MainController> load_saved_sim_stat_menu_command_;
	MenuCommand<MainController> run_sim_menu_command_;
	MenuCommand<MainController> save_model_menu_command_;
	MenuCommand<MainController> save_model_spec_menu_command_;
	MenuCommand<MainController> cancel_opt_menu_command_;
	MenuCommand<MainController> suspend_opt_menu_command_;
	MenuCommand<MainController> continue_opt_menu_command_;
	MenuCommand<MainController> exit_program_menu_command_;

	ConsoleUI program_console_;
	std::thread console_thread_;

	
	/// simulation visualizer window parameters (GenericWindowRenderer)
	std::vector<GenericPanel*> panel_params_;

	// visual rotation sensitivity, PRECONFIGURED FROM FILE
	genmath::LongDouble rotation_precision_;
	// visual zoom sensitivity, PRECONFIGURED FROM FILE
	genmath::LongDouble zoom_precision_;
	// visual translation sensitivity, PRECONFIGURED FROM FILE
	genmath::LongDouble translation_precision_;
	HeatMapModelVisualizer* playback_presenter_;
	GenericWindowRenderer* playback_window_;
	

	/// simulation visualization parameters (ModelVisualizer)
	
	// PRECONFIGURED FROM FILE
	genmath::LongDouble position_half_step_;
	// PRECONFIGURED FROM FILE
	genmath::LongDouble thermal_lower_bound_;
	// PRECONFIGURED FROM FILE
	genmath::LongDouble thermal_upper_bound_;
	// PRECONFIGURED FROM FILE
	genmath::LongDouble thermal_step_;

	PrinterSequenceOptimizer printer_sequence_optimizer_;
	std::list<genmath::String> command_sequence_optimization_task_list_;
};
#endif// MAINCONTROLLER_H_INCLUDED
