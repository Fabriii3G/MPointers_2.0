#include "MemoryManager.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif
#include <sstream>
#include <string>

#include <fstream>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif


#include <unordered_map>
#include "MemoryBlock.h"


using namespace std;
MemoryManager::MemoryManager(size_t sizeMB)
    : totalSize(sizeMB * 1024 * 1024),
      head(nullptr),
      nextId(1),
      gc(this) {

#ifdef _WIN32
    _mkdir("Dumps");
#else
    mkdir("Dumps", 0777);
#endif

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
    gc.start();
}

MemoryManager::~MemoryManager() {
    gc.stop();
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
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
    if (!lock.owns_lock()) {
        std::cout << "[GC] Memoria en uso. Saltando recoleccion.\n";
        return;  // Evita deadlock si otro hilo ya tiene el lock
    }

    std::cout << "[GC] Iniciando recoleccion de basura...\n";

    MemoryBlock* current = head;
    while (current) {
        if (current->refCount == 0 && !current->free) {
            std::cout << "Liberando bloque en " << current->address << " (Tamano: " << current->size << ")\n";
            current->free = true;
        }
        current = current->next;
    }

    std::cout << "[GC] Recoleccion finalizada.\n";
}



int MemoryManager::create(int size, const std::string& type) {
    cout << size;
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
                if (type == "INT") {
                    current->size  = sizeof(int);
                } else if (type == "DOUBLE") {
                    current->size = sizeof(double);
                } else if (type == "FLOAT") {
                    current->size = sizeof(float);
                } else {
                    current->size = sizeof(char);
                }
            }


            // Ahora almacenamos el tipo correctamente
            current->type = type;



            current->free = false;
            allocations[nextId] = current;
            std::cout << "CREATE: Asignado ID " << nextId << " (Tamano: " << current->size << ", Tipo: " << type << ", Direccion: " << current->address << ")\n";
            dumpMemoryState("CREATE " + type, current->address, "");
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

bool MemoryManager::set(int id, const std::string& type, void* value) {
    if (type == "int") return setInt(id, *static_cast<int*>(value));
    if (type == "float") return setFloat(id, *static_cast<float*>(value));
    if (type == "double") return setDouble(id, *static_cast<double*>(value));

    return false;
}

bool MemoryManager::setInt(int id, int value) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "[DEBUG] SET (int) llamado para ID " << id << std::endl;

    if (allocations.find(id) == allocations.end()) {
        std::cerr << "SET: Error, ID no encontrado.\n";
        return false;
    }

    MemoryBlock* block = allocations[id];
    if (sizeof(int) > block->size) {
        std::cerr << "SET: Error, valor demasiado grande para el bloque.\n";
        return false;
    }

    std::memcpy(block->address, &value, sizeof(int));  // <- aquí la corrección
    std::cout << "SET: Guardado en ID " << id << " -> " << value << "\n";

    dumpMemoryState("SET INT", block->address, std::to_string(value));
    return true;
}

bool MemoryManager::setDouble(int id, double value) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "[DEBUG] SET (double) llamado para ID " << id << std::endl;

    if (allocations.find(id) == allocations.end()) {
        std::cerr << "SET: Error, ID no encontrado.\n";
        return false;
    }

    MemoryBlock* block = allocations[id];
    if (sizeof(double) > block->size) {
        std::cerr << "SET: Error, valor demasiado grande para el bloque.\n";
        return false;
    }

    std::memcpy(block->address, &value, sizeof(double));
    std::cout << "SET: Guardado en ID " << id << " -> " << value << "\n";

    dumpMemoryState("SET DOUBLE", block->address, std::to_string(value));


    return true;
}

bool MemoryManager::setFloat(int id, float value) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "[DEBUG] SET (float) llamado para ID " << id << std::endl;

    if (allocations.find(id) == allocations.end()) {
        std::cerr << "SET: Error, ID no encontrado.\n";
        return false;
    }

    MemoryBlock* block = allocations[id];
    if (sizeof(float) > block->size) {
        std::cerr << "SET: Error, valor demasiado grande para el bloque.\n";
        return false;
    }

    std::memcpy(block->address, &value, sizeof(float));
    std::cout << "SET: Guardado en ID " << id << " -> " << value << "\n";

    dumpMemoryState("SET FLOAT", block->address, std::to_string(value));

    return true;
}


string MemoryManager::getType(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (allocations.find(id) != allocations.end()) {
        return allocations[id]->type;
    }
    return "unknown";
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
        dumpMemoryState("DECREF (Liberado)", block->address, "");
    }

    return true;
}


