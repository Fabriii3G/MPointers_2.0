//
// Created by alear on 19/3/2025.
//

#include "ConnectionHandler.h"
#include <iostream>
#include <sstream>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

void ConnectionHandler::handleClient(int clientSocket, MemoryManager& memoryManager) {
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
            size_t size;
            ss >> size;
            int id = memoryManager.create(size);
            response = (id != -1) ? "CREATED " + std::to_string(id) : "ERROR No memory";
        } else if (action == "SET") {
            int id;
            std::string value;
            ss >> id >> value;
            memoryManager.set(id, value.c_str(), value.size());
            response = "SET OK";
        } else if (action == "GET") {
            int id;
            ss >> id;
            void* data = memoryManager.get(id);
            response = data ? std::string("VALUE ") + static_cast<char*>(data) : "ERROR Not found";
        } else if (action == "INCREF") {
            int id;
            ss >> id;
            memoryManager.increaseRefCount(id);
            response = "INCREF OK";
        } else if (action == "DECREF") {
            int id;
            ss >> id;
            memoryManager.decreaseRefCount(id);
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
