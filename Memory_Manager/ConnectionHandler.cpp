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
    MPointer<int>::setMemoryManager(&memoryManager);
    MPointer<bool>::setMemoryManager(&memoryManager);
    MPointer<float>::setMemoryManager(&memoryManager);
    MPointer<double>::setMemoryManager(&memoryManager);

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
            std::string type;
            ss >> type;

            if (type == "INT") {
                MPointer<int> ptr = MPointer<int>::New();
                response = "CREATED " + std::to_string(ptr.GetID());
            } else if (type == "CHAR") {
                MPointer<char> ptr = MPointer<char>::New();
                response = "CREATED " + std::to_string(ptr.GetID());
            } else if (type == "FLOAT") {
                MPointer<float> ptr = MPointer<float>::New();
                response = "CREATED " + std::to_string(ptr.GetID());
            } else if (type == "DOUBLE") {
                MPointer<double> ptr = MPointer<double>::New();
                response = "CREATED " + std::to_string(ptr.GetID());
            } else {
                response = "ERROR: Tipo no soportado.";
            }

        } else if (action == "SET") {
            int id;
            std::string value;
            ss >> id >> value;

            std::string type = memoryManager.getType(id);

            if (type == "integer") {
                try {
                    int intValue = std::stoi(value);
                    MPointer<int> ptr(id);
                    *ptr = intValue;
                    response = "SET OK";
                } catch (...) {
                    response = "SET ERROR: Valor inválido para tipo integer.";
                }
            } else if (type == "char") {
                if (value.length() == 1) {
                    MPointer<char> ptr(id);
                    *ptr = value[0];
                    response = "SET OK";
                } else {
                    response = "SET ERROR: Se esperaba un solo caracter para tipo char.";
                }
            } else if (type == "float") {
                try {
                    float floatValue = std::stof(value);
                    MPointer<float> ptr(id);
                    *ptr = floatValue;
                    response = "SET OK";
                } catch (...) {
                    response = "SET ERROR: Valor inválido para tipo float.";
                }
            } else if (type == "double") {
                try {
                    double doubleValue = std::stod(value);
                    MPointer<double> ptr(id);
                    *ptr = doubleValue;
                    response = "SET OK";
                } catch (...) {
                    response = "SET ERROR: Valor inválido para tipo double.";
                }
            } else {
                response = "SET ERROR: Tipo no soportado.";
            }

        } else if (action == "GET") {
            int id;
            ss >> id;

            std::string type = memoryManager.getType(id);

            try {
                if (type == "integer") {
                    MPointer<int> ptr(id);
                    response = "VALUE " + std::to_string(*ptr);
                } else if (type == "character") {
                    MPointer<char> ptr(id);
                    response = std::string("VALUE ") + (*ptr ? "true" : "false");
                } else if (type == "float") {
                    MPointer<float> ptr(id);
                    response = "VALUE " + std::to_string(*ptr);
                } else if (type == "double") {
                    MPointer<double> ptr(id);
                    response = "VALUE " + std::to_string(*ptr);
                } else {
                    response = "GET ERROR: Tipo no compatible o desconocido.";
                }
            } catch (const std::exception& e) {
                response = std::string("GET ERROR: ") + e.what();
            }

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
