#include "FDM3.h"

// 3D FINITE DIFFERENCE METHOD

template <class T>
printerheatconduction::FDM3<T>::FDM3() 
	: genmath::IterativeModel<NullObj, T>() {

	operate_fdm_ = true;
	suspend_sim_ = false;
	no_concurrent_threads_ = 0;
	active_threads_ = 0;
	operator_called_ = false;
}

template <class T>
printerheatconduction::FDM3<T>::FDM3(const size_t x_length,
	const size_t y_length,
	const size_t z_length,
	std::vector<genmath::Vector<T> >* x_values,
	std::vector<genmath::Vector<T> >* y_values,
	std::vector<genmath::Vector<T> >* z_values,
	std::vector<genmath::Vector<T> >* x_coeffs,
	std::vector<genmath::Vector<T> >* y_coeffs,
	std::vector<genmath::Vector<T> >* z_coeffs,
	const T space_step,
	const bool force_non_parallel) 
	: genmath::IterativeModel<NullObj, T>() {

	space_step_ = space_step;
	// MATHEMATICAL OPTIMIZATION: set possible maximal coefficient value for numerical stability increase
	time_step_ = (*z_coeffs)[0][0] * space_step * T("3.0");
	recent_time_ = "0.0";

	x_length_ = x_length;
	y_length_ = y_length;
	z_length_ = z_length;
	x_values_ = x_values;
	y_values_ = y_values;
	z_values_ = z_values;
	x_coeffs_ = x_coeffs;
	y_coeffs_ = y_coeffs;
	z_coeffs_ = z_coeffs;
	x_components_ = std::vector<FDM1<T> >();
	y_components_ = std::vector<FDM1<T> >();
	z_components_ = std::vector<FDM1<T> >();

	// creating x axis based 1D FDM simulation objects
	size_t max_i = x_values_->size();
	x_components_.reserve(max_i);
	for (size_t i = 0; i < max_i; ++i)
		x_components_.push_back(
			FDM1<T>(&(*x_values_)[i], &(*x_coeffs_)[i], space_step_, T("0.0")));

	// creating y axis based 1D FDM simulation objects
	max_i = y_values_->size();
	y_components_.reserve(max_i);
	for (size_t i = 0; i < max_i; ++i)
		y_components_.push_back(
			FDM1<T>(&(*y_values_)[i], &(*y_coeffs_)[i], space_step_, time_step_ * T("1.0") / T("3.0")));

	// creating z axis based 1D FDM simulation objects
	max_i = z_values_->size();
	z_components_.reserve(max_i);
	for (size_t i = 0; i < max_i; ++i)
		z_components_.push_back(
			FDM1<T>(&(*z_values_)[i], &(*z_coeffs_)[i], space_step_, time_step_ * T("2.0") / T("3.0")));

	no_concurrent_threads_ = force_non_parallel ? 0 : std::thread::hardware_concurrency();
	active_threads_ = no_concurrent_threads_;

	if (no_concurrent_threads_) {

		for (uint16_t id = 0; id < no_concurrent_threads_; ++id)
			exec_threads_.push_back(std::thread(&FDM3::ProcessComponents, this, id));
	}
	else {

		no_concurrent_threads_ = 0;
	}
	operate_fdm_ = true;
	suspend_sim_ = false;
	operator_called_ = false;
	direction_ = true;
}

template <class T>
printerheatconduction::FDM3<T>::FDM3(const FDM3<T>& orig) 
	: genmath::IterativeModel<NullObj, T>(orig) {

	space_step_ = orig.space_step_;
	time_step_ = orig.time_step_;
	recent_time_ = orig.recent_time_;
	x_length_ = orig.x_length_;
	y_length_ = orig.y_length_;
	z_length_ = orig.z_length_;
	x_values_ = orig.x_values_;
	y_values_ = orig.y_values_;
	z_values_ = orig.z_values_;
	x_coeffs_ = orig.x_coeffs_;
	y_coeffs_ = orig.y_coeffs_;
	z_coeffs_ = orig.z_coeffs_;
	x_components_ = orig.x_components_;
	y_components_ = orig.y_components_;
	z_components_ = orig.z_components_;
	no_concurrent_threads_ = orig.no_concurrent_threads_;
	active_threads_ = no_concurrent_threads_;
	operator_called_ = orig.operator_called_;
	operate_fdm_ = true;
	suspend_sim_ = false;
	direction_ = orig.direction_ ? true : false;

	// processing threads will be destroyed at call of destructor
	if (no_concurrent_threads_)
		for (uint16_t id = 0; id < no_concurrent_threads_; ++id)
			exec_threads_.push_back(std::thread(&FDM3::ProcessComponents, this, id));
}

