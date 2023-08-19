#ifndef RMIGEN_H_INCLUDED
#define RMIGEN_H_INCLUDED
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <string>

namespace distrsys {

	class RMIGen {

		static inline struct RMIInitializer {

			RMIInitializer() {

				InitRMIServers();
			}

			static void InitRMIServers() {

				std::cout << "Initialising Winsock... (RMIServer)" << std::endl;
				if (WSAStartup(MAKEWORD(2, 2), &wsa_) != 0) {

					throw std::exception((std::string("Failed to create winsocket,  ")
						+ std::to_string(WSAGetLastError()) + " (RMIServer).").c_str());
				}
			}

			~RMIInitializer() {

				WSACleanup();
			}

		} initializer_;

		static WSADATA wsa_;
	};
}
#endif// RMIGEN_H_INCLUDED
