//
// Created by alear on 21/3/2025.
//

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "MemoryBlock.h"
#include <unordered_map>
#include <mutex>
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include <thread>

using namespace std;

class MemoryManager {
private:
    int serverSocket;
    int port;
    struct sockaddr_in serverAddr;
    std::vector<std::thread> clientThreads;

    void acceptConnections();
    static void handleClient(int clientSocket);

    void* memoryPool;
    size_t totalSize;
    MemoryBlock* head;
    std::unordered_map<int, MemoryBlock*> allocations;
    std::mutex mtx;
    int nextId;
    void splitBlock(MemoryBlock* block, size_t size);

public:
    MemoryManager(size_t sizeMB);
    ~MemoryManager();

    void startServer(int port);
    size_t getTypeSize(const std::string& type);

    int create(size_t size, const std::string& type);
    void* get(int id);
    bool set(int id, const std::string& type, void* value);
    bool setInt(int id,  int value);
    bool setDouble(int id, double value);
    bool setFloat(int id,  float value);
    bool setChar(int id,  char value);
    string getType(int id);

    bool increaseRefCount(int id);
    bool decreaseRefCount(int id);
    void collectGarbage();  // Nuevo metodo para el GC
    void dumpMemoryState();

};

#endif // MEMORY_MANAGER_H