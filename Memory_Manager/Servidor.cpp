//
// Created by alear on 20/3/2025.
//
#include "Server.h"
#include <iostream>

int main() {
    std::cout << "Servidor en ejecución..." << std::endl;

    // Crear una instancia del servidor en el puerto 6060
    Server server(6060, 10);

    // Iniciar el servidor
    server.start();

    return 0;
}