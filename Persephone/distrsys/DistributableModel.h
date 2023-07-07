#pragma once
#include <type_traits>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace distrsys {

	struct Splittable {

		/// <summary>
		/// It estimates the required abstract size of the provided fragment (actual) in aware of the 
		/// fragment informations. It is useful at unwrapping the model onto a fragment processor node
		/// </summary>
		/// <returns>Returns the assumed size of the unwrapped (to be) model chunk</returns>
		virtual size_t EstimateFragmentSize() = 0;

		/// <summary> 
		/// Parallel runnable executions, fragment chunks are in order starting from the first fragment 
		///  to last to be computed in order including parallel processings
		/// </summary>
		/// <param name="fragment_weights">The linearized, one dimensional container that stores the 
		/// weighthed portions of original model</param>
		/// <returns>Returns the created model fragments, portions according to the weighthening 
		/// ratios</returns>
		virtual std::vector<std::vector<Splittable*> > Split(std::vector<double> fragment_weights) = 0;
	};
	
	struct Mergable {

		/// <summary>
		/// Merge fragment results into a united result object
		/// </summary>
		/// <param name="fragments">The fragment result to be merged</param>
		/// <returns>A united result object is returned</returns>
		virtual Mergable& Merge(std::vector<Mergable*> fragments) = 0;
	};

	struct Synchronous {


		/// <summary>
		/// synchronization method of descriptive wrapper, 
		///  upper synchronizer method handler (controller) function calls other externally dependent 
		///  model fragment for synchronization (wait)
		/// </summary>
		virtual void Synchronize() = 0;

		/// <summary>
		/// Internally called function from explicit model implementation through descriptor class 
		/// of pointer
		/// </summary>
		virtual void Release() = 0;

		
		/// <summary>
		/// Gets fragment dependencies, fragments that make this fragment dependent from them
		/// </summary>
		/// <returns></returns>
		virtual std::vector<std::string> GetFragmentDependencies() = 0;

		std::vector<std::string> fragment_dependencies_;
		std::mutex synch_mutex_;
		std::condition_variable synch_cond_;
	};

	/// <summary>
	/// Constraint of distributable computation models
	/// </summary>
	/// <typeparam name="FT">Fragment type initial parameters of computation model, 
	/// this can be DistributableModel as well, but implicit wrapper too</typeparam>
	/// <typeparam name="RT">Fragment result type of computation model</typeparam>
	template <class FT, class RT>
	class DistributableModel {

		static_assert(std::is_base_of<Splittable, FT>::value, "Fragment type is not splittable.");
		static_assert(std::is_base_of<Mergable, RT>::value, "Result type is not mergable.");

	public:
		DistributableModel();

		DistributableModel(size_t num_of_fragments_to_be_created);

		DistributableModel(const DistributableModel& orig);

		virtual ~DistributableModel();

		DistributableModel<FT, RT>& operator=(const DistributableModel& orig);

		// iterative fragment computation generation
		struct FragmentIterator {

			FragmentIterator();

			FragmentIterator(bool end_of_it);

			FragmentIterator(const FragmentIterator& orig);

			virtual ~FragmentIterator();

			void operator++();

			operator FT() const;

			bool operator==(const FragmentIterator& orig) const;

			bool operator!=(const FragmentIterator& orig) const;

			FT data;
			bool end_it;
		};

		FragmentIterator Begin();
		FragmentIterator End();
		
		// end of generation by throwing exception
		virtual FT GenNextFrag() = 0;

		// merge computed result fragments
		virtual RT MergeFragments(std::vector<RT> result_fragments) = 0;

		// data serialization to string format
		virtual operator std::string() = 0;

		// getting implicit wrapper of computation model fragment
		virtual FT& GetWrapper() = 0;

		// modifying current computation model fragment
		virtual void ChangeModel(FT* new_model) = 0;

		// lock the model for arbitrary operations
		virtual void AcquireLock() = 0;

		// unlock the model after arbitrary operations are performed
		virtual void ReleaseLock() = 0;

	protected:

		size_t fragment_ind_;// index of generated fragment
		size_t fragment_amount_limit_;// number of fragment to be created from entire computation model
		FT wrapper_;
	};
}
