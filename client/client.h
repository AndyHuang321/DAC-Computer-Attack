#pragma once

#include "connection.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class ServerConnection : private Connection {
	void parse();
};

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