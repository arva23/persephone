#define RELEASE_MODE
#ifndef RELEASE_MODE
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "gui/GenericWindowRenderer.h"
#include "cli/ConsoleUI.h"
#include "MainController.h"
#include "genmath/LongDouble.h"
struct ConsoleUITests {

	static void TestConsole() {
	
		ConsoleUI test_object;

		// zero number of command options
		try {

			test_object.RunConsole();
		}
		catch (std::exception err) {

			std::cout << err.what() << std::endl;
		}

		TestHandler handler_object_;
		MenuCommand<TestHandler> test_cmd_menu_command_ = MenuCommand<TestHandler>(1, &handler_object_,
			&TestHandler::TestHandlerFunction, "[parameter0]", "this is a test for proper handler function call");
		test_object.RegisterCommand("test-cmd", &test_cmd_menu_command_);

		// testing reduandancy check at menu command subscription
		try {
		
			test_object.RegisterCommand("test-cmd", &test_cmd_menu_command_);
		}
		catch (std::exception err) {
		
			if (err.what() == std::string("The requested subscription of command option already "
				"exists in the name of given command parameter."))
				std::cout << "Successful redundancy avoidance." << std::endl;
		}


		test_object.RunConsole();
		// calling a non-exsisting command
		// type non-exsisting command parameter..

		// calling existing command with inappropriate parameter list..
		// type exsisting command option with inappropriate parameter..

		// testing arbitrary message printing
		test_object.Println("test message");
	}
};

struct InputControlTests {

	static void ObjectConstructionTest() {
	
		InputControl test_object;
	}

	static void RunAndDestructionTest() {
	
		InputControl test_object;
		test_object.Run();
		GenericWindowRenderer::ActiveRendererObjectCount = 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		GenericWindowRenderer::ActiveRendererObjectCount = 0;
	}
};

struct GenericWindowRendererTests {

	static void GenericRenderingTest() {
	
		// testing GenericWindowRenderer with ComplexInputManagement SDL event manager

		GenericPanel panel_0 = GenericPanel(0, 800, 0, 600);
		std::vector<GenericPanel*> panels;
		panels.push_back(&panel_0);
		ConsoleUI program_console;
		GenericWindowRenderer test_object = GenericWindowRenderer(
			program_console, 0.2, 0.2, 0.2, panels, 800, 600);
		test_object.Init();
		test_object.Render();
	}

	static void GenericRenderingExitTest() {
	
		GenericPanel panel_0 = GenericPanel(0, 800, 0, 600);
		std::vector<GenericPanel*> panels;
		panels.push_back(&panel_0);
		ConsoleUI program_console;
		GenericWindowRenderer test_object = GenericWindowRenderer(
			program_console, 0.2, 0.2, 0.2, panels, 800, 600);
		std::thread exit_delay([&test_object]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			test_object.CloseWindow();
		});

		test_object.Init();
		test_object.Render();
		exit_delay.join();
	}
};

struct ModelVisualizerTests {

	static void SingleStaticObjectTest() {
	
		using T = genmath::LongDouble;
		std::vector<std::vector<genmath::Vector<T> > > test_model(
			1, std::vector<genmath::Vector<T> >(1, genmath::Vector<T>(1, T("180"))));
		std::vector<std::vector<genmath::Vector<T> > > test_model_coeffs(
			1, std::vector<genmath::Vector<T> >(1, genmath::Vector<T>(1, T("0.17"))));
		HeatMapModelVisualizer* panel_0 = new HeatMapModelVisualizer(800, 600, 0.5, 20.0, 250.0, T("0.1"), T("190"));
		std::vector<GenericPanel*> panels;
		panels.push_back(panel_0);
		ConsoleUI program_console;
		GenericWindowRenderer test_object = GenericWindowRenderer(
			program_console, 0.2, 0.2, 0.2, panels, 800, 600);
		test_object.Init();
		std::cout << "Window renderer has loaded." << std::endl;
		std::thread load_model([&panel_0, &test_model, &test_model_coeffs]() {

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			panel_0->LoadComputedModel(1, 1, 1, 1.0, test_model, test_model_coeffs);
			std::cout << "Model loaded." << std::endl;
		});
		test_object.Render();

		while (true) { std::this_thread::yield(); }
	}

	static void SingleDynamicObjectTest() {
	
		using T = genmath::LongDouble;
		std::vector<std::vector<genmath::Vector<T> > > test_model(
			10000, std::vector<genmath::Vector<T> >(1, genmath::Vector<T>(1, T("200"))));
		std::vector<std::vector<genmath::Vector<T> > > test_model_coeffs(
			10000, std::vector<genmath::Vector<T> >(1, genmath::Vector<T>(1, T("0.17"))));
		
		for (size_t t = 0; t < 10000; t += 2) {
		
			test_model[t][0][0] = genmath::LongDouble("100");
			test_model_coeffs[t][0][0] = genmath::LongDouble("0.17");
		}

		HeatMapModelVisualizer* panel_0 = new HeatMapModelVisualizer(800, 600, 0.5, 20.0, 250.0, T("0.1"), T("190"));
		std::vector<GenericPanel*> panels;
		panels.push_back(panel_0);
		ConsoleUI program_console;
		GenericWindowRenderer test_object = GenericWindowRenderer(
			program_console, 0.2, 0.2, 0.2, panels, 800, 600);
		test_object.Init();
		std::cout << "Window renderer has loaded." << std::endl;
		std::thread load_model([&panel_0, &test_model, &test_model_coeffs, &test_object]() {

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			panel_0->LoadComputedModel(1, 1, 1, 0.5, test_model, test_model_coeffs);
			std::cout << "Model loaded." << std::endl;
		});
		test_object.Render();

		while (true) { std::this_thread::yield(); }
	}

