//
// Created by alear on 18/3/2025.
//
/*
#include "Server.h"
#include <iostream>

Server::Server(int port, size_t memorySize) : port(port), memoryManager(memorySize) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error al enlazar el socket");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Error al escuchar conexiones");
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
#ifdef _WIN32
    closesocket(serverSocket);
#else
    close(serverSocket);
#endif

    for (auto& t : clientThreads) {
        if (t.joinable()) t.join();
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

void Server::acceptConnections() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            perror("Error al aceptar conexión");
            continue;
        }

        std::cout << "Cliente conectado" << std::endl;

    }
}

void Server::start() {
    std::cout << "Servidor iniciado en el puerto " << port << std::endl;
    acceptConnections();
}

MemoryManager& Server::getMemoryManager() {
    return memoryManager;
}
*/