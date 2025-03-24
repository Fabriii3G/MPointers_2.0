#ifndef MPOINTER_TPP
#define MPOINTER_TPP

#include "MPointer.h"
#include "MemoryManager.h"

// Constructor privado
template <typename T>
MPointer<T>::MPointer() : ptr(nullptr), id(-1) {}

// Metodo estático New
template <typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> newPtr;
    newPtr.id = MemoryManager::create(sizeof(T), typeid(T).name());  // Solicita un nuevo bloque de memoria al MemoryManager
    return newPtr;
}

// Destructor
template <typename T>
MPointer<T>::~MPointer() {
    if (id != -1) {
        MemoryManager::decreaseRefCount(id);  // Indica que la referencia se ha destruido
    }
}

// Sobrecarga del operador *
template <typename T>
T& MPointer<T>::operator*() {
    return *static_cast<T*>(MemoryManager::get(id));  // Obtiene el valor del MemoryManager
}

// Sobrecarga del operador &
template <typename T>
T* MPointer<T>::operator&() {
    return reinterpret_cast<T*>(id);  // Retorna la dirección (ID) del bloque de memoria
}

// Sobrecarga del operador = (asignación de otro MPointer)
template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    if (this != &other) {
        if (id != -1) {
            MemoryManager::decreaseRefCount(id);  // Libera la referencia anterior
        }
        id = other.id;
        MemoryManager::increaseRefCount(id);  // Incrementa el conteo de referencias
    }
    return *this;
}

// Sobrecarga del operador = (asignación de valor)
template <typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    if (id != -1) {
        MemoryManager::set(id, std::to_string(value));  // Almacena el valor en el MemoryManager
    }
    return *this;
}

// Sobrecarga del operador = (asignación nullptr)
template <typename T>
MPointer<T>& MPointer<T>::operator=(std::nullptr_t) {
    if (id != -1) {
        MemoryManager::decreaseRefCount(id);  // Libera la referencia
        id = -1;
    }
    return *this;
}

// Obtiene el ID del MPointer
template <typename T>
int MPointer<T>::GetID() const {
    return id;
}

// Constructor de copia
template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    id = other.id;
    MemoryManager::increaseRefCount(id);  // Incrementa la referencia en el MemoryManager
}

#endif // MPOINTER_TPP