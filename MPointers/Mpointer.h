#ifndef MPOINTER_H
#define MPOINTER_H

#include "../Memory_Manager/MemoryManager.h"
#include "../Memory_Manager/SocketClient.h"
template <typename T>
class MPointer {
private:
    static SocketClient* socketClient;
    T* ptr;  // Puntero al tipo T
    int id;  // ID en el MemoryManager
    static MemoryManager* memoryManager;  // Referencia al MemoryManager



public:
    static void Init(int port);
    static void setMemoryManager(MemoryManager* manager);  // Inicializa MemoryManager
    static MPointer<T> New();  // Crea un nuevo MPointer
    MPointer(int id);  // Constructor privado que recibe un ID
    ~MPointer();  // Destructor


    T& operator*();  // Sobrecarga del operador *
    T* operator&();  // Sobrecarga del operador &

    MPointer<T>& operator=(const MPointer<T>& other);  // Asignación entre MPointers
    MPointer<T>& operator=(const T& value);  // Asignación directa de valores
    MPointer<T>& operator=(std::nullptr_t);  // Asignación de nullptr

    int GetID() const;  // Devuelve el ID del objeto
    MPointer(const MPointer<T>& other);  // Constructor de copia
};

// Definición del puntero estático a MemoryManager
template<typename T>
MemoryManager* MPointer<T>::memoryManager = nullptr;

#include "MPointer.tpp"

#endif // MPOINTER_H
