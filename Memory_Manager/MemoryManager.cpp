//
// Created by alear on 21/3/2025.
//

#include "MemoryManager.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

MemoryManager::MemoryManager(size_t sizeMB)
    : totalSize(sizeMB * 1024 * 1024),
      head(nullptr),
      nextId(1) {

#ifdef _WIN32
    memoryPool = VirtualAlloc(nullptr, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    memoryPool = std::malloc(totalSize);
#endif

    if (!memoryPool) {
        std::cerr << "Error: No se pudo reservar " << totalSize << " bytes de memoria." << std::endl;
        throw std::runtime_error("Error al reservar memoria.");
    }

    head = new MemoryBlock(memoryPool, totalSize);
    head->free = true;

    std::cout << "Inicializacion: head -> Direccion: " << head->address
          << " | Tamano: " << head->size
          << " | Libre: " << (head->free ? "Si" : "No") << "\n";

}

MemoryManager::~MemoryManager() {
#ifdef _WIN32
    VirtualFree(memoryPool, 0, MEM_RELEASE);
#else
    std::free(memoryPool);
#endif

    while (head) {
        MemoryBlock* temp = head;
        head = head->next;
        delete temp;
    }
}
void MemoryManager::splitBlock(MemoryBlock* block, size_t size) {
    if (block->size > size + sizeof(MemoryBlock)) {
        void* newAddress = (char*)block->address + size;
        MemoryBlock* newBlock = new MemoryBlock(newAddress, block->size - size - sizeof(MemoryBlock));
        newBlock->free = true;
        newBlock->next = block->next;
        block->next = newBlock;
        block->size = size;

        std::cout << " splitBlock(): Se creo un nuevo bloque en " << newBlock->address
                  << " de tamano " << newBlock->size << "\n";
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

int MemoryManager::create(size_t size, const std::string& type) {
    std::cout << "Estado inicial de la memoria antes de CREATE:\n";
    dumpMemoryState();

    std::lock_guard<std::mutex> lock(mtx);
    MemoryBlock* current = head;

    while (current) {
        std::cout << "Revisando bloque en " << current->address
                  << " | Tamano: " << current->size
                  << " | Libre: " << (current->free ? "Si" : "No") << "\n";

        if (current->free && current->size >= size) {

            if (current->size > size + sizeof(MemoryBlock)) {
                void* newAddress = static_cast<char*>(current->address) + size;
                MemoryBlock* newBlock = new MemoryBlock(newAddress, current->size - size - sizeof(MemoryBlock));
                newBlock->free = true;
                newBlock->next = current->next;
                current->next = newBlock;
                current->size = size;
            }

            current->free = false;
            allocations[nextId] = current;
            std::cout << "CREATE: Asignado ID " << nextId << " (Tamano: " << size << ", Tipo: " << type << ")\n";
            return nextId++;
        }
        current = current->next;
    }

    std::cerr << "CREATE: No hay memoria disponible.\n";
    return -1;
}

void* MemoryManager::get(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (allocations.find(id) != allocations.end()) {
        return allocations[id]->address;
    }
    return nullptr;
}


bool MemoryManager::set(int id, const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);

    if (allocations.find(id) == allocations.end()) {
        std::cerr << "SET: Error, ID no encontrado.\n";
        return false;
    }

    MemoryBlock* block = allocations[id];
    if (value.size() > block->size) {
        std::cerr << "SET: Error, valor demasiado grande para el bloque.\n";
        return false;
    }

    std::memcpy(block->address, value.c_str(), value.size() + 1); // +1 para '\0'
    std::cout << "SET: Guardado en ID " << id << " -> " << value << "\n";
    return true;
}


bool MemoryManager::increaseRefCount(int id) {
    std::lock_guard<std::mutex> lock(mtx);

    if (allocations.find(id) == allocations.end()) {
        std::cerr << "INCREF: Error, ID no encontrado.\n";
        return false;
    }

    allocations[id]->refCount++;
    std::cout << "INCREF: ID " << id << " ahora tiene refCount " << allocations[id]->refCount << "\n";
    return true;
}

bool MemoryManager::decreaseRefCount(int id) {
    std::lock_guard<std::mutex> lock(mtx);

    if (allocations.find(id) == allocations.end()) {
        std::cerr << "DECREF: Error, ID no encontrado.\n";
        return false;
    }

    MemoryBlock* block = allocations[id];
    block->refCount--;

    std::cout << "DECREF: ID " << id << " ahora tiene refCount " << block->refCount << "\n";

    if (block->refCount <= 0) {
        block->free = true;
        allocations.erase(id);
        std::cout << "DECREF: Bloque liberado.\n";
    }

    return true;
}


void MemoryManager::dumpMemoryState() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Estado actual de la memoria:\n";
    MemoryBlock* current = head;
    while (current) {
        std::cout << "Direccion: " << current->address
                  << " | Tamano: " << current->size
                  << " | RefCount: " << current->refCount
                  << " | Estado: " << (current->free ? "Libre" : "Ocupado") << "\n";
        current = current->next;
    }
}
