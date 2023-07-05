#include "FDM1.h"


template <class T>
printerheatconduction::FDM1<T>::FDM1() : IterativeModel<genmath::Vector<T>, T>() {

	time_step_ = "1.0";
	space_step_ = "1.0";
	recent_time_ = "0.0";
	lhs_operand_ = genmath::TriDiagMatrix<T>(3);
	rhs_operand_ = genmath::TriDiagMatrix<T>(3);
	identity_matrix_ = genmath::TriDiagMatrix<T>(3);
}


template <class T>
printerheatconduction::FDM1<T>::FDM1(genmath::Vector<T>* values, genmath::Vector<T>* coeffs, const T space_step, const T start_time)
	: IterativeModel<genmath::Vector<T>, T>(){

	if (values->Size() != coeffs->Size())
		throw std::exception("Sizes of containers are different (values and coefficients) (FDM1).");

	// tridiagonality check is in Matrix class
	size_t size_of_values = values->Size();

	if(size_of_values < 3) throw std::exception("Size of values are less than 3 (FDM1).");

	recent_values_ = values;
	
	// coefficient boundary examination is not required, user makes sure to provide proper values
	coeffs_ = coeffs;

	space_step_ = space_step;
	//time_step_ = space_step_ * (*coeffs_)[0];
	time_step_ = (space_step_ * space_step_) / (genmath::LongDouble("2.0") * (*coeffs_)[0]);
	
	if (space_step_ <= T("0.0")) throw std::exception("Space step can not be zero or negative (FDM1).");
	
	if (start_time < T("0.0")) throw std::exception("Start time can not be negative (FDM1).");

	recent_time_ = start_time;


	std::vector<T> lhs_data;
	lhs_data.reserve(4 + (size_of_values - 2) * 3);
	std::vector<T> rhs_data;
	rhs_data.reserve(4 + (size_of_values - 2) * 3);
	std::vector<T> identity_params;
	identity_params.reserve(4 + (size_of_values - 2) * 3);
	
	// OPTIMIZATION:
	//  make lhs_operand_, rhs_operand_ and identity_matrix_
	//  static read only (after init.) for memory optimization
	//  in case of identical matrices on each FDM1 object

	T null_elem("0.0");
	T unit_elem("1.0");
	T mtx_coeff = time_step_ / (T("2.0") * space_step_ * space_step_);

	lhs_data.push_back(T("2.0") * mtx_coeff);//1 + 2 * mtx_coeff
	lhs_data.push_back(T("-1.0") * mtx_coeff);//(-1) * mtx_coeff
	
	rhs_data.push_back(T("-2.0") * mtx_coeff);//1 - 2 * mtx_coeff
	rhs_data.push_back(mtx_coeff);//mtx_coeff

	identity_params.push_back(unit_elem);
	identity_params.push_back(null_elem);

	for (size_t i = 1; i < size_of_values - 1; ++i) {

		lhs_data.push_back(T("-1.0") * mtx_coeff);//(-1) * mtx_coeff
		lhs_data.push_back(T("2.0") * mtx_coeff);//1 + 2 * mtx_coeff
		lhs_data.push_back(T("-1.0") * mtx_coeff);//(-1) * mtx_coeff	
	
		rhs_data.push_back(mtx_coeff);//mtx_coeff
		rhs_data.push_back(T("-2.0") * mtx_coeff);//1 - 2 * mtx_coeff
		rhs_data.push_back(mtx_coeff);//mtx_coeff
	
		identity_params.push_back(null_elem);
		identity_params.push_back(unit_elem);
		identity_params.push_back(null_elem);
	}
		
	lhs_data.push_back(T("-1.0") * mtx_coeff);//(-1) * mtx_coeff
	lhs_data.push_back(T("2.0") * mtx_coeff);//1 + 2 * mtx_coeff
		
	rhs_data.push_back(mtx_coeff);//mtx_coeff
	rhs_data.push_back(T("-2.0") * mtx_coeff);//1 - 2 * mtx_coeff
	
	identity_params.push_back(null_elem);
	identity_params.push_back(unit_elem);
	
	lhs_operand_ = genmath::TriDiagMatrix<T>(lhs_data);
	rhs_operand_ = genmath::TriDiagMatrix<T>(rhs_data);
	identity_matrix_ = genmath::TriDiagMatrix<T>(identity_params);
	
	null_vector_ = genmath::Vector<T>(size_of_values);
	
}

template <class T>
printerheatconduction::FDM1<T>::FDM1(const FDM1<T>& orig) : IterativeModel<genmath::Vector<T>, T>(orig) {

	space_step_ = orig.space_step_;
	time_step_ = orig.time_step_;
	recent_time_ = orig.recent_time_;
	recent_values_ = orig.recent_values_;
	coeffs_ = orig.coeffs_;
	lhs_operand_ = orig.lhs_operand_;
	rhs_operand_ = orig.rhs_operand_;
	identity_matrix_ = orig.identity_matrix_;
	null_vector_ = orig.null_vector_;
}

template <class T>
printerheatconduction::FDM1<T>::~FDM1(){

}