void MemoryManager::dumpMemoryState(const std::string& action, void* affectedAddress, const std::string& value) {
    std::lock_guard<std::mutex> lock(dumpMtx);

    // Obtener tiempo actual con milisegundos
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&timeNow);

    // Formatear nombre del archivo
    std::ostringstream fileName;
    fileName << "Dumps/memory_"
             << std::put_time(localTime, "%Y%m%d_%H%M%S")
             << "_" << std::setw(3) << std::setfill('0') << ms.count()
             << ".txt";

    std::ofstream logFile(fileName.str());
    if (!logFile.is_open()) {
        std::cerr << "[ERROR] No se pudo crear el archivo de dump." << std::endl;
        return;
    }

    // Encabezado
    logFile << "===== Estado de la Memoria =====\n";
    logFile << "Accion: " << action << "\n";
    logFile << "Direccion afectada: " << affectedAddress << "\n";
    if (!value.empty()) {
        logFile << "Valor modificado: " << value << "\n";
    }
    logFile << "Fecha y hora: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
            << "." << std::setw(3) << std::setfill('0') << ms.count() << "\n";
    logFile << "---------------------------------\n";

    // Detalle de los bloques
    MemoryBlock* current = head;
    while (current) {
        logFile << "Direccion: " << current->address
                << " | Tamano: " << current->size
                << " | RefCount: " << current->refCount
                << " | Estado: " << (current->free ? "Libre" : "Ocupado")
                << " | Tipo: " << current->type << "\n";
        current = current->next;
    }

    logFile.close();
}


size_t MemoryManager::getTypeSize(const std::string& type) {
    if (type == "INT") return sizeof(int);
    if (type == "FLOAT") return sizeof(float);
    if (type == "DOUBLE") return sizeof(double);
    if (type == "BOOL") return sizeof(bool);
    if (type == "CHAR") return sizeof(char);
    return 0; // Tipo desconocido
}

void MemoryManager::startServer(int port) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock" << std::endl;
        return;
    }
#endif

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket del servidor\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error al enlazar el socket\n";
        return;
    }

    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error al escuchar conexiones\n";
        return;
    }

    std::cout << "Servidor escuchando en el puerto " << port << std::endl;

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        std::cerr << "Error al aceptar conexión\n";
        return;
    }

    std::cout << "Cliente conectado\n";

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "Cliente desconectado\n";
            break;
        }

        std::string command(buffer);
        std::istringstream ss(command);
        std::string action;
        ss >> action;

        std::string response;

        if (action == "CREATE") {
            std::string type;
            ss >> type;
            size_t size = getTypeSize(type);

            int id = create(size, type);
            response = (id != -1) ? "CREATED " + std::to_string(id) : "ERROR No memory";
        }
        else if (action == "SET") {
            int id;
            std::string valueStr;
            ss >> id >> valueStr;

            std::string type = getType(id);
            bool result = false;

            if (type == "INT") {
                int val = std::stoi(valueStr);
                result = setInt(id, val);
            } else if (type == "FLOAT") {
                float val = std::stof(valueStr);
                result = setFloat(id, val);
            } else if (type == "DOUBLE") {
                double val = std::stod(valueStr);
                result = setDouble(id, val);
            } else {
                response = "ERROR Unknown type";
            }

            if (response.empty()) {
                response = result ? "SET OK" : "ERROR SET failed";
            }
        }
        else if (action == "GET") {
            int id;
            ss >> id;
            //response = "VALUE " + get(id);
        }
        else if (action == "INCREF") {
            int id;
            ss >> id;
            response = increaseRefCount(id) ? "INCREF OK" : "ERROR ID not found";
        }
        else if (action == "DECREF") {
            int id;
            ss >> id;
            response = decreaseRefCount(id) ? "DECREF OK" : "ERROR ID not found";
        }
        else {
            response = "ERROR Unknown command";
        }

        send(clientSocket, response.c_str(), response.size(), 0);
    }

#ifdef _WIN32
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
#else
    close(clientSocket);
    close(serverSocket);
#endif
}


void MemoryManager::acceptConnections() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            perror("Error al aceptar conexión");
            continue;
        }

        std::cout << "[MemoryManager] Cliente conectado" << std::endl;
        clientThreads.emplace_back(&MemoryManager::handleClient, clientSocket);
    }
}

void MemoryManager::handleClient(int clientSocket) {
    char buffer[1024] = {0};

    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "[MemoryManager] Cliente desconectado" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0';
        std::string response = std::string("ECHO: ") + buffer;

        send(clientSocket, response.c_str(), response.size(), 0);
    }

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