template <class T>
printerheatconduction::FDM3<T>& printerheatconduction::FDM3<T>::operator=(const FDM3<T>& orig) {

	genmath::IterativeModel<NullObj, T>::operator=(orig);
	
	space_step_ = orig.space_step_;
	time_step_ = orig.time_step_;
	recent_time_ = orig.recent_time_;
	x_length_ = orig.x_length_;
	y_length_ = orig.y_length_;
	z_length_ = orig.z_length_;
	x_values_ = orig.x_values_;
	y_values_ = orig.y_values_;
	z_values_ = orig.z_values_;
	x_coeffs_ = orig.x_coeffs_;
	y_coeffs_ = orig.y_coeffs_;
	z_coeffs_ = orig.z_coeffs_;
	x_components_ = orig.x_components_;
	y_components_ = orig.y_components_;
	z_components_ = orig.z_components_;
	no_concurrent_threads_ = orig.no_concurrent_threads_;
	active_threads_ = no_concurrent_threads_;
	operator_called_ = orig.operator_called_;
	direction_ = orig.direction_ ? true : false;

	// processing threads will be destroyed at call of destructor
	if (no_concurrent_threads_)
		for (uint16_t id = 0; id < no_concurrent_threads_; ++id)
			exec_threads_.push_back(std::thread(&FDM3::ProcessComponents, this, id));

	operate_fdm_ = true;
	suspend_sim_ = false;

	return *this;
}

template <class T>
printerheatconduction::FDM3<T>::~FDM3() {

	if (no_concurrent_threads_) {

		operate_fdm_ = false;
		start_exec_cond_.notify_all();

		for (uint16_t id = 0; id < exec_threads_.size(); ++id)
			exec_threads_[id].join();
	}
	else {

		operate_fdm_ = false;
	}
}

