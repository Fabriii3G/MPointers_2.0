//
// Created by alear on 18/3/2025.
//

#ifndef SERVER_H
#define SERVER_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;  // Definir socklen_t en Windows
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include <vector>
#include <thread>

class Server {
private:
    int serverSocket;
    int port;
    struct sockaddr_in serverAddr;
    std::vector<std::thread> clientThreads;

    void acceptConnections();
public:
    explicit Server(int port);
    ~Server();
    void start();
};

#endif // SERVER_H
