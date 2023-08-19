#ifndef GCODECOMMAND_H_INCLUDED
#define GCODECOMMAND_H_INCLUDED
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <sstream>
#include <algorithm>
#include <exception>
#include "../genmath/ObjectBase.h"
#include "../genmath/LongDouble.h"
#include <type_traits>

namespace printersimulation {

	template <class T>
	concept StringAssignable = std::is_base_of<genmath::ObjectBase, T>::value;

	template <class T> requires StringAssignable<T>
	class GCodeCommand : public genmath::ObjectBase {

	public:
		// GCodeCommand reference model commands
		struct Cmd {

			Cmd() {

				name_ = "";
				valueless_params_ = true;
			}
		
			Cmd(std::string name) {

				name_ = name;
				valueless_params_ = true;
			};
		
			Cmd(std::string name, std::vector<std::string> params, bool valueless_params = false) {

				name_ = name;
				params_ = params;
				valueless_params_ = valueless_params;
			}
			Cmd& operator=(const Cmd& orig) {

				name_ = orig.name_;
				params_ = orig.params_;
				valueless_params_ = orig.valueless_params_;
				return *this;
			}

			virtual ~Cmd() {}

			std::string name_;
			std::vector<std::string> params_;
			bool valueless_params_;
		};

		// implicit params (auxiliary augmentation for further processing)
		// command patterns, list of sample commands without parameter realization (value assignment)
		static struct CmdModels {
			CmdModels();
			CmdModels(const CmdModels& orig);
			~CmdModels();

			std::vector<Cmd> models_;
			size_t size_of_models_;

		} CommandModels;

		GCodeCommand();
		/// <summary>
		/// GCode command creation
		/// </summary>
		/// <param name="name">name of the command</param>
		/// <param name="params">container of command parameters</param>
		GCodeCommand(std::string name, std::map<std::string, T> params);
		GCodeCommand(std::string raw_data);
		GCodeCommand(const GCodeCommand<T>& orig);
		~GCodeCommand();

		GCodeCommand<T>& operator=(const GCodeCommand<T>& orig);

		/// <summary>
		/// converts raw string data into GCodeCommand type
		/// </summary>
		/// <param name="raw_data">incoming raw data</param>
		/// <returns>flag about operation status (true is the failure)</returns>
		bool operator=(std::string raw_data) override;
		operator std::string() const override;

		std::string& GetName();
		bool IsSet(std::string key);
		T& Get(std::string key);
		void Set(std::string key, T value);// true if key not found, then new inserted

		static char DelimChar;

	private:
		std::string name_;
		std::map<std::string, T> params_;
		Cmd* model_ref_;// command model reference
		bool valueless_params_;
	};
}

template class printersimulation::GCodeCommand<genmath::LongDouble>;template class printersimulation::GCodeCommand<genmath::LongDouble>;
#endif// GCODECOMMAND_H_INCLUDED