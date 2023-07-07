#include "DistributableModel.h"

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::DistributableModel() {


}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::DistributableModel(size_t num_of_fragments_to_be_created) {

	if (!num_of_fragments_to_be_created)
		throw std::exception("Number of required computation model fragments is zero (distrsys::DistributableModel).");

	fragment_amount_limit_ = num_of_fragments_to_be_created;
	fragment_ind_ = 0;
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::DistributableModel(const DistributableModel& orig) {

	fragment_ind_ = orig.fragment_ind_;
	fragment_amount_limit_ = orig.fragment_amount_limit_;
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::~DistributableModel() {

	// RAII at original instantiation of object of object_ref_
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>& distrsys::DistributableModel<FT, RT>::operator=(const DistributableModel<FT, RT>& orig) {

	fragment_ind_ = orig.fragment_ind_;
	fragment_amount_limit_ = orig.fragment_amount_limit_;

	return *this;
}
template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator::FragmentIterator() {

	end_it = false;
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator::FragmentIterator(bool end_of_it) {

	end_it = end_of_it;
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator::FragmentIterator(const FragmentIterator& orig) {

	data = orig.data;
	end_it = orig.end_it;
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator::~FragmentIterator() {


}

template <class FT, class RT>
void distrsys::DistributableModel<FT, RT>::FragmentIterator::operator++() {

	try {

		data = this->GenNextFrag();
	}
	catch (std::exception err) {
	
		std::cout << err.what() << std::endl;
		end_it = true;
	}
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator::operator FT() const {

	return data;
}

template <class FT, class RT>
bool distrsys::DistributableModel<FT, RT>::FragmentIterator::operator==(const FragmentIterator& orig) const {

	return data == orig.data && end_it == orig.end_it;
}

template <class FT, class RT>
bool distrsys::DistributableModel<FT, RT>::FragmentIterator::operator!=(const FragmentIterator& orig) const {

	return data != orig.data && end_it != orig.end_it;
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator distrsys::DistributableModel<FT, RT>::Begin() {

	fragment_ind_ = 0;
	
	try {
	
		return FragmentIterator(this->GenNextFrag());
	}
	catch (std::exception err) {
	
		std::cout << err.what() << std::endl;
		return FragmentIterator(true);
	}
}

template <class FT, class RT>
distrsys::DistributableModel<FT, RT>::FragmentIterator distrsys::DistributableModel<FT, RT>::End() {

	return FragmentIterator(true);
}
