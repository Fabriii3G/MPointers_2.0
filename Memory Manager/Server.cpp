//
// Created by alear on 18/3/2025.
//

#include "Server.h"
#include "ConnectionHandler.h"
#include <iostream>
#include <unistd.h>

Server::Server(int port) : port(port) {

#ifdef _WIN32
    // Inicializar Winsock
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
    close(serverSocket);
    for (auto& t : clientThreads) {
        if (t.joinable()) t.join();
    }
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
        clientThreads.emplace_back(&ConnectionHandler::handleClient, clientSocket);
    }
}

void Server::start() {
    std::cout << "Servidor iniciado en el puerto " << port << std::endl;
    acceptConnections();
}
