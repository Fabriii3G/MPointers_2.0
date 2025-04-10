#ifndef MPOINTER_TPP
#define MPOINTER_TPP
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "MPointer.h"
#include "../List/LinkedList.tpp"
#include "../Memory_Manager/SocketClient.h"



template <typename T>
SocketClient* MPointer<T>::socketClient = nullptr;

template<typename T>
void MPointer<T>::Init(int port) {
    if (!socketClient) {
        socketClient = new SocketClient("127.0.0.1", port);
    }
}


// Constructor privado: Se usa solo dentro de New()
template <typename T>
MPointer<T>::MPointer(int objectID) : id(objectID) {
    if (memoryManager) {
        ptr = static_cast<T*>(memoryManager->get(id));
    } else {
        ptr = nullptr;
    }
    std::cout << "[DEBUG] MPointer creado con ID " << id << std::endl;
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
    if (std::is_same<T, int>::value) type = "INT";
    else if (std::is_same<T, char>::value) type = "CHAR";
    else if (std::is_same<T, float>::value) type = "FLOAT";
    else if (std::is_same<T, double>::value) type = "DOUBLE";
    else if (std::is_same<T, Node<int>>::value) type = "NODE";
    else throw std::runtime_error("Tipo no soportado");

    std::string command = "CREATE " + type;
    std::string response = socketClient->sendCommand(command); // Implementa esta clase
    cout << command;
    if (response.find("CREATED") == 0) {
        int id = std::stoi(response.substr(8));
        return MPointer<T>(id);
    } else {
        throw std::runtime_error("Error al crear objeto: " + response);
    }
}


// Destructor: Libera referencia en MemoryManager
template <typename T>
MPointer<T>::~MPointer() {
    std::cout << "[DEBUG] Destructor de MPointer llamado para ID " << id << std::endl;
    if (memoryManager && id != -1) {
        std::string command = "DECREF " + id;
        std::string response = socketClient->sendCommand(command); // Implementa esta clase
    }
}

template <typename T>
MPointer<T>& MPointer<T>::operator*() {
    return *this;
}


// Sobrecarga del operador & (Devuelve la direccion de memoria)
template <typename T>
T* MPointer<T>::operator&() {
    return ptr;
}

// Asignacion de un MPointer a otro
template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    std::cout << "[DEBUG] Operador = llamado: " << id << " -> " << other.id << std::endl;
    std::string command1 = "INCREF " + std::to_string(other.id);
    std::cout << "Comando 1: " << command1 << std::endl;
    std::string response1 = socketClient->sendCommand(command1); // Implementa esta clase
    std::string command2 = "DECREF " + std::to_string(id);
    std::cout << "Comando 2: " << command2 << std::endl;
    std::string response2 = socketClient->sendCommand(command2); // Implementa esta clase
    id = other.id;
    return *this;
}

// Asignacion de un valor directamente a MPointer
template <typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    std::string command = "SET " + std::to_string(this->id) + " " + std::to_string(value);
    std::string response = socketClient->sendCommand(command);

    if (response.find("SET OK") == 0) {
        std::cout << "[MPointer] Valor asignado exitosamente: " << value << std::endl;
        //*this->ptr = value;
    } else {
         std::cerr << "[ERROR] Falló el comando SET: " << response << std::endl;
    }

    return *this;
}


// Asignacion de nullptr (Libera la memoria) (Revisar si sirve)
template <typename T>
MPointer<T>& MPointer<T>::operator=(std::nullptr_t) {
    std::string command = "DECREF " + id;
    std::string response = socketClient->sendCommand(command);
    ptr = nullptr;
    id = -1;
    return *this;
}

// Devuelve el ID del objeto
template <typename T>
int MPointer<T>::GetID() const {
    return id;
}

// Constructor de copia (Revisar si sirve)
template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    id = other.id;
    ptr = other.ptr;
    std::string command = "INCREF " + id;
    std::string response = socketClient->sendCommand(command);
}

#endif // MPOINTER_TPP