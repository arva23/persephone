#ifndef RMISERVER_H_INCLUDED
#define RMISERVER_H_INCLUDED
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdint.h>
#include <exception>
#include <atomic>
#include <sstream>
#include <thread>

#include "../genmath/ObjectBase.h"
#include "../genmath/LongDouble.h"
#include "../logging/Logger.h"
#include "../genmath/String.h"
#include "RMIGen.h"

namespace distrsys {

	#ifndef WITH_DEBUG_MSG
	#define WITH_DEBUG_MSG
	#endif

	// literal conversion contraint
	#ifndef STRINGASSIGNABLE_HAS_SET
	template <class T>
	concept StringAssignable = std::is_base_of<genmath::ObjectBase, T>::value;
	#define STRINGASSIGNABLE_HAS_SET
	#endif

	// a server can handle multiple client connections at the same time
	// an object can be stored by its dedicated identifier (name) to the given port once
	//  to create multiple RMIServer about an object, instantiate RMIServer with different object
	//  identifier and port
	template <class S>
	class RMIServer {

	public:
		RMIServer();

		/// <summary>
		/// rmi server object creation
		/// </summary>
		/// <param name="object_id">string identifier of referenced remote object</param>
		/// <param name="object">pointer to the handle object</param>
		/// <param name="port">port for rmi server connection requests</param>
		/// <param name="buffer_size">message buffer size</param>
		/// <param name="delim">data delimiter character for data separation</param>
		RMIServer(std::string object_id, S* object, uint64_t port, size_t buffer_size = 2000, char delim = '|',
			size_t timeout = 200);

		RMIServer(RMIServer& orig);

		virtual ~RMIServer();

		/// <summary>
		/// member function registrations with different parameter configurations
		/// </summary>
		/// <typeparam name="R">return type of called member function of the handle object</typeparam>
		/// <typeparam name="...TS">parameter pack type of member function</typeparam>
		/// <param name="method_id">string identifier for call of member function</param>
		/// <param name="method_ref">pointer to referenced member function</param>
		template <class R, class... TS> requires StringAssignable<TS...>
		void RegisterMethod(std::string method_id, R(S::* method_ref)(TS...));

		// server processing thread, high level TCP communication with literal processing
		void RunServer();

		void StopClientRequestListening();

		size_t GetNumOfConnectedClients();

		void StopServerAll();// stopping all clients

		void StopServerI(size_t client_id);// stop ith client

		void SetTimeout(SOCKET s, size_t timeout);

		std::string GetObjectId();
	private:
		struct MethodBase {

			virtual std::string operator()(std::string params);
		};

		template <class R, class... TS> requires StringAssignable<TS...>
		struct VariadicMethod : public MethodBase {

			VariadicMethod(S* object, R(S::* method_ref)(TS...));

			template <size_t I = 0>
			inline typename std::enable_if <(I == sizeof ...(TS)), std::string>::type Deserialize(std::string sparams);

			template <size_t I = 0>
			inline typename std::enable_if <(I >= 0), std::string>::type Deserialize(std::string sparams);

			std::string operator()(std::string params) override;

			const size_t size_of_params_ = sizeof ...(TS);
			std::tuple<TS...> op_params_;
			std::stringstream ss_sparams_;
			std::string sdata_;

			R(S::* method_ref_)(TS...);
			S* object_;
		};

		void ClientProcessorThread(size_t client_id);

		S* object_;
		std::map<std::string, MethodBase*> method_list_;

		std::atomic_bool stop_client_request_listening_;

		uint64_t port_;

		SOCKET server_socket_;
		std::vector<SOCKET> client_sockets_;
		std::vector<std::thread> client_processors_;
		std::vector<std::mutex> client_processors_mutexes_;
		std::vector<bool> operate_client_;
		struct sockaddr_in server_;
		struct sockaddr_in client_;
		size_t timeout_;

		std::string object_id_;
		size_t buffer_size_;
		char delim_;// delimiter character for messages


		static std::map<std::string, std::set<uint64_t> >rmi_data_;// <object pointer, port>
	};
}

#endif// RMISERVER_H_INCLUDED