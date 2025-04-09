#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
//#include "LinkedList.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Inicializar Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock" << std::endl;
        return 1;
    }

    // Crear el socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket del cliente" << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar la dirección del servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6060);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Conectar al servidor
    std::cout << "Intentando conectar al servidor en 127.0.0.1:6060..." << std::endl;
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar al servidor" << std::endl;
        std::cerr << "Código de error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado al servidor" << std::endl;

    // Interacción con el servidor en bucle
    std::string message;
    char buffer[1024] = {0};
    while (true) {
        std::cout << "Ingrese comando (CREATE, SET, GET, INCREF, DECREF o EXIT): ";
        std::getline(std::cin, message);

        if (message == "EXIT") {
            break;
        }

        send(clientSocket, message.c_str(), message.size(), 0);

        // Recibir la respuesta del servidor
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Conexion cerrada por el servidor" << std::endl;
            break;
        }

        std::cout << "Respuesta del servidor: " << buffer << std::endl;
    }

    // Cerrar el socket y limpiar Winsock
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}