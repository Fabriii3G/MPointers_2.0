//
// Created by alear on 21/3/2025.
//

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "MemoryBlock.h"
#include "GarbageCollector.h"
#include <unordered_map>
#include <mutex>
#include <string>


class MemoryManager {
private:
    void* memoryPool;
    size_t totalSize;
    MemoryBlock* head;
    std::unordered_map<int, MemoryBlock*> allocations;
    std::mutex mtx;
    int nextId;
    void splitBlock(MemoryBlock* block, size_t size);

    //GarbageCollector gc;  // Agregar Garbage Collector

public:
    MemoryManager(size_t sizeMB);
    ~MemoryManager();

    int create(size_t size, const std::string& type);
    void* get(int id);
    bool set(int id,  std::string& value);
    bool increaseRefCount(int id);
    bool decreaseRefCount(int id);
    void collectGarbage();  // Nuevo metodo para el GC
    void dumpMemoryState();

};

#endif // MEMORY_MANAGER_H