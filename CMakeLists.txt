cmake_minimum_required(VERSION 3.30)
project(MPointers_2_0)

#Configura el estándar de C++ (C++14 en este caso)
set(CMAKE_CXX_STANDARD 14)

#Define el ejecutable del servidor
add_executable(Server
"Memory_Manager/Servidor.cpp"
"Memory_Manager/Server.cpp"
"Memory_Manager/ConnectionHandler.cpp"
        "Memory_Manager/MemoryManager.cpp"
"Memory_Manager/GarbageCollector.cpp"
)

#Enlaza la biblioteca ws2_32 (necesaria para sockets en Windows)
target_link_libraries(Server ws2_32)

#Define el ejecutable del cliente
add_executable(Client
"Memory_Manager/Client.cpp"
        MPointers/MPointer.tpp
        MPointers/Mpointer.h
)
# Enlaza la biblioteca ws2_32 (necesaria para sockets en Windows)
target_link_libraries(Client ws2_32)
include_directories(${CMAKE_SOURCE_DIR}/MPointers)