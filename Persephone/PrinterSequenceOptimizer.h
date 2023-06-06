#pragma once
#define TEST_MODULES
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <utility>
#include <initializer_list>
#include "genmath/ObjectBase.h"
#include "genmath/String.h"
#include "genmath/LongDouble.h"
#include "genmath/Vector.h"
#include "genmath/TriDiagMatrix.h"
#include "locresctl/FileControl.h"
#include "printerheatconduction/GCodeCommand.h"
#include "printerheatconduction/Section.h"
#include "printerheatconduction/FDM3.h"
#include "PrinterSequenceOptimizer.h"
#include "cli/ConsoleUI.h"


class PrinterSequenceOptimizer {

#ifdef TEST_MODULES
friend class MainController;
#endif

public:

	PrinterSequenceOptimizer();
	PrinterSequenceOptimizer(ConsoleUI& program_console);
	PrinterSequenceOptimizer(const PrinterSequenceOptimizer& orig);
	PrinterSequenceOptimizer& operator=(const PrinterSequenceOptimizer& orig);
	virtual ~PrinterSequenceOptimizer();

	std::vector<genmath::ObjectBase*> GetParamNameList();
	std::vector<genmath::ObjectBase*> GetParamList();
	bool ValidateAndInitConfigParams();
	genmath::LongDouble& GetEnvMediumCoeff();
	genmath::LongDouble& GetConsolidationTempBound();
	genmath::LongDouble& GetLineWidth();
	genmath::LongDouble& GetUnitSpaceStep();
	bool& GetSimulationActive();
	bool& GetSimulationSuspended();
	void StopSimulation();
	void SuspendSimulation();
	void ContinueSimulation();
	void SeekCmdI(size_t cmd_i);
	size_t GetCmdI();
	size_t GetNumOfGCodeCommands();
	std::vector<GCodeCommand<genmath::LongDouble> >* GetGCodeCommands();
	std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* GetHeatMapTimeFrames();
	std::vector<std::vector<genmath::Vector<genmath::LongDouble> > >* GetHeatMapTimeFramesCoeffs();
	size_t GetXLen();
	size_t GetYLen();
	size_t GetZLen();
	void SetRecentLineImage();// setting last values of disc_z_lines_
	void SetRecentCoeffLineImage();// setting last values of disc_z_lines_coeffs_
	genmath::LongDouble GetTimeStep();

#ifndef TEST_MODULES
private:
#endif

	ConsoleUI program_console_;

	// parameter name list for debugging
	genmath::String line_width_name_;
	genmath::String unit_space_step_name_;
	genmath::String rel_nozzle_dia_name_;
	genmath::String rel_nozzle_height_name_;
	genmath::String extruded_material_avg_temp_offset_name_;
	genmath::String nozzle_surface_avg_temp_offset_name_;
	genmath::String consolidation_temp_bound_offset_name_;
	genmath::String nozzle_mat_melting_temp_name_;
	genmath::String material_cond_coeff_name_;
	genmath::String build_plate_temp_name_;
	genmath::String build_plate_coeff_name_;
	genmath::String env_temp_name_;
	genmath::String env_medium_coeff_name_;
	genmath::String min_stepper_rev_spd_name_;


	/// current higher level manager parameters (MainController)

	// printing line width (printer head nozzle diameter), PRECONFIGURED FROM FILE
	genmath::LongDouble line_width_;
	// defined by source model command sequence
	genmath::LongDouble layer_height_;
	// number of indices for discretized line representation
	size_t rel_width_ind_limit_;
	// number of indices for discretized layer representation
	size_t rel_layer_ind_limit_;
	// distance between two discrete thermal points after discretization of model, PRECONFIGURED FROM FILE
	genmath::LongDouble unit_space_step_;

	// coordinate offset to translate the initial lower bound to 0 (including the half length of print head dimension)
	// position offsets, translate coordinates to center of coordinate system, this shrinks the
	//  simulation space to the relevant "subspace"
	genmath::LongDouble x_offset_;
	// coordinate offset to translate the initial lower bound to 0 (including the half length of print head dimension)
	genmath::LongDouble y_offset_;
	//genmath::LongDouble z_offset;// always zero, due to gravitational force

