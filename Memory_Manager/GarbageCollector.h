//
// Created by alear on 21/3/2025.
//

#ifndef GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_H

#include "MemoryManager.h"
#include <thread>
#include <atomic>

class MemoryManager;

class GarbageCollector {
private:
    MemoryManager* memoryManager;
    std::atomic<bool> running;
    std::thread gcThread;

    void run();  // Metodo del hilo

public:
    explicit GarbageCollector(MemoryManager* manager);
    ~GarbageCollector();

    void start();
    void stop();
};

#endif // GARBAGE_COLLECTOR_H