	static void MultipleStaticObjectTest(){
	
		using T = genmath::LongDouble;

		size_t sim_duration = 1;
		size_t number_of_unit_vols = 20;

		std::vector<std::vector<genmath::Vector<T> > > test_model(sim_duration, std::vector<genmath::Vector<T> >(
			number_of_unit_vols * number_of_unit_vols, genmath::Vector<T>(number_of_unit_vols, T("20.0"))));

		std::vector<std::vector<genmath::Vector<T> > > test_model_coeffs(sim_duration, std::vector<genmath::Vector<T> >(
			number_of_unit_vols * number_of_unit_vols, genmath::Vector<T>(number_of_unit_vols, T("0.1"))));

		for (size_t t = 0; t < sim_duration; ++t) {

			for (size_t x = 0; x < number_of_unit_vols; ++x) {

				for (size_t y = 0; y < number_of_unit_vols; ++y) {

					for (size_t z = 0; z < number_of_unit_vols; ++z) {

						if (x % 2) {

							test_model[t][x * number_of_unit_vols + y][z] = "120";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (y % 3) {

							test_model[t][x * number_of_unit_vols + y][z] = "140";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (z % 4) {
							test_model[t][x * number_of_unit_vols + y][z] = "180";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (x % 5) {

							test_model[t][x * number_of_unit_vols + y][z] = "200";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (y % 6) {

							test_model[t][x * number_of_unit_vols + y][z] = "230";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (z % 7) {

							test_model[t][x * number_of_unit_vols + y][z] = "250";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
					}
				}
			}
		}


		HeatMapModelVisualizer* panel_0 = new HeatMapModelVisualizer(800, 600, 0.5f, 25.0, 250.0, T("0.1"), T("190"));
		std::vector<GenericPanel*> panels;
		panels.push_back(panel_0);
		ConsoleUI program_console;
		GenericWindowRenderer test_object = GenericWindowRenderer(
			program_console, 0.2, 0.2, 0.2, panels, 800, 600);
		test_object.Init();
		std::cout << "Window renderer has loaded." << std::endl;
		std::thread load_model([&panel_0, &test_model, &test_model_coeffs, &number_of_unit_vols]() {

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			panel_0->LoadComputedModel(number_of_unit_vols, number_of_unit_vols, number_of_unit_vols,
				0.5, test_model, test_model_coeffs);
		});
		test_object.Render();
		std::cout << "Model loaded." << std::endl;

		while (true) { std::this_thread::yield(); }
	}

	static void MultipleDynamicObjectTest() {
	
		using T = genmath::LongDouble;

		size_t sim_duration = 100;
		size_t number_of_unit_vols = 29;

		std::vector<std::vector<genmath::Vector<T> > > test_model(sim_duration, std::vector<genmath::Vector<T> >(
			number_of_unit_vols * number_of_unit_vols, genmath::Vector<T>(number_of_unit_vols, T("150.0"))));

		std::vector<std::vector<genmath::Vector<T> > > test_model_coeffs(sim_duration, std::vector<genmath::Vector<T> >(
			number_of_unit_vols * number_of_unit_vols, genmath::Vector<T>(number_of_unit_vols, T("0.1"))));

		for (size_t t = 0; t < sim_duration; ++t) {

			for (size_t x = 0; x < number_of_unit_vols; ++x) {

				for (size_t y = 0; y < number_of_unit_vols; ++y) {

					for (size_t z = 0; z < number_of_unit_vols; ++z) {

						if (x % 2 && t % 2) {

							test_model[t][x * number_of_unit_vols + y][z] = "100";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (y % 3 && t % 2) {

							test_model[t][x * number_of_unit_vols + y][z] = "120";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (z % 4 && t % 2) {
							test_model[t][x * number_of_unit_vols + y][z] = "140";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (x % 5 && t % 2) {

							test_model[t][x * number_of_unit_vols + y][z] = "180";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (y % 6 && t % 2) {

							test_model[t][x * number_of_unit_vols + y][z] = "200";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
						else if (z % 7 && t % 2) {

							test_model[t][x * number_of_unit_vols + y][z] = "240";
							test_model_coeffs[t][x * number_of_unit_vols + y][z] = "0.17";
						}
					}
				}
			}
		}


		HeatMapModelVisualizer* panel_0 = new HeatMapModelVisualizer(800, 600, 0.5f, 20.0, 250.0, T("0.1"), T("190"));
		std::vector<GenericPanel*> panels;
		panels.push_back(panel_0);
		ConsoleUI program_console;
		GenericWindowRenderer test_object = GenericWindowRenderer(
			program_console, 0.2, 0.2, 0.2, panels, 800, 600);
		test_object.Init();
		std::cout << "Window renderer has loaded." << std::endl;
		std::thread load_model([&panel_0, &test_model, &test_model_coeffs, &number_of_unit_vols]() {

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			panel_0->LoadComputedModel(number_of_unit_vols, number_of_unit_vols, number_of_unit_vols, 0.5,
				test_model, test_model_coeffs);
			std::cout << "Model loaded." << std::endl;
		});
		test_object.Render();

		while (true) { std::this_thread::yield(); }
	}
};

struct MainControllerTests {

	static void SaveCurrSimStatTest() {
	
		// using LoadSavedSimStatTest content for simpler test code
		// NOTE: THE CONSISTENCY OF THE FOLLOWING DATA IS NOT GUARANTEED
		// IT IS ONLY FOR TEST, CONDITIONS ARE NOT TESTED
		MainController test_object;
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.5";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.env_medium_coeff_ = "0.0255";
		test_object.printer_sequence_optimizer_.build_plate_coeff_ = "1.5";
		Section::MinResSpd = "0.015";
		test_object.printer_sequence_optimizer_.LoadCommandSequence("cylinder_print_test.gcode");
		test_object.printer_sequence_optimizer_.LoadModelConfig();
		test_object.printer_sequence_optimizer_.cmd_i_ = 300;// command sequence contains more than 300 lines


		// case of parameter list failure
		std::cout << "Parameter list failure" << std::endl;
		std::vector<std::string> params;
		test_object.SaveCurrSimStat(params);
		params.push_back("param0");
		params.push_back("param1");
		test_object.SaveCurrSimStat(params);
		
		// case of running simulation
		std::cout << "No suspended simulation" << std::endl;
		test_object.printer_sequence_optimizer_.simulation_suspended_ = false;
		params.clear();
		params.push_back("save_sim_stat_test");
		test_object.SaveCurrSimStat(params);

		// nominal case
		std::cout << "Nominal case" << std::endl;
		test_object.printer_sequence_optimizer_.simulation_suspended_ = true;
		test_object.SaveCurrSimStat(params);
	}

	static void LoadSavedSimStatTest() {
	
		MainController test_object;
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.5";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.env_medium_coeff_ = "0.0255";
		test_object.printer_sequence_optimizer_.build_plate_coeff_ = "1.5";
		Section::MinResSpd = "0.015";

		test_object.playback_presenter_ = new HeatMapModelVisualizer();
		test_object.playback_window_ = new GenericWindowRenderer();
		test_object.printer_sequence_optimizer_.simulation_suspended_ = true;
		
		// case of parameter list failure
		std::cout << "Parameter list failure" << std::endl;
		std::vector<std::string> params;
		test_object.SaveCurrSimStat(params);
		params.push_back("param0");
		params.push_back("param1");
		test_object.LoadSavedSimStat(params);

		// case of running simulation
		std::cout << "Not suspended simulation" << std::endl;
		test_object.printer_sequence_optimizer_.simulation_suspended_ = false;
		params.clear();
		params.push_back("save_sim_stat_test");
		test_object.LoadSavedSimStat(params);

		// nominal case
		std::cout << "Nominal case" << std::endl;
		test_object.printer_sequence_optimizer_.simulation_suspended_ = true;
		test_object.LoadSavedSimStat(params);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test_object.ExitProgram(params);
	}

	static void SetUnitStepTest() {
	
		MainController test_object;
		
		// size of params is under 1
		std::vector<std::string> params;
		test_object.SetUnitStep(params);
		
		// size of params is above 1
		params.resize(2);
		test_object.SetUnitStep(params);

		// wrong value format
		params.clear();
		params.push_back("2,2");
		test_object.SetUnitStep(params);

		// value is equal to line width
		params[0] = "1.0";
		test_object.SetUnitStep(params);
		
		// value is greater than line with (data loss)
		params[0] = "2.0";
		test_object.SetUnitStep(params);
		
		// successful value change
		params[0] = "0.9";
		test_object.SetUnitStep(params);
	}
	
	static void SaveResultIntoFileTest() {

		MainController test_object;
		test_object.printer_sequence_optimizer_.LoadCommandSequence("cone_test.gcode");
		std::vector<std::string> params;

		std::cout << "Empty paramter list" << std::endl;
		test_object.SaveResultIntoFile(params);

		std::cout << "Specified output file" << std::endl;
		params.push_back("test_output");
		test_object.SaveResultIntoFile(params);
	}

	// command sequence processing functions
	static void LoadcommandSequenceTest() {
	
		MainController test_object;
		
		try {
		
			// inaccessible file
			std::cout << "Inaccessible file" << std::endl;
			test_object.printer_sequence_optimizer_.LoadCommandSequence("wrong file path");
		}
		catch (std::exception err) {
			
			std::cout << "Error message: " << err.what() << std::endl;
		}

		try {
			// wrong value format
			std::cout << "Wrong value format" << std::endl;
			test_object.printer_sequence_optimizer_.LoadCommandSequence("wrong_value_format.gcode");
		}
		catch (std::exception err) {

			std::cout << "Error message: " << err.what() << std::endl;
		}

		// accessible file
		std::cout << "Accessible file" << std::endl;
		test_object.printer_sequence_optimizer_.LoadCommandSequence("cone_test.gcode");
	}
	
	static void ValidateLocalEnvTempValsTest() {
	
		MainController test_object;
		
		// setting up micro environment manually, independent test from SetupLocalEnvironmentalPoints()
		test_object.printer_sequence_optimizer_.x_len_ = 10;
		test_object.printer_sequence_optimizer_.y_len_ = 10;
		test_object.printer_sequence_optimizer_.z_len_ = 10;
		test_object.printer_sequence_optimizer_.layer_height_ = "0.2";
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.1";
		test_object.printer_sequence_optimizer_.line_width_ = "0.4";
		test_object.printer_sequence_optimizer_.rel_layer_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.layer_height_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_width_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.line_width_
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.consolidation_temp_bound_ = "180.0";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.disc_z_lines_ =
			std::vector<genmath::Vector<genmath::LongDouble> >(test_object.printer_sequence_optimizer_.x_len_ 
				* test_object.printer_sequence_optimizer_.y_len_,
				genmath::Vector<genmath::LongDouble>(test_object.printer_sequence_optimizer_.z_len_, 
					test_object.printer_sequence_optimizer_.env_temp_));
		
		size_t err_x, err_y, err_z;
		err_x = err_y = err_z = 0;
		if (!test_object.printer_sequence_optimizer_.ValidateLocalEnvTempVals(5, 5, 5, &err_x, &err_y, &err_z)
			&& !err_x && !err_y && !err_z) {
			
			std::cout << "Ambient temperature validation test passed." << std::endl;
		}

		for (size_t x = 2; x < 5; ++x)
			for (size_t y = 2; y < 5; ++y)
				for (size_t z = 2; z < 5; ++z)
					test_object.printer_sequence_optimizer_.disc_z_lines_[x * 10 + y][z] = "200.0";

		if (test_object.printer_sequence_optimizer_.ValidateLocalEnvTempVals(5, 5, 5, &err_x, &err_y, &err_z)
			&& err_x && err_y && err_z) {
		
			std::cout << "Consolidation boundary temp violation test case passed with paramaters: "
				"(" << err_x << ", " << err_y << ", " << err_z << ")." << std::endl;
		}		
	}
	
	static void SetupLocalEnvironmentalPointsTest() {
	
		// setting up micro menvironment manually
		MainController test_object;
		test_object.printer_sequence_optimizer_.x_len_ = 10;
		test_object.printer_sequence_optimizer_.y_len_ = 10;
		test_object.printer_sequence_optimizer_.z_len_ = 20;
		test_object.printer_sequence_optimizer_.layer_height_ = "0.2";
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.1";
		test_object.printer_sequence_optimizer_.line_width_ = "0.4";
		test_object.printer_sequence_optimizer_.rel_layer_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.layer_height_
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_width_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.line_width_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		
		test_object.printer_sequence_optimizer_.extruded_material_avg_temp_ = "210.0";
		test_object.printer_sequence_optimizer_.material_cond_coeff_ = "0.17";
		test_object.printer_sequence_optimizer_.rel_nozzle_dia_ = "0.8";
		test_object.printer_sequence_optimizer_.rel_nozzle_height_ = "0.8";
		test_object.printer_sequence_optimizer_.rel_nozzle_hor_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.rel_nozzle_dia_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_nozzle_vert_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.rel_nozzle_height_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.nozzle_surface_avg_temp_ = "170.0";
		test_object.printer_sequence_optimizer_.env_temp_ = "0.0";
		test_object.printer_sequence_optimizer_.env_medium_coeff_ = "0.0255";
		
		test_object.printer_sequence_optimizer_.disc_z_lines_ =
			std::vector<genmath::Vector<genmath::LongDouble> >(test_object.printer_sequence_optimizer_.x_len_
				* test_object.printer_sequence_optimizer_.y_len_,
			genmath::Vector<genmath::LongDouble>(test_object.printer_sequence_optimizer_.z_len_,
				test_object.printer_sequence_optimizer_.env_temp_));
		test_object.printer_sequence_optimizer_.disc_z_lines_coeffs_ =
			std::vector<genmath::Vector<genmath::LongDouble> >(test_object.printer_sequence_optimizer_.x_len_ 
				* test_object.printer_sequence_optimizer_.y_len_,
				genmath::Vector<genmath::LongDouble>(test_object.printer_sequence_optimizer_.z_len_, 
					test_object.printer_sequence_optimizer_.env_medium_coeff_));

		test_object.printer_sequence_optimizer_.SetupLocalEnvironmentalPoints(5, 5, 4);

		size_t err_x, err_y, err_z;
		err_x = err_y = err_z = 0;
		if (test_object.printer_sequence_optimizer_.ValidateLocalEnvTempVals(5, 5, 5, &err_x, &err_y, &err_z)
			&& err_x && err_y && err_z) {

			std::cout << "Setup of local point has occurred, test passed at point of heat value violation: "
				"(" << err_x << ", " << err_y << ", " << err_z << ")." << std::endl;
		}

		std::cout << "Printing heat map in linearized form: " << std::endl;
		for (size_t x = 0; x < test_object.printer_sequence_optimizer_.x_len_; ++x) {
		
			for (size_t y = 0; y < test_object.printer_sequence_optimizer_.y_len_; ++y) {
			
				for (size_t z = 0; z < test_object.printer_sequence_optimizer_.z_len_; ++z) {
					
					std::cout << test_object.printer_sequence_optimizer_.disc_z_lines_[
						x * test_object.printer_sequence_optimizer_.y_len_ + y][z] << " ";
				}

				std::cout << std::endl;
			}

			std::cout << std::endl;
		}
	
		std::cout << "Printing coefficient map in linearized form: " << std::endl;
		for (size_t x = 0; x < test_object.printer_sequence_optimizer_.x_len_; ++x) {

			for (size_t y = 0; y < test_object.printer_sequence_optimizer_.y_len_; ++y) {

				for (size_t z = 0; z < test_object.printer_sequence_optimizer_.z_len_; ++z) {

					std::cout << test_object.printer_sequence_optimizer_.disc_z_lines_[
						x * test_object.printer_sequence_optimizer_.y_len_ + y][z] << " ";
				}

				std::cout << std::endl;
			}

			std::cout << std::endl;
		}
	}
	
	static void ProcessCurrUnitDurSectTest() {
		
		// The following multiple layer test creates a spiral test object which has a simultaneously 
		//  heated first layer. The second layer will be applied for this. The algorithm has to cool
		//  the first layer down before applying the second layer. If this happens, the test passed.

		// static section discretization of time dependent execution sections (linearized)
		MainController test_object;
		test_object.printer_sequence_optimizer_.build_volume_width_ = "5.0";
		test_object.printer_sequence_optimizer_.build_volume_length_ = "5.0";
		test_object.printer_sequence_optimizer_.build_volume_height_ = "3.0";
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.2";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.env_medium_coeff_ = "0.0255";
		test_object.printer_sequence_optimizer_.build_plate_coeff_ = "0.17";
		test_object.printer_sequence_optimizer_.InitiateThermalSimulation();

		std::cout << "Simulation space index dimensions: " << test_object.printer_sequence_optimizer_.x_len_ << "x "
			<< test_object.printer_sequence_optimizer_.y_len_ << "y " << test_object.printer_sequence_optimizer_.z_len_ << "z" << std::endl;
		test_object.printer_sequence_optimizer_.build_plate_temp_ = "84.0";
		test_object.printer_sequence_optimizer_.ResetBoundaryValues();
		test_object.printer_sequence_optimizer_.layer_height_ = "0.2";
		test_object.printer_sequence_optimizer_.line_width_ = "0.4";
		test_object.printer_sequence_optimizer_.rel_layer_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.layer_height_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_width_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.line_width_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.consolidation_temp_bound_ = "180.0";
		test_object.printer_sequence_optimizer_.extruded_material_avg_temp_ = "210.0";
		test_object.printer_sequence_optimizer_.material_cond_coeff_ = "0.17";
		test_object.printer_sequence_optimizer_.rel_nozzle_dia_ = "1.0";
		test_object.printer_sequence_optimizer_.rel_nozzle_height_ = "1.0";
		test_object.printer_sequence_optimizer_.rel_nozzle_hor_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.rel_nozzle_dia_ 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_nozzle_vert_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.rel_nozzle_height_
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.nozzle_surface_avg_temp_ = "170.0";
		test_object.printer_sequence_optimizer_.unit_time_step_ = 
			test_object.printer_sequence_optimizer_.thermal_space_model_.GetTimeStep();
		
		// CASE OF RELATIVE SECOND LAYER (printing spiral form)
		std::string raw_data = "";
		//genmath::LongDouble avg_spd_limit = "5.0";
		genmath::LongDouble dist = "3.0";
		genmath::LongDouble inc = test_object.printer_sequence_optimizer_.line_width_;
		
		// FIRST LAYER
		Section::XJerk = "8.0";
		Section::YJerk = "8.0";
		Section::XAcc = "500.0";
		Section::YAcc = "500.0";
		Section::MinResSpd = "0.015";
		Section::Init();
		Section::UpdateGlobalResSpd(genmath::LongDouble("5.0"));
		
		test_object.printer_sequence_optimizer_.gcode_commands_.reserve(200);

		genmath::LongDouble x_offset = test_object.printer_sequence_optimizer_.rel_nozzle_dia_ / genmath::LongDouble("2.0");
		genmath::LongDouble y_offset = test_object.printer_sequence_optimizer_.rel_nozzle_dia_ / genmath::LongDouble("2.0");
		for (genmath::LongDouble offset = "0.0"; offset < dist / genmath::LongDouble("2.0"); offset += inc) {
		
			raw_data = "G1 X" + std::string(offset + x_offset) + " Y" + std::string(offset + y_offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
			raw_data = "G1 X" + std::string(offset + x_offset) + " Y" + std::string(dist - offset + y_offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
			raw_data = "G1 X" + std::string(dist - offset + x_offset) + " Y" + std::string(dist - offset + y_offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
			raw_data = "G1 X" + std::string(dist - offset + x_offset) + " Y" + std::string(offset + inc + y_offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
		}

		test_object.printer_sequence_optimizer_.x_offset_ = "0.0";
		test_object.printer_sequence_optimizer_.y_offset_ = "0.0";
		x_offset = "0.0";
		y_offset = "0.0";
		std::cout << "Created commands for first layer." << std::endl;
		for (size_t j = 0; j < test_object.printer_sequence_optimizer_.gcode_commands_.size(); ++j)
			std::cout << std::string(test_object.printer_sequence_optimizer_.gcode_commands_[j]) << std::endl;
		
		
		// creating section history of previous layer
		size_t num_of_commands = test_object.printer_sequence_optimizer_.gcode_commands_.size();
		Section* sect = new Section(&test_object.printer_sequence_optimizer_.gcode_commands_[0]);
		size_t i = 1;

		genmath::LongDouble curr_z_pos = "0.2";
		for (; i < num_of_commands; ++i) {
		
			sect->UpdateSection(&test_object.printer_sequence_optimizer_.gcode_commands_[i], 
				test_object.printer_sequence_optimizer_.unit_time_step_);
			test_object.printer_sequence_optimizer_.proj_sect_hist_.push_back(sect);
			sect = new Section(*sect);
			sect->SetPrevSect(test_object.printer_sequence_optimizer_.proj_sect_hist_.back());
			
			std::cout << "Section time interval [" << sect->start_time_ << ", " << sect->end_time_ << "]" << std::endl;

			genmath::LongDouble sect_end_time = genmath::LongDouble::RawRound(
				sect->end_time_, test_object.printer_sequence_optimizer_.unit_time_step_);
			genmath::LongDouble start_virt_x, start_virt_y, end_virt_x, end_virt_y;// virtualized points
			std::pair<genmath::LongDouble, genmath::LongDouble> virt_point;
			bool first_virt_point = true;
			size_t curr_z_ind = curr_z_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
				curr_z_pos, test_object.printer_sequence_optimizer_.unit_space_step_) 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);

			for (genmath::LongDouble curr_time = genmath::LongDouble::RawRound(
				sect->start_time_, test_object.printer_sequence_optimizer_.unit_time_step_);
				curr_time < sect_end_time; curr_time += test_object.printer_sequence_optimizer_.unit_time_step_) {

				virt_point = (*sect)(curr_time);// generating next linear section based point

				if (first_virt_point) {
					start_virt_x = virt_point.first;
					start_virt_y = virt_point.second;
					first_virt_point = false;
				}
				else {

					end_virt_x = virt_point.first;
					end_virt_y = virt_point.second;

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
					
						for (; curr_virt_c > end_virt_c; curr_virt_c -= test_object.printer_sequence_optimizer_.unit_space_step_) {

							curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * ratio_x - test_object.printer_sequence_optimizer_.x_offset_
								+ start_virt_x, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * dir_aux * ratio_y - test_object.printer_sequence_optimizer_.y_offset_
								+ start_virt_y, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							std::cout << "(" << (curr_x_ind) << ", " << (curr_y_ind) << ", " << (curr_z_ind) << ")" << std::endl;
							test_object.printer_sequence_optimizer_.SetupLocalEnvironmentalPoints(curr_x_ind, curr_y_ind, curr_z_ind);
						}
					}
					else {
					
						for (; curr_virt_c < end_virt_c; curr_virt_c += test_object.printer_sequence_optimizer_.unit_space_step_) {

							curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * ratio_x - test_object.printer_sequence_optimizer_.x_offset_
								+ start_virt_x, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * dir_aux * ratio_y - test_object.printer_sequence_optimizer_.y_offset_ 
								+ start_virt_y, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							std::cout << "(" << (curr_x_ind) << ", " << (curr_y_ind) << ", " << (curr_z_ind) << ")" << std::endl;
							test_object.printer_sequence_optimizer_.SetupLocalEnvironmentalPoints(curr_x_ind, curr_y_ind, curr_z_ind);
						}
					}
									
					start_virt_x = end_virt_x;
					start_virt_y = end_virt_y;
				}
			}
		}

		for (size_t j = 0; j < test_object.printer_sequence_optimizer_.disc_z_lines_.size(); ++j) {
		
			std::cout << std::string(test_object.printer_sequence_optimizer_.disc_z_lines_[j]) << std::endl;
		}
		// setting up grid points of first layer (overheated for optimization test  - decreasing speed)
		
		// SECOND LAYER
		for (genmath::LongDouble offset = "0.0"; offset < dist / genmath::LongDouble("2.0"); offset += inc) {
		
			raw_data = "G1 X" + std::string(offset) + " Y" + std::string(offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
			raw_data = "G1 X" + std::string(offset) + " Y" + std::string(dist - offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
			raw_data = "G1 X" + std::string(dist - offset) + " Y" + std::string(dist - offset);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
			raw_data = "G1 X" + std::string(dist - offset) + " Y" + std::string(offset + inc);
			test_object.printer_sequence_optimizer_.gcode_commands_.push_back(
				GCodeCommand<genmath::LongDouble>(raw_data));
		}

		std::cout << "Created commands for second layer." << std::endl;
		for (size_t j = i + 1; j < test_object.printer_sequence_optimizer_.gcode_commands_.size(); ++j)
			std::cout << std::string(test_object.printer_sequence_optimizer_.gcode_commands_[j]) << std::endl;

		// creating partial current layer
		num_of_commands = test_object.printer_sequence_optimizer_.gcode_commands_.size();
		// first point of second layer
		++i;
		sect = new Section(&test_object.printer_sequence_optimizer_.gcode_commands_[i]);
		++i;
		curr_z_pos = "0.4";
		// process some section from the current layer then validate one
		for (; i < num_of_commands - 4; ++i) {
		
			sect->UpdateSection(&test_object.printer_sequence_optimizer_.gcode_commands_[i],
				test_object.printer_sequence_optimizer_.unit_time_step_);
			test_object.printer_sequence_optimizer_.proj_sect_.push_back(sect);
			sect = new Section(*sect);
			sect->SetPrevSect(test_object.printer_sequence_optimizer_.proj_sect_.back());
			std::cout << "Section time interval [" << sect->start_time_ << ", " << sect->end_time_ << "]" << std::endl;

			genmath::LongDouble sect_end_time = genmath::LongDouble::RawRound(sect->end_time_,
				test_object.printer_sequence_optimizer_.unit_time_step_);
			genmath::LongDouble start_virt_x, start_virt_y, end_virt_x, end_virt_y;// virtualized points
			std::pair<genmath::LongDouble, genmath::LongDouble> virt_point;
			bool first_virt_point = true;
			size_t curr_z_ind = curr_z_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
				curr_z_pos, test_object.printer_sequence_optimizer_.unit_space_step_) 
				/ test_object.printer_sequence_optimizer_.unit_space_step_);
			for (genmath::LongDouble curr_time = genmath::LongDouble::RawRound(
				sect->start_time_, test_object.printer_sequence_optimizer_.unit_time_step_);
				curr_time < sect_end_time; curr_time += test_object.printer_sequence_optimizer_.unit_time_step_) {

				virt_point = (*sect)(curr_time);// generating next linear section based point

				if (first_virt_point) {
					start_virt_x = virt_point.first;
					start_virt_y = virt_point.second;
					first_virt_point = false;
				}
				else {

					end_virt_x = virt_point.first;
					end_virt_y = virt_point.second;

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

						for (; curr_virt_c > end_virt_c; curr_virt_c -= test_object.printer_sequence_optimizer_.unit_space_step_) {

							curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * ratio_x - test_object.printer_sequence_optimizer_.x_offset_
								+ start_virt_x, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * dir_aux * ratio_y - test_object.printer_sequence_optimizer_.y_offset_ 
								+ start_virt_y, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							std::cout << "(" << (curr_x_ind) << ", " << (curr_y_ind) << ", " << (curr_z_ind) << ")" << std::endl;
							test_object.printer_sequence_optimizer_.SetupLocalEnvironmentalPoints(
								curr_x_ind, curr_y_ind, curr_z_ind);
						}
					}
					else {

						for (; curr_virt_c < end_virt_c; curr_virt_c += test_object.printer_sequence_optimizer_.unit_space_step_) {

							curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * ratio_x - test_object.printer_sequence_optimizer_.x_offset_ 
								+ start_virt_x, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
								curr_virt_c * dir_aux * ratio_y - test_object.printer_sequence_optimizer_.y_offset_
								+ start_virt_y, test_object.printer_sequence_optimizer_.unit_space_step_)
								/ test_object.printer_sequence_optimizer_.unit_space_step_);

							std::cout << "(" << (curr_x_ind) << ", " << (curr_y_ind) << ", " << (curr_z_ind) << ")" << std::endl;
							test_object.printer_sequence_optimizer_.SetupLocalEnvironmentalPoints(
								curr_x_ind, curr_y_ind, curr_z_ind);
						}
					}

					start_virt_x = end_virt_x;
					start_virt_y = end_virt_y;
				}
			}
		}
		
		for (size_t j = 0; j < test_object.printer_sequence_optimizer_.disc_z_lines_.size(); ++j) {

			std::cout << std::string(test_object.printer_sequence_optimizer_.disc_z_lines_[j]) << std::endl;
		}

		// validate one dynamic section (recorded section in command sequence)
		++i;
		sect->UpdateSection(&test_object.printer_sequence_optimizer_.gcode_commands_[i],
			test_object.printer_sequence_optimizer_.unit_time_step_);
		test_object.printer_sequence_optimizer_.proj_sect_.push_back(sect);
		std::cout << "Generating points of current section." << std::endl;
		genmath::LongDouble sect_end_time = genmath::LongDouble::RawRound(sect->end_time_,
			test_object.printer_sequence_optimizer_.unit_time_step_);
		genmath::LongDouble curr_time = genmath::LongDouble::RawRound(sect->start_time_,
			test_object.printer_sequence_optimizer_.unit_time_step_);
		genmath::LongDouble start_virt_x, start_virt_y, end_virt_x, end_virt_y;// virtualized points
		std::pair<genmath::LongDouble, genmath::LongDouble> virt_point;
		bool first_virt_point = true;
		curr_z_pos = "0.4";

		size_t curr_x_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
			genmath::LongDouble(sect->start_point_->Get("X")) - test_object.printer_sequence_optimizer_.x_offset_,
			test_object.printer_sequence_optimizer_.unit_space_step_) / test_object.printer_sequence_optimizer_.unit_space_step_);

		size_t curr_y_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
			genmath::LongDouble(sect->start_point_->Get("Y")) - test_object.printer_sequence_optimizer_.y_offset_, 
			test_object.printer_sequence_optimizer_.unit_space_step_) / test_object.printer_sequence_optimizer_.unit_space_step_);

		size_t curr_z_ind = curr_z_ind = 1 + static_cast<unsigned long>(genmath::LongDouble::RawRound(
			curr_z_pos, test_object.printer_sequence_optimizer_.unit_space_step_)
			/ test_object.printer_sequence_optimizer_.unit_space_step_);
		
		for (; curr_time <= sect_end_time; curr_time += test_object.printer_sequence_optimizer_.unit_time_step_) {

			virt_point = (*sect)(curr_time);// generating next linear section based point

			if (first_virt_point) {

				start_virt_x = virt_point.first;
				start_virt_y = virt_point.second;
				first_virt_point = false;
				
				test_object.printer_sequence_optimizer_.PointProcessing(
					curr_x_ind, curr_y_ind, curr_z_ind);
				test_object.printer_sequence_optimizer_.SetupLocalEnvironmentalPoints(
					start_virt_x, start_virt_y, curr_z_ind);
			}
			else {

				end_virt_x = virt_point.first;
				end_virt_y = virt_point.second;

				test_object.printer_sequence_optimizer_.ProcessCurrUnitDurSect(
					start_virt_x, start_virt_y, end_virt_x,
					end_virt_y, curr_z_ind, sect);

				start_virt_x = end_virt_x;
				start_virt_y = end_virt_y;
			}
		}		
	}
	
	static void ResetBoundaryValuesTest() {
	
		// setting up micro menvironment manually
		MainController test_object;
		test_object.printer_sequence_optimizer_.x_len_ = 10;
		test_object.printer_sequence_optimizer_.y_len_ = 10;
		test_object.printer_sequence_optimizer_.z_len_ = 20;
		test_object.printer_sequence_optimizer_.build_plate_temp_ = "84.0";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.disc_z_lines_ =
			std::vector<genmath::Vector<genmath::LongDouble> >(
				test_object.printer_sequence_optimizer_.x_len_ * test_object.printer_sequence_optimizer_.y_len_,
				genmath::Vector<genmath::LongDouble>(test_object.printer_sequence_optimizer_.z_len_,
				test_object.printer_sequence_optimizer_.env_temp_));

		// modifying environmental boundary temperature values
		for (size_t y = 0; y < test_object.printer_sequence_optimizer_.y_len_; ++y) {

			for (size_t x = 0; x < test_object.printer_sequence_optimizer_.x_len_; ++x) {

				test_object.printer_sequence_optimizer_.disc_z_lines_[
					x * test_object.printer_sequence_optimizer_.y_len_ + y][0] = "74.0";
					
				test_object.printer_sequence_optimizer_.disc_z_lines_[
					x * test_object.printer_sequence_optimizer_.y_len_ + y][
						test_object.printer_sequence_optimizer_.z_len_ - 1] =
					genmath::LongDouble(std::rand() * (20.0 / 32767.0));
			}
		}

		for (size_t z = 1; z < test_object.printer_sequence_optimizer_.z_len_; ++z) {

			for (size_t y = 0; y < test_object.printer_sequence_optimizer_.y_len_; ++y) {

				test_object.printer_sequence_optimizer_.disc_z_lines_[
					0 * test_object.printer_sequence_optimizer_.y_len_ + y][z] =
					genmath::LongDouble(std::rand() * (20.0 / 32767.0));
					
				test_object.printer_sequence_optimizer_.disc_z_lines_[
					(test_object.printer_sequence_optimizer_.x_len_ - 1) 
					* test_object.printer_sequence_optimizer_.y_len_ + y][z] =
					genmath::LongDouble(std::rand() * (20.0 / 32767.0));
			}
		}

		for (size_t z = 1; z < test_object.printer_sequence_optimizer_.z_len_; ++z) {

			for (size_t x = 0; x < test_object.printer_sequence_optimizer_.x_len_; ++x) {

				test_object.printer_sequence_optimizer_.disc_z_lines_[
					x * test_object.printer_sequence_optimizer_.y_len_ + 0][z] =
					genmath::LongDouble(std::rand() * (20.0 / 32767.0));
					
				test_object.printer_sequence_optimizer_.disc_z_lines_[
					x * test_object.printer_sequence_optimizer_.y_len_
					+ test_object.printer_sequence_optimizer_.y_len_ - 1][z] =
					genmath::LongDouble(std::rand() * (20.0 / 32767.0));
			}
		}
		
		std::cout << "Printing heat map in linearized form before reset: " << std::endl;
		for (size_t x = 0; x < test_object.printer_sequence_optimizer_.x_len_; ++x) {

			for (size_t y = 0; y < test_object.printer_sequence_optimizer_.y_len_; ++y) {

				for (size_t z = 0; z < test_object.printer_sequence_optimizer_.z_len_; ++z) {

					std::cout << test_object.printer_sequence_optimizer_.disc_z_lines_[
						x * test_object.printer_sequence_optimizer_.y_len_ + y][z] << " ";
				}

				std::cout << std::endl;
			}

			std::cout << std::endl;
		}

		test_object.printer_sequence_optimizer_.ResetBoundaryValues();
		std::cout << "Printing heat map in linearized form reset: " << std::endl;
		for (size_t x = 0; x < test_object.printer_sequence_optimizer_.x_len_; ++x) {

			for (size_t y = 0; y < test_object.printer_sequence_optimizer_.y_len_; ++y) {

				for (size_t z = 0; z < test_object.printer_sequence_optimizer_.z_len_; ++z) {

					std::cout << test_object.printer_sequence_optimizer_.disc_z_lines_[
						x * test_object.printer_sequence_optimizer_.y_len_ + y][z] << " ";
				}

				std::cout << std::endl;
			}

			std::cout << std::endl;
		}
	}
	
	static void InitiateThermalSimulationTest() {
	
		MainController test_object;
		test_object.playback_presenter_ = new HeatMapModelVisualizer();
		test_object.playback_window_ = new GenericWindowRenderer();
		test_object.printer_sequence_optimizer_.build_volume_width_ = "5.0";
		test_object.printer_sequence_optimizer_.build_volume_length_ = "7.0";
		test_object.printer_sequence_optimizer_.build_volume_height_ = "10.0";
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.5";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.env_medium_coeff_ = "0.0255";
		test_object.printer_sequence_optimizer_.build_plate_coeff_ = "1.5";
		test_object.printer_sequence_optimizer_.InitiateThermalSimulation();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test_object.printer_sequence_optimizer_.simulation_active_ = true;
		test_object.printer_sequence_optimizer_.thermal_space_model_.StopSimulation();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		// redundancy check
		std::vector<std::string> params;
		test_object.printer_sequence_optimizer_.InitiateThermalSimulation();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test_object.printer_sequence_optimizer_.simulation_active_ = true;
		test_object.processing_thread_ = std::thread([]() {
			
			std::cout << "Operating thread imitation.." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		test_object.ExitProgram(params);
	}
	
	static void ProcessSourceModelTest() {
	
		// static section discretization of time dependent execution sections (linearized)
		MainController test_object;
		Section::MinResSpd = "0.015";
		test_object.printer_sequence_optimizer_.build_volume_width_ = "4.0";
		test_object.printer_sequence_optimizer_.build_volume_length_ = "4.0";
		test_object.printer_sequence_optimizer_.build_volume_height_ = "2.0";
		test_object.printer_sequence_optimizer_.unit_space_step_ = "0.2";
		test_object.printer_sequence_optimizer_.env_temp_ = "20.0";
		test_object.printer_sequence_optimizer_.env_medium_coeff_ = "0.0255";
		test_object.printer_sequence_optimizer_.build_plate_coeff_ = "0.17";
		//test_object.InitiateThermalSimulation();
		test_object.printer_sequence_optimizer_.build_plate_temp_ = "84.0";
		//test_object.ResetBoundaryValues();
		test_object.printer_sequence_optimizer_.layer_height_ = "0.2";
		test_object.printer_sequence_optimizer_.line_width_ = "0.4";
		test_object.printer_sequence_optimizer_.rel_layer_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.layer_height_
			/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_width_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.line_width_
			/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.consolidation_temp_bound_ = "180.0";
		test_object.printer_sequence_optimizer_.extruded_material_avg_temp_ = "210.0";
		test_object.printer_sequence_optimizer_.material_cond_coeff_ = "0.17";
		test_object.printer_sequence_optimizer_.rel_nozzle_dia_ = "1.0";
		test_object.printer_sequence_optimizer_.rel_nozzle_height_ = "1.0";
		test_object.printer_sequence_optimizer_.rel_nozzle_hor_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.rel_nozzle_dia_ 
			/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.rel_nozzle_vert_ind_limit_ =
			static_cast<unsigned long>(test_object.printer_sequence_optimizer_.rel_nozzle_height_
			/ test_object.printer_sequence_optimizer_.unit_space_step_);
		test_object.printer_sequence_optimizer_.nozzle_surface_avg_temp_ = "170.0";
		test_object.printer_sequence_optimizer_.unit_time_step_ =
			test_object.printer_sequence_optimizer_.thermal_space_model_.GetTimeStep();

		std::cout << "Initialization went successfully." << std::endl;

		test_object.printer_sequence_optimizer_.LoadCommandSequence("cylinder_print_test.gcode");
		
		std::cout << "Source loaded." << std::endl;
		test_object.printer_sequence_optimizer_.LoadModelConfig();
		test_object.printer_sequence_optimizer_.ProcessSourceModel();

		std::cout << "Source optimization has done." << std::endl;
	}
	
	// test the singleton class after the test of its subcomponents, modules, methods
	static void ObjectResourceManagement() {
	
		// CLASS IS TESTED AT PROGRAM EXECUTION..
		
		//MainController();
		//MainController(uint64_t window_width, uint64_t window_height);
		//MainController(const MainController & orig);
		//virtual ~MainController();
		// RAII, threads are managed in Run()
	}
};
#endif// !RELEASE_MODE

#include "gui/GenericWindowRenderer.h"
#include "MainController.h"
#include <iostream>
#include <stdint.h>
#include "include/SDL.h"

#include "logging/Logger.h"

int main(int argc, char** args) {

#ifndef RELEASE_MODE
	
	try {
		//ConsoleUITests::TestConsole();// TEST PASSED
		
		
		//InputControlTests::ObjectConstructionTest();// TEST PASSED

		//InputControlTests::RunAndDestructionTest();// TEST PASSED


		//GenericWindowRendererTests::GenericRenderingTest();// TEST PASSED

		//GenericWindowRendererTests::GenericRenderingExitTest();// TEST PASSED
		
		
		//ModelVisualizerTests::SingleStaticObjectTest();// TEST PASSED
		
		//ModelVisualizerTests::SingleDynamicObjectTest();// TEST PASSED
		
		//ModelVisualizerTests::MultipleStaticObjectTest();// TEST PASSED
		
		//ModelVisualizerTests::MultipleDynamicObjectTest();// TEST PASSED


		//MainControllerTests::SaveCurrSimStatTest();// TEST PASSED

		//MainControllerTests::LoadSavedSimStatTest();// TEST PASSED

		//MainControllerTests::SetUnitStepTest();// TEST PASSED

		//MainControllerTest::ExitProgramTest
		// TESTED AT PROGRAM EXECUTION..
		// TEST PASSED
		
		//MainControllerTests::LoadModelTest
		// TESTED AT PROGRAM EXECUTION..
		// load a model and wait until it finishes the processing of optimization
		// TEST PASSED
		
		////MainControllerTests::RunSimulationTest
		// TESTED AT PROGRAM EXECUTION..
		// load a model and wait for sufficient number of time frames to be displayed at playback
		// choose 'run-sim' command to prepare the playback
		// after successful preparation, simulation playback starts by pressing SPACE
		// TEST PASSED
		
		//MainControllerTests::SaveResultIntoFileTest();// TEST PASSED
		
		//MainControllerTests::CancelOptimizationTest
		// TESTED AT PROGRAM EXECUTION..
		// run an optimization and cancel it before it finishes
		// TEST PASSED
		
		//MainControllerTests::SuspendOptimizationTest
		// TESTED AT PROGRAM EXECUTION..
		// run an optimization and at an arbitrary time suspend the optimization process 
		//  by choosing 'suspend-opt' command
		// TEST PASSED
		
		////MainControllerTests::ContinueOptimizationTest
		// TESTED AT PROGRAM EXECUTION..
		// run an optimization, suspend it at an arbitrary time and then continue the optimization 
		// process by choosing 'continue-opt'
		// test case of no running optimization can be tested by calling 'continue-opt' without
		//  optimization process
		// TEST PASSED

		//MainControllerTests::LoadcommandSequenceTest();// TEST PASSED
		
		//MainControllerTests::ValidateLocalEnvTempValsTest();// TEST PASSED
		
		//MainControllerTests::SetupLocalEnvironmentalPointsTest();// TEST PASSED
		
		//MainControllerTests::ProcessCurrUnitDurSectTest();// TEST PASSED
		
		//MainControllerTests::ResetBoundaryValuesTest();// TEST PASSED
		
		//MainControllerTests::InitiateThermalSimulationTest();// TEST PASSED
		
		//MainControllerTests::ProcessSourceModelTest();// TEST PASSED
	}
	catch (std::exception err) {
	
		std::cout << "An error has occurred:\n" << err.what() << std::endl;
	}
	return 0;
#else
	
	MainController main_ctl("config.cfg");
	return main_ctl.Run();
	//return 0;
#endif// RELEASE_MODE
}
