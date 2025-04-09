#ifndef MPOINTER_TPP
#define MPOINTER_TPP

#include "MPointer.h"

// Constructor privado: Se usa solo dentro de New()
template <typename T>
MPointer<T>::MPointer(int objectID) : id(objectID) {
    if (memoryManager) {
        ptr = static_cast<T*>(memoryManager->get(id));
    } else {
        ptr = nullptr;
    }
    std::cout << "[DEBUG] MPointer creado con ID " << id << " en direccion " << ptr << std::endl;
}

// Metodo para inicializar el MemoryManager antes de usar MPointer
template <typename T>
void MPointer<T>::setMemoryManager(MemoryManager* manager) {
    memoryManager = manager;
}

// Crea un nuevo MPointer, asignando memoria en MemoryManager
template <typename T>
MPointer<T> MPointer<T>::New() {
    std::string type;

    if (std::is_same<T, int>::value) {
        type = "integer";
    } else if (std::is_same<T, char>::value) {
        type = "character";
        std::cout << sizeof(T) << std::endl;
    } else if (std::is_same<T, float>::value) {
        type = "float";
    } else if (std::is_same<T, double>::value) {
        type = "double";
    } else throw std::runtime_error("Tipo no soportado");

    std::cout << "PP" << std::endl;
    int objectID = memoryManager->create(sizeof(T), type);  // Crea memoria en MemoryManager
    std::cout << "Hola" << std::endl;
    return MPointer<T>(objectID);  // Devuelve un MPointer asociado al ID
}

// Destructor: Libera referencia en MemoryManager
template <typename T>
MPointer<T>::~MPointer() {
    std::cout << "[DEBUG] Destructor de MPointer llamado para ID " << id << std::endl;
    if (memoryManager && id != -1) {
        //SmemoryManager->decreaseRefCount(id);
    }
}

// Sobrecarga del operador * (Acceso a datos)
template <typename T>
T& MPointer<T>::operator*() {
    if (std::is_same<T, int>::value) {
        memoryManager->setInt(id, *ptr);
    } else if (std::is_same<T, char>::value) {
        //memoryManager->setFloat(id, *ptr);
        std::cout << sizeof(T) << std::endl;
    } else if (std::is_same<T, float>::value) {
        memoryManager->setFloat(id, *ptr);
    } else if (std::is_same<T, double>::value) {
        memoryManager->setDouble(id, *ptr);
    } else throw std::runtime_error("Tipo no soportado");

    //memoryManager->set(id, std::to_string(*ptr));
    return *ptr;
}

// Sobrecarga del operador & (Devuelve la dirección de memoria)
template <typename T>
T* MPointer<T>::operator&() {
    return ptr;
}

// Asignación de un MPointer a otro
template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    std::cout << "[DEBUG] Operador = llamado: " << id << " -> " << other.id << std::endl;
    if (this != &other) {
        if (memoryManager) {
            memoryManager->increaseRefCount(other.id);
            memoryManager->decreaseRefCount(id);
        }
        id = other.id;
        ptr = other.ptr;
    }
    return *this;
}

// Asignación de un valor directamente a MPointer
template <typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    if (ptr) {
        *ptr = value;
    }
    return *this;
}

// Asignación de nullptr (Libera la memoria)
template <typename T>
MPointer<T>& MPointer<T>::operator=(std::nullptr_t) {
    if (memoryManager) {
        memoryManager->decreaseRefCount(id);
    }
    ptr = nullptr;
    id = -1;
    return *this;
}

// Devuelve el ID del objeto
template <typename T>
int MPointer<T>::GetID() const {
    return id;
}

// Constructor de copia
template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    id = other.id;
    ptr = other.ptr;
    if (memoryManager) {
        memoryManager->increaseRefCount(id);
    }
}

#endif // MPOINTER_TPP