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

class Connection {
	SOCKET socket;
	uint16_t messageLen; 
	uint16_t cursor;
	std::string in;
	std::string out;

	Connection(SOCKET sock) {
		socket = sock;
		messageLen = sizeof(messageLen);
		cursor = 0;
		in.resize(messageLen);
	}

	~Connection() {
		if (socket != INVALID_SOCKET) {
			closesocket(socket);
		}
	}

	void recv();

	void send();

	virtual void parse() = 0;
};