// thread pool method
template <class T>
void printerheatconduction::FDM3<T>::ProcessComponents(const uint16_t thread_ind) {

	size_t start_fdm1_x_ind, end_fdm1_x_ind;
	size_t start_fdm1_y_ind, end_fdm1_y_ind;
	size_t start_fdm1_z_ind, end_fdm1_z_ind;

	size_t remainder_size = 0;
	if (remainder_size = x_components_.size() % no_concurrent_threads_ && remainder_size > thread_ind) {

		start_fdm1_x_ind = thread_ind * x_components_.size() / no_concurrent_threads_;
		end_fdm1_x_ind = (thread_ind + 1) * (x_components_.size() / no_concurrent_threads_ + 1);
	}
	else {

		start_fdm1_x_ind = thread_ind * x_components_.size() / no_concurrent_threads_;
		end_fdm1_x_ind = (thread_ind + 1) * x_components_.size() / no_concurrent_threads_;
	}

	if (remainder_size = y_components_.size() % no_concurrent_threads_ && remainder_size > thread_ind) {

		start_fdm1_y_ind = thread_ind * y_components_.size() / no_concurrent_threads_;
		end_fdm1_y_ind = (thread_ind + 1) * (y_components_.size() / no_concurrent_threads_ + 1);
	}
	else {

		start_fdm1_y_ind = thread_ind * y_components_.size() / no_concurrent_threads_;
		end_fdm1_y_ind = (thread_ind + 1) * y_components_.size() / no_concurrent_threads_;
	}

	if (remainder_size = z_components_.size() % no_concurrent_threads_ && remainder_size > thread_ind) {

		start_fdm1_z_ind = thread_ind * z_components_.size() / no_concurrent_threads_;
		end_fdm1_z_ind = (thread_ind + 1) * (z_components_.size() / no_concurrent_threads_ + 1);
	}
	else {

		start_fdm1_z_ind = thread_ind * z_components_.size() / no_concurrent_threads_;
		end_fdm1_z_ind = (thread_ind + 1) * z_components_.size() / no_concurrent_threads_;
	}

	std::unique_lock<std::mutex> pause_exec(start_execution_mutex_);

	// delay of start of computation process controller thread
	std::this_thread::sleep_for(std::chrono::milliseconds(thread_ind * 10 + 10));
	
	while (operate_fdm_) {

		--active_threads_;
		start_exec_cond_.wait(pause_exec);
		if (!operate_fdm_) {

			--active_threads_;
			return;
		}

		// updating values vector and coefficient vectors in simulation stepper function
		//  for computing x coordinate component based 1D FDMs

		if (direction_) {

			for (size_t curr_fdm1_x_ind = start_fdm1_x_ind; curr_fdm1_x_ind < end_fdm1_x_ind; ++curr_fdm1_x_ind) {

				++x_components_[curr_fdm1_x_ind];
				x_components_[curr_fdm1_x_ind].SetTime(recent_time_);
			}
		}
		else {

			for (size_t curr_fdm1_x_ind = start_fdm1_x_ind; curr_fdm1_x_ind < end_fdm1_x_ind; ++curr_fdm1_x_ind) {

				--x_components_[curr_fdm1_x_ind];
				x_components_[curr_fdm1_x_ind].SetTime(recent_time_);
			}
		}
		--active_threads_;
		start_exec_cond_.wait(pause_exec);
		if (!operate_fdm_) {

			--active_threads_;
			return;
		}

		// updating value vectors and coeffcient vectors in simulation stepper function
		//  for computing y coordinate component based 1D FDMs

		if (direction_) {

			for (size_t curr_fdm1_y_ind = start_fdm1_y_ind; curr_fdm1_y_ind < end_fdm1_y_ind; ++curr_fdm1_y_ind) {

				++y_components_[curr_fdm1_y_ind];
				y_components_[curr_fdm1_y_ind].SetTime(recent_time_ + time_step_ * T("1.0") / T("3.0"));
			}
		}
		else {

			for (size_t curr_fdm1_y_ind = start_fdm1_y_ind; curr_fdm1_y_ind < end_fdm1_y_ind; ++curr_fdm1_y_ind) {

				--y_components_[curr_fdm1_y_ind];
				y_components_[curr_fdm1_y_ind].SetTime(recent_time_ - time_step_ * T("1.0") / T("3.0"));
			}
		}
		--active_threads_;
		start_exec_cond_.wait(pause_exec);
		if (!operate_fdm_) {

			--active_threads_;
			return;
		}

		// updating value vectors and coefficient vectors in simulation stepper function
		//  for computing z coordinate component based 1D FDMs

		if (direction_) {

			for (size_t curr_fdm1_z_ind = start_fdm1_z_ind; curr_fdm1_z_ind < end_fdm1_z_ind; ++curr_fdm1_z_ind) {

				++z_components_[curr_fdm1_z_ind];
				z_components_[curr_fdm1_z_ind].SetTime(recent_time_ + time_step_ * T("2.0") / T("3.0"));
			}
		}
		else {

			for (size_t curr_fdm1_z_ind = start_fdm1_z_ind; curr_fdm1_z_ind < end_fdm1_z_ind; ++curr_fdm1_z_ind) {

				--z_components_[curr_fdm1_z_ind];
				z_components_[curr_fdm1_z_ind].SetTime(recent_time_ - time_step_ * T("2.0") / T("3.0"));
			}
		}
	}
}