	// defined by the source model command sequence, limited by rel_nozzle_dia_
	genmath::LongDouble build_volume_width_;
	// defined by the source model command sequence, limited by rel_nozzle_height_
	genmath::LongDouble build_volume_height_;
	// defined by the source model command sequence, limited by rel_nozzle_dia_
	genmath::LongDouble build_volume_length_;
	// discretized x axial meshgrid length, defined by the source model command sequence
	size_t x_len_;
	// discretized y axial meshgrid length, defined by the source model command sequence
	size_t y_len_;
	// discretized z axial meshgrid length, defined by the source model command sequence
	size_t z_len_;
	// nozzle horizontal diameter, PRECONFIGURED FROM FILE
	genmath::LongDouble rel_nozzle_dia_;
	// nozzle vertical dimension, PRECONFIGURED FROM FILE
	genmath::LongDouble rel_nozzle_height_;
	// number of indices for discretized nozzle width/diameter representation
	size_t rel_nozzle_hor_ind_limit_;
	// number of indices for discretized nozzle height representation
	size_t rel_nozzle_vert_ind_limit_;
	// thermal difference between extruded material (that comes immediately from the printer nozzle)
	//  and nozzle melting temperature, PRECONFIGURED FROM FILE
	genmath::LongDouble extruded_material_avg_temp_offset_;
	genmath::LongDouble extruded_material_avg_temp_;
	// nozzle surface average temperature offset for local area heating, PRECONFIGURED FROM FILE
	genmath::LongDouble nozzle_surface_avg_temp_offset_;
	genmath::LongDouble nozzle_surface_avg_temp_;
	// consolidation upper threshold temperature, thermal level where the material reaches
	//  the glass transition matter phase at cooling, defined by offset according to nozzle
	// melting temperature, PRECONFIGURED FROM FILE
	genmath::LongDouble consolidation_temp_bound_offset_;
	genmath::LongDouble consolidation_temp_bound_;
	// default nozzle material melting temperature, modified by source gcode command sequence later
	// PRECONFIGURED FROM FILE
	genmath::LongDouble nozzle_mat_melting_temp_;
	// heat conduction coefficient of extruded material, PRECONFIGURED FROM FILE
	genmath::LongDouble material_cond_coeff_;
	// heatbed, build plate temperature if heateable build plate is available on the specified printer
	// PRECONFIGURED FROM FILE
	genmath::LongDouble build_plate_temp_;
	// average heat conduction coefficient of build plate, PRECONFIGURED FROM FILE
	genmath::LongDouble build_plate_coeff_;
	// local finite environment based ambient temperature (e.g.: room temperature), PRECONFIGURED FROM FILE
	genmath::LongDouble env_temp_;
	// average heat conduction coefficient of environmental medium, PRECONFIGURED FROM FILE
	genmath::LongDouble env_medium_coeff_;


	/// heat map vectors by dimensions, defined by the source model
	std::vector<genmath::Vector<genmath::LongDouble> > disc_x_lines_;
	std::vector<genmath::Vector<genmath::LongDouble> > disc_y_lines_;
	// using this status container for cyclic simulation step by step and for visualization
	std::vector<genmath::Vector<genmath::LongDouble> > disc_z_lines_;


	/// heat conduction coefficients of gridpoints by dimensions, defined by the source model
	std::vector<genmath::Vector<genmath::LongDouble> > disc_x_lines_coeffs_;
	std::vector<genmath::Vector<genmath::LongDouble> > disc_y_lines_coeffs_;
	std::vector<genmath::Vector<genmath::LongDouble> > disc_z_lines_coeffs_;


	// defined by FDM simulation
	genmath::LongDouble unit_time_step_;
	// spatial thermal model of loaded model and its cuboid surrounding environment

	FDM3<genmath::LongDouble> thermal_space_model_;
	// simulation statuses by unit time steps [<time frame>][<planar line>][<line value>]
	std::vector<std::vector<genmath::Vector<genmath::LongDouble> > > heat_map_time_frames_;
	std::vector<std::vector<genmath::Vector<genmath::LongDouble> > > heat_map_time_frames_coeffs_;
	// layer (projective) specific linear section history for posteriori, retrospective
	//  (after simulation evaluation) printing speed adjustment, First version: keep the recently
	//  finished layer and the currently printed section on the actual layer only. The heat conduction
	//  simulation assures the cooling time for all layer after each other
	std::vector<Section*> proj_sect_hist_;
	// current layer sections
	std::vector<Section*> proj_sect_;

	// file object of model source file
	FileControl input_file_;
	// defined by input gcode source file
	std::vector<GCodeCommand<genmath::LongDouble> > gcode_commands_;
	size_t cmd_i_;// current gcode command index
	size_t num_of_layers_;


	// whether there is an active optimization
	bool simulation_active_;
	bool simulation_suspended_;

public:
	// loading data from source model file
	void LoadCommandSequence(std::string file_path);
#ifndef TEST_MODULES
private:
#endif
	// validate local environmental temperature values of neighbor points in the previous layer 
	//  according to the current point
	bool ValidateLocalEnvTempVals(size_t curr_x, size_t curr_y, size_t curr_z,
		size_t* err_x, size_t* err_y, size_t* err_z);
	// assigning gridpoints to printed object and modify heat values of medium by position of heated nozzle
	void SetupLocalEnvironmentalPoints(size_t curr_x, size_t curr_y, size_t curr_z);

	// embedded body of static section evaluation (part of ProcessCurrUnitDurSect method)
	inline void PointProcessing(size_t& curr_x_ind, size_t& curr_y_ind, size_t& curr_z_ind);
	// embedded function for unit time simulation purpose at current given section
	inline void ProcessCurrUnitDurSect(genmath::LongDouble& start_virt_x, genmath::LongDouble& start_virt_y,
		genmath::LongDouble& end_virt_x, genmath::LongDouble& end_virt_y, size_t& curr_z_ind, Section* sect);
	// resetting evaluated environmental boundary values according to condition of heat loss
	// (not gain and unchanged)
	void ResetBoundaryValues();
	// setting up initial spatial thermal space for simulation processing
	void InitiateThermalSimulation();
public:
	// loading local configuration from model source
	void LoadModelConfig();
	// main method of printer command sequence optimization using 3D finite difference method
	// load command sequence and optimize it
	void ProcessSourceModel();
};

