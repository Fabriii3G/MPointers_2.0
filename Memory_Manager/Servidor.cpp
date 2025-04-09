//
// Created by alear on 20/3/2025.
//
#include "MemoryManager.h"
#include <iostream>

int main() {
    std::cout << "[MAIN] Iniciando MemoryManager con servidor...\n";

    MemoryManager mm(70); // 70MB
    mm.startServer(6060);

    return 0;
}