template <class T>
printerheatconduction::FDM1<T>& printerheatconduction::FDM1<T>::operator=(const FDM1<T>& orig) {

	IterativeModel<genmath::Vector<T>, T>::operator=(orig);
	
	space_step_ = orig.space_step_;
	time_step_ = orig.time_step_;
	recent_time_ = orig.recent_time_;
	recent_values_ = orig.recent_values_;
	coeffs_ = orig.coeffs_;
	lhs_operand_ = orig.lhs_operand_;
	rhs_operand_ = orig.rhs_operand_;
	identity_matrix_ = orig.identity_matrix_;
	null_vector_ = orig.null_vector_;

	return *this;
}

template <class T>
genmath::Vector<T>& printerheatconduction::FDM1<T>::GetRecentValues() { 
	
	return *recent_values_; 
}

template <class T>
genmath::Vector<T>& printerheatconduction::FDM1<T>::GetCoeffs() { 
	
	return *coeffs_; 
}

template <class T>
T printerheatconduction::FDM1<T>::GetTime() {

	return recent_time_;
}

template <class T>
void printerheatconduction::FDM1<T>::SetTime(const T time) {

	recent_time_ = time;
}

// INHERITED FUNCTIONS FROM ITERATIVEMODEL

template <class T>
bool printerheatconduction::FDM1<T>::operator==(const FDM1<T>& orig) const {

	bool equals = true;
	equals = equals && space_step_ == orig.space_step_;
	equals = equals && time_step_ == orig.time_step_;
	equals = equals && recent_time_ == orig.recent_time_;
	equals = equals && recent_values_ == orig.recent_values_;
	equals = equals && coeffs_ == orig.coeffs_;
	equals = equals && lhs_operand_ == orig.lhs_operand_;
	equals = equals && rhs_operand_ == orig.rhs_operand_;
	equals = equals && identity_matrix_ == orig.identity_matrix_;
	equals = equals && null_vector_ == orig.null_vector_;

	return equals;
}

template <class T>
genmath::Vector<T>& printerheatconduction::FDM1<T>::operator++(){

	genmath::TriDiagMatrix<T> mod_lhs = identity_matrix_ + lhs_operand_.GenLinComb(*coeffs_);
	genmath::Vector<T> mod_rhs = (identity_matrix_ + rhs_operand_.GenLinComb(*coeffs_)) * (*recent_values_);
	*recent_values_ = mod_lhs.SolveGauss(mod_rhs);
	
	recent_time_ += time_step_;

	return *recent_values_;
}

template <class T>
genmath::Vector<T> printerheatconduction::FDM1<T>::operator++(int){
	
	genmath::Vector<T> ret(*recent_values_);
	genmath::TriDiagMatrix<T> mod_lhs = identity_matrix_ + lhs_operand_.GenLinComb(*coeffs_);
	genmath::Vector<T> mod_rhs = (identity_matrix_ + rhs_operand_.GenLinComb(*coeffs_)) * (*recent_values_);
	*recent_values_ = mod_lhs.SolveGauss(mod_rhs);
	
	recent_time_ += time_step_;
	
	return ret;
}

template <class T>
genmath::Vector<T>& printerheatconduction::FDM1<T>::operator--() {

	genmath::TriDiagMatrix<T> mod_rhs = identity_matrix_ + rhs_operand_.GenLinComb(*coeffs_);
	genmath::Vector<T> mod_lhs = (identity_matrix_ + lhs_operand_.GenLinComb(*coeffs_)) * (*recent_values_);
	*recent_values_ = mod_rhs.SolveGauss(mod_lhs);

	recent_time_ -= time_step_;

	return *recent_values_;
}

template <class T>
genmath::Vector<T> printerheatconduction::FDM1<T>::operator--(int) {

	genmath::Vector<T> ret(*recent_values_);
	genmath::TriDiagMatrix<T> mod_rhs = identity_matrix_ + rhs_operand_.GenLinComb(*coeffs_);
	genmath::Vector<T> mod_lhs = (identity_matrix_ + lhs_operand_.GenLinComb(*coeffs_)) * (*recent_values_);
	*recent_values_ = mod_rhs.SolveGauss(mod_lhs);

	recent_time_ -= time_step_;

	return ret;
}

template <class T>
genmath::Vector<T>& printerheatconduction::FDM1<T>::operator+=(const T duration) {

	if (T("0.0") >= duration || duration < time_step_) {
	
		throw std::exception("Requested duration is not valid or is less than the time step "
			"(operator+=) (FDM1).");
	}
	else {
	
		for (T rel_t = "0.0"; rel_t <= duration; rel_t += time_step_) this->operator++();
	}
	
	return *recent_values_;
}

template <class T>
genmath::Vector<T>& printerheatconduction::FDM1<T>::operator-=(const T duration) {

	if (T("0.0") >= duration || duration < time_step_) {
	
		throw std::exception("Requested duration is not valid or is less than the time step "
			"(operator-=) (FDM1).");
	}
	else if (recent_time_ - duration < T("0.0")) {

		throw std::exception("Requested duration decrements time into negative value (operator-=) (FDM1).");
	}
	else {
	
		for (T rel_t = duration; rel_t >= T("0.0"); rel_t -= time_step_) this->operator--();
	}

	return *recent_values_;
}