template <class T>
[[noreturn]] printerheatconduction::NullObj& printerheatconduction::FDM3<T>::operator++() {

	direction_ = true;
	operator_called_ = true;

	if (no_concurrent_threads_) [[likely]] {

		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
		{
			std::scoped_lock<std::mutex> update_lock(start_execution_mutex_);

			// updating vectors and coefficients for x 1D FDMs
			model_accessibility_ = 0;// x values are updated
			std::unique_lock<std::mutex> inter_access_lock(leveled_sequential_mutexes_[model_accessibility_]);
			this->UpdateXObjects();
			active_threads_ = no_concurrent_threads_;
		}
		start_exec_cond_.notify_all();

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		// parallel execution with threads is starting
		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
		{
			std::scoped_lock<std::mutex> update_lock(start_execution_mutex_);

			// updating vectors and coefficients for y 1D FDMs
			this->UpdateYObjects();
			model_accessibility_ = 1;// y values are updated
			active_threads_ = no_concurrent_threads_;
		}
		start_exec_cond_.notify_all();

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
		{
			std::scoped_lock<std::mutex> update_lock(start_execution_mutex_);

			// updating vectors and coefficients for z 1D FDMs
			this->UpdateZObjects();
			model_accessibility_ = 2;
			active_threads_ = no_concurrent_threads_;
		}
		start_exec_cond_.notify_all();

		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
	}
	else {
		// execution without thread creations

		while (suspend_sim_) std::this_thread::yield();
		// updating vectors and coefficients for x 1D FDMs
		this->UpdateXObjects();
		model_accessibility_ = 0;

		// computing x axis based 1D FDM objects
		for (FDM1<T>& x_component : x_components_) {

			++x_component;
			x_component.SetTime(recent_time_);
		}

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		while (suspend_sim_) std::this_thread::yield();
		// updating vectors and coefficients for y 1D FDMs
		this->UpdateYObjects();
		model_accessibility_ = 1;

		// computing y axis based 1D FDM objects
		for (FDM1<T>& y_component : y_components_) {

			++y_component;
			y_component.SetTime(recent_time_ + time_step_ * T("1.0") / T("3.0"));
		}

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		while (suspend_sim_) std::this_thread::yield();
		// updating vectors and coefficients for z 1D FDMs
		this->UpdateZObjects();
		model_accessibility_ = 2;

		// computing z axis based 1D FDM objects
		for (FDM1<T>& z_component : z_components_) {

			++z_component;
			z_component.SetTime(recent_time_ + time_step_ * T("2.0") / T("3.0"));
		}
		while (suspend_sim_) std::this_thread::yield();
	}

	recent_time_ += time_step_;
	operator_called_ = false;
}

template <class T>
[[noreturn]] printerheatconduction::NullObj& printerheatconduction::FDM3<T>::operator--() {

	direction_ = false;
	operator_called_ = true;

	if (no_concurrent_threads_) [[likely]] {

		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
		{
			std::scoped_lock<std::mutex> update_lock(start_execution_mutex_);

			// updating vectors and coefficients for x 1D FDMs
			this->UpdateXObjects();
			model_accessibility_ = 0;
			active_threads_ = no_concurrent_threads_;
		}
		start_exec_cond_.notify_all();

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		// parallel execution with threads is starting
		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
		{
			std::scoped_lock<std::mutex> update_lock(start_execution_mutex_);

			// updating vectors and coefficients for y 1D FDMs
			this->UpdateYObjects();
			model_accessibility_ = 1;
			active_threads_ = no_concurrent_threads_;
		}
		start_exec_cond_.notify_all();

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
		{
			std::scoped_lock<std::mutex> update_lock(start_execution_mutex_);

			// updating vectors and coefficients for z 1D FDMs
			this->UpdateZObjects();
			model_accessibility_ = 2;
			active_threads_ = no_concurrent_threads_;
		}
		start_exec_cond_.notify_all();

		while (active_threads_) std::this_thread::yield();
		while (suspend_sim_) std::this_thread::yield();
	}
	else {
		// execution without thread creations

		while (suspend_sim_) std::this_thread::yield();
		// updating vectors and coefficients for x 1D FDMs
		this->UpdateXObjects();
		model_accessibility_ = 0;

		// computing x axis based 1D FDM objects
		for (FDM1<T>& x_component : x_components_) {

			--x_component;
			x_component.SetTime(recent_time_);
		}

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		while (suspend_sim_) std::this_thread::yield();
		// updating vectors and coefficients for y 1D FDMs
		this->UpdateYObjects();
		model_accessibility_ = 1;

		// computing y axis based 1D FDM objects
		for (FDM1<T>& y_component : y_components_) {

			--y_component;
			y_component.SetTime(recent_time_ - time_step_ * T("1.0") / T("3.0"));
		}

		if (!operate_fdm_) {

			close_wait_cond_.notify_one();
			return null_val_;
		}

		while (suspend_sim_) std::this_thread::yield();
		// updating vectors and coefficients for z 1D FDMs
		this->UpdateZObjects();
		model_accessibility_ = 2;

		// computing z axis based 1D FDM objects
		for (FDM1<T>& z_component : z_components_) {

			--z_component;
			z_component.SetTime(recent_time_ - time_step_ * T("2.0") / T("3.0"));
		}
		while (suspend_sim_) std::this_thread::yield();
	}

	recent_time_ += time_step_;
	operator_called_ = false;
}

