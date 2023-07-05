#pragma once
#include <vector>
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <exception>
#include <type_traits>

#include "IterativeModel.h"
#include "../genmath/LongDouble.h"
#include "../genmath/Vector.h"
#include "../genmath/QMatrix.h"
#include "../genmath/TriDiagMatrix.h"

namespace printerheatconduction {

	// one dimensional Finite Difference Method using Crank - Nicolson method
	template <class T>
	class FDM1 : public IterativeModel<genmath::Vector<T>, T> {

	public:
		FDM1();
	
		/// <summary>
		/// one dimensional finite difference method (FDM)
		/// </summary>
		/// <param name="values">Vector pointer to the values to be simulated</param>
		/// <param name="coeffs">Vector pointer to the coefficients to be used at simulation</param>
		/// <param name="space_step">spatial step on the imaginary 1D line to determine the distance
		/// between values</param>
		/// <param name="start_time">start time of the simulation</param>
		FDM1(genmath::Vector<T>* values, genmath::Vector<T>* coeffs, const T space_step, const T start_time);
	
		FDM1(const FDM1& orig);
	
		FDM1<T>& operator=(const FDM1<T>& orig);
	
		virtual ~FDM1();
		
		genmath::Vector<T>& GetRecentValues();

		genmath::Vector<T>& GetCoeffs();

		T GetTime();

		void SetTime(T time);

		// INHERITED FUNCTIONS FROM ITERATIVEMODEL

		bool operator==(const FDM1<T>& orig) const;

		/// <summary>
		/// prefix stepping the simulation by defined time step
		/// </summary>
		/// <returns>returns the computed new values</returns>
		genmath::Vector<T>& operator++() override;

		/// <summary>
		/// prefix backstepping the simulation by defined time step
		/// </summary>
		/// <returns>returns the computed new values</returns>
		genmath::Vector<T>& operator--() override;

		/// <summary>
		/// postfix stepping the simulation by defined time step
		/// </summary>
		/// <returns>returns the previous values before the calculation</returns>
		genmath::Vector<T> operator++(int) override;
 
		/// <summary>
		/// postfix backstepping the simulation by defined time step
		/// </summary>
		/// <param name="">returns the previous values before the calculation</param>
		/// <returns></returns>
		genmath::Vector<T> operator--(int) override;

		/// <summary>
		/// runs the simulation forward until the given value is reached, exceeded
		/// </summary>
		/// <param name="duration">duration of time to be simulated</param>
		/// <returns>returns the calculated new values</returns>
		genmath::Vector<T>& operator+=(const T duration) override;

		/// <summary>
		/// runs the simulation backward until the given value is reached, exceeded
		/// </summary>
		/// <returns>returns the calculated new values</returns>
		genmath::Vector<T>& operator-=(const T duration) override;

	private:
		T space_step_;
		T time_step_;
		T recent_time_;
		genmath::Vector<T>* recent_values_;
		genmath::Vector<T>* coeffs_;
		genmath::TriDiagMatrix<T> lhs_operand_;// making member static is forbidden due to critical section 
		genmath::TriDiagMatrix<T> rhs_operand_;
		genmath::TriDiagMatrix<T> identity_matrix_;
		genmath::Vector<T> null_vector_;
	};
}

template class printerheatconduction::FDM1<genmath::LongDouble>;
template class IterativeModel<genmath::Vector<genmath::LongDouble>, genmath::LongDouble>;
