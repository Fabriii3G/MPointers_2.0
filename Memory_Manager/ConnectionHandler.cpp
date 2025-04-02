//
// Created by alear on 19/3/2025.
//

#include "ConnectionHandler.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include "MPointer.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

void ConnectionHandler::handleClient(int clientSocket, MemoryManager& memoryManager) {
    //  Establecer MemoryManager en MPointer antes de crear cualquier instancia
    MPointer<int>::setMemoryManager(&memoryManager);

    char buffer[1024] = {0};

    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "Cliente desconectado" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0';
        std::string command(buffer);
        std::stringstream ss(command);
        std::string action;
        ss >> action;

        std::string response;

        if (action == "CREATE") {
            MPointer<int> ptr = MPointer<int>::New();
            response = "CREATED " + std::to_string(ptr.GetID());

        } else if (action == "SET") {
            int id, value;
            ss >> id >> value;

            MPointer<int> ptr(id);  // Se asocia al ID existente en MemoryManager
            *ptr = value;  // Se asigna el valor al objeto en memoria

            response = "SET OK";

        } else if (action == "GET") {
            int id;
            ss >> id;

            MPointer<int> ptr(id);  // Obtener el MPointer con el ID
            response = "VALUE " + std::to_string(*ptr);

        } else if (action == "INCREF") {
            int id;
            ss >> id;

            MPointer<int> ptr(id);
            response = "INCREF OK";  // El constructor ya incrementa la referencia

        } else if (action == "DECREF") {
            int id;
            ss >> id;

            MPointer<int> ptr(id);
            ptr = nullptr;  // Libera referencia

            response = "DECREF OK";

        } else {
            response = "ERROR Unknown command";
        }

        send(clientSocket, response.c_str(), response.size(), 0);
    }

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}
