#pragma once

namespace genmath {

	/// <summary>
	/// constraint of iterative computation models
	/// </summary>
	/// <typeparam name="FT">fragment type of computation model</typeparam>
	/// <typeparam name="T">atomic type of fragment type</typeparam>
	template <class FT, class T>
	class IterativeModel {

	public:
		IterativeModel();

		IterativeModel(const IterativeModel& orig);

		virtual ~IterativeModel();

		IterativeModel<FT, T>& operator=(const IterativeModel<FT, T>& orig);

		// throws exception in case of lack of next element (forward stepping)
		virtual FT& operator++() = 0;

		// throws exception in case of lack of next element (backward stepping)
		virtual FT& operator--() = 0;

		// throws exceptio in case of lack of next element (forward stepping)
		virtual FT operator++(int) = 0;

		// throws exceptio in case of lack of next element (backward stepping);
		virtual FT operator--(int) = 0;
	
		// throws exception in case of lack of next element (forward multiple stepping)
		virtual FT& operator+=(const T shift) = 0;

		// throws exception in case of lack of previous element (backward multiple stepping)
		virtual FT& operator-=(const T shift) = 0;
	};
}

