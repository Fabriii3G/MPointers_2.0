#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <string>
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class SocketClient {
private:
    int sock;
#ifdef _WIN32
    WSADATA wsa;
#endif

public:
    SocketClient(const std::string& ip, int port);
    ~SocketClient();

    std::string sendCommand(const std::string& command);
};

#endif // SOCKETCLIENT_H
