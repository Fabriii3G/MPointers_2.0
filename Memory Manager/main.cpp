//
// Created by alear on 19/3/2025.
//

#include "Server.h"
#include <iostream>

int main() {
    std::cout << "Iniciando servidor..." << std::endl;

    // Crear una instancia del servidor en el puerto 8080
    Server server(8080);

    // Iniciar el servidor
    server.start();

    std::cout << "Servidor en ejecución. Presiona Enter para salir..." << std::endl;
    std::cin.get();  // Esperar a que el usuario presione Enter para salir

    return 0;
}