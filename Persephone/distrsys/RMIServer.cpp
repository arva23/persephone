#include "RMIServer.h"

// RMI SERVER
//  RMI SERVER VARIADIC DESERIALIZATION METHOD TYPE

template <class S>
std::string distrsys::RMIServer<S>::MethodBase::operator()(std::string params) {

	return std::string();
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
distrsys::RMIServer<S>::VariadicMethod<R, TS...>::VariadicMethod(S* object, R(S::* method_ref)(TS...)) : MethodBase() {

	object_ = object;
	method_ref_ = method_ref;
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
template <size_t I>
typename std::enable_if <(I == sizeof ...(TS)), std::string>::type distrsys::RMIServer<S>::VariadicMethod<R, TS...>::Deserialize(
	std::string sparams) {

	return std::string(R(std::apply((object_->*(method_ref_)), op_params_)));
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
template <size_t I>
typename std::enable_if <(I >= 0), std::string>::type distrsys::RMIServer<S>::VariadicMethod<R, TS...>::Deserialize(
	std::string sparams) {

	ss_sparams_ = std::stringstream(sparams);

	if (ss_sparams_.eof() && size_of_params_) [[unlikely]]
		throw std::exception("Empty parameter list (RMIServer).");

	// deserialization of raw data
	// serialized data pattern, separated by delimiter characters
	std::getline(ss_sparams_, sdata_, delim_);
	if (std::get<I>(op_params_) = sdata_) [[unlikely]]
		throw std::exception("Data conversion error at parameter assignment (RMIServer).");

	return ProcessRawData<I + 1>(sparams);
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
std::string distrsys::RMIServer<S>::VariadicMethod<R, TS...>::operator()(std::string sparams) {

	return Deserialize<0>(sparams);
}


// RMI SERVER

template <class S>
distrsys::RMIServer<S>::RMIServer() {

	buffer_size_ = 0;
	client_ = sockaddr_in();
	delim_ = '|';
	object_ = nullptr;
	port_ = 8888;
	server_ = sockaddr_in();
	server_socket_ = SOCKET();
}

template <class S>
distrsys::RMIServer<S>::RMIServer(std::string object_id, S* object, uint64_t port, size_t buffer_size, char delim, size_t timeout) {

	if (object_id.empty()) [[unlikely]]
		throw std::exception("Computation model identifier is null/empty (RMIServer).");

	object_id_ = object_id;

	if (delim == ' ') [[unlikely]]
		throw std::exception("Delimiter character is white space (RMIServer).");

	delim_ = delim;

	if (object_ == nullptr) [[unlikely]]
		throw std::exception("Computation model pointer is null (RMIServer).");

	object_ = object;

	if (!buffer_size) [[unlikely]]
		throw std::exception("Buffer size is 0 (RMIServer).");

	buffer_size_ = buffer_size;

	if (!port)
		throw std::exception("Port number is zero (RMIServer).");

	// object id redundancy check is not interprocess redundancy checker, 
	//  object identifier redundancy might happen by user among processes of this server program
	if (rmi_data_.count(object_id) && rmi_data_[object_id].count(port)) [[unlikely]]
		throw std::exception("RMIServer has been registered with the provided object identifier and port (RMIServer).");

	port_ = port;
	rmi_data_[object_id_].insert(port_);

#ifdef WITH_DEBUG_MSG
	std::cout << "Creating socket... (RMIServer)" << std::endl;
#endif
	
	if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) [[unlikely]]
		throw std::exception((std::string("Could not create socket, ")
			+ std::to_string(WSAGetLastError()) + " (RMIServer).").c_str());

	server_.sin_family = AF_INET;
	server_.sin_addr.s_addr = INADDR_ANY;
	server_.sin_port = htons(port_);

	// binding server
#ifdef WITH_DEBUG_MSG
	std::cout << "Binding server... (RMIServer)" << std::endl;
#endif

	if (bind(server_socket_, (struct sockaddr*)&server_, sizeof(server_)) == SOCKET_ERROR) [[unlikely]]
		throw std::exception((std::string("Bind failed with error code, ")
			+ std::to_string(WSAGetLastError()) + " (RMIServer).").c_str());
	//exit(EXIT_FAILURE);

	if (!rmi_data_.count(object_id_)) [[likely]] {

		rmi_data_[object_id_] = std::set<uint64_t>();
		rmi_data_[object_id_].insert(port);
	}

	stop_client_request_listening_ = false;

	SetTimeout(timeout);
	timeout_ = timeout;
}

template <class S>
distrsys::RMIServer<S>::RMIServer(RMIServer& orig) {

	object_ = orig.object_;
	method_list_ = orig.method_list_;
	stop_client_request_listening_ = orig.stop_client_request_listening_ ? true : false;
	port_ = orig.port_;
	server_socket_ = orig.server_socket_;
	client_sockets_ = orig.client_sockets_;
	orig.StopServerAll();

	operate_client_ = orig.operate_client_;
	server_ = orig.server_;
	client_ = orig.client_;
	object_id_ = orig.object_id_;
	buffer_size_ = orig.buffer_size_;
	delim_ = orig.delim_;
}

template <class S>
distrsys::RMIServer<S>::~RMIServer() {

	StopServerAll();

	typename std::map<std::string, MethodBase*>::iterator it = method_list_.begin();
	typename std::map<std::string, MethodBase*>::iterator end_it = method_list_.end();

	for (; it != end_it; ++it) delete it->second;

	closesocket(server_socket_);
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
void distrsys::RMIServer<S>::RegisterMethod(std::string method_id, R(S::* method_ref)(TS...)) {

	if (method_list_[method_id].count(method_id))
		throw std::exception("Such method with this id has already been inserted (RMIServer).");
	else [[likely]]
		method_list_[method_id] = new VariadicMethod<R, TS...>(object_, method_ref);
}


template <class S>
void distrsys::RMIServer<S>::RunServer() {

	stop_client_request_listening_ = false;

	// waiting for connection of individual, dedicated client (1 to 1 connection can be established)
#ifdef WITH_DEBUG_MSG
	std::cout << "Waiting for clients connection... (RMIServer)" << std::endl;
#endif

	listen(server_socket_, 3);

	int c = sizeof(struct sockaddr_in);
	
#ifdef WITH_DEBUG_MSG
	std::cout << "Accepting connection requests... (RMIServer)" << std::endl;
#endif

	size_t client_id = 0;

	while (!stop_client_request_listening_) {

		client_sockets_.push_back(SOCKET());
		client_processors_.push_back(std::thread());

		if ((client_sockets_.back() = accept(server_socket_, (struct sockaddr*)&client_, &c)) == INVALID_SOCKET) [[unlikely]]
			throw std::exception("Error at acception client connection (RMIServer).");

		operate_client_.push_back(true);

		client_processors_.back() = std::thread(&RMIServer::ClientProcessorThread, this, ++client_id);

		SetTimeout(client_sockets_.back(), timeout_);
	}

#ifdef WITH_DEBUG_MSG
	std::cout << "Accepting client connection requests has been stopped (RMIServer)." << std::endl;
#endif
}

template <class S>
void distrsys::RMIServer<S>::StopClientRequestListening() {

	stop_client_request_listening_ = true;
}

template <class S>
size_t distrsys::RMIServer<S>::GetNumOfConnectedClients() {

	return client_sockets_.size();
}

template <class S>
void distrsys::RMIServer<S>::StopServerAll() {

	size_t size_of_operate_client = operate_client_.size();

	for (size_t i = 0; i < size_of_operate_client; ++i) {

		operate_client_[i] = false;
		client_processors_[i].join();
	}

	for (size_t i = 0; i < size_of_operate_client; ++i) {

		std::scoped_lock<std::mutex> client_lock(client_processors_mutexes_[i]);
		closesocket(client_sockets_[i]);
	}
}

template <class S>
void distrsys::RMIServer<S>::StopServerI(size_t client_id) {

	operate_client_[client_id] = false;
	client_processors_[client_id].detach();
}

template <class S>
void distrsys::RMIServer<S>::SetTimeout(SOCKET s, size_t timeout) {

	if (!timeout)// setting timout at least 1ms
		throw std::exception("Operation timeout is too small (RMIServer).");

	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout_, sizeof(size_t)))
		throw std::exception("There was an error at setting new timeout value of receive operation (RMIServer).");

	if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout_, sizeof(size_t)))
		throw std::exception("There was and error at setting new timeout value of send operation (RMIServer).");
}

