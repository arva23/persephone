// https://www.binarytides.com/winsock-socket-programming-tutorial/
// https://stackoverflow.com/questions/6755637/recv-windows-socket-takes-infinite-time-how-to-timeout

#ifndef RMICLIENT_H_INCLUDED
#define RMICLIENT_H_INCLUDED
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

	// stored computation model is implicit, interface can be defined by its general signature
	// only one server connection can be handled at a time
	template <class S>
	class RMIClient {

	public:
		RMIClient();

		/// <summary>
		/// rmi client creation, connection is established immediately after calling constructor
		/// </summary>
		/// <param name="object_id">string identifier of the handle object</param>
		/// <param name="server_ip_address">rmi server ip address</param>
		/// <param name="port">rmi server ip port</param>
		/// <param name="buffer_size">size of message buffer</param>
		/// <param name="delim">delimiter character for data separation</param>
		RMIClient(std::string object_id, std::string server_ip_address, uint64_t port,
			size_t buffer_size = 2000, char delim = '|', size_t timeout = 200);

		RMIClient(const RMIClient& orig);

		virtual ~RMIClient();

		/// <summary>
		/// member function registrations with different parameter configurations
		/// </summary>
		/// <typeparam name="R">return type of referenced function</typeparam>
		/// <typeparam name="...TS">parameter pack for referenced function argument</typeparam>
		/// <param name="method_id">string reference for convenient call of referenced function</param>
		/// <param name="method_ref">pointer to referenced method, callee method</param>
		template <class R, class... TS> requires StringAssignable<TS...>
		void RegisterMethod(std::string method_id, R(S::* method_ref)(TS...));

		/// <summary>
		/// calling remote, resolving called function by literal id
		/// </summary>
		/// <typeparam name="R">return type of referenced function</typeparam>
		/// <typeparam name="...TS">parameter pack for referenced function argument</typeparam>
		/// <param name="method_id">string reference for convinient call of referenced function</param>
		/// <param name="...params">unpacked parameter list</param>
		/// <returns>the result of remote function</returns>
		template <class R, class... TS> requires StringAssignable<TS...>
		R operator()(std::string method_id, TS... params);

		void SetTimeout(size_t new_timout);

		std::string GetObjectId();

	private:

		struct MethodBase {

			virtual std::string operator()(std::string params);
		};

		template <class R, class... TS> requires StringAssignable<TS...>
		struct VariadicMethod : public MethodBase {

			VariadicMethod(R(S::* method_ref)(TS...));

			template <size_t I = 0>
			inline typename std::enable_if <(I == sizeof ...(TS)), std::string>::type Serialize(TS... params);

			template <size_t I = 0>
			inline typename std::enable_if <(I >= 0), std::string>::type Serialize(TS... params);

			std::string operator()(TS... params) override;

			const size_t size_of_params_ = sizeof ...(TS);
			std::tuple <TS...> op_params_;
			std::string sparams_;
			std::stringstream ss_sparams_;
			R(S::* method_ref_)(TS...);
		};


		//SOCKET client_socket_;
		SOCKET server_socket_;
		std::string server_ip_address_;
		PVOID bin_server_ip_address_;
		uint64_t port_;
		struct sockaddr_in server_;
		size_t timeout_;

		std::string object_id_;
		char delim_;
		size_t buffer_size_;
		std::map<std::string, MethodBase*> method_list_;
	};
}
#endif// RMICLIENT_H_INCLUDED