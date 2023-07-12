#include "GCodeCommand.h"

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::CmdModels::CmdModels() {

	models_ = std::vector<Cmd>();
	// intializing allowed GCode commands
	models_.reserve(21);
	models_.push_back(Cmd("G0", std::vector<std::string>{ "F", "X", "Y", "Z" }));
	models_.push_back(Cmd("G1", std::vector<std::string>{ "X", "Y", "Z", "F", "E" }));
	models_.push_back(Cmd("G28"));
	models_.push_back(Cmd("G90"));
	models_.push_back(Cmd("G91"));
	models_.push_back(Cmd("G92", std::vector<std::string>{ "E" }));
	models_.push_back(Cmd("M82"));
	models_.push_back(Cmd("M84", std::vector<std::string>{ "X", "Y", "Z", "E" }, true));
	models_.push_back(Cmd("M104", std::vector<std::string>{ "S" }));
	models_.push_back(Cmd("M105"));
	models_.push_back(Cmd("M106", std::vector<std::string>{ "S" }));
	models_.push_back(Cmd("M107"));
	models_.push_back(Cmd("M109", std::vector<std::string>{ "S" }));
	models_.push_back(Cmd("M140", std::vector<std::string>{ "S" }));
	models_.push_back(Cmd("M190", std::vector<std::string>{ "S" }));
	models_.push_back(Cmd("M201", std::vector<std::string>{ "X", "Y", "Z", "E" }));
	models_.push_back(Cmd("M203", std::vector<std::string>{ "X", "Y", "Z", "E" }));
	models_.push_back(Cmd("M204", std::vector<std::string>{ "P", "R", "T" }));
	models_.push_back(Cmd("M205", std::vector<std::string>{ "X", "Y", "Z", "E" }));
	models_.push_back(Cmd("M220", std::vector<std::string>{ "S" }));
	models_.push_back(Cmd("M221", std::vector<std::string>{ "S" }));
	size_of_models_ = models_.size();
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::CmdModels::CmdModels(const CmdModels& orig) {

	models_ = orig.models_, size_of_models_ = orig.size_of_models_; 
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::CmdModels::~CmdModels() {


}

template <class T> requires printersimulation::StringAssignable<T>
typename printersimulation::GCodeCommand<T>::CmdModels printersimulation::GCodeCommand<T>::CommandModels = printersimulation::GCodeCommand<T>::CmdModels();

template <class T> requires printersimulation::StringAssignable<T>
char printersimulation::GCodeCommand<T>::DelimChar = ' ';

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::GCodeCommand() : ObjectBase() {

	name_ = "";
	params_ = std::map<std::string, T>();
	model_ref_ = nullptr;
	valueless_params_ = false;
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::GCodeCommand(std::string name, std::map<std::string, T> params) : ObjectBase() {

	bool found = false;
	size_t size_of_cmds = CommandModels.models_.size();

	for (size_t i = 0; i < size_of_cmds; ++i) {
	
		if (CommandModels.models_[i].name_ == name) {
		
			found = true;
			model_ref_ = &CommandModels.models_[i];
			valueless_params_ = model_ref_->valueless_params_;
			break;
		}
	}
	
	if(!found) [[unlikely]] throw std::exception("No such command (GCodeCommand).");

	name_ = name;
	T valueless_test("0.0");

	bool at_least_one_param = false;
	typename std::map<std::string, T>::const_iterator params_end_it = params.end();

	for (typename std::map<std::string, T>::const_iterator params_it = params.begin();
		params_it != params_end_it; ++params_it) {
	
		found = false;
		size_t cmds_i_params_size = model_ref_->params_.size();
		
		for (size_t j = 0; j < cmds_i_params_size && !found; ++j)
			found = params_it->first == model_ref_->params_[j] || params_it->first[0] == ';';

		if (params_it->first[0] == ';') [[unlikely]] break;

		if (!found) [[unlikely]] throw std::exception("Parameter name is not valid (GCodeCommand).");

		if (valueless_params_ && params_it->second != valueless_test) [[unlikely]]
			throw std::exception("Wrong value format. Command has valueless parameters (GCodeCommand).");
		
		params_.insert(params_.end(), {params_it->first, params_it->second} );
		at_least_one_param = true;
	}

	if (model_ref_->params_.size() > 0 && !at_least_one_param) [[unlikely]]
		throw std::exception("Insufficient number of parameters (GCodeCommand).");
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::GCodeCommand(std::string raw_data) {

	if (this->operator=(raw_data)) [[unlikely]]
		throw std::exception("There was an error during conversion raw data to object (GCodeCommand).");
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::GCodeCommand(const GCodeCommand<T>& orig) : ObjectBase(orig) {

	name_ = orig.name_;
	params_ = orig.params_;
	model_ref_ = orig.model_ref_;
	valueless_params_ = orig.valueless_params_;
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::~GCodeCommand() {

}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>& printersimulation::GCodeCommand<T>::operator=(const GCodeCommand<T>& orig) {

	ObjectBase::operator=(orig);

	name_ = orig.name_;
	params_ = orig.params_;
	model_ref_ = orig.model_ref_;
	valueless_params_ = orig.valueless_params_;

	return *this;
}

template <class T> requires printersimulation::StringAssignable<T>
bool printersimulation::GCodeCommand<T>::operator=(std::string raw_data){

	std::stringstream ss_data(raw_data);
	std::string name = "";
	name_ = "";
	params_.clear();
	std::getline(ss_data, name, printersimulation::GCodeCommand<T>::DelimChar);


	if (name.substr(0, 1) == ";") {
		
		// ;TYPE:WALL-INNER
		// ;TYPE:WALL-OUTER
		// ;TYPE:SKIN
		// ;TYPE:FILL
		// ;LAYER:<numeric>
		// ;TIME_ELAPSED:<numeric>
		// ;MESH:<mesh_name-file_name
		//..
		name_ = name;
		valueless_params_ = true;
	}
	else {

		bool at_least_one_param = false;
		bool found = false;

		// c style index declaration caused runtime error
		for (size_t i = 0; i < CommandModels.size_of_models_; ++i) {

			if (CommandModels.models_[i].name_ == name) {
				
				found = true;
				model_ref_ = &CommandModels.models_[i];
				valueless_params_ = model_ref_->valueless_params_;
				break;
			}
		}

		if (!found) {

			// no such command
			name_ = "";
			return true;
		}
		
		name_ = name;
		bool at_leas_one_param = false;
		size_t length_of_model_cmd_param = 0;
		T value = "0.0";// it is placeholder at valueless commands


		while (!ss_data.eof()) {
			
			std::getline(ss_data, name, printersimulation::GCodeCommand<T>::DelimChar);

			found = false;
			size_t cmds_i_params_size = model_ref_->params_.size();
			
			for (size_t j = 0; j < cmds_i_params_size && !found; ++j) {
				
				length_of_model_cmd_param = model_ref_->params_[j].size();

				found = (length_of_model_cmd_param <= name.size()
					&& model_ref_->params_[j] == name.substr(0, length_of_model_cmd_param)
					|| name[0] == ';');
			}

			if (name[0] == ';') [[unlikely]] break;// ignoring tail comments
			
			if (!found) [[unlikely]] {

				// parameter name is not valid
				name_ = "parameter name is not valid";
				params_.clear();
				return true;
			}
			
			if (!valueless_params_ && (value = name.substr(length_of_model_cmd_param))) [[unlikely]] {
			
				name_ = "wrong value format";
				return true;
			}

			params_.insert(params_.end(), { std::string(1, name[0]), value });
			
			at_least_one_param = true;
		}

		if (model_ref_->params_.size() > 0 && !at_least_one_param) [[unlikely]] {

			// insufficient number of parameters
			name_ = "insuffient parameters";
			return true;
		}
	}

	return false;
}

template <class T> requires printersimulation::StringAssignable<T>
printersimulation::GCodeCommand<T>::operator std::string() const {

	std::string ret = "";
	ret += name_;
	typename std::map<std::string, T>::const_iterator end_it = params_.end();

	if (valueless_params_) [[unlikely]] {
	
		for (typename std::map<std::string, T>::const_iterator it = params_.begin(); it != end_it; ++it)
			ret += (DelimChar + it->first);
	}
	else [[likely]] {
	
		for (typename std::map<std::string, T>::const_iterator it = params_.begin(); it != end_it; ++it)
			ret += (DelimChar + it->first + std::string(it->second));
	}

	return ret;
}

template <class T> requires printersimulation::StringAssignable<T>
std::string& printersimulation::GCodeCommand<T>::GetName() {

	return name_;
}

template <class T> requires printersimulation::StringAssignable<T>
bool printersimulation::GCodeCommand<T>::IsSet(std::string key) {

	if (!model_ref_->params_.size()) return false;

	if (params_.count(key))	return true;

	return false;
}

template <class T> requires printersimulation::StringAssignable<T>
T& printersimulation::GCodeCommand<T>::Get(std::string key) {

	if (valueless_params_) [[unlikely]]
		throw std::exception("Command contains no parameters. It is valueless (GCodeCommand).");

	if (!params_.count(key)) [[unlikely]]
		throw std::exception("Requested parameter is not set or inaccessable (GCodeCommand).");
	
	return params_[key];
}

template <class T> requires printersimulation::StringAssignable<T>
void printersimulation::GCodeCommand<T>::Set(std::string key, T value) {

	if (valueless_params_) [[unlikely]]
		throw std::exception("Command contains valueless parameters (GCodeCommand).");

	bool exists = false;
	size_t params_size = model_ref_->params_.size();

	for(size_t i = 0; i < params_size && !exists; ++i)
		exists = key == model_ref_->params_[i];
	
	if(!exists) [[unlikely]]
		throw std::exception("Requested parameter can not be set or inaccessable (GCodeCommand).");
	
	if (params_.count(key)) params_[key] = value;
	else params_.insert(params_.end(), { key, value });
}
