//
// Created by alear on 21/3/2025.
//

#include "MemoryManager.h"
#include <iostream>
#include <cstring>

MemoryManager::MemoryManager(size_t sizeMB)
    : totalSize(sizeMB * 1024 * 1024),
      memoryPool(malloc(totalSize)),
      head(new MemoryBlock(memoryPool, totalSize)),
      nextId(1) {
}

MemoryManager::~MemoryManager() {
    free(memoryPool);
    while (head) {
        MemoryBlock* temp = head;
        head = head->next;
        delete temp;
    }
}

void MemoryManager::collectGarbage() {
    std::lock_guard<std::mutex> lock(mtx);
    MemoryBlock* current = head;
    while (current) {
        if (current->refCount == 0 && !current->free) {
            std::cout << "Liberando bloque en " << current->address << " (Tamaño: " << current->size << ")\n";
            current->free = true;
        }
        current = current->next;
    }
}

int MemoryManager::create(size_t size) {
    std::lock_guard<std::mutex> lock(mtx);

    MemoryBlock* current = head;
    while (current) {
        if (current->free && current->size >= size) {
            current->free = false;
            allocations[nextId] = current;
            return nextId++;
        }
        current = current->next;
    }

    return -1; // No hay espacio suficiente
}

void* MemoryManager::get(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (allocations.find(id) != allocations.end()) {
        return allocations[id]->address;
    }
    return nullptr;
}

void MemoryManager::set(int id, const void* data, size_t dataSize) {
    std::lock_guard<std::mutex> lock(mtx);
    if (allocations.find(id) != allocations.end()) {
        MemoryBlock* block = allocations[id];
        if (dataSize <= block->size) {
            memcpy(block->address, data, dataSize);
        }
    }
}

void MemoryManager::increaseRefCount(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (allocations.find(id) != allocations.end()) {
        allocations[id]->refCount++;
    }
}

void MemoryManager::decreaseRefCount(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (allocations.find(id) != allocations.end()) {
        MemoryBlock* block = allocations[id];
        block->refCount--;
        if (block->refCount <= 0) {
            block->free = true;
            allocations.erase(id);
        }
    }
}

void MemoryManager::dumpMemoryState() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Estado de la memoria:\n";
    MemoryBlock* current = head;
    while (current) {
        std::cout << "Bloque en " << current->address << " - Tamaño: "
                  << current->size << " - RefCount: " << current->refCount
                  << " - Estado: " << (current->free ? "Libre" : "Ocupado") << std::endl;
        current = current->next;
    }
}