#include "ConsoleUI.h"

GenericMenuCommand::GenericMenuCommand() {


}

GenericMenuCommand::GenericMenuCommand(const GenericMenuCommand& orig) {


}

GenericMenuCommand::~GenericMenuCommand() {

}

void GenericMenuCommand::operator()(const std::vector<std::string>& params) {


}

const std::string GenericMenuCommand::GetParamListHint() const {

	return std::string("");
}


const std::string GenericMenuCommand::GetExplanation() const {

	return std::string("");
}

const uint8_t GenericMenuCommand::GetNumOfParams() const {

	return 0;
}

template <class HANDLER>
MenuCommand<HANDLER>::MenuCommand() : GenericMenuCommand() {

	num_of_params_ = 0;
	handler_object_ = nullptr;
	handler_function_ = nullptr;
	param_list_hint_ = "";
	explanation_ = "";
}

template <class HANDLER>
MenuCommand<HANDLER>::MenuCommand(uint8_t num_of_params, HANDLER* handler_object,
	void(HANDLER::* handler_function)(const std::vector<std::string>&), std::string param_list_hint,
	std::string explanation) : GenericMenuCommand(){

	if (handler_object == nullptr)
		throw std::exception("No handler object has given (ConsoleUI).");

	if (explanation.empty())
		throw std::exception("No command explanation has provided. (ConsoleUI).");

	num_of_params_ = num_of_params;
	handler_object_ = handler_object;
	handler_function_ = handler_function;
	param_list_hint_ = param_list_hint;
	explanation_ = explanation;
}

template <class HANDLER>
MenuCommand<HANDLER>::MenuCommand(const MenuCommand<HANDLER>& orig) : GenericMenuCommand(orig) {

	num_of_params_ = orig.num_of_params_;
	handler_object_ = orig.handler_object_;
	handler_function_ = orig.handler_function_;
	param_list_hint_ = orig.param_list_hint_;
	explanation_ = orig.explanation_;
}

template <class HANDLER>
MenuCommand<HANDLER>::~MenuCommand() {

	
}

template <class HANDLER>
void MenuCommand<HANDLER>::operator()(const std::vector<std::string>& params) {

	(handler_object_->*handler_function_)(params);
}

template <class HANDLER>
const std::string MenuCommand<HANDLER>::GetParamListHint() const {

	return param_list_hint_;
}

template <class HANDLER>
const std::string MenuCommand<HANDLER>::GetExplanation() const {

	return explanation_;
}

template <class HANDLER>
const uint8_t MenuCommand<HANDLER>::GetNumOfParams() const {

	return num_of_params_;
}

ConsoleUI::ConsoleUI() {

	with_default_exit_ = true;
}

ConsoleUI::ConsoleUI(std::string splash_text, bool with_default_exit) {

	splash_text_ = splash_text;
	with_default_exit_ = with_default_exit;
}

ConsoleUI::ConsoleUI(const ConsoleUI& orig) {

	splash_text_ = orig.splash_text_;
	with_default_exit_ = orig.with_default_exit_;
	menu_commands_ = orig.menu_commands_;
}

ConsoleUI& ConsoleUI::operator=(const ConsoleUI& orig) {

	splash_text_ = orig.splash_text_;
	with_default_exit_ = orig.with_default_exit_;
	menu_commands_ = orig.menu_commands_;
}

ConsoleUI::~ConsoleUI(){

	// event handler object is responsible for command menu object desctruction
}

void ConsoleUI::RegisterCommand(std::string name, GenericMenuCommand* menu_command_param) {

	if (menu_commands_.count(name))
		throw std::exception("The requested subscription of command option already exists in the "
			"name of given command parameter.");

	menu_commands_.insert({ name, menu_command_param});
}

void ConsoleUI::GetCommandList() {

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "COMMAND LIST:" << std::endl;
	
	std::cout << "   list-commands" << std::endl
		<< "      listing available commands" << std::endl;

	std::map<std::string, GenericMenuCommand*>::iterator it = menu_commands_.begin();
	std::map<std::string, GenericMenuCommand*>::iterator end_it = menu_commands_.end();
	
	for (; it != end_it; ++it) {

		std::cout << "   " << it->first << " " << it->second->GetParamListHint() << std::endl
			<< "      " << it->second->GetExplanation() << std::endl;
		std::cout << std::endl;
	}

	// In case of user defined command with 'exit' command name the defined command
	//  externs the default exit command
	if(with_default_exit_)
		std::cout << "   exit" << std::endl << "      close program" << std::endl;
}

void ConsoleUI::RunConsole() {

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << splash_text_ << std::endl;
	
	if (!menu_commands_.size())
		throw std::exception("No at least one command has subscribed (ConsoleUI).");

	this->GetCommandList();

	bool exit = false;
	std::string req_cmd = "";// requested command with its parameter(s)
	std::string cmd_name;
	std::vector<std::string> params;
	
	while (!exit) {
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		std::cout << "--------------------------------------------------" << std::endl;
		std::lock_guard<std::mutex> cli_lock(cli_mutex_);

		std::getline(std::cin, req_cmd);

		std::stringstream ss(req_cmd);
		ss >> cmd_name;

		if (cmd_name == "list-commands") {
			
			this->GetCommandList();
		}
		else if (cmd_name == "exit" && with_default_exit_) {
		
			exit = true;
		}
		else if (cmd_name == "exit" && !with_default_exit_) {
		
			exit = true;
			(*menu_commands_["exit"])(params);
		}
		else if (!menu_commands_.count(cmd_name)) {
		
			std::cout << "There is no such command." << std::endl;
		}
		else [[likely]] {

			params.clear();

			while (!ss.eof()) {

				params.push_back(std::string());
				ss >> params.back();
			}

			if (params.size() != menu_commands_[cmd_name]->GetNumOfParams())
				std::cout << "Invalid parameter list." << std::endl;
			else
				(*menu_commands_[cmd_name])(params);
		}
	}
}

void ConsoleUI::Println(std::string msg) {

	std::lock_guard<std::mutex> cli_lock(cli_mutex_);

	std::cout << msg << std::endl;
}