#pragma once
#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <exception>
#include <sstream>
#include <thread>
#include <mutex>

struct TestHandler {

	TestHandler() {}

	TestHandler(const TestHandler& orig) {}

	~TestHandler() {}

	void TestHandlerFunction(const std::vector<std::string>& test_params) {

		std::cout << "Test handler function has called with parameter: '" << test_params[0] << "'." << std::endl;
	}
};

struct GenericMenuCommand {

	GenericMenuCommand();
	GenericMenuCommand(const GenericMenuCommand& orig);
	virtual ~GenericMenuCommand();

	virtual void operator()(const std::vector<std::string>& params);
	virtual const std::string GetParamListHint() const;
	virtual const std::string GetExplanation() const;
	virtual const uint8_t GetNumOfParams() const;
};

template <class HANDLER>
struct MenuCommand : public GenericMenuCommand {

	MenuCommand();
	/// <summary>
	/// it creates new menu command (menu option)
	/// </summary>
	/// <param name="name">name of command, to be typed when it is called</param>
	/// <param name="num_of_params">number of function parameters to the original
	/// handler function</param>
	/// <param name="handler_object">object to act on, the called object of the handler function</param>
	/// <param name="handler_function">the event handler function</param>
	/// <param name="explanation">a short description about the command/option</param>
	MenuCommand(uint8_t num_of_params, HANDLER* handler_object,
		void(HANDLER::* handler_function)(const std::vector<std::string>&),	std::string param_list_hint,
		std::string explanation);
	MenuCommand(const MenuCommand& orig);
	virtual ~MenuCommand();

	void operator()(const std::vector<std::string>& params) override;
	const std::string GetParamListHint() const override;
	const std::string GetExplanation() const override;
	const uint8_t GetNumOfParams() const override;
	
	uint8_t num_of_params_;
	HANDLER* handler_object_;
	void(HANDLER::* handler_function_)(const std::vector<std::string>&);
	std::string param_list_hint_;
	std::string explanation_;
};

class ConsoleUI {

public:
	ConsoleUI();
	ConsoleUI(std::string splash_text, bool with_default_exit);
	ConsoleUI(const ConsoleUI& orig);
	ConsoleUI& operator=(const ConsoleUI& orig);
	virtual ~ConsoleUI();

	void RegisterCommand(std::string name, GenericMenuCommand* menu_command_param);
	void GetCommandList();
	void RunConsole();
	void Println(std::string msg);

private:

	std::mutex cli_mutex_;

	std::string splash_text_;
	bool with_default_exit_;
	std::map<std::string, GenericMenuCommand*> menu_commands_;
};

class TestHandler;
class MainController;
template class MenuCommand<MainController>;
template class MenuCommand<TestHandler>;