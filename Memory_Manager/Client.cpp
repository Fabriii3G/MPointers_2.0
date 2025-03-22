//
// Created by alear on 19/3/2025.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

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
    serverAddr.sin_port = htons(6060);  // Puerto del servidor
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Dirección IP del servidor (localhost)

    // Conectar al servidor
    std::cout << "Intentando conectar al servidor en 127.0.0.1:6060..." << std::endl;

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error al conectar al servidor" << std::endl;
        std::cerr << "Código de error: " << WSAGetLastError() << std::endl;  // Solo en Windows
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado al servidor" << std::endl;

    // Enviar un mensaje al servidor
    const char* message = "Hola, servidor!";
    send(clientSocket, message, strlen(message), 0);

    // Recibir la respuesta del servidor
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Respuesta del servidor: " << buffer << std::endl;

    // Cerrar el socket y limpiar Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}