//
// Created by alear on 19/3/2025.
//

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")  // Enlaza la librería de sockets en Windows
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif


#include "ConnectionHandler.h"
#include <iostream>
#include <unistd.h>

void ConnectionHandler::handleClient(int clientSocket) {
    char buffer[1024] = {0};

    while (true) {
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            std::cout << "Cliente desconectado" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Mensaje recibido: " << buffer << std::endl;

        std::string response = "Mensaje recibido\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
}
