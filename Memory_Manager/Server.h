//
// Created by alear on 18/3/2025.
//
/*
#ifndef SERVER_H
#define SERVER_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include <vector>
#include <thread>
#include "MemoryManager.h"

class Server {
private:
    int serverSocket;
    int port;
    struct sockaddr_in serverAddr;
    std::vector<std::thread> clientThreads;
    MemoryManager memoryManager;  // Agregar MemoryManager

    void acceptConnections();
public:
    explicit Server(int port, size_t memorySize);
    ~Server();
    void start();
    MemoryManager& getMemoryManager();  // Metodo para acceder a MemoryManager
};

#endif // SERVER_H
*/