//
// Created by alear on 21/3/2025.
//

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "MemoryBlock.h"
#include "GarbageCollector.h"
#include <unordered_map>
#include <mutex>

class MemoryManager {
private:
    void* memoryPool;
    size_t totalSize;
    MemoryBlock* head;
    std::unordered_map<int, MemoryBlock*> allocations;
    std::mutex mtx;
    int nextId;
    //GarbageCollector gc;  // Agregar Garbage Collector

public:
    MemoryManager(size_t sizeMB);
    ~MemoryManager();

    int create(size_t size);
    void* get(int id);
    void set(int id, const void* data, size_t dataSize);
    void increaseRefCount(int id);
    void decreaseRefCount(int id);
    void collectGarbage();  // Nuevo metodo para el GC
    void dumpMemoryState();

};

#endif // MEMORY_MANAGER_H