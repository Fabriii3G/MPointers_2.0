//
// Created by alear on 19/3/2025.
//
#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "MemoryManager.h"

class ConnectionHandler {
public:
    static void handleClient(int clientSocket, MemoryManager& memoryManager);
};

#endif // CONNECTION_HANDLER_H
