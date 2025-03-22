//
// Created by alear on 21/3/2025.
//

#include "GarbageCollector.h"
#include "MemoryManager.h"
#include <iostream>
#include <chrono>

GarbageCollector::GarbageCollector(MemoryManager* manager) : memoryManager(manager), running(false) {}

GarbageCollector::~GarbageCollector() {
    stop();
}

void GarbageCollector::start() {
    running = true;
    gcThread = std::thread(&GarbageCollector::run, this);
}

void GarbageCollector::stop() {
    running = false;
    if (gcThread.joinable()) {
        gcThread.join();
    }
}

void GarbageCollector::run() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(5));  // Revisión cada 5s
        memoryManager->collectGarbage();
    }
}
