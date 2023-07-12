#include "RMIClient.h"

// RMI CLIENT
//  RMI CLIENT VARIADIC SERIALIZATION METHOD TYPE

template <class S>
std::string distrsys::RMIClient<S>::MethodBase::operator()(std::string params) {

	return std::string();
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
distrsys::RMIClient<S>::VariadicMethod<R, TS...>::VariadicMethod(R(S::* method_ref)(TS...)) : MethodBase() {

	method_ref_ = method_ref;
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
template <size_t I>
typename std::enable_if <(I == sizeof ...(TS)), std::string>::type distrsys::RMIClient<S>::VariadicMethod<R, TS...>::Serialize(
	TS... params) {
	
	op_params_ = std::make_tuple<TS...>(params...);
	sparams_ = "";
	return sparams_;
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
template <size_t I>
typename std::enable_if <(I >= 0), std::string>::type distrsys::RMIClient<S>::VariadicMethod<R, TS...>::Serialize(
	TS... params) {

	sparams_ += std::string(std::get<I>(op_params_)) + delim_;
	return ProcessRawData<I + 1>(params);
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
std::string distrsys::RMIClient<S>::VariadicMethod<R, TS...>::operator()(TS... params) {

	return Serialize<0>(params...);
}


// RMI CLIENT
template <class S>
distrsys::RMIClient<S>::RMIClient() {

	bin_server_ip_address_ = nullptr;
	buffer_size_ = 2000;
	delim_ = '|';
	port_ = 0;
	server_ = sockaddr_in();
	server_socket_ = SOCKET();
}

template <class S>
distrsys::RMIClient<S>::RMIClient(std::string object_id, std::string server_ip_address, uint64_t port, size_t buffer_size, 
	char delim, size_t timeout) {
	
	if (object_id.empty()) [[unlikely]]
		throw std::exception("Computation model identifier is null/empty (RMIClient).");

	object_id_ = object_id;

	if (!buffer_size) [[unlikely]]
		throw std::exception("Buffer size is 0 (RMIServer).");

	buffer_size_ = buffer_size;

	if (delim == ' ') [[unlikely]]
		throw std::exception("Delimiter character is white space (RMIClient).");

	delim_ = delim;

	size_t size_of_server_ip_address = server_ip_address.size();
	bool valid_address = true;
	char ch = server_ip_address[0];

	if (server_ip_address.empty() || server_ip_address.size() < 3) [[unlikely]]
		throw std::exception("Empty server ip address (RMIClient).");

	int64_t ip_blocks = 3;
	for (size_t i = 0; i < size_of_server_ip_address && valid_address; ++i) {
	
		ch = server_ip_address[i];
		
		valid_address = valid_address && (ch == '0' || ch == '1' || ch == '2' || ch == '3' 
			|| ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '.');

		if (ch == '.') --ip_blocks;
	}

	if (!valid_address || ip_blocks) [[unlikely]]
		throw std::exception("Invalid ip format (RMIClient).");

	server_ip_address_ = server_ip_address;

	if (!port)
		throw std::exception("Port number is zero (RMIClient).");

	port_ = port;

#ifdef WITH_DEBUG_MSG
	std::cout << "Creating socket... (RMIClient)" << std::endl;
#endif

	if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		throw std::exception((std::string("Could not create socket, ")
			+ std::to_string(WSAGetLastError()) + " (RMIClient).").c_str());

	server_.sin_family = AF_INET;
	server_.sin_addr.s_addr = inet_pton(AF_INET, server_ip_address_.c_str(), bin_server_ip_address_);
	server_.sin_port = htons(port_);

#ifdef WITH_DEBUG_MSG
	std::cout << "Conneting to server... (RMIClient)" << std::endl;
#endif

	if (connect(server_socket_, (struct sockaddr*)&server_, sizeof(server_)) < 0)
		throw std::exception("Could not connect to server (RMIClient).");

	SetTimeout(timeout);
	timeout_ = timeout;
}

template <class S>
distrsys::RMIClient<S>::RMIClient(const RMIClient& orig) {

	server_socket_ = orig.server_socket_;
	server_ip_address_ = orig.server_ip_address_;
	bin_server_ip_address_ = orig.bin_server_ip_address_;
	port_ = orig.port_;
	server_ = orig.server_;
	timeout_ = orig.timeout_;
	object_id_ = orig.object_id_;
	delim_ = orig.delim_;
	buffer_size_ = orig.buffer_size_;
	method_list_ = orig.method_list_;

}

template <class S>
distrsys::RMIClient<S>::~RMIClient() {

	closesocket(server_socket_);
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
void distrsys::RMIClient<S>::RegisterMethod(std::string method_id, R(S::* method_ref)(TS... params)) {

	if (method_list_[method_id].count(method_id))
		throw std::exception("Such method with this id has already been inserted (RMIClient).");
	else [[likely]]
		method_list_[method_id] = new VariadicMethod<R, TS...>(method_ref);
}

template <class S>
template <class R, class... TS> requires StringAssignable<TS...>
R distrsys::RMIClient<S>::operator()(std::string method_id, TS... params) {

	if (!method_list_.count(method_id)) [[unlikely]]
		throw std::exception("Requested method does not exist (RMIClient).");

	// converting parameters to string form
	 std::string msg = method_id + delim_ + (*method_list_[method_id])(params...);

	 // sending call request to server
	 if (send(server_socket_, msg.c_str(), msg.size(), 0) < 0) [[unlikely]]
		 throw std::exception("Error at sending request to server object (RMIClient).");

	 // receiving result
	 if (recv(server_socket_, msg.data(), buffer_size_, 0) < 0) [[unlikely]]
		 throw std::exception("Error at receiving result message (RMIClient).");

	 return R(msg);
}

template <class S>
void distrsys::RMIClient<S>::SetTimeout(size_t timeout) {

	if (!timeout)// setting timout at least 1ms
		throw std::exception("Operation timeout is too small (RMIClient).");

	if (setsockopt(server_socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout_, sizeof(size_t)))
		throw std::exception("There was an error at setting new timeout value of receive operation (RMIClient).");

	if (setsockopt(server_socket_, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout_, sizeof(size_t)))
		throw std::exception("There was and error at setting new timeout value of send operation (RMIClient).");
}

template <class S>
std::string distrsys::RMIClient<S>::GetObjectId() {

	return object_id_;
}
