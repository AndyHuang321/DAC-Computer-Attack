#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <process.h>
#include <string>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class Client {
public:
	static bool connected;

	Client(std::string ip, int port);
	bool connectToServer();
	bool closeConnection();
	bool sendString(std::string message);
	std::string recvString();

private:
	SOCKET connection; //This client's connection to the server
	SOCKADDR_IN address; //Address to be binded to our Connection socket
};