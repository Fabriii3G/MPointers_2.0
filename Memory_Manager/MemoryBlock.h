//
// Created by alear on 21/3/2025.
//

#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <cstddef>

// Estructura para representar un bloque de memoria dentro del Memory Manager
struct MemoryBlock {
    void* address;    // Dirección del bloque dentro del espacio reservado
    size_t size;      // Tamaño del bloque en bytes
    int refCount;     // Contador de referencias
    bool free;        // Indica si el bloque está libre
    MemoryBlock* next; // Apunta al siguiente bloque en la lista enlazada

    MemoryBlock(void* addr, size_t sz)
        : address(addr), size(sz), refCount(1), free(false), next(nullptr) {}
};


#endif //MEMORYBLOCK_H
