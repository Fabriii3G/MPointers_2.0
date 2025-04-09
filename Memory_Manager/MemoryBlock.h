#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <cstddef>
#include <string>  // Para almacenar el tipo como string

// Estructura para representar un bloque de memoria dentro del Memory Manager
struct MemoryBlock {
    void* address;    // Dirección del bloque dentro del espacio reservado
    int size;      // Tamaño del bloque en bytes
    int refCount;     // Contador de referencias
    bool free;        // Indica si el bloque está libre
    MemoryBlock* next; // Apunta al siguiente bloque en la lista enlazada
    std::string type;  // <-- Nuevo campo para almacenar el tipo de dato

    MemoryBlock(void* addr, size_t sz, std::string dataType = "")
        : address(addr), size(sz), refCount(1), free(false), next(nullptr), type(dataType) {}
};

#endif //MEMORYBLOCK_H
