#pragma once

#include <vector>
#include <stdint.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <exception>
#include <type_traits>

#include "IterativeModel.h"
#include "../genmath/LongDouble.h"
#include "../genmath/Vector.h"
#include "../genmath/QMatrix.h"
#include "../genmath/TriDiagMatrix.h"
#include "FDM1.h"

struct NullObj {};

// implements Crank-Nicolson method in 3D
template <class T>
class FDM3 : 
	public IterativeModel<NullObj, T>,
{

public:
	FDM3();


	/// <summary>
	/// 3D finite difference method simulation (FDM)
	/// It operates with three axis based containers which store the 1D values and coefficients
	/// for the simulations. During the simulation the containers will be used alternately after each other.
	/// THE RESULT OF THE SIMULATION CAN BE OBTAINED THROUGH THE 1D Z VALUE AXIS LINE CONTAINERS.
	/// </summary>
	/// <param name="x_length">length of simulation space of value container x 1D lines</param>
	/// <param name="y_length">length of simulation space of value container y 1D lines</param>
	/// <param name="z_length">length of simulation space of value container z 1D lines</param>
	/// <param name="x_values">value container of x axis 1D lines</param>
	/// <param name="y_values">value container of y axis 1D lines</param>
	/// <param name="z_values">value container of z axis 1D lines</param>
	/// <param name="x_coeffs">coefficient container of x axis 1D lines</param>
	/// <param name="y_coeffs">coefficient container of y axis 1D lines</param>
	/// <param name="z_coeffs">coefficient container of z axis 1D lines</param>
	/// <param name="space_step">the space step of the spatial grid points of values</param>
	/// <param name="force_non_parallel">enable non parallel simulation
	/// (may highly increased computation time will be occurred)</param>
	FDM3(const size_t x_length,
		const size_t y_length,
		const size_t z_length,
		std::vector<genmath::Vector<T> >* x_values,
		std::vector<genmath::Vector<T> >* y_values,
		std::vector<genmath::Vector<T> >* z_values,
		std::vector<genmath::Vector<T> >* x_coeffs,
		std::vector<genmath::Vector<T> >* y_coeffs,
		std::vector<genmath::Vector<T> >* z_coeffs,
		const T space_step,
		const bool force_non_parallel = false,
		size_t num_of_fragments = 1);

	FDM3(const FDM3<T>& orig);

	FDM3<T>& operator=(const FDM3<T>& orig);

	virtual ~FDM3();

	void ProcessComponents(const uint16_t thread_ind);


	// OVERRIDED FUNCTIONS OF INTERATIVEMODEL BASE CLASS
	/// <summary>
	/// simulate the system forward with one time step
	/// </summary>
	[[noreturn]] NullObj& operator++() override;

	/// <summary>
	/// simulate the system backward with one time step
	/// </summary>
	[[noreturn]] NullObj& operator--() override;

	/// <summary>
	/// simulate the system forward with predefined duration, the simulation stops if the simulation 
	/// time reaches the duration increased target time or overruns it
	/// </summary>
	/// <param name="duration">duration of time to be simulated</param>
	[[noreturn]] NullObj& operator+=(const T duration) override;

	/// <summary>
	/// simulate the system backward with predefined duration, the simulation stops if the simualtion 
	/// time reaches the duration decreased target time or underruns it
	/// </summary>
	/// <param name="duration">duration of time to be simulated</param>
	[[noreturn]] NullObj& operator-=(const T duration) override;


	T GetTimeStep();

	T GetTime();

	void SetTime(const T time);

	// suspend simulation (suspension occurs among different axial 1D FDMs)
	void SuspendSimulation();

	// continue simulation (continuation resumes simulation among different axial 1D FDMs)
	void ContinueSimulation();

	void StopSimulation();


	operator std::string() override;


private:

	inline void UpdateXObjects();

	inline void UpdateYObjects();

	inline void UpdateZObjects();

	NullObj null_val_;

	// thread dependent parallel executions
	std::mutex start_execution_mutex_;
	std::condition_variable start_exec_cond_;
	volatile std::atomic_uint16_t active_threads_;
	// thread pool operation abortion at object destruction
	std::atomic_bool operate_fdm_;
	std::atomic_bool suspend_sim_;

	T space_step_;
	T time_step_;
	T recent_time_;
	size_t x_length_;
	size_t y_length_;
	size_t z_length_;
	std::vector<genmath::Vector<T> >* x_values_;
	std::vector<genmath::Vector<T> >* y_values_;
	std::vector<genmath::Vector<T> >* z_values_;
	std::vector<genmath::Vector<T> >* x_coeffs_;
	std::vector<genmath::Vector<T> >* y_coeffs_;
	std::vector<genmath::Vector<T> >* z_coeffs_;
	
	std::atomic_uint8_t recent_model_accessibility_;// previous lock value
	std::atomic_uint8_t model_accessibility_;// for external lock of model data processing
	std::mutex[3] leveled_sequential_mutexes_;
	std::unique_lock<std::mutex>[3] leveled_sequential_locks_;
	std::vector<FDM1<T> > x_components_;// x coordinate component of gradient
	std::vector<FDM1<T> > y_components_;// y coordinate component of gradient
	std::vector<FDM1<T> > z_components_;// z coordinate component of gradient
	uint16_t no_concurrent_threads_;
	std::vector<std::thread> exec_threads_;

	std::condition_variable close_wait_cond_;
	std::mutex close_wait_mutex_;
	bool operator_called_;// flag for terminating operation
	std::atomic_bool direction_;// true : ++, false : --
};

template class FDM3<genmath::LongDouble>;
template class IterativeModel<NullObj, genmath::LongDouble>;