template <class S>
std::string distrsys::RMIServer<S>::GetObjectId() {

	return object_id_;
}

template <class S>
void distrsys::RMIServer<S>::ClientProcessorThread(size_t client_id) {

	std::string msg(object_id_);

	// receiving local (server) object identifier
	if (recv(server_socket_, msg.data(), msg.size(), 0) == SOCKET_ERROR) [[unlikely]]
		throw std::exception("Error at receiving object identifier from client connection request (RMIServer).");

	if (msg != object_id_) {

		msg = "Bad object identifier (" + msg + " !=  " + object_id_ + ") (RMIServer).";

		if (send(client_sockets_[client_id], msg.c_str(), msg.size(), 0) < 0) [[unlikely]]
			throw std::exception("Error at sending object identifier mismatch error message to client (RMIServer).");

		throw std::exception("Requested remote object identifier differs from server identifier (RMIServer).");
	}

#ifdef WITH_DEBUG_MSG
	std::cout << "Connection has successfully established (RMIServer)." << std::endl;
	std::cout << "Starting function call listenings (RMIServer)." << std::endl;
#endif

	msg.reserve(buffer_size_);
	std::string method_id;

	std::unique_lock<std::mutex> client_lock(client_processors_mutexes_[client_id]);
	client_lock.unlock();

	while (operate_client_[client_id]) {

		// receiving message with waiting
		client_lock.lock();
		if (recv(client_sockets_[client_id], msg.data(), buffer_size_, 0) < 0) [[unlikely]]
			throw std::exception("Error at receiving call command (RMIServer).");

		client_lock.unlock();

		std::stringstream ss_data(msg);
		std::getline(ss_data, msg, delim_);
		msg = msg.substr(method_id.size());
		msg = "";

		// calling requested local(remote) function
		if (method_list_.count(method_id)) [[likely]]
			msg = (*method_list_[method_id])(msg);
		else
			throw std::exception("No such registered method by id (RMIServer).");

		// sending result of called method to caller
		client_lock.lock();
		if (send(client_sockets_[client_id], msg.c_str(), msg.size(), 0) < 0) [[unlikely]]
			throw std::exception("Err at sending result to command (RMIServer).");
		client_lock.unlock();
	}

#ifdef WITH_DEBUG_MSG
	std::cout << "Client connection " << client_id << " has closed (RMIServer)." << std::endl;
#endif
}