template <class T>
[[noreturn]] printerheatconduction::NullObj printerheatconduction::FDM3<T>::operator++(int) {

	return operator++();
}

template <class T>
[[noreturn]] printerheatconduction::NullObj printerheatconduction::FDM3<T>::operator--(int) {

	return operator--();
}

template <class T>
[[noreturn]] printerheatconduction::NullObj& printerheatconduction::FDM3<T>::operator+=(const T duration) {

	if (T("0.0") >= duration || duration < time_step_) {

		throw std::exception("Desired duration is negative or less than the time step "
			"(operator+=) (FDM3).");
	}
	else {

		for (T rel_t = "0.0"; rel_t <= duration; rel_t += time_step_) this->operator++();
	}

	return null_val_;
}

template <class T>
[[noreturn]] printerheatconduction::NullObj& printerheatconduction::FDM3<T>::operator-=(const T duration) {

	if (T("0.0") >= duration || duration < time_step_) {

		throw std::exception("Requested duration is not valid or is less than the time step "
			"(operator-=) (FDM1).");
	}
	else if (recent_time_ - duration < T("0.0")) {

		throw std::exception("Requested duration decrements time into negative value (operator-=) (FDM3).");
	}
	else {

		for (T rel_t = duration; rel_t >= T("0.0"); rel_t -= time_step_) this->operator--();
	}

	return null_val_;
}

template <class T>
T printerheatconduction::FDM3<T>::GetTimeStep() {

	return time_step_;
}

template <class T>
T printerheatconduction::FDM3<T>::GetTime() {

	return recent_time_;
}

template <class T>
void printerheatconduction::FDM3<T>::SetTime(const T time) {

	recent_time_ = time;
}

template <class T>
void printerheatconduction::FDM3<T>::SuspendSimulation() {

	suspend_sim_ = true;
}

template <class T>
void printerheatconduction::FDM3<T>::ContinueSimulation() {

	suspend_sim_ = false;
}

template <class T>
void printerheatconduction::FDM3<T>::StopSimulation() {

	if (operate_fdm_) {

		operate_fdm_ = false;
		suspend_sim_ = false;

		while (active_threads_) std::this_thread::yield();
		start_exec_cond_.notify_all();

		if (operator_called_) {

			std::unique_lock<std::mutex> close_lock(close_wait_mutex_);
			close_wait_cond_.wait(close_lock);
		}
	}
}

template <class T>
void printerheatconduction::FDM3<T>::UpdateXObjects() {

	for (size_t z = 0; z < z_length_; ++z) {

		for (size_t y = 0; y < y_length_; ++y) {

			for (size_t x = 0; x < x_length_; ++x) {

				(*x_values_)[z * y_length_ + y][x] = (*z_values_)[y * x_length_ + x][z];
				(*x_coeffs_)[z * y_length_ + y][x] = (*z_coeffs_)[y * x_length_ + x][z];
			}
		}
	}
}

template <class T>
void printerheatconduction::FDM3<T>::UpdateYObjects() {

	for (size_t z = 0; z < z_length_; ++z) {

		for (size_t x = 0; x < x_length_; ++x) {

			for (size_t y = 0; y < y_length_; ++y) {

				(*y_values_)[z * x_length_ + x][y] = (*x_values_)[z * y_length_ + y][x];
				(*y_coeffs_)[z * x_length_ + x][y] = (*x_coeffs_)[z * y_length_ + y][x];
			}
		}
	}
}

template <class T>
void printerheatconduction::FDM3<T>::UpdateZObjects() {

	for (size_t x = 0; x < x_length_; ++x) {

		for (size_t y = 0; y < y_length_; ++y) {

			for (size_t z = 0; z < z_length_; ++z) {

				(*z_values_)[y * x_length_ + x][z] = (*y_values_)[z * x_length_ + x][y];
				(*z_coeffs_)[y * x_length_ + x][z] = (*y_coeffs_)[z * x_length_ + x][y];
			}
		}
	}
}
