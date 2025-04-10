#ifndef MPOINTER_H
#define MPOINTER_H

#include "../Memory_Manager/MemoryManager.h"
#include "../Memory_Manager/SocketClient.h"

// Forward declaration
template <typename T>
class MPointer;

template <typename T>
class MPointerReference {
private:
    MPointer<T>& mp;
public:
    MPointerReference(MPointer<T>& pointer) : mp(pointer) {}

    MPointerReference<T>& operator=(const T& value) {
        std::string type;
        if (std::is_same<T, int>::value) type = "INT";
        else if (std::is_same<T, char>::value) type = "CHAR";
        else if (std::is_same<T, float>::value) type = "FLOAT";
        else if (std::is_same<T, double>::value) type = "DOUBLE";
        else throw std::runtime_error("Tipo no soportado");

        std::string command = "SET " + std::to_string(mp.GetID()) + " " + std::to_string(value);
        std::string response = MPointer<T>::socketClient->sendCommand(command);
        if (response.find("OK") == std::string::npos) {
            throw std::runtime_error("Error en SET: " + response);
        }

        *(mp.ptr) = value;
        return *this;
    }

    operator T() const {
        std::string command = "GET " + std::to_string(mp.GetID());
        std::string response = MPointer<T>::socketClient->sendCommand(command);

        if (response.find("VALUE ") == 0) {
            T value;
            std::istringstream iss(response.substr(6));
            iss >> value;
            *(mp.ptr) = value;
            return value;
        } else {
            throw std::runtime_error("Error en GET: " + response);
        }
    }
};


template <typename T>
class MPointer {
private:
    T* ptr;  // Puntero al tipo T
    int id;  // ID en el MemoryManager
    static MemoryManager* memoryManager;  // Referencia al MemoryManager
    static SocketClient* socketClient;
    friend class MPointerReference<T>;


public:
    static void Init(int port);
    static void setMemoryManager(MemoryManager* manager);  // Inicializa MemoryManager
    static MPointer<T> New();  // Crea un nuevo MPointer
    MPointer(int id);  // Constructor privado que recibe un ID
    ~MPointer();  // Destructor


    MPointerReference<T> operator*(); // Sobrecarga del operador *
